// CProcess.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CProcess.h"
#include "afxdialogex.h"
#include "resource.h"
#include <Psapi.h>
#include <winternl.h>
#include "CThread.h"
#include "CModule.h"

//#define DLLPATH "E:\\office2010\\3298663070\\FileRecv\\window原理006day\\x64\\Debug\03_inlineHook.dll"


// CProcess 对话框

IMPLEMENT_DYNAMIC(CProcess, CDialogEx)

CProcess::CProcess(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CProcess, pParent)
{

}

CProcess::~CProcess()
{
}

void CProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CProcess, CDialogEx)
	ON_WM_TIMER()
	
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CProcess::OnNMRClickList1)
	ON_COMMAND(ID_FILR_32771, &CProcess::OnFilr32771)
	ON_COMMAND(ID_FILR_32772, &CProcess::OnFilr32772)
	ON_COMMAND(ID_FILR_32773, &CProcess::OnFilr32773)
	ON_COMMAND(ID_FILR_32780, &CProcess::OnFilr32780)
	ON_COMMAND(ID_FILR_32782, &CProcess::OnFilr32782)
END_MESSAGE_MAP()


// CProcess 消息处理程序


BOOL CProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_menu.LoadMenu(IDRETRY);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, L"索引", 0, 45);
	m_list.InsertColumn(1, L"进程名", 0, 120);
	m_list.InsertColumn(2, L"PID", 0, 50);
	m_list.InsertColumn(3, L"CPU", 0, 70);
	m_list.InsertColumn(4, L"内存", 0, 70);
	m_list.InsertColumn(5, L"线程数", 0, 60);
	m_list.InsertColumn(6, L"用户名", 0, 100);
	m_list.InsertColumn(7, L"路径", 0, 300);

	DWORD dwSize=MAX_PATH;
	TCHAR PATH[MAX_PATH] = { 0 };
	CString buffer;
	QueryFullProcessImageName(GetCurrentProcess(), 0, PATH, &dwSize);
	PathRemoveFileSpec(PATH);
	buffer.Format(L"%s", PATH);
	buffer = buffer + L"\\Dll1.dll";
	wcscpy_s(DLLPATH, buffer);

	SetTimer(0,500,
		NULL/*回调函数, 如果为空,那么本窗口将接收到WM_TIMER的消息*/);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CProcess::OnTimer(UINT_PTR nIDEvent)
{
	RefreshPro();

	CDialogEx::OnTimer(nIDEvent);
}


void WINAPI CProcess::RefreshPro()
{
	std::vector<PROCESSINFO> newProcList;

	getAllProc(&newProcList,true,NULL,NULL);
	if (m_procList.size() == 0)
	{
		m_procList.swap(newProcList);
	}
	else if (newProcList.size() > m_procList.size())
	{
		CString buffer;
		int index = 0;
		for (int i=0;i< newProcList.size()- m_procList.size();++i)
		{
			m_procList.push_back(newProcList[m_procList.size()+i]);
			index = m_procList.size()-1;
			buffer.Format(L"%d", index);
			m_list.InsertItem(index, buffer);

			m_list.SetItemText(index, 1, (CString)m_procList[index].szExeFile);
			buffer.Format(_T("%d"), m_procList[index].th32ProcessID);
			m_list.SetItemText(index, 2, buffer);
			buffer.Format(_T("%.4lf"), m_procList[index].dwCpuUsg);
			m_list.SetItemText(index, 3, buffer);
			buffer.Format(_T("%.0lfKb"), m_procList[index].dwMemUsg);
			m_list.SetItemText(index, 4, buffer);
			buffer.Format(_T("%d"), m_procList[index].cntThreads);

			m_list.SetItemText(index, 5, buffer);
			m_list.SetItemText(index, 6, m_procList[index].szUserName);
			m_list.SetItemText(index, 7, m_procList[index].szPath);
		}
		return;
	}
	else if (newProcList.size() < m_procList.size())
	{
		m_procList.pop_back();
		m_list.DeleteItem(m_procList.size());
		return;
	}
	else if (newProcList.size() == m_procList.size())
	{
		return;
	}

	int index = 0;
	for (auto&i : m_procList) {
		// 插入一行节点
		//m_procList.push_back(i);
		// 插入到列表控件中
		CString buffer;
		buffer.Format(L"%d", index);
		m_list.InsertItem(index, buffer);
		buffer.Format(_T("%d"), i.th32ProcessID);
		m_list.SetItemText(index, 1, (CString)i.szExeFile);
		m_list.SetItemText(index, 2, buffer);
		buffer.Format(_T("%.4lf"), i.dwCpuUsg);
		m_list.SetItemText(index, 3,buffer);
		buffer.Format(_T("%.0lfKb"), i.dwMemUsg);
		m_list.SetItemText(index, 4, buffer);
		buffer.Format(_T("%d"), i.cntThreads);

		m_list.SetItemText(index, 5,buffer);

		m_list.SetItemText(index, 6, i.szUserName);
		m_list.SetItemText(index, 7,i.szPath);


		index++;
	}
}


void CProcess::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu* pSubMenu = m_menu.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
	*pResult = 0;
}

//结束进程
void CProcess::OnFilr32771()
{
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;


	CString strPid = m_list.GetItemText(index,2);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	if (hProc == 0) {
		CString error;
		error.Format(L"打开进程失败:%d", GetLastError());
		MessageBox(error);
	}
	else {
		TerminateProcess(hProc, 0);
		CloseHandle(hProc);
	}
}

void EnumThread(DWORD dwPid, std::vector<THREADENTRY32>* thdList)
{
	//1.创建线程快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

	//2.第一次遍历线程
	THREADENTRY32 th32 = { sizeof(th32) };
	if (Thread32First(hSnap, &th32))
	{
		//3.循环遍历线程
		do
		{
			//判断该线程是否属于这个进程
			if (th32.th32OwnerProcessID == dwPid)
			{
				thdList->push_back(th32);
			}

		} while (Thread32Next(hSnap, &th32));
	}

	CloseHandle(hSnap);
}
//遍历线程
void CProcess::OnFilr32772()
{
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;

	CString strPid = m_list.GetItemText(index, 2);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	CThread mThread;
	EnumThread(dwPid, &mThread.m_thread);
	mThread.DoModal();
}

void EnumModule(DWORD dwPid, std::vector<MODULEENTRY32>* modList)
{
	//1.创建模块快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	if (hSnap == INVALID_HANDLE_VALUE) {
		return;
	}

	//2.第一次遍历模块
	MODULEENTRY32 stcMd = { sizeof(stcMd) };
	if (Module32First(hSnap, &stcMd))
	{
		//3.循环遍历模块Next
		do
		{
			modList->push_back(stcMd);
		} while (Module32Next(hSnap, &stcMd));
	}
	CloseHandle(hSnap);

}
//遍历模块
void CProcess::OnFilr32773()
{
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;

	CString strPid = m_list.GetItemText(index, 2);// 第1列是pid
	DWORD dwPid = _wtoi(strPid);
	CModule mModule;
	EnumModule(dwPid, &mModule.mmodule);
	mModule.DoModal();
}

void CProcess::OnFilr32780()
{
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;
	CString strPath = m_list.GetItemText(index, 7);//路径
	ShellExecute(NULL,
		_T("open"),
		_T("Explorer.exe"),
		L"/select,"+strPath,
		NULL,
		SW_SHOWDEFAULT);
}

//hook
void CProcess::OnFilr32782()
{
	hFile = CreateFileMapping(NULL, NULL, PAGE_EXECUTE_READWRITE, 0, 0x1000, L"WU");

	VOID * pFile = MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, NULL, NULL, 0x1000);
	int index = (int)m_list.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;

	CString strPid = m_list.GetItemText(index, 2);// 第1列是pid
	WCHAR * tempPID = strPid.GetBuffer();
	DWORD dwPID = _wtoi64(tempPID);
	//输入完保护的进程的PID了
	//要传给DLL了
	//那么先放到pFile中
	*(DWORD64*)pFile = dwPID;

	// 1.打开目标进程
	HWND hWnd = ::FindWindowW(NULL, L"任务管理器");
	if (hWnd==NULL)
	{
		MessageBoxW(L"没有打开任务管理器", L"警告", 0);
		return;
	}
	DWORD dwTaskPID;

	GetWindowThreadProcessId(hWnd, &dwTaskPID);//拿到任务管理的

		// 1.打开目标进程
	HANDLE hProcess = OpenProcess(
		PROCESS_ALL_ACCESS,		// 打开权限
		FALSE,					// 是否继承
		dwTaskPID);						// 进程PID

// 2.在目标进程中申请空间
	LPVOID lpPathAddr = VirtualAllocEx(
		hProcess,					// 目标进程句柄
		0,							// 指定申请地址
		wcslen(DLLPATH) * 2 + 2,	// 申请空间大小
		MEM_RESERVE | MEM_COMMIT,	// 内存的状态
		PAGE_READWRITE);			// 内存属性


	// 3.在目标进程中写入Dll路径
	SIZE_T dwWriteSize = 0;
	WriteProcessMemory(
		hProcess,					// 目标进程句柄
		lpPathAddr,					// 目标进程地址
		DLLPATH,					// 写入的缓冲区
		wcslen(DLLPATH) * 2 + 2,	// 缓冲区大小
		&dwWriteSize);				// 实际写入大小

	// 4.在目标进程中创建线程
	HANDLE hThread = CreateRemoteThread(
		hProcess,					// 目标进程句柄
		NULL,						// 安全属性
		NULL,						// 栈大小
		(PTHREAD_START_ROUTINE)LoadLibrary,	// 回调函数
		lpPathAddr,					// 回调函数参数
		NULL,						// 标志
		NULL						// 线程ID
	);

	// 5.等待线程结束
	WaitForSingleObject(hThread, -1);

	// 6.清理环境
	VirtualFreeEx(hProcess, lpPathAddr, 0, MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);

}
