// CRELOCATION.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CRELOCATION.h"
#include "afxdialogex.h"


// CRELOCATION 对话框

IMPLEMENT_DYNAMIC(CRELOCATION, CDialogEx)

CRELOCATION::CRELOCATION(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RELOCATION, pParent)
{

}

CRELOCATION::~CRELOCATION()
{
}

void CRELOCATION::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_LsCtlSection);
	DDX_Control(pDX, IDC_LIST1, m_LsCtlModule);
}


BEGIN_MESSAGE_MAP(CRELOCATION, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST3, &CRELOCATION::OnLvnItemchangedList3)
END_MESSAGE_MAP()


// CRELOCATION 消息处理程序


BOOL CRELOCATION::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(L"重定位表");
	m_LsCtlModule.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_LsCtlSection.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	CString strSection[] = { L"索引",L"区段",L"RVA",L"项目" };
	CString strModule[] = { L"索引",L"RVA",L"FOA",L"类型",L"FAR地址",L"数据解释" };
	for (int i = 0; i < sizeof(strSection) / sizeof(strSection[0]); ++i)
	{
		m_LsCtlSection.InsertColumn(i, strSection[i], 0, 100);
	}
	for (int i = 0; i < sizeof(strModule) / sizeof(strModule[0]) - 1; ++i)
	{
		m_LsCtlModule.InsertColumn(i, strModule[i], 0, 80);
	}
	m_LsCtlModule.InsertColumn(5, strModule[5], 0, 200);

	PIMAGE_BASE_RELOCATION pRelocaltion = mpe.pRelocal;
	CString str;
	int i = 0;
	while (pRelocaltion->SizeOfBlock)
	{
		str.Format(L"%d", i + 1);
		m_LsCtlSection.InsertItem(i, str);
		m_LsCtlSection.SetItemText(i, 1, mpe.GetSection(pRelocaltion->VirtualAddress));
		str.Format(L"%08X", pRelocaltion->VirtualAddress);
		m_LsCtlSection.SetItemText(i, 2, str);
		DWORD numOfModule = (pRelocaltion->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
		str.Format(L"%02Xh / %dd", numOfModule, numOfModule);
		m_LsCtlSection.SetItemText(i, 3, str);

		pRelocaltion = (PIMAGE_BASE_RELOCATION)
			((long)pRelocaltion + pRelocaltion->SizeOfBlock);
		++i;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void GetArryImp(DWORD* arryThunk, int numOfImp)
{
	PIMAGE_IMPORT_DESCRIPTOR ptempImport = mpe.pImport;
	//获取IAT的地址
	PIMAGE_THUNK_DATA pIAT = (PIMAGE_THUNK_DATA)
		(mpe.Rva2Foa(ptempImport->FirstThunk) + (DWORD)mpe.buff);
	PIMAGE_THUNK_DATA m_pRVAThunk = (PIMAGE_THUNK_DATA)
		(ptempImport->FirstThunk);
	for (int i = 0; i < numOfImp; i += 2)
	{
		
		arryThunk[i] = ptempImport->FirstThunk;
		m_pRVAThunk = (PIMAGE_THUNK_DATA)
			(arryThunk[i]);
		//遍历IAT中的函数
		pIAT = (PIMAGE_THUNK_DATA)
			(mpe.Rva2Foa(arryThunk[i]) + (DWORD)mpe.buff);
		while (pIAT->u1.Ordinal)
		{
			m_pRVAThunk++;

			pIAT++;
		}
		arryThunk[i + 1] = (DWORD)(m_pRVAThunk-1);
		ptempImport++;
	}
}

void CRELOCATION::OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_LsCtlModule.DeleteAllItems();
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int nIdx = (int)m_LsCtlSection.GetFirstSelectedItemPosition();
	if (nIdx == 0)
	{
		*pResult = 0;
		return;
	}
	//定义TYPEOFFSET结构体
	typedef struct
	{
		WORD Offset : 12;
		WORD Type : 4;
	}*PTYPEOFFSET;
	PIMAGE_BASE_RELOCATION pRelocaltion = mpe.pRelocal;
	CString str;
	for (int i = 0; i < nIdx - 1; ++i)
	{
		pRelocaltion = (PIMAGE_BASE_RELOCATION)
			((long)pRelocaltion + pRelocaltion->SizeOfBlock);
	}
	PTYPEOFFSET pRelocalBlock = (PTYPEOFFSET)(pRelocaltion + 1);
	DWORD numOfModule = (pRelocaltion->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
	PIMAGE_IMPORT_DESCRIPTOR ptempImport = mpe.pImport;
	int numOfImp = 0;
	while (ptempImport->Name)
	{
		numOfImp++;
		ptempImport++;
	}
	ptempImport = mpe.pImport;
	DWORD* arry = new DWORD[numOfImp * 2];
	GetArryImp(arry, numOfImp * 2);

	for (int i = 0; i < numOfModule; ++i)
	{
		str.Format(L"%d", i + 1);
		m_LsCtlModule.InsertItem(i, str);
		// （x32位PE文件）类型为0x03	表示需要重定位的数据  
			// （x64位PE文件）类型为0x10	表示需要重定位的数据  
		if (pRelocalBlock[i].Type == 3)
		{
			DWORD BlockRva = pRelocalBlock[i].Offset + pRelocaltion->VirtualAddress;
			str.Format(L"%08X", BlockRva);
			m_LsCtlModule.SetItemText(i, 1, str);
			str.Format(L"%08X", mpe.Rva2Foa(BlockRva));
			m_LsCtlModule.SetItemText(i, 2, str);
			str.Format(L"%d", pRelocalBlock[i].Type);
			m_LsCtlModule.SetItemText(i, 3, str);
			DWORD VA = *(PDWORD)(mpe.Rva2Foa(BlockRva) + (long)mpe.buff);
			str.Format(L"%08X", VA);
			DWORD offset = mpe.Rva2Foa(VA - mpe.pNt->OptionalHeader.ImageBase) + (long)mpe.buff;
			m_LsCtlModule.SetItemText(i, 4, str);

			CString str2;
			BYTE tmpASCII = *(PBYTE)offset;
			for (int j=0;j<10;j++)
			{
				if (tmpASCII>=32&&tmpASCII<=126)
				{
					str.Format(L"%c", *(PBYTE)(offset+j));
				}
				else str.Format(L"%02X ", *(PBYTE)(offset + j));
				str2 += str;
			}
			
			for (int j = 0; j < numOfImp * 2; j+=2)
			{
				if (VA - mpe.pNt->OptionalHeader.ImageBase >= arry[j] &&
					VA - mpe.pNt->OptionalHeader.ImageBase < arry[j + 1])
				{
					char* pImportName = (char*)
						(mpe.Rva2Foa(ptempImport->Name) + (DWORD)mpe.buff);

					str2.Format(L"%s的IAT Thunk值", (CString)pImportName);

				}
				ptempImport++;
			}
			ptempImport = mpe.pImport;
			m_LsCtlModule.SetItemText(i, 5, str2);
		}
	}
	*pResult = 0;
}
