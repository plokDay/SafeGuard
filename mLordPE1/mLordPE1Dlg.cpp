
// mLordPE1Dlg.cpp: 实现文件
//

#include "stdafx.h"
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")

#include "mLordPE1.h"
#include "mLordPE1Dlg.h"
#include "afxdialogex.h"
#include "CLORDPE.h"
#include "CWindows.h"
#include "CProcess.h"
#include "CClean.h"
#include "resource.h"
#include "CService.h"
#include "CHomePage.h"
#include "AHome.h"
#include "CSOFT.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmLordPE1Dlg 对话框



CmLordPE1Dlg::CmLordPE1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MLORDPE1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmLordPE1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabCtl);
	
}

BEGIN_MESSAGE_MAP(CmLordPE1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CmLordPE1Dlg::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CmLordPE1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CmLordPE1Dlg 消息处理程序
void GetCpuState(CString& nStr)
{
	static LONGLONG nFree{}, nNt{}, nUser{};
	static LONGLONG nFreet{}, nNtt{}, nUsert{};
	static DOUBLE nHave{};
	GetSystemTimes((PFILETIME)&nFree, (PFILETIME)&nNt, (PFILETIME)&nUser);
	nHave = (DOUBLE)(nFree - nFreet) / (DOUBLE)(nNt - nNtt + nUser - nUsert);
	nStr.Format(L"%d", DWORD(100.0 - nHave * 100));
	nFreet = nFree;
	nNtt = nNt;
	nUsert = nUser;
}
DWORD GetMemState()
{
	MEMORYSTATUSEX nStatex;

	nStatex.dwLength = sizeof(nStatex);

	GlobalMemoryStatusEx(&nStatex);

	DOUBLE nAll = (DOUBLE)nStatex.ullTotalPhys / 1048576;
	DOUBLE nHave = (DOUBLE)nStatex.ullAvailPhys / 1048576;
	return (DWORD)((nAll - nHave) / nAll * 100);
}

BOOL CmLordPE1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 在OnInitDialog加入这段代码
	ChangeWindowMessageFilter(WM_DROPFILES, MSGFLT_ADD);
	ChangeWindowMessageFilter(0x0049, MSGFLT_ADD);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetWindowText(_T("安全卫士"));
	QueryPrivileges();
	m_bar.Create(this);
	UINT Indicators[] = { IDS_STRING176 ,IDS_STRING177,IDS_STRING178,IDS_STRING179 };
	m_bar.SetIndicators(Indicators, sizeof(Indicators)/sizeof(UINT));
	m_bar.SetPaneInfo(0, IDS_STRING176, SBPS_STRETCH, 30);
	m_bar.SetPaneInfo(1, IDS_STRING177, SBPS_STRETCH, 30);
	m_bar.SetPaneInfo(2, IDS_STRING178, SBPS_STRETCH, 30);
	m_bar.SetPaneInfo(3, IDS_STRING179, SBPS_STRETCH, 30);

	

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	addTabWnd(L"首页", new AHome, IDD_AHOME);
	addTabWnd(L"查杀", new CHomePage, IDD_Home);

	addTabWnd(L"进程", new CProcess, IDD_CProcess);
	addTabWnd(L"窗口", new CWindows, IDD_CWindows);
	
	addTabWnd(L"清理", new CClean, IDD_CClean);
	
	addTabWnd(L"PE", new CLORDPE, IDD_LORDPE);
	addTabWnd(L"服务", new CService, IDD_CService);
	addTabWnd(L"软件", new CSOFT, IDD_SOFT);

	m_tabCtl.SetCurSel(0);
	m_tabSubWnd[m_tabCtl.GetCurSel()]->ShowWindow(SW_SHOW);


	//注册热键
	::RegisterHotKey(GetSafeHwnd(), 0xa001, MOD_CONTROL | MOD_SHIFT ,'H');//隐藏
	::RegisterHotKey(GetSafeHwnd(), 0xa002, MOD_CONTROL | MOD_SHIFT, 'F');//关机
	::RegisterHotKey(GetSafeHwnd(), 0xa003, MOD_CONTROL | MOD_SHIFT, 'R');//重启
	::RegisterHotKey(GetSafeHwnd(), 0xa004, MOD_CONTROL | MOD_SHIFT, 'L');//锁屏

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmLordPE1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmLordPE1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmLordPE1Dlg::addTabWnd(const CString& title, CDialogEx* pSubWnd, UINT uId)
{
	m_tabCtl.InsertItem(m_tabCtl.GetItemCount(), title);
	pSubWnd->Create(uId, &m_tabCtl);
	CRect rect;
	m_tabCtl.GetClientRect(rect);
	rect.DeflateRect(1, 23, 1, 1);
	pSubWnd->MoveWindow(rect);
	m_tabSubWnd.push_back(pSubWnd);
}


void CmLordPE1Dlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	for (auto&i : m_tabSubWnd)
	{
		i->ShowWindow(SW_HIDE);
	}
	m_tabSubWnd[m_tabCtl.GetCurSel()]->ShowWindow(SW_SHOW);
	*pResult = 0;
}


void CmLordPE1Dlg::OnBnClickedButton1()
{
	// 1. 隐藏当前窗口
	//ShowWindow(hWnd, SW_HIDE);
	this->ShowWindow(SW_HIDE);
	// 2. 获取当前程序路径
	WCHAR szApplication[MAX_PATH] = { 0 };
	DWORD cchLength = _countof(szApplication);
	QueryFullProcessImageName(GetCurrentProcess(), 0,
		szApplication, &cchLength);
	// 3. 以管理员权限重新打开进程
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	sei.lpVerb = L"runas";      // 请求提升权限
	sei.lpFile = szApplication; // 可执行文件路径
	sei.lpParameters = NULL;          // 不需要参数
	sei.nShow = SW_SHOWNORMAL; // 正常显示窗口
	if (ShellExecuteEx(&sei))
		exit(0);
	else
		::ShowWindow(m_hWnd, SW_SHOWNORMAL);
}
//提权
BOOL CmLordPE1Dlg::QueryPrivileges()
{
	// 1. 获得本进程的令牌
	HANDLE hToken = NULL;
	if (!OpenProcessToken(
		GetCurrentProcess(),	// 当前进程句柄
		TOKEN_QUERY,			// 打开这个令牌作用
		&hToken))				// 返回令牌句柄
		return false;
	// 2. 获取提升类型
	TOKEN_ELEVATION_TYPE ElevationType = TokenElevationTypeDefault;
	BOOL                 bIsAdmin = false;							// 是否管理员
	DWORD                dwSize = 0;								// 权限信息结构体大小

	// 获取令牌信息
	if (GetTokenInformation(
		hToken,							// 令牌句柄
		TokenElevationType,				// 获取令牌信息类型
		&ElevationType,					// 返回令牌信息类型对应的结构体
		sizeof(TOKEN_ELEVATION_TYPE),	// 结构体大小
		&dwSize))						// 实际大小
	{
		// 2.1 创建管理员组的对应SID
		BYTE adminSID[SECURITY_MAX_SID_SIZE];		//管理员的SID标识
		dwSize = sizeof(adminSID);					//管理员的SID标识的大小
		// 获取管理员的SID
		CreateWellKnownSid(
			WinBuiltinAdministratorsSid,			// 查询类型
			NULL,									// NULL
			&adminSID,								// 存放管理员的SID缓冲区
			&dwSize);								// 缓冲区大小


		// 2.2 判断当前进程运行用户角色是否为管理员
		// 当前权限是滤令牌（低权限令牌）
		if (ElevationType == TokenElevationTypeLimited) {

			// a. 获取连接令牌的句柄
			HANDLE hUnfilteredToken = NULL;

			GetTokenInformation(hToken,			//令牌句柄
				TokenLinkedToken,				//链接令牌信息（账户令牌）
				(PVOID)&hUnfilteredToken,		//链接令牌的句柄
				sizeof(HANDLE),
				&dwSize);

			// b. 检查这个原始的令牌是否包含管理员的SID
			if (!CheckTokenMembership(
				hUnfilteredToken,		// 链接令牌句柄
				&adminSID,				// 管理员的SID
				&bIsAdmin))				// 返回是否管理员
			{
				return false;
			}
			CloseHandle(hUnfilteredToken);		//关闭句柄
		}
		else {
			// 如果令牌是最高权限-或者关闭UAC
			// 那么直接获取当前用户是否是管理员
			bIsAdmin = IsUserAnAdmin();
		}
		CloseHandle(hToken);
	}
	// 3. 判断具体的权限状况
	BOOL bFullToken = false;
	// 判断提升类型
	switch (ElevationType) {
	case TokenElevationTypeDefault: /* 默认的用户或UAC被禁用 */
		if (IsUserAnAdmin())  bFullToken = true; // 默认用户有管理员权限
		else                  bFullToken = false;// 默认用户不是管理员组
		break;
	case TokenElevationTypeFull:    /* 已经成功提高进程权限 */
		if (IsUserAnAdmin())  bFullToken = true; //当前以管理员权限运行
		else                  bFullToken = false;//当前未以管理员权限运行
		break;
	case TokenElevationTypeLimited: /* 进程在以有限的权限运行 */
		if (bIsAdmin)  bFullToken = false;//用户有管理员权限，但进程权限有限
		else           bFullToken = false;//用户不是管理员组，且进程权限有限
	}

	// 4. 根据权限的不同控制按钮的显示
	if (!bFullToken)
		// 给按钮添加一个盾牌
		Button_SetElevationRequiredState(::GetDlgItem(m_hWnd, IDC_BUTTON1), !bFullToken);
	else
		// 隐藏按钮
		::ShowWindow(::GetDlgItem(m_hWnd, IDC_BUTTON1), SW_HIDE);

	return 0;
}


void CmLordPE1Dlg::mpower(int n)
{
	CString buffer;
	if (n== EWX_POWEROFF)
	{
		buffer = L"确定关机吗?";
	}
	else buffer = L"确定重启吗?";
	if (MessageBox(buffer, _T("警告"), MB_YESNO) == IDYES)
	{
		HANDLE hToken; TOKEN_PRIVILEGES tkp;
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		{
			MessageBox(_T("重启失败..."), _T("警告"), MB_OK); return;
		}
		LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, NULL);
		if (GetLastError() != ERROR_SUCCESS) { MessageBox(_T("重启失败..."), _T("警告"), MB_OK); return; }
		ExitWindowsEx(n | EWX_FORCE, 0);

	}
}

//老板键
BOOL CmLordPE1Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_HOTKEY&&(pMsg->wParam==0xa001))
	{
		if (isHide)
		{
			ShowWindow(SW_HIDE);
			isHide = false;
		}
		else
		{
			ShowWindow(SW_SHOW);
			isHide = true;
		}
	}
	//关机
	BOOL res = TRUE;
	if (pMsg->message == WM_HOTKEY && (pMsg->wParam == 0xa002))
	{
		mpower(EWX_POWEROFF);
		
	}
	//重启
	if (pMsg->message == WM_HOTKEY && (pMsg->wParam == 0xa003))
	{
		mpower(EWX_REBOOT);
	}
	//锁屏
	if (pMsg->message == WM_HOTKEY && (pMsg->wParam == 0xa004))
	{
		LockWorkStation();
	}
	

	return CDialogEx::PreTranslateMessage(pMsg);
}
