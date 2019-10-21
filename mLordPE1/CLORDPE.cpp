// CLORDPE.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CLORDPE.h"
#include "afxdialogex.h"
#include"CRVATOFOA.h"
#include "CSECTION.h"
#include "CDIRECTORY.h"

// CLORDPE 对话框

IMPLEMENT_DYNAMIC(CLORDPE, CDialogEx)

CLORDPE::CLORDPE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LORDPE, pParent)
	, m_AddressOfEntryPoint(_T(""))
	, m_ImageBase(_T(""))
	, m_TimeDateStamp(_T(""))
	, m_NumberOfSections(_T(""))
	, m_SubSystem(_T(""))
	, m_SizeOfImage(_T(""))
	, m_SectionAlignment(_T(""))
	, m_FileAlignment(_T(""))
	, m_BaseOfCode(_T(""))
	, m_BaseOfData(_T(""))
	, m_SizeOfOptionalHeader(_T(""))
	, m_SizeOfHeaders(_T(""))
	, m_DllCharacteristics(_T(""))
	, m_CheckSum(_T(""))
	, m_Magic(_T(""))
	, m_NumberOfRvaAndSizes(_T(""))
{

}

CLORDPE::~CLORDPE()
{
}

void CLORDPE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_AddressOfEntryPoint);
	DDX_Text(pDX, IDC_EDIT3, m_ImageBase);
	DDX_Text(pDX, IDC_EDIT6, m_TimeDateStamp);
	DDX_Text(pDX, IDC_EDIT4, m_NumberOfSections);
	DDX_Text(pDX, IDC_EDIT2, m_SubSystem);
	DDX_Text(pDX, IDC_EDIT5, m_SizeOfImage);
	DDX_Text(pDX, IDC_EDIT11, m_SectionAlignment);
	DDX_Text(pDX, IDC_EDIT13, m_FileAlignment);
	DDX_Text(pDX, IDC_EDIT7, m_BaseOfCode);
	DDX_Text(pDX, IDC_EDIT9, m_BaseOfData);
	DDX_Text(pDX, IDC_EDIT14, m_SizeOfOptionalHeader);
	DDX_Text(pDX, IDC_EDIT8, m_SizeOfHeaders);
	DDX_Text(pDX, IDC_EDIT10, m_DllCharacteristics);
	DDX_Text(pDX, IDC_EDIT12, m_CheckSum);
	DDX_Text(pDX, IDC_EDIT15, m_Magic);
	DDX_Text(pDX, IDC_EDIT16, m_NumberOfRvaAndSizes);
	DDX_Control(pDX, IDC_BUTTON1, m_CButPath);
}


BEGIN_MESSAGE_MAP(CLORDPE, CDialogEx)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON1, &CLORDPE::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CLORDPE::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CLORDPE::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLORDPE 消息处理程序

//拖拽文件
void CLORDPE::OnDropFiles(HDROP hDropInfo)
{
	//获取文件路径
	TCHAR  szPatch[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPatch, MAX_PATH);
	m_path = szPatch;

	
	SetDlgItemTextW(IDC_STATICPATH, m_path);
	//解析
	if (!InitPE(m_path))
		MessageBox(0, L"不是PE文件", 0);
	else
		{
		mpe.buff = m_buff;
		mpe.pNt = GetNTHeaders();
		mpe.pSection = GetSectionHeader();
		mpe.pExport = GetExportDirectory();
		mpe.pDirectory = GetNTHeaders()->OptionalHeader.DataDirectory;
		mpe.pImport = GetImportDescriptor();
		mpe.pResource = GetResourceDirectory();
		mpe.pRelocal = GetRelocaltion();
		mpe.pTLS = GetTLSDirectory();
		mpe.pDelay = GetDelayLoadDirectory();
			ShowInfo();
			
			UpdateData(FALSE);
		}


	CDialogEx::OnDropFiles(hDropInfo);
}
bool CLORDPE::InitPE(TCHAR *path)
{
	m_path = path;
	HANDLE hFile = CreateFile(
		path,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	m_size = GetFileSize(hFile, 0);

	m_buff = new unsigned char[m_size];
	DWORD dwReadSize;

	ReadFile(hFile, m_buff, m_size, &dwReadSize, 0);
	return IsPE(m_buff);
}

bool CLORDPE::IsPE(unsigned char* pbuff)
{
	PIMAGE_DOS_HEADER pDos = GetDosHeader();
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return false;
	}
	PIMAGE_NT_HEADERS pNt = GetNTHeaders();
	if (pNt->Signature != IMAGE_NT_SIGNATURE)
	{
		return false;
	}
	return true;
}

PIMAGE_DOS_HEADER CLORDPE::GetDosHeader()
{
	return (PIMAGE_DOS_HEADER)m_buff;
}

PIMAGE_NT_HEADERS CLORDPE::GetNTHeaders()
{
	PIMAGE_DOS_HEADER pDos = GetDosHeader();
	PIMAGE_NT_HEADERS pNt =
		(PIMAGE_NT_HEADERS)(pDos->e_lfanew + (DWORD)m_buff);
	return pNt;
}
PIMAGE_SECTION_HEADER CLORDPE::GetSectionHeader()
{
	PIMAGE_NT_HEADERS pNt = GetNTHeaders();
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	return pSection;
}

PIMAGE_EXPORT_DIRECTORY CLORDPE::GetExportDirectory()
{
	PIMAGE_NT_HEADERS pNt = GetNTHeaders();
	DWORD ExportRva = pNt->OptionalHeader.DataDirectory[0].VirtualAddress;
	DWORD dwOffset = mpe.Rva2Foa(ExportRva);
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)
		(dwOffset + (DWORD)m_buff);
	return pExport;
}

PIMAGE_IMPORT_DESCRIPTOR CLORDPE::GetImportDescriptor()
{
	PIMAGE_NT_HEADERS pNt = GetNTHeaders();
	DWORD ImportRva = pNt->OptionalHeader.DataDirectory[1].VirtualAddress;
	DWORD dwOffset = mpe.Rva2Foa(ImportRva);
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)
		(dwOffset + (DWORD)m_buff);
	return pImport;
}



PIMAGE_RESOURCE_DIRECTORY CLORDPE::GetResourceDirectory()
{
	DWORD dwRva = GetNTHeaders()->OptionalHeader.DataDirectory[2].VirtualAddress;
	PIMAGE_RESOURCE_DIRECTORY pRes = (PIMAGE_RESOURCE_DIRECTORY)
		(mpe.Rva2Foa(dwRva) + (DWORD)m_buff);
	return pRes;
}

PIMAGE_BASE_RELOCATION CLORDPE::GetRelocaltion()
{
	DWORD dwRva = GetNTHeaders()->OptionalHeader.DataDirectory[5].VirtualAddress;
	if (dwRva == 0)
	{
		return 0;
	}
	PIMAGE_BASE_RELOCATION pRelocal = (PIMAGE_BASE_RELOCATION)
		(mpe.Rva2Foa(dwRva) + (DWORD)m_buff);
	return pRelocal;
}

PIMAGE_TLS_DIRECTORY CLORDPE::GetTLSDirectory()
{
	DWORD dwRva = GetNTHeaders()->OptionalHeader.DataDirectory[9].VirtualAddress;
	if (dwRva == 0)
	{
		return 0;
	}
	DWORD FOA = mpe.Rva2Foa(dwRva);
	PIMAGE_TLS_DIRECTORY pTLS = (PIMAGE_TLS_DIRECTORY)
		(mpe.Rva2Foa(dwRva) + (DWORD)m_buff);
	return pTLS;
}

PIMAGE_DELAYLOAD_DESCRIPTOR CLORDPE::GetDelayLoadDirectory()
{
	DWORD dwRva = GetNTHeaders()->OptionalHeader.DataDirectory[13].VirtualAddress;
	if (dwRva == 0)
	{
		return 0;
	}
	DWORD FOA = mpe.Rva2Foa(dwRva);
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = (PIMAGE_DELAYLOAD_DESCRIPTOR)
		(mpe.Rva2Foa(dwRva) + (DWORD)m_buff);
	return pDelay;
}

void CLORDPE::ShowInfo()
{
	PIMAGE_NT_HEADERS pNt = GetNTHeaders();
	//拓展头大小
	m_SizeOfOptionalHeader.Format(L"%08X", pNt->FileHeader.SizeOfOptionalHeader);
	//区段数目
	m_NumberOfSections.Format(L"%d", pNt->FileHeader.NumberOfSections);
	//显示时间戳
	time_t FileTime = (time_t)pNt->FileHeader.TimeDateStamp;
	struct tm tmFile;
	gmtime_s(&tmFile,&FileTime);
	m_TimeDateStamp.Format(L"%02d:%02d %d/%d/%d", tmFile.tm_hour+8,tmFile.tm_min,tmFile.tm_mon+1,
		tmFile.tm_mday, tmFile.tm_year+1900);
	//显示入口点地址
	m_AddressOfEntryPoint.Format(L"%08X", pNt->OptionalHeader.AddressOfEntryPoint);
	//显示加载基址
	m_ImageBase.Format(L"%08X", pNt->OptionalHeader.ImageBase);
	//显示子系统
	m_SubSystem.Format(L"%x", pNt->OptionalHeader.Subsystem);
	//显示镜像大小
	m_SizeOfImage.Format(L"%08X", pNt->OptionalHeader.SizeOfImage);
	//内存对齐
	m_SectionAlignment.Format(L"%x", pNt->OptionalHeader.SectionAlignment);
	//文件对齐
	m_FileAlignment.Format(L"%x", pNt->OptionalHeader.FileAlignment);
	//代码区段起始RVA
	m_BaseOfCode.Format(L"%08X", pNt->OptionalHeader.BaseOfCode);
	//数据区段起始RVA

	//m_BaseOfData.Format(L"%08X", pNt->OptionalHeader.BaseOfData);

	//m_BaseOfData.Format(L"%08X", pNt->OptionalHeader.BaseOfData);

//
	//首部大小
	m_SizeOfHeaders.Format(L"%08X", pNt->OptionalHeader.SizeOfHeaders);
	//特征值
	m_DllCharacteristics.Format(L"%08X", pNt->OptionalHeader.DllCharacteristics);
	//校验和
	m_CheckSum.Format(L"%08X", pNt->OptionalHeader.CheckSum);
	//标志字
	m_Magic.Format(L"%08X", pNt->OptionalHeader.Magic);
	//Rva数目
	m_NumberOfRvaAndSizes.Format(L"%X", pNt->OptionalHeader.NumberOfRvaAndSizes);






}

//Rva2Foa
void CLORDPE::OnBnClickedButton1()
{
	
	CRVATOFOA* rvatofoaDlg = new CRVATOFOA();
	rvatofoaDlg->Create(IDD_RVATOFOA);
	rvatofoaDlg->ShowWindow(SW_SHOW);
}

//Section
void CLORDPE::OnBnClickedButton3()
{
	CSECTION* secDlg=new CSECTION();
	secDlg->Create(IDD_SECTION);
	secDlg->ShowWindow(SW_SHOW);

}

//Directory
void CLORDPE::OnBnClickedButton2()
{
	

	CDIRECTORY* directDlg = new CDIRECTORY();
	directDlg->Create(IDD_DIRECTORY);
	directDlg->ShowWindow(SW_SHOW);

}
