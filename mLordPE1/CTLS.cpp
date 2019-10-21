// CTLS.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CTLS.h"
#include "afxdialogex.h"


// CTLS 对话框

IMPLEMENT_DYNAMIC(CTLS, CDialogEx)

CTLS::CTLS(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TLS, pParent)
	, m_StartAddressOfRawData(_T(""))
	, m_EndAddressOfRawData(_T(""))
	, m_AddressOfIndex(_T(""))
	, m_AddressOfCallBacks(_T(""))
	, m_SizeOfZeroFill(_T(""))
	, m_Characteristics(_T(""))
{

}

CTLS::~CTLS()
{
}

void CTLS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StartAddressOfRawData);
	DDX_Text(pDX, IDC_EDIT17, m_EndAddressOfRawData);
	DDX_Text(pDX, IDC_EDIT18, m_AddressOfIndex);
	DDX_Text(pDX, IDC_EDIT44, m_AddressOfCallBacks);
	DDX_Text(pDX, IDC_EDIT3, m_SizeOfZeroFill);
	DDX_Text(pDX, IDC_EDIT45, m_Characteristics);
}


BEGIN_MESSAGE_MAP(CTLS, CDialogEx)
END_MESSAGE_MAP()


// CTLS 消息处理程序


BOOL CTLS::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(L"TLS表");

	PIMAGE_TLS_DIRECTORY pTLS = mpe.pTLS;
	m_StartAddressOfRawData.Format(L"%p", pTLS->StartAddressOfRawData);
	m_EndAddressOfRawData.Format(L"%p", pTLS->EndAddressOfRawData);
	m_AddressOfIndex.Format(L"%08X", pTLS->AddressOfIndex);
	m_AddressOfCallBacks.Format(L"%p", pTLS->AddressOfCallBacks);
	m_SizeOfZeroFill.Format(L"%08X", pTLS->SizeOfZeroFill);
	m_Characteristics.Format(L"%08X", pTLS->Characteristics);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
