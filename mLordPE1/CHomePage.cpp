// CHomePage.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CHomePage.h"
#include "afxdialogex.h"
#include "Md5.h"
#include "CClientSocket.h"
#include "resource.h"
// CHomePage 对话框
#define MY_MSG_SCANVIRUS (WM_USER+102)
#define MY_MSG_SCANMD5 (WM_USER+105)
#define MY_MSG_SCANWHITE (WM_USER+106)

IMPLEMENT_DYNAMIC(CHomePage, CDialogEx)

//连接服务器
CClientSocket m_CloudAnti;
CHomePage::CHomePage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Home, pParent)
	
	, m_response(_T(""))
{

}

CHomePage::~CHomePage()
{
}

void CHomePage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comBVirus);
	DDX_Control(pDX, IDC_LIST1, m_lsCtlVirus);
	DDX_Control(pDX, IDC_NOTE, m_staticPath);
	DDX_Text(pDX, IDC_NOTE, m_response);
}


BEGIN_MESSAGE_MAP(CHomePage, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CHomePage::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CHomePage::OnBnClickedButton1)
	ON_MESSAGE(MY_MSG_SCANVIRUS, &CHomePage::OnMyMsgScanvirus)
	ON_MESSAGE(MY_MSG_SCANMD5, &CHomePage::OnMyMsgScanmd5)
	ON_BN_CLICKED(IDC_BUTTON3, &CHomePage::OnBnClickedButton3)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CHomePage::OnNMRClickList1)
	ON_COMMAND(ID_CLEAN_32776, &CHomePage::OnClean32776)
	ON_COMMAND(ID_CLEAN_32777, &CHomePage::OnClean32777)
	ON_MESSAGE(MY_MSG_SCANWHITE, &CHomePage::OnMyMsgScanwhite)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CLEAN_32775, &CHomePage::OnClean32775)
END_MESSAGE_MAP()


// CHomePage 消息处理程序


BOOL CHomePage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_comBVirus.AddString(L"全路径查杀");
	m_comBVirus.AddString(L"自定义查杀");
	m_comBVirus.AddString(L"白名单查杀");
	m_comBVirus.AddString(L"云查杀");

	m_comBVirus.AddString(L"获取文件MD5");
	m_comBVirus.AddString(L"获取进程MD5");
	m_comBVirus.AddString(L"上传本地病毒库");

	m_menuVirus.LoadMenu(IDR_MENU1);

	m_lsCtlVirus.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);
	m_lsCtlVirus.InsertColumn(0, L"名称", 0, 250);
	m_lsCtlVirus.InsertColumn(1, L"", 0, 300);
	m_lsCtlVirus.InsertColumn(2, L"", 0, 300);

	
	m_response = m_CloudAnti.m_responce;
	UpdateData(FALSE);
	//SetTimer(0x1000, 100, 0);//显示扫描信息
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//改变查杀类型
void CHomePage::OnCbnSelchangeCombo1()
{

	int selIdx = m_comBVirus.GetCurSel();
	switch (selIdx)
	{
	case 0://全路径
	{
		GetDlgItem(IDC_STATICPATH1)->ShowWindow(SW_HIDE);

	}break;
	case 1:case 4://自定义路径/获取文件MD5
	{
		GetDlgItem(IDC_STATICPATH1)->ShowWindow(SW_SHOW);

		GetDlgItem(IDC_BUTTON3)->SetWindowTextW(L"加入本地病毒库");
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
				GetDlgItem(IDC_STATICPATH1)->SetWindowTextW(szFolderPath);
				UpdateData(FALSE);
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

	}break;
	case 2://白名单查杀
	{
		GetDlgItem(IDC_STATICPATH1)->ShowWindow(SW_HIDE);


	}break;
	case 3://云查杀
	{
		GetDlgItem(IDC_STATICPATH1)->ShowWindow(SW_SHOW);
		CFileDialog dlg(TRUE, _T("exe"), NULL,
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("可执行程序|*.exe|所有文件|*||"));
		if (dlg.DoModal() == IDOK) {

			//获取文件路径
			CString FilePath = dlg.GetPathName();

		
			GetDlgItem(IDC_STATICPATH1)->SetWindowTextW(FilePath);
			UpdateData(FALSE);
			
		}

	}break;
	case 5://获取进程MD5
	{
		GetDlgItem(IDC_STATICPATH1)->ShowWindow(SW_HIDE);

		GetDlgItem(IDC_BUTTON3)->SetWindowTextW(L"加入白名单");


	}break;
	case 6://
	{

	}
	default:
		break;
	}
	

}
DWORD CALLBACK scanVirProc(LPVOID p)
{
	CHomePage* _this = (CHomePage*)p;
	mpe.findFiles(_this->m_pathVirus, 0, _this->m_hWnd, MY_MSG_SCANVIRUS);
	_this->SetTimer(0x1000, -1, NULL);//停止定时刷新
	_this->m_staticPath.SetWindowTextW(L"完成");
	_this->m_IsScaning = FALSE;
	_this->SetDlgItemText(IDC_BUTTON1, L"开始扫描");
	_this->SetTimer(0x1000, -1, NULL);//停止定时刷新

	_this->m_IsScaning = FALSE;

	return 0;
}
DWORD CALLBACK scanMD5Proc(LPVOID p)
{
	CHomePage* _this = (CHomePage*)p;
	mpe.findFiles(_this->m_pathVirus, L"getMD5", _this->m_hWnd, MY_MSG_SCANMD5);
	_this->SetTimer(0x1000, -1, NULL);//停止定时刷新
	_this->m_staticPath.SetWindowTextW(L"完成");
	_this->m_IsScaning = FALSE;
	_this->SetDlgItemText(IDC_BUTTON1, L"开始扫描");
	_this->SetTimer(0x1000, -1, NULL);//停止定时刷新
	_this->m_IsScaning = FALSE;

	return 0;
}
//获取进程的MD5
DWORD CALLBACK getProcessMD5Proc(LPVOID p)
{
	CHomePage* _this = (CHomePage*)p;
	std::vector<PROCESSINFO> scanProcList;

	getAllProc(&scanProcList,1,0, 0);
	int index = 0; CString processMD5;
	for (auto&i : scanProcList)
	{
		_this->m_lsCtlVirus.InsertItem(index, i.szExeFile);
		processMD5 = md5FileValue(i.szPath);

		_this->m_lsCtlVirus.SetItemText(index, 1, processMD5);
		_this->m_lsCtlVirus.SetItemText(index, 2, i.szPath);

		index++;
		//滚动到最后一行
		_this->m_lsCtlVirus.Scroll(CSize(0, 100000));
	}
	_this->m_staticPath.SetWindowTextW(L"完成");
	_this->SetDlgItemText(IDC_BUTTON1, L"开始扫描");
	_this->SetTimer(0x1000, -1, NULL);//停止定时刷新
	_this->m_IsScaning = FALSE;
	return 0;
}
//白名单查杀
DWORD CALLBACK scanProcessMD5Proc(LPVOID p)
{
	CHomePage* _this = (CHomePage*)p;
	std::vector<PROCESSINFO> scanProcList;
	_this->m_lsCtlVirus.DeleteAllItems();

	getAllProc(&scanProcList, 2,_this->m_hWnd, MY_MSG_SCANWHITE);
	
	_this->SetDlgItemText(IDC_BUTTON1, L"开始扫描");
	_this->SetTimer(0x1000, -1, NULL);//停止定时刷新
	_this->m_staticPath.SetWindowTextW(L"完成");
	_this->m_IsScaning = FALSE;

	return 0;
}
//云查杀
DWORD CALLBACK CloudProc(LPVOID p)
{
	CHomePage* _this = (CHomePage*)p;
	

	while (true)
	{
		m_CloudAnti.recv();
		DataPackResult* pBuff = m_CloudAnti.m_respPack;
		if (pBuff != nullptr)
		{

			if (pBuff->type == scanVirus && pBuff->status==1 )//
			{
				MessageBox(0, (CString)pBuff->data, 0, 0);
				break;
			}
			if (pBuff->type==pushVirus && pBuff->status==1)
			{
				CString buffer; //int i = _this->m_lsCtlVirus.GetItemCount();
				//按\t切割
				AfxExtractSubString(buffer, (CString)pBuff->data, 0, '\t');
				_this->m_lsCtlVirus.InsertItem(0,buffer);
				AfxExtractSubString(buffer, (CString)pBuff->data, 1, '\t');
				_this->m_lsCtlVirus.SetItemText(0,1, buffer);
				_this->m_lsCtlVirus.SetItemText(0,2, L"上传成功");
					
			}
			if (pBuff->type == pushVirus && pBuff->status == 2)//已经存在
			{
				CString buffer; //int i = _this->m_lsCtlVirus.GetItemCount();
				//按\t切割
				AfxExtractSubString(buffer, (CString)pBuff->data, 0, '\t');
				_this->m_lsCtlVirus.InsertItem(0, buffer);
				AfxExtractSubString(buffer, (CString)pBuff->data, 1, '\t');
				_this->m_lsCtlVirus.SetItemText(0, 1, buffer);
				_this->m_lsCtlVirus.SetItemText(0, 2, L"已经存在");
			}
			if (pBuff->type==done)
			{
				break;
			}
			
		}
	}
	
	_this->SetDlgItemText(IDC_BUTTON1, L"开始扫描");
	_this->SetTimer(0x1000, -1, NULL);//停止定时刷新
	_this->m_staticPath.SetWindowTextW(L"完成");
	_this->m_IsScaning = FALSE;
	
	return 0;
}
	
std::vector<CString> GetDriverList()
{
	std::vector<CString>nDiverList;
	TCHAR nDrive[MAX_PATH];
	GetLogicalDriveStrings(100, (LPWSTR)nDrive);
	TCHAR* pName = nDrive;

	while (*pName != 0) {
		nDiverList.push_back(pName);
		pName += _tcslen(pName) + 1;
	}
	return nDiverList;
}
//读取本地病毒库
void LoadVirusLib()
{
	FILE* pFile = nullptr; CString buffer; char* cbuff; 
	if (fopen_s(&pFile, "MyVirus.txt", "rb+"))
	{
		MessageBox(0, L"打开文件失败", 0,0);
	}
	
	VIRUSINFO tmpVirus = {0};
	while(!feof(pFile))
	{
		fwscanf_s(pFile, L"%s\n", buffer, MAX_PATH);
		lstrcpy(tmpVirus.FileName, buffer);
		fwscanf_s(pFile, L"%s\n", buffer, MAX_PATH);
		lstrcpy(tmpVirus.FileMd5, buffer);

		mpe.VirusLib.push_back(tmpVirus);
		
	}
	fclose(pFile);
}
//读取白名单
void LoadWhiteLib()
{
	FILE* pFile = nullptr; CString buffer;
	if (fopen_s(&pFile, "Whitelist.txt", "rb+"))
	{
		MessageBox(0, L"打开文件失败", 0, 0);
	}
	VIRUSINFO tmpWhite = {0};
	while (!feof(pFile))
	{
		fwscanf_s(pFile, L"%s\n", buffer, MAX_PATH);
		lstrcpy(tmpWhite.FileName, buffer);
		fwscanf_s(pFile, L"%s\n", buffer, MAX_PATH);
		lstrcpy(tmpWhite.FileMd5, buffer);
		mpe.WhiteLib.push_back(tmpWhite);
	}
	fclose(pFile);
}

void CHomePage::pushVirusLib()
{
	m_CloudAnti.starConnect();
	if (m_response == L"服务器连接失败")
	{
		return;
	}
	FILE* pFile = nullptr; CString buffer; char* cbuff;
	if (fopen_s(&pFile, "MyVirus.txt", "rb+"))
	{
		MessageBox(0, L"打开文件失败", 0);
	}

	VIRUSINFO tmpVirus = { 0 };
	//创建线程接收消息
	vecHThread.push_back(CreateThread(0, 0, CloudProc, this, 0, 0));
	while (!feof(pFile))
	{
		fwscanf_s(pFile, L"%s\n", buffer, MAX_PATH);
		lstrcpy(tmpVirus.FileName, buffer);
		fwscanf_s(pFile, L"%s\n", buffer, MAX_PATH);
		lstrcpy(tmpVirus.FileMd5, buffer);
		
		//发送消息
		CStringA buffer = CClientSocket::CStrW2CStrA(tmpVirus.FileName) +
			"\t" + CClientSocket::CStrW2CStrA(tmpVirus.FileMd5);
		m_CloudAnti.send(pushVirus, buffer, buffer.GetLength());
		
	}
	fclose(pFile);
	//发生结束消息
	CStringA buffe = "结束";
	m_CloudAnti.send(done, buffe, buffe.GetLength());

}
//开始扫描
void CHomePage::OnBnClickedButton1()
{
	if (m_IsScaning)
	{
		for (auto&i:vecHThread)
		{
			TerminateThread(i, 0);
		}
		m_IsScaning = FALSE;
		SetDlgItemText(IDC_BUTTON1, L"开始扫描");
		SetTimer(0x1000, -1, NULL);//停止定时刷新
		m_staticPath.SetWindowTextW(L"完成");
		return;
	}
	m_IsScaning = TRUE;
	SetTimer(0x1000, 30, NULL);//开启定时刷新
	SetDlgItemText(IDC_BUTTON1, L"停止扫描");
	LoadVirusLib();
	m_lsCtlVirus.DeleteAllItems();
	GetDlgItem(IDC_STATICPATH1)->GetWindowText(m_pathVirus);
	int selIdx = m_comBVirus.GetCurSel();
	switch (selIdx)
	{
		case 0://全路径
		{
			std::vector<CString>nDiverList;
			nDiverList = GetDriverList();
			
			for (int i = 0; i < nDiverList.size(); ++i)
			{
				m_pathVirus = nDiverList[i];
				vecHThread.push_back(CreateThread(0, 0, scanVirProc, this, 0, 0));
			}
		}break;
		case 1://自定义路径
		{
			vecHThread.push_back(CreateThread(0, 0, scanVirProc, this, 0, 0));


		}break;
		case 2://白名单查杀
		{
			LoadWhiteLib();
			vecHThread.push_back(CreateThread(0, 0, scanProcessMD5Proc, this, 0, 0));


		}break;
		case 3://点击云查杀
		{
			CString FilePath;
			GetDlgItem(IDC_STATICPATH1)->GetWindowTextW(FilePath);
			m_CloudAnti.starConnect();
			if (m_response == L"服务器连接失败")
			{
				break;
			}
			CStringA FileMD5 = TCHAR2char(md5FileValue(FilePath));
			m_lsCtlVirus.InsertItem(0, FilePath);
			m_lsCtlVirus.SetItemText(0, 1, md5FileValue(FilePath));
			//发送消息
			m_CloudAnti.send(scanVirus, FileMD5, FileMD5.GetLength());
			//创建线程接收消息
			vecHThread.push_back(CreateThread(0, 0, CloudProc, this, 0, 0));
		}break;
		case 4://获取文件MD5
		{
			
			vecHThread.push_back(CreateThread(0, 0, scanMD5Proc, this, 0, 0));


		}break;
		case 5://获取进程MD5
		{

			vecHThread.push_back(CreateThread(0, 0, getProcessMD5Proc, this, 0, 0));


		}break;
		case 6://上传本地病毒库
		{
			pushVirusLib();
		}break;
	
	default:
		break;
	}
	
}

//接收到消息就填充vector
afx_msg LRESULT CHomePage::OnMyMsgScanvirus(WPARAM wParam, LPARAM lParam)
{
	FILEINFO* fInfo = (FILEINFO*)wParam;
	int row = m_lsCtlVirus.GetItemCount();
	m_lsCtlVirus.InsertItem(row, fInfo->path);
	CString buffer;
	buffer.Format(_T("%dKb"), (fInfo->nFileSizeHigh << 32 | fInfo->nFileSizeLow) / 1024);
	m_lsCtlVirus.SetItemText(row, 1, buffer);
	//滚动到最后一行
	m_lsCtlVirus.Scroll(CSize(0, 100000));
	return 1;
}

//接收到文件MD5的消息
afx_msg LRESULT CHomePage::OnMyMsgScanmd5(WPARAM wParam, LPARAM lParam)
{
	VIRUSINFO* fvirus = (VIRUSINFO*)wParam;

	int row = m_lsCtlVirus.GetItemCount();
	m_lsCtlVirus.InsertItem(row, (CString)fvirus->FileName);
	m_lsCtlVirus.SetItemText(row, 1, (CString)fvirus->FileMd5);
	//滚动到最后一行
	m_lsCtlVirus.Scroll(CSize(0, 100000));
	return 1;
}

//一键删除/加入病毒库/加入白名单
void CHomePage::OnBnClickedButton3()
{

	int selIdx = m_comBVirus.GetCurSel();
	switch (selIdx)
	{
	case 0://全路径
	{
	}break;
	case 1://自定义路径
	{
	}break;
	case 2://白名单查杀
	{
	}break;
	case 3://云查杀
	{
	}break;
	case 5://加入白名单
	{
		
		std::vector<int> selIdx; FILE* pFile = nullptr; CString buffer;
		if (fopen_s(&pFile, "WhiteList.txt", "wb+"))
		{
			MessageBox(0, L"打开文件失败", 0);
		}
		for (int i = 0; i < m_lsCtlVirus.GetItemCount(); ++i)
		{

			if (m_lsCtlVirus.GetCheck(i))
			{
				
				CString buffer = m_lsCtlVirus.GetItemText(i, 0);
				fwprintf_s(pFile, L"%s\n", buffer);
				buffer = m_lsCtlVirus.GetItemText(i, 1);
				fwprintf_s(pFile, L"%s\n", buffer);

			}
		}
		fclose(pFile);
	}break;
	case 4://加入病毒库
	{
		FILE* pFile = nullptr; CString buffer;
		if (fopen_s(&pFile, "MyVirus.txt", "wb+"))
		{
			MessageBox(0, L"打开文件失败", 0);
		}
		for (int i = 0; i < m_lsCtlVirus.GetItemCount(); ++i)
		{
			if (m_lsCtlVirus.GetCheck(i))
			{
				buffer = m_lsCtlVirus.GetItemText(i, 0);
				fwprintf_s(pFile, L"%s\n", buffer);
				buffer = m_lsCtlVirus.GetItemText(i, 1);
				fwprintf_s(pFile, L"%s\n", buffer);
			}
		}
		fclose(pFile);
	}break;
	default:
		break;
	}
	m_staticPath.SetWindowTextW(L"完成");
}


void CHomePage::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==0x1000)
	{
		m_staticPath.SetWindowTextW(mpe.strNote);
	}

	CDialogEx::OnTimer(nIDEvent);
}

//右键
void CHomePage::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu* pSubMenu = m_menuVirus.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
	*pResult = 0;
	*pResult = 0;
}

//全选
void CHomePage::OnClean32776()
{
	int size = m_lsCtlVirus.GetItemCount();
	for (int i = 0; i < size; ++i) {
		m_lsCtlVirus.SetCheck(i, TRUE);
	}
}

//全不选
void CHomePage::OnClean32777()
{
	int size = m_lsCtlVirus.GetItemCount();
	for (int i = 0; i < size; ++i) {
		m_lsCtlVirus.SetCheck(i, FALSE);
	}
}


afx_msg LRESULT CHomePage::OnMyMsgScanwhite(WPARAM wParam, LPARAM lParam)
{
	PROCESSINFO* scanProc = (PROCESSINFO*)wParam;
	int index = 0;
	CString Buffer;
	m_lsCtlVirus.InsertItem(index, scanProc->szExeFile);
	m_lsCtlVirus.SetItemText(index, 1, scanProc->szPath);
	if (scanProc->nSafe == 1)  Buffer = L"安全";
	else Buffer = L"危险";
	
	m_lsCtlVirus.SetItemText(index, 2, Buffer);

	
	

	return 0;
}


void CHomePage::OnClose()
{
	//关闭所有线程
	for (auto&i:vecHThread)
	{
		TerminateThread(i, 0);
	}

	CDialogEx::OnClose();
}
//打开文件位置

void CHomePage::OnClean32775()
{
	int index = (int)m_lsCtlVirus.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;
	CString strPath = m_lsCtlVirus.GetItemText(index, 2);//路径
	ShellExecute(NULL,
		_T("open"),
		_T("Explorer.exe"),
		L"/select," + strPath,
		NULL,
		SW_SHOWDEFAULT);
}
