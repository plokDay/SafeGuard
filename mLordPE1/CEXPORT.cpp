// CEXPORT.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CEXPORT.h"
#include "afxdialogex.h"


// CEXPORT 对话框

IMPLEMENT_DYNAMIC(CEXPORT, CDialogEx)

CEXPORT::CEXPORT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXPORT, pParent)
	, m_name(_T(""))
	, m_Base(_T(""))
	, m_Foa(_T(""))
	, m_AddressOfFunctions(_T(""))
	, m_NumberOfFunctions(_T(""))
	, m_AddressOfOridinals(_T(""))
	, m_NumberOfNames(_T(""))
	, m_AddressOfNames(_T(""))
{

}

CEXPORT::~CEXPORT()
{
}

void CEXPORT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT17, m_Base);
	DDX_Text(pDX, IDC_EDIT3, m_Foa);
	DDX_Text(pDX, IDC_EDIT45, m_AddressOfFunctions);
	DDX_Text(pDX, IDC_EDIT18, m_NumberOfFunctions);
	DDX_Text(pDX, IDC_EDIT46, m_AddressOfOridinals);
	DDX_Text(pDX, IDC_EDIT44, m_NumberOfNames);
	DDX_Text(pDX, IDC_EDIT47, m_AddressOfNames);
	DDX_Control(pDX, IDC_LIST2, m_lsCtl);
}


BEGIN_MESSAGE_MAP(CEXPORT, CDialogEx)
END_MESSAGE_MAP()


// CEXPORT 消息处理程序


BOOL CEXPORT::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(L"导出表");
	char* pDllName = (char*)(mpe.Rva2Foa(mpe.pExport->Name) + (DWORD)mpe.buff);
	//char* ==> Cstring
	m_name = (CString)pDllName;

	m_Foa.Format(L"%08X", mpe.Rva2Foa(mpe.pNt->OptionalHeader.DataDirectory[0].VirtualAddress) + (DWORD)mpe.buff);
	
	m_NumberOfFunctions.Format(L"%d", mpe.pExport->NumberOfFunctions);
	m_NumberOfNames.Format(L"%d", mpe.pExport->NumberOfNames);
	m_Base.Format(L"%d", mpe.pExport->Base);
	m_AddressOfFunctions.Format(L"%08X", mpe.pExport->AddressOfFunctions);
	m_AddressOfNames.Format(L"%08X", mpe.pExport->AddressOfNames);
	m_AddressOfOridinals.Format(L"%08X", mpe.pExport->AddressOfNameOrdinals);

	UpdateData(FALSE);
	DWORD* EAToffset = (DWORD*)(mpe.Rva2Foa(mpe.pExport->AddressOfFunctions) + (DWORD)mpe.buff);
	WORD* EOToffset = (WORD*)(mpe.Rva2Foa(mpe.pExport->AddressOfNameOrdinals) + (DWORD)mpe.buff);
	DWORD* ENToffset = (DWORD*)(mpe.Rva2Foa(mpe.pExport->AddressOfNames) + (DWORD)mpe.buff);


	m_lsCtl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lsCtl.InsertColumn(0, L"序号", 0, 100);
	m_lsCtl.InsertColumn(1, L"RVA", 0, 100);
	m_lsCtl.InsertColumn(2, L"FOA", 0, 100);
	m_lsCtl.InsertColumn(3, L"函数名", 0, 100);
	//遍历地址表
	for (int i = 0; i < mpe.pExport->NumberOfFunctions; ++i)
	{
		CString str;
		str.Format(L"%d", i + mpe.pExport->Base);
		m_lsCtl.InsertItem(i, str);
		str.Format(L"%08X", EAToffset[i]);
		m_lsCtl.SetItemText(i, 1, str);
		str.Format(L"%08X", mpe.Rva2Foa(EAToffset[i]));
		m_lsCtl.SetItemText(i, 2, str);
		//遍历名称表
		for (int j = 0; j < mpe.pExport->NumberOfNames; ++j)
		{
			//地址表的序号==序号表的内容，说明这个函数有名字
			if (i == EOToffset[j])
			{
				char* pFunName = (char*)
					(mpe.Rva2Foa(ENToffset[j]) + (DWORD)mpe.buff);
				//printf("%s", pFunName);
				m_lsCtl.SetItemText(i, 3, (CString)pFunName);
				break;
			}
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
