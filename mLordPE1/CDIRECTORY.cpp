// CDIRECTORY.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CDIRECTORY.h"
#include "afxdialogex.h"
#include "CEXPORT.h"
#include "CIMPORT.h"
#include "CRESOURCE.h"
#include "CRELOCATION.h"
#include "CTLS.h"
#include "CDELAY.h"

// CDIRECTORY 对话框

IMPLEMENT_DYNAMIC(CDIRECTORY, CDialogEx)

CDIRECTORY::CDIRECTORY(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRECTORY, pParent)
	, m_ExportRVA(_T(""))
	, m_ExportSize(_T(""))
	, m_ImportRVA(_T(""))
	, m_ImportSize(_T(""))
	, m_ResourceRVA(_T(""))
	, m_ResourceSize(_T(""))
	, m_ExceptRVA(_T(""))
	, m_ExceptSize(_T(""))
	, m_SecureRVA(_T(""))
	, m_SecureSize(_T(""))
	, m_RelocRVA(_T(""))
	, m_RelocSize(_T(""))
	, m_DebugRVA(_T(""))
	, m_DebugSize(_T(""))
	, m_CopyRightRVA(_T(""))
	, m_CopyRightSize(_T(""))
	, m_GlobalRVA(_T(""))
	, m_GlobalSize(_T(""))
	, m_TLSRVA(_T(""))
	, m_TLSSize(_T(""))
	, m_LoadRVA(_T(""))
	, m_LoadSize(_T(""))
	, m_BoundRVA(_T(""))
	, m_BoundSize(_T(""))
	, m_ITARVA(_T(""))
	, m_ITASize(_T(""))
	, m_DelayRVA(_T(""))
	, m_DelaySize(_T(""))
	, m_COMRVA(_T(""))
	, m_ReserveRVA(_T(""))
	, m_ReserveSize(_T(""))
	, m_COMSIZE(_T(""))
{
	
}

CDIRECTORY::~CDIRECTORY()
{
}

void CDIRECTORY::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ExportRVA);
	DDX_Text(pDX, IDC_EDIT18, m_ExportSize);
	DDX_Text(pDX, IDC_EDIT2, m_ImportRVA);
	DDX_Text(pDX, IDC_EDIT19, m_ImportSize);
	DDX_Text(pDX, IDC_EDIT8, m_ResourceRVA);
	DDX_Text(pDX, IDC_EDIT20, m_ResourceSize);
	DDX_Text(pDX, IDC_EDIT21, m_ExceptRVA);
	DDX_Text(pDX, IDC_EDIT22, m_ExceptSize);
	DDX_Text(pDX, IDC_EDIT10, m_SecureRVA);
	DDX_Text(pDX, IDC_EDIT23, m_SecureSize);
	DDX_Text(pDX, IDC_EDIT12, m_RelocRVA);
	DDX_Text(pDX, IDC_EDIT24, m_RelocSize);
	DDX_Text(pDX, IDC_EDIT14, m_DebugRVA);
	DDX_Text(pDX, IDC_EDIT25, m_DebugSize);
	DDX_Text(pDX, IDC_EDIT26, m_CopyRightRVA);
	DDX_Text(pDX, IDC_EDIT27, m_CopyRightSize);
	DDX_Text(pDX, IDC_EDIT28, m_GlobalRVA);
	DDX_Text(pDX, IDC_EDIT29, m_GlobalSize);
	DDX_Text(pDX, IDC_EDIT30, m_TLSRVA);
	DDX_Text(pDX, IDC_EDIT31, m_TLSSize);
	DDX_Text(pDX, IDC_EDIT32, m_LoadRVA);
	DDX_Text(pDX, IDC_EDIT33, m_LoadSize);
	DDX_Text(pDX, IDC_EDIT34, m_BoundRVA);
	DDX_Text(pDX, IDC_EDIT35, m_BoundSize);
	DDX_Text(pDX, IDC_EDIT36, m_ITARVA);
	DDX_Text(pDX, IDC_EDIT37, m_ITASize);
	DDX_Text(pDX, IDC_EDIT38, m_DelayRVA);
	DDX_Text(pDX, IDC_EDIT39, m_DelaySize);
	DDX_Text(pDX, IDC_EDIT40, m_COMRVA);
	DDX_Control(pDX, IDC_EDIT41, m_COMSize);
	DDX_Text(pDX, IDC_EDIT42, m_ReserveRVA);
	DDX_Text(pDX, IDC_EDIT43, m_ReserveSize);
	DDX_Text(pDX, IDC_EDIT41, m_COMSIZE);
}


BEGIN_MESSAGE_MAP(CDIRECTORY, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDIRECTORY::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDIRECTORY::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDIRECTORY::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDIRECTORY::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDIRECTORY::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDIRECTORY::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDIRECTORY 消息处理程序


BOOL CDIRECTORY::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetWindowText(L"数据目录表");
	m_ExportRVA.Format(L"%08X",mpe.pDirectory[0].VirtualAddress);
	m_ExportSize.Format(L"%08X", mpe.pDirectory[0].Size);
	m_ImportRVA.Format(L"%08X", mpe.pDirectory[1].VirtualAddress);
	m_ImportSize.Format(L"%08X", mpe.pDirectory[1].Size);
	m_ResourceRVA.Format(L"%08X", mpe.pDirectory[2].VirtualAddress);
	m_ResourceSize.Format(L"%08X", mpe.pDirectory[2].Size);
	m_ExceptRVA.Format(L"%08X", mpe.pDirectory[3].VirtualAddress);
	m_ExceptSize.Format(L"%08X", mpe.pDirectory[3].Size);
	m_SecureRVA.Format(L"%08X", mpe.pDirectory[4].VirtualAddress);
	m_SecureSize.Format(L"%08X", mpe.pDirectory[4].Size);
	m_RelocRVA.Format(L"%08X", mpe.pDirectory[5].VirtualAddress);
	m_RelocSize.Format(L"%08X", mpe.pDirectory[5].Size);
	m_DebugRVA.Format(L"%08X", mpe.pDirectory[6].VirtualAddress);
	m_DebugSize.Format(L"%08X", mpe.pDirectory[6].Size);
	m_CopyRightRVA.Format(L"%08X", mpe.pDirectory[7].VirtualAddress);
	m_CopyRightSize.Format(L"%08X", mpe.pDirectory[7].Size);
	m_GlobalRVA.Format(L"%08X", mpe.pDirectory[8].VirtualAddress);
	m_GlobalSize.Format(L"%08X", mpe.pDirectory[8].Size);
	m_TLSRVA.Format(L"%08X", mpe.pDirectory[9].VirtualAddress);
	m_TLSSize.Format(L"%08X", mpe.pDirectory[9].Size);
	m_LoadRVA.Format(L"%08X", mpe.pDirectory[10].VirtualAddress);
	m_LoadSize.Format(L"%08X", mpe.pDirectory[10].Size);
	m_BoundRVA.Format(L"%08X", mpe.pDirectory[11].VirtualAddress);
	m_BoundSize.Format(L"%08X", mpe.pDirectory[11].Size);
	m_ITARVA.Format(L"%08X", mpe.pDirectory[12].VirtualAddress);
	m_ITASize.Format(L"%08X", mpe.pDirectory[12].Size);
	m_DelayRVA.Format(L"%08X", mpe.pDirectory[13].VirtualAddress);
	m_DelaySize.Format(L"%08X", mpe.pDirectory[13].Size);
	m_COMRVA.Format(L"%08X", mpe.pDirectory[14].VirtualAddress);
	m_COMSIZE.Format(L"%08X", mpe.pDirectory[14].Size);
	m_ReserveRVA.Format(L"%08X", mpe.pDirectory[15].VirtualAddress);
	m_ReserveSize.Format(L"%08X", mpe.pDirectory[15].Size);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//显示导出表信息
void CDIRECTORY::OnBnClickedButton1()
{
	if (mpe.pNt->OptionalHeader.DataDirectory[0].Size==0)
	{
		MessageBox(L"当前PE没有导出数据",0, 0);
		return;
	}
	CEXPORT* exportDlg = new CEXPORT();
	exportDlg->Create(IDD_EXPORT);
	exportDlg->ShowWindow(SW_SHOW);
}

//导入表信息
void CDIRECTORY::OnBnClickedButton2()
{
	CIMPORT* importDlg = new CIMPORT;
	importDlg->Create(IDD_IMPORT);
	importDlg->ShowWindow(SW_SHOW);
}

//资源表
void CDIRECTORY::OnBnClickedButton3()
{
	CRESOURCE* pResDlg=new CRESOURCE();
	pResDlg->Create(IDD_RESOURCE);
	pResDlg->ShowWindow(SW_SHOW);
}

//重定位表
void CDIRECTORY::OnBnClickedButton4()
{
	if (mpe.pRelocal==0x0)
	{
		MessageBox(L"当前PE没有重定位信息", 0, 0);
		return;
	}
	CRELOCATION* pRelocaDlg = new CRELOCATION();
	pRelocaDlg->Create(IDD_RELOCATION);
	pRelocaDlg->ShowWindow(SW_SHOW);
}


void CDIRECTORY::OnBnClickedButton5()
{
	if (mpe.pTLS == 0x0)
	{
		MessageBox(L"当前PE没有TLS信息", 0, 0);
		return;
	}
	CTLS* pTLS  = new CTLS();
	pTLS->Create(IDD_TLS);
	pTLS->ShowWindow(SW_SHOW);
}


void CDIRECTORY::OnBnClickedButton6()
{
	if (mpe.pDelay == 0x0)
	{
		MessageBox(L"当前PE没有延迟加载信息", 0, 0);
		return;
	}
	CDELAY* pDelDlg = new CDELAY();
	pDelDlg->Create(IDD_DELAY);
	pDelDlg->ShowWindow(SW_SHOW);
}
