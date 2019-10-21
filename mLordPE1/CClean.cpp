// CClean.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CClean.h"
#include "afxdialogex.h"
#include "resource.h"
#include "Psapi.h"
// CClean 对话框
#define MY_MSG_SCANFILE (WM_USER+100)

IMPLEMENT_DYNAMIC(CClean, CDialogEx)

CClean::CClean(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CClean, pParent)
	, m_filter(_T(""))
	, m_dir(_T(""))
{

}

CClean::~CClean()
{
}

void CClean::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);

	DDX_Control(pDX, IDC_COMBO1, m_comB);
	DDX_Control(pDX, IDC_BUTTONPATH, m_buttPah);
}


BEGIN_MESSAGE_MAP(CClean, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CClean::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTONPATH, &CClean::OnBnClickedButtonpath)
	ON_BN_CLICKED(IDC_BUTTON3, &CClean::OnBnClickedButton3)
	ON_MESSAGE(MY_MSG_SCANFILE, &CClean::OnMyMsgScanfile)
	ON_BN_CLICKED(IDC_ALL, &CClean::OnBnClickedAll)
	ON_BN_CLICKED(IDC_BUTTON2, &CClean::OnBnClickedButton2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CClean::OnRclickList1)
	ON_COMMAND(ID_CLEAN_32776, &CClean::OnClean32776)
	ON_COMMAND(ID_CLEAN_32777, &CClean::OnClean32777)
	ON_COMMAND(ID_CLEAN_32775, &CClean::OnClean32775)
END_MESSAGE_MAP()


// CClean 消息处理程序



BOOL CClean::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetDlgItem(IDC_BUTTONPATH)->ShowWindow(SW_HIDE);
	m_menuFile.LoadMenu(IDR_MENU1);
	m_comB.AddString(L"VS垃圾扫描");

	m_comB.AddString(L"自动扫描");
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_list.InsertColumn(0, L"文件名", 0, 600);
	m_list.InsertColumn(1, L"大小", 0, 80);


	m_comB.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//拖拽按钮是否显示
void CClean::OnCbnSelchangeCombo1()
{
	if (m_comB.GetCurSel() == 0)//VS垃圾清理
	{
		GetDlgItem(IDC_BUTTONPATH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_TRASH)->EnableWindow(FALSE);
		GetDlgItem(IDC_SYS)->EnableWindow(FALSE);
		GetDlgItem(IDC_BROWSE)->EnableWindow(FALSE);

		GetDlgItem(IDC_ALL)->EnableWindow(FALSE);

	}
	else if (m_comB.GetCurSel() == 1)//自动扫描
	{
		GetDlgItem(IDC_BUTTONPATH)->ShowWindow(SW_HIDE);


	}

	UpdateData(FALSE);
}

//拖拽按钮打开文件夹
void CClean::OnBnClickedButtonpath()
{
	TCHAR			szFolderPath[MAX_PATH] = { 0 };
	BROWSEINFO		sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("请选择一个文件夹：");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_USENEWUI | BIF_BROWSEINCLUDEFILES;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			//m_dir = szFolderPath;
			GetDlgItem(IDC_BUTTONPATH)->SetWindowTextW(szFolderPath);
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
	else
	{
		return;
	}
}


DWORD CALLBACK ScanFileProc(VOID* arg)
{
	CClean* _this = (CClean*)arg;
	CString DirName = _this->m_dir;
	if(!mpe.findFiles(_this->m_dir, _this->m_filter,_this->m_hWnd, MY_MSG_SCANFILE))
		_this->MessageBox(L"路径错误");
	else
		_this->MessageBox(L"扫描完毕");


	return 0;
}
//开始扫描
void CClean::OnBnClickedButton3()
{
	m_list.DeleteAllItems();
	int stateVSTrash = ((CButton *)GetDlgItem(IDC_TRASH))->GetCheck();
	int stateSys = ((CButton *)GetDlgItem(IDC_SYS))->GetCheck();
	int stateBrowse = ((CButton *)GetDlgItem(IDC_BROWSE))->GetCheck();
	if (m_comB.GetCurSel() == 0)//如果是VS垃圾清理
	{
		while (m_list.DeleteColumn(0));
		m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

		m_list.InsertColumn(0, L"文件名", 0, 600);
		m_list.InsertColumn(1, L"大小", 0, 80);
		GetDlgItem(IDC_BUTTONPATH)->GetWindowText(m_dir);
		
		if (m_dir == L"选择或拖拽")
		{
			MessageBox(L"请选择路径", 0, 0);
			return;
		}
		m_filter = L".ilk .pdb .obj .log .pch .tlog .lastbuildstate .sdf .idb .ipch .res .o .lst .knl .img .bin .db";
		
		// 开启线程扫描垃圾文件
		m_hScanThread = CreateThread(0, 0, ScanFileProc, this, 0, 0);
	}
	else if (m_comB.GetCurSel() == 1)//如果是自动扫描
	{
		if (stateVSTrash)
		{
			//初始化
			SHQUERYRBINFO RecycleBinInfo = {};
			RecycleBinInfo.cbSize = sizeof(SHQUERYRBINFO);

			//查询信息
			SHQueryRecycleBin(NULL, &RecycleBinInfo);

			while (m_list.DeleteColumn(0));
			m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

			m_list.InsertColumn(0, L"文件个数", 0, 100);
			m_list.InsertColumn(1, L"文件大小", 0, 100);

			CString buff;
			buff.Format(L"%d", RecycleBinInfo.i64NumItems);
			m_list.InsertItem(0, buff);
			buff.Format(L"%d", RecycleBinInfo.i64Size);

			m_list.SetItemText(0, 1, buff);
			MessageBox( L"扫描完毕");
			
		}
		if (stateSys)
		{
			while (m_list.DeleteColumn(0));
			m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

			m_list.InsertColumn(0, L"文件名", 0, 600);
			m_list.InsertColumn(1, L"大小", 0, 80);
			m_filter = L".tmp ._mp .log .gid .chk .old .bak";
			BOOL bSuccess = GetTempPath(MAX_PATH, m_dir.GetBuffer(MAX_PATH));

			m_dir.ReleaseBuffer();
			if (bSuccess)//得到系统临时目录
			{
				m_hScanThread = CreateThread(0, 0, ScanFileProc, this, 0, 0);
			}
		}
		if (stateBrowse)
		{
			while (m_list.DeleteColumn(0));
			m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

			m_list.InsertColumn(0, L"文件名", 0, 600);
			m_list.InsertColumn(1, L"大小", 0, 80);
			m_filter = L".tmp ._mp .log .gid .chk .old .bak";
			BOOL bSuccess = SHGetSpecialFolderPath(NULL, m_dir.GetBuffer(MAX_PATH), CSIDL_INTERNET_CACHE, FALSE);
			m_dir.ReleaseBuffer();
			
			if (bSuccess)
			{
				m_hScanThread = CreateThread(0, 0, ScanFileProc, this, 0, 0);
			}
			bSuccess = SHGetSpecialFolderPath(NULL, m_dir.GetBuffer(MAX_PATH), CSIDL_COOKIES, FALSE);
			if (bSuccess)
			{
				m_hScanThread = CreateThread(0, 0, ScanFileProc, this, 0, 0);
			}

		}
		
	}

}

//接收消息
afx_msg LRESULT CClean::OnMyMsgScanfile(WPARAM wParam, LPARAM lParam)
{
	FILEINFO* fInfo = (FILEINFO*)wParam;

	m_totalSize += (fInfo->nFileSizeHigh << 32 | fInfo->nFileSizeLow);

	int row = m_list.GetItemCount();
	m_list.InsertItem(row, fInfo->path);
	CString buffer;
	buffer.Format(_T("%dKb"), (fInfo->nFileSizeHigh << 32 | fInfo->nFileSizeLow) / 1024);
	m_list.SetItemText(row, 1, buffer);
	//滚动到最后一行
	m_list.Scroll(CSize(0, 100000));
	return 1;
}

//全选复选框
void CClean::OnBnClickedAll()
{
	if (((CButton *)GetDlgItem(IDC_ALL))->GetCheck())
	{
		((CButton *)GetDlgItem(IDC_TRASH))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_SYS))->SetCheck(1);

		((CButton *)GetDlgItem(IDC_BROWSE))->SetCheck(1);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_TRASH))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_SYS))->SetCheck(0);

		((CButton *)GetDlgItem(IDC_BROWSE))->SetCheck(0);
	}

}

//删除选中
void CClean::OnBnClickedButton2()
{
	//如果是清空回收站
	if (m_comB.GetCurSel() == 1 && ((CButton *)GetDlgItem(IDC_TRASH))->GetCheck())
	{
		//清空
		SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION  | SHERB_NOSOUND);
	}

	for (int i = 0; i < m_list.GetItemCount(); ) {
		if (m_list.GetCheck(i)) {

			CString path = m_list.GetItemText(i, 0);
			OutputDebugString(L"删除文件: " + path);
			 			if (DeleteFile(path)) {
			 				m_list.DeleteItem(i);
			 				continue;
			 			}
// 			m_list.DeleteItem(i);
// 			continue;
		}
		++i;
	}
	MessageBox(L"删除完毕");
}

//右键菜单
void CClean::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu* pSubMenu = m_menuFile.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
	*pResult = 0;
}

//选中所有
void CClean::OnClean32776()
{
	int size = m_list.GetItemCount();
	for (int i = 0; i < size; ++i) {
		m_list.SetCheck(i, TRUE);
	}
}

//反选所有
void CClean::OnClean32777()
{
	int size = m_list.GetItemCount();
	for (int i = 0; i < size; ++i) {
		m_list.SetCheck(i, !m_list.GetCheck(i));
	}
}

//打开文件位置

void CClean::OnClean32775()
{
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;
	CString strPath = m_list.GetItemText(index, 0);//路径
	ShellExecute(NULL,
		_T("open"),
		_T("Explorer.exe"),
		L"/select," + strPath,
		NULL,
		SW_SHOWDEFAULT);
}
