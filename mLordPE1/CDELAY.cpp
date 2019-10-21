// CDELAY.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CDELAY.h"
#include "afxdialogex.h"


// CDELAY 对话框

IMPLEMENT_DYNAMIC(CDELAY, CDialogEx)

CDELAY::CDELAY(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DELAY, pParent)
{

}

CDELAY::~CDELAY()
{
}

void CDELAY::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_lsCtlDll);
	DDX_Control(pDX, IDC_LIST1, m_lsCtlFunc);
}


BEGIN_MESSAGE_MAP(CDELAY, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CDELAY::OnNMClickList3)
END_MESSAGE_MAP()


// CDELAY 消息处理程序


BOOL CDELAY::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(L"延迟加载表");
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = mpe.pDelay;
	m_lsCtlDll.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lsCtlFunc.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strDll[] = { L"Dll名称",L"INT(RVA)",L"IAT(RVA)"};
	CString strFunc[] = { L"序号",L"名字" };
	for (int i = 0; i < sizeof(strDll)/sizeof(strDll[0]); ++i)
	{
		m_lsCtlDll.InsertColumn(i, strDll[i], 0, 100);
	}
	for (int i = 0; i < sizeof(strFunc) / sizeof(strFunc[0]); ++i)
	{
		m_lsCtlFunc.InsertColumn(i, strFunc[i], 0, 100);
	}
	CString str;
	int i = 0;

	while (pDelay->DllNameRVA)
	{
		CHAR* DllName =(CHAR*) (mpe.Rva2Foa(pDelay->DllNameRVA) + (DWORD)mpe.buff);
		str.Format(L"%s", (CString)(DllName));
		m_lsCtlDll.InsertItem(i,str);
		str.Format(L"%08X", pDelay->ImportAddressTableRVA);
		m_lsCtlDll.SetItemText(i, 1, str);
		str.Format(L"%08X", pDelay->ImportNameTableRVA);
		m_lsCtlDll.SetItemText(i, 2, str);
		i++;
		pDelay++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDELAY::OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	m_lsCtlFunc.DeleteAllItems();
	CString str;
	int m_nIdx = (int)m_lsCtlDll.GetFirstSelectedItemPosition();
	if (m_nIdx == 0) return;
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = mpe.pDelay;
	pDelay += m_nIdx - 1;
	//为什么用IAT找不到？？
	PIMAGE_THUNK_DATA  m_pDelayThunk= PIMAGE_THUNK_DATA
	(mpe.Rva2Foa(pDelay->ImportNameTableRVA)+(DWORD)mpe.buff);
	//循环查找
	int i = 0;
	while (m_pDelayThunk->u1.AddressOfData)
	{
		//序号命名
		if (IMAGE_SNAP_BY_ORDINAL32(m_pDelayThunk->u1.Ordinal))
		{
			str.Format(L"%X", IMAGE_ORDINAL32(m_pDelayThunk->u1.Ordinal));
			m_lsCtlFunc.InsertItem(i,str);
			m_lsCtlFunc.SetItemText(i, 1, L"--");
		}
		//名称命名
		else
		{
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(mpe.buff +
				mpe.Rva2Foa(m_pDelayThunk->u1.AddressOfData));
			str.Format(L"%08X", pName->Hint);
			m_lsCtlFunc.InsertItem(i, str);
			str.Format(L"%s", (CString)(CHAR*)pName->Name);
			m_lsCtlFunc.SetItemText(i,1, str);
		}
		
		m_pDelayThunk++;
		i++;
	}
	*pResult = 0;
}
