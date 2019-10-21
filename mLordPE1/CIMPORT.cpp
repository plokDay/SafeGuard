// CIMPORT.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CIMPORT.h"
#include "afxdialogex.h"


// CIMPORT 对话框

IMPLEMENT_DYNAMIC(CIMPORT, CDialogEx)

CIMPORT::CIMPORT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMPORT, pParent)
{

}

CIMPORT::~CIMPORT()
{
}

void CIMPORT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_lsCtlImport);
	DDX_Control(pDX, IDC_LIST1, m_lsCtlFunc);
	DDX_Control(pDX, IDC_CHECK1, m_Check);
}


BEGIN_MESSAGE_MAP(CIMPORT, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_LIST3, &CIMPORT::OnClickShowFunc)
	ON_BN_CLICKED(IDC_CHECK1, &CIMPORT::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CIMPORT 消息处理程序


BOOL CIMPORT::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(L"导入表");
	m_lsCtlImport.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_lsCtlFunc.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	CString strImport[] = { L"Dll名称",L"OriginFirstThunk",L"FirstThunk",L"时间戳",L"ForwarderChain" };
	CString strFunc[] = { L"ThunkRVA",L"ThunkFOA",L"Thunk值",L"序号",L"API名称" };
	for (int i=0;i<5;++i)
	{
		m_lsCtlImport.InsertColumn(i, strImport[i], 0, 120);
	}
	for (int i = 0; i < 4; ++i)
	{
		m_lsCtlFunc.InsertColumn(i, strFunc[i], 0, 80);
	}
	m_lsCtlFunc.InsertColumn(4, strFunc[4], 0, 200);
	int i = 0;
	PIMAGE_IMPORT_DESCRIPTOR tmp = mpe.pImport;
	while (mpe.pImport->Name)
	{
		char* pImportName = (char*)
			(mpe.Rva2Foa(mpe.pImport->Name) + (DWORD)mpe.buff);
		m_lsCtlImport.InsertItem(i, (CString)pImportName);
		CString str;
		str.Format(L"%08X", mpe.pImport->OriginalFirstThunk);
		m_lsCtlImport.SetItemText(i, 1, str);
		str.Format(L"%08X", mpe.pImport->FirstThunk);
		m_lsCtlImport.SetItemText(i,2,str);
		str.Format(L"%08X", mpe.pImport->TimeDateStamp);
		m_lsCtlImport.SetItemText(i, 3, str);
		str.Format(L"%08X", mpe.pImport->ForwarderChain);
		m_lsCtlImport.SetItemText(i, 4, str);
		i++;
		mpe.pImport++;
	}
	
	mpe.pImport = tmp;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CIMPORT::OnClickShowFunc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	PIMAGE_IMPORT_DESCRIPTOR tmp = mpe.pImport;
	
	m_nIdx = (int)m_lsCtlImport.GetFirstSelectedItemPosition();
	if (m_nIdx == 0) return;
	mpe.pImport += m_nIdx - 1;


	if (m_Check.GetCheck())
	{
		//获取INT地址
		m_pFOAThunk = (PIMAGE_THUNK_DATA)
			mpe.Rva2Foa(mpe.pImport->OriginalFirstThunk);
		m_pThunk = (PIMAGE_THUNK_DATA)
			(mpe.Rva2Foa(mpe.pImport->OriginalFirstThunk) + (DWORD)mpe.buff);
		m_pRVAThunk = (PIMAGE_THUNK_DATA)
			(mpe.pImport->OriginalFirstThunk);
	}
	else
	{
		//获取IAT的地址
		m_pFOAThunk = (PIMAGE_THUNK_DATA)
			mpe.Rva2Foa(mpe.pImport->FirstThunk);
		m_pThunk = (PIMAGE_THUNK_DATA)
			(mpe.Rva2Foa(mpe.pImport->FirstThunk) + (DWORD)mpe.buff);
		m_pRVAThunk = (PIMAGE_THUNK_DATA)
			(mpe.pImport->FirstThunk);
	}
	m_lsCtlFunc.DeleteAllItems();
	int j = 0;
	//遍历IAT中的函数
	while (m_pThunk->u1.Ordinal)
	{
		CString str;
		str.Format(L"%08X", m_pRVAThunk++);
		m_lsCtlFunc.InsertItem(j, str);
		str.Format(L"%08X", m_pFOAThunk++);
		m_lsCtlFunc.SetItemText(j, 1, str);
		//判断是名称导入还是仅序号导入
		if (m_pThunk->u1.Ordinal & 0x80000000)//最高位是1，仅序号导入
		{
			str.Format(L"%08X", m_pThunk->u1.Function & 0x7FFFFFFFF);
			m_lsCtlFunc.SetItemText(j, 3,str);
			//printf("序号:%d\n", pIAT->u1.Function & 0x7FFFFFFFF);
		}
		else//最高位0，名称导入，前三个值都无效
		{
			//找到PIMAGE_IMPORT_BY_NAME结构体的地址
			str.Format(L"%08X", m_pThunk->u1.AddressOfData);
			m_lsCtlFunc.SetItemText(j, 2, str);
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)
				(mpe.Rva2Foa(m_pThunk->u1.AddressOfData) + (DWORD)mpe.buff);
			//printf("序号:%08d\t函数名:%s\n", pName->Hint, pName->Name);
			str.Format(L"%04X", pName->Hint);
			m_lsCtlFunc.SetItemText(j, 3, str);
			m_lsCtlFunc.SetItemText(j, 4, (CString)pName->Name);

		}
		m_pThunk++;
		j++;
	}
	mpe.pImport = tmp;
	*pResult = 0;
}


void CIMPORT::OnBnClickedCheck1()
{
	if (m_nIdx == -1) return;
	PIMAGE_IMPORT_DESCRIPTOR tmp = mpe.pImport;
	mpe.pImport += m_nIdx - 1;
	
	if (m_Check.GetCheck())
	{
		//获取INT地址
		m_pFOAThunk = (PIMAGE_THUNK_DATA)
			mpe.Rva2Foa(mpe.pImport->OriginalFirstThunk);
		m_pThunk = (PIMAGE_THUNK_DATA)
			(mpe.Rva2Foa(mpe.pImport->OriginalFirstThunk) + (DWORD)mpe.buff);
		m_pRVAThunk = (PIMAGE_THUNK_DATA)
			(mpe.pImport->OriginalFirstThunk);
	}
	else
	{
		//获取IAT的地址
		m_pFOAThunk = (PIMAGE_THUNK_DATA)
			mpe.Rva2Foa(mpe.pImport->FirstThunk);
		m_pThunk = (PIMAGE_THUNK_DATA)
			(mpe.Rva2Foa(mpe.pImport->FirstThunk) + (DWORD)mpe.buff);
		m_pRVAThunk = (PIMAGE_THUNK_DATA)
			(mpe.pImport->FirstThunk);
	}
	m_lsCtlFunc.DeleteAllItems();
	int j = 0;
	//遍历IAT中的函数
	while (m_pThunk->u1.Ordinal)
	{
		CString str;
		str.Format(L"%08X", m_pRVAThunk++);
		m_lsCtlFunc.InsertItem(j, str);
		str.Format(L"%08X", m_pFOAThunk++);
		m_lsCtlFunc.SetItemText(j, 1, str);
		//判断是名称导入还是仅序号导入
		if (m_pThunk->u1.Ordinal & 0x80000000)//最高位是1，仅序号导入
		{
			str.Format(L"%08X", m_pThunk->u1.Function & 0x7FFFFFFFF);
			m_lsCtlFunc.SetItemText(j, 3, str);
			//printf("序号:%d\n", pIAT->u1.Function & 0x7FFFFFFFF);
		}
		else//最高位0，名称导入，前三个值都无效
		{
			//找到PIMAGE_IMPORT_BY_NAME结构体的地址
			str.Format(L"%08X", m_pThunk->u1.AddressOfData);
			m_lsCtlFunc.SetItemText(j, 2, str);
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)
				(mpe.Rva2Foa(m_pThunk->u1.AddressOfData) + (DWORD)mpe.buff);
			//printf("序号:%08d\t函数名:%s\n", pName->Hint, pName->Name);
			str.Format(L"%04X", pName->Hint);
			m_lsCtlFunc.SetItemText(j, 3, str);
			m_lsCtlFunc.SetItemText(j, 4, (CString)pName->Name);

		}
		m_pThunk++;
		j++;
	}
	mpe.pImport = tmp;
}
