// CService.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CService.h"
#include "afxdialogex.h"
#include <winsvc.h>


// CService 对话框

IMPLEMENT_DYNAMIC(CService, CDialogEx)

CService::CService(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CService, pParent)
{

}

CService::~CService()
{
}

void CService::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListServices);
}


BEGIN_MESSAGE_MAP(CService, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CService::OnRclickList1)
	ON_COMMAND(ID_SERVICE_32778, &CService::OnService32778)
	ON_COMMAND(ID_SERVICE_32779, &CService::OnService32779)
END_MESSAGE_MAP()


// CService 消息处理程序


BOOL CService::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ListServices.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_menuService.LoadMenu(IDR_MENU2);
	m_ListServices.InsertColumn(0, L"索引", 0, 45);
	m_ListServices.InsertColumn(1, L"名称", 0,150);
	m_ListServices.InsertColumn(2, L"描述", 0, 400);
	m_ListServices.InsertColumn(3, L"状态", 0, 80);
	m_ListServices.InsertColumn(4, L"类型", 0, 100);
	UpDataService();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
void CService::UpDataService()
{
	
	m_ListServices.DeleteAllItems();
	GetServices();
}

BOOL CService::GetServices()
{
	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//第一次调用,获取所需内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);
	//申请所需内存
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];

	//第二次枚举
	bool bStatus = false;
	bStatus = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,
		SERVICE_STATE_ALL, (PBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);

	CString buff;
	//遍历信息
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		buff.Format(L"%d", i);
		m_ListServices.InsertItem(i, buff);
		//服务名
		m_ListServices.SetItemText(i, 1,pEnumService[i].lpServiceName);

		//状态
		DWORD dwStatus = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		m_ListServices.SetItemText(i, 3, ServiceStatus[dwStatus]);

		//详细信息
		SC_HANDLE hService = OpenService(hSCM, pEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);

		//获取缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);

		DWORD dwType = pServiceConfig->dwStartType;
		m_ListServices.SetItemText(i, 4, ServiceStatus[dwType]);


		if (dwType > 0)
		{
			//获取描述信息大小
			QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwSize);
			LPSERVICE_DESCRIPTION pServiceDesc = (LPSERVICE_DESCRIPTION)new char[dwSize];
			//获取描述
			QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)pServiceDesc, dwSize, &dwSize);
			m_ListServices.SetItemText(i, 2, pServiceDesc->lpDescription);

		}
		else
		{
			//由于权限或什么原因，可能获取不到杀软服务的信息
			m_ListServices.SetItemText(i, 1, L"--");

		}
	}
	return 0;
}

//右键菜单
void CService::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu* pSubMenu = m_menuService.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
	*pResult = 0;
}

//停止服务
void CService::OnService32778()
{
	//获取选中项
	int nSel = m_ListServices.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	//获取服务名
	CString strServiceName = m_ListServices.GetItemText(nSel, 1);
	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//打开服务
	SC_HANDLE hService = OpenService(hSCM, strServiceName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);

	//获取服务状态
	SERVICE_STATUS Status;
	if (QueryServiceStatus(hService, &Status) == FALSE)
	{
		MessageBox(L"获取服务状态失败！");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}
	BOOL ret = ControlService(hService, SERVICE_CONTROL_STOP, &Status);
	if (ret == FALSE)
	{
		MessageBox(L"停止服务失败！");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}
	
	//等待服务停止
	while (QueryServiceStatus(hService, &Status) == TRUE)
	{
		Sleep(Status.dwWaitHint);
		if (Status.dwCurrentState == SERVICE_STOPPED)
		{
			MessageBox(L"停止服务成功！");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			
		}
	}
	UpDataService();
}

//启动
void CService::OnService32779()
{
	//获取选中项
	int nSel = m_ListServices.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	//获取服务名
	CString strServiceName = m_ListServices.GetItemText(nSel, 1);
	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//打开服务
	SC_HANDLE hService = OpenService(hSCM, strServiceName,
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);

	//获取服务状态
	SERVICE_STATUS Status;
	if (QueryServiceStatus(hService, &Status) == FALSE)
	{
		MessageBox(L"获取服务状态失败！");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}
	
	if (StartService(hService, NULL, NULL) == FALSE)
	{
		MessageBox(L"启动服务失败！");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}
	//等待服务启动完成
	while (QueryServiceStatus(hService, &Status) == TRUE)
	{
		Sleep(Status.dwWaitHint);
		if (Status.dwCurrentState == SERVICE_RUNNING)
		{
			MessageBox(L"启动服务成功！");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			
		}
	}
	UpDataService();
}


