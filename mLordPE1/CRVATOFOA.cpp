// CRVATOFOA.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CRVATOFOA.h"
#include "afxdialogex.h"


// CRVATOFOA 对话框

IMPLEMENT_DYNAMIC(CRVATOFOA, CDialogEx)

CRVATOFOA::CRVATOFOA(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RVATOFOA, pParent)
	, m_VA(_T(""))
	, m_RVA(_T(""))
	, m_FOA(_T(""))
	, m_Section(_T(""))
{
}

CRVATOFOA::~CRVATOFOA()
{
}

void CRVATOFOA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_VA);
	DDX_Text(pDX, IDC_EDIT17, m_RVA);
	DDX_Text(pDX, IDC_EDIT18, m_FOA);
	DDX_Text(pDX, IDC_EDIT2, m_Section);
}


BEGIN_MESSAGE_MAP(CRVATOFOA, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRVATOFOA::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRVATOFOA 消息处理程序


void CRVATOFOA::OnBnClickedButton1()
{
	//控件-->变量
	UpdateData(TRUE);
	
	//将变量转化成16进制
	DWORD dwRVA = wcstol(m_RVA, NULL, 16);

	DWORD dwVA = mpe.pNt->OptionalHeader.ImageBase+dwRVA;
	m_VA.Format(L"%08X", dwVA);
	m_RVA.Format(L"%08X", dwRVA);
	m_Section = mpe.GetSection(dwRVA);
	DWORD dwFOA = mpe.Rva2Foa(dwRVA);
	if (dwRVA==-1)
	{
		MessageBox(0, L"地址错误", 0);
		return;
	}
	m_FOA.Format(L"%08X", dwFOA);
	UpdateData(FALSE);
}



BOOL CRVATOFOA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowTextW(L"位置计算");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
