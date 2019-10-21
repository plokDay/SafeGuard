// CSECTION.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CSECTION.h"
#include "afxdialogex.h"


// CSECTION 对话框

IMPLEMENT_DYNAMIC(CSECTION, CDialogEx)

CSECTION::CSECTION(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SECTION, pParent)
{
	
}

CSECTION::~CSECTION()
{
}

void CSECTION::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lsCtr);
}


BEGIN_MESSAGE_MAP(CSECTION, CDialogEx)
END_MESSAGE_MAP()


// CSECTION 消息处理程序


BOOL CSECTION::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(L"区段");

	m_lsCtr.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lsCtr.InsertColumn(0, L"名称", 0, 100);
	m_lsCtr.InsertColumn(1, L"vOffSet",0, 100);
	m_lsCtr.InsertColumn(2, L"vSize",0, 100);
	m_lsCtr.InsertColumn(4, L"rOffSet",0, 100);
	m_lsCtr.InsertColumn(5, L"rSize", 0, 100);
	m_lsCtr.InsertColumn(6, L"标志", 0, 100);

	//
	DWORD secCount = mpe.pNt->FileHeader.NumberOfSections;
	for (int i = 0; i < secCount; ++i)
	{
		//Name[IMAGE_SIZEOF_SHORT_NAME]不是\0结尾，所以不能char*打印
		char cName[IMAGE_SIZEOF_SHORT_NAME + 1] = { 0 };
		memcpy(cName, mpe.pSection[i].Name, IMAGE_SIZEOF_SHORT_NAME);
		m_lsCtr.InsertItem(i, (CString)cName);
		CString str;
		str.Format(L"%08X", mpe.pSection[i].VirtualAddress);
		m_lsCtr.SetItemText(i, 1, str);
		str.Format(L"%08X", mpe.pSection[i].Misc.VirtualSize);
		m_lsCtr.SetItemText(i, 2, str);
		str.Format(L"%08X", mpe.pSection[i].PointerToRawData);
		m_lsCtr.SetItemText(i, 3, str);
		str.Format(L"%08X", mpe.pSection[i].SizeOfRawData);
		m_lsCtr.SetItemText(i, 4, str);
		str.Format(L"%08X", mpe.pSection[i].Characteristics);
		m_lsCtr.SetItemText(i, 5, str);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
