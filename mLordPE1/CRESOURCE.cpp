// CRESOURCE.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CRESOURCE.h"
#include "afxdialogex.h"


// CRESOURCE 对话框

IMPLEMENT_DYNAMIC(CRESOURCE, CDialogEx)

CRESOURCE::CRESOURCE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESOURCE, pParent)
	, m_NumName(_T(""))
	, m_NumID(_T(""))
	, m_NumName2(_T(""))
	, m_NumID2(_T(""))
	, m_ResRVA(_T(""))
	, m_ResFOA(_T(""))
	, m_ResSize(_T(""))
{

}

CRESOURCE::~CRESOURCE()
{
}

void CRESOURCE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_treeCtl);
	DDX_Text(pDX, IDC_EDIT1, m_NumName);
	DDX_Text(pDX, IDC_EDIT17, m_NumID);
	DDX_Text(pDX, IDC_EDIT18, m_NumName2);
	DDX_Text(pDX, IDC_EDIT44, m_NumID2);
	DDX_Text(pDX, IDC_EDIT19, m_ResRVA);
	DDX_Text(pDX, IDC_EDIT45, m_ResFOA);
	DDX_Text(pDX, IDC_EDIT46, m_ResSize);
}


BEGIN_MESSAGE_MAP(CRESOURCE, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CRESOURCE::OnTvnSelchangedTree1)
END_MESSAGE_MAP()

const WCHAR* RES[25] = {
	(L"") ,				// 0
					(L"光标") ,				// 1 
					(L"位图") ,				// 2 RT_BITMAP
					(L"图标") ,			    // 3 RT_ICON
					(L"菜单") ,				// 4 RT_MENU
					(L"对话框") ,			// 5 RT_DIALOG      
					(L"字符串表") ,			// 6 RT_STRING      
					(L"字体目录") ,			// 7 RT_FONTDIR     
					(L"字体") ,				// 8 RT_FONT        
					(L"加速键") ,			// 9 RT_ACCELERATOR 
					(L"RC数据") ,		// 10 RT_RCDATA      
					(L"消息表") ,			// 11 RT_MESSAGETABLE
					(L"光标组") ,			// 12 
					(L"") ,				   // 13 
					(L"图标组") ,			// 14 
					L"" ,					// 15
					(L"版本信息") ,			// 16
					(L"对话框包含目录") ,	// 17 #define RT_DLGINCLUDE   MAKEINTRESOURCE(17)
					L"" ,								// 18 #define RT_PLUGPLAY     MAKEINTRESOURCE(19)
					L"" ,								// 19 #define RT_VXD          MAKEINTRESOURCE(20)
					L"" ,								// 20 #define RT_ANICURSOR    MAKEINTRESOURCE(21)
					L"" ,								// 21 #define RT_ANIICON      MAKEINTRESOURCE(22)
					L"" ,									// 22 
					(L"HTML") ,						    // 23 #define RT_HTML         MAKEINTRESOURCE(23)
					(L"清单文件")			// 24 RT_MANIFEST
};
// CRESOURCE 消息处理程序


BOOL CRESOURCE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(L"资源表");
	//第一层
	//获取资源目录
	PIMAGE_RESOURCE_DIRECTORY pResDirOne = mpe.pResource;
	//资源目录个数
	DWORD dwOneCount = pResDirOne->NumberOfIdEntries + pResDirOne->NumberOfNamedEntries;
	m_NumID.Format(L"%d", pResDirOne->NumberOfIdEntries);
	m_NumName.Format(L"%d", pResDirOne->NumberOfNamedEntries);
	//获取资源目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);
	CString str;
	//遍历第一层资源信息
	for (int i = 0; i < dwOneCount; ++i)
	{
		//判断是什么资源类型
		if (pResOneEntry[i].NameIsString)//资源是字符串作为标识,则nameoffset有效
		{
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)
				(pResOneEntry[i].NameOffset + (DWORD)pResDirOne);
			WCHAR wResName[100];
			wcsncpy_s(wResName, pName->NameString, pName->Length);
			//输出资源类型名
			//wprintf(L"%s\n", wResName);
			hRoot = m_treeCtl.InsertItem(wResName, 0, 3, NULL, TVI_LAST);
			m_treeCtl.SetItemData(hRoot, i);
			
		}
		else//资源是id作为标识
		{
			//系统提供的资源类型
			if (pResOneEntry[i].Id < 25)
			{
				//wprintf(L"%s\n", RES[pResOneEntry[i].Id]);

				hRoot = m_treeCtl.InsertItem(RES[pResOneEntry[i].Id], 0, 3, NULL, TVI_LAST);
				m_treeCtl.SetItemData(hRoot, i);
			}
			else
			{ 
				//wprintf(L"%08x\n", pResOneEntry[i].Id);
				str.Format(L"%08x", pResOneEntry[i].Id);
				
				hRoot = m_treeCtl.InsertItem(str, 0, 3, NULL, TVI_LAST);
				m_treeCtl.SetItemData(hRoot, i);
			}
				
		}
		//第二层，资源名称
		if (pResOneEntry[i].DataIsDirectory)//如果存在第二层
		{
			//第二层目录表
			PIMAGE_RESOURCE_DIRECTORY pResDirTwo = (PIMAGE_RESOURCE_DIRECTORY)
				(pResOneEntry[i].OffsetToDirectory + (DWORD)pResDirOne);
			DWORD ResTwoCount = pResDirTwo->NumberOfIdEntries + pResDirTwo->NumberOfNamedEntries;
			
			//第二层目录项
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoRntry =
				(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);
			//遍历所有此类资源
			for (int j = 0; j < ResTwoCount; ++j)
			{
				if (pResTwoRntry[j].NameIsString)
				{
					PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)
						(pResTwoRntry[j].NameOffset + (DWORD)pResDirOne);
					//输出资源名
					WCHAR wResName[100];
					wcsncpy_s(wResName, pName->NameString, pName->Length);
					//wprintf_s(L"\t%s\n", wResName);
					hChild=m_treeCtl.InsertItem(wResName, 1, 3, hRoot, TVI_LAST);
					m_treeCtl.SetItemData(hChild, j);
				}
				else {
					str.Format(L"%d", pResTwoRntry[j].Id);
					//printf("\t%08x\n", pResTwoRntry[j].Id);
					hChild=m_treeCtl.InsertItem(str, 1, 3, hRoot, TVI_LAST);
					m_treeCtl.SetItemData(hChild, j);
				}

			}
		}
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CRESOURCE::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int num1 = 0, num2 = 0;
	CString str;
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem = m_treeCtl.GetSelectedItem();
	HTREEITEM hParent = m_treeCtl.GetParentItem(hItem);
	HTREEITEM hCh = m_treeCtl.GetChildItem(hItem);
	if (hParent ==NULL&&hCh==NULL)
	{
		*pResult = 0;
		return;
	}
	//获取第一层目录
	PIMAGE_RESOURCE_DIRECTORY pResDirOne = mpe.pResource;
	//获取第一层目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResOneEntry =
		(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirOne + 1);
	//第二层目录表
	PIMAGE_RESOURCE_DIRECTORY pResDirTwo = NULL;
	//第二层目录项
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResTwoRntry = NULL;
	if (hParent==NULL&&hCh!=NULL)
	{
		num1 = m_treeCtl.GetItemData(hItem);
		pResDirTwo = (PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[num1].OffsetToDirectory + (DWORD)pResDirOne);
		pResTwoRntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);
		m_NumID2.Format(L"%d", pResDirTwo->NumberOfIdEntries);
		m_NumName2.Format(L"%d", pResDirTwo->NumberOfNamedEntries);
		
	}
	
	if (hParent!=NULL&&hCh==NULL)
	{
		num1 = m_treeCtl.GetItemData(hParent);
		num2 = m_treeCtl.GetItemData(hItem);
		pResDirTwo = (PIMAGE_RESOURCE_DIRECTORY)(pResOneEntry[num1].OffsetToDirectory + (DWORD)pResDirOne);
		pResTwoRntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirTwo + 1);
		//第三层目录表
		PIMAGE_RESOURCE_DIRECTORY pResDirThree = (PIMAGE_RESOURCE_DIRECTORY)
			(pResTwoRntry[num2].OffsetToDirectory + (DWORD)pResDirOne);
		//第三层目录项
		PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThreeEntry =
			(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResDirThree + 1);
		//获取资源真正信息
		PIMAGE_RESOURCE_DATA_ENTRY pResData = (PIMAGE_RESOURCE_DATA_ENTRY)
			(pResThreeEntry->OffsetToData + (DWORD)pResDirOne);
		m_ResRVA.Format(L"%08X", pResData->OffsetToData);


		m_ResFOA.Format(L"%08X", mpe.Rva2Foa(pResData->OffsetToData));


		m_ResSize.Format(L"%08X", pResData->Size);
	}
	
	
	
	
	
	
	UpdateData(FALSE);
	*pResult = 0;
}
