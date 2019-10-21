// AHome.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "AHome.h"
#include "afxdialogex.h"
#include <VersionHelpers.h>
#include <Psapi.h>


// AHome 对话框

IMPLEMENT_DYNAMIC(AHome, CDialogEx)

AHome::AHome(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AHOME, pParent)
{

}

AHome::~AHome()
{
}

void AHome::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_proCPU);
	DDX_Control(pDX, IDC_PROGRESS2, m_proMem);
}


BEGIN_MESSAGE_MAP(AHome, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &AHome::OnBnClickedButton1)
END_MESSAGE_MAP()


// AHome 消息处理程序

void AHome::GetSysInfo()
{
	CString sysverion;

	if (IsWindowsServer()) {
		sysverion = L" Windows Server";
	}

	if (IsWindowsXPSP3OrGreater()) {
		sysverion = L" Windows XP";
	}

	if (IsWindowsVistaOrGreater()) {
		sysverion = L" Windows Viata";
	}

	if (IsWindows7OrGreater()) {
		sysverion = L" Windows 7";
	}

	if (IsWindows8OrGreater()) {
		sysverion = L" Windows 8或10";
	}

	if (IsWindows8Point1OrGreater()) {
		sysverion = L" Windows 8.1或10";
	}
	if (SystemIsX32())
	{
		sysverion=sysverion + L"  32位";
	}
	else
	{
		sysverion=sysverion + L"  64位";

	}
	
	SetDlgItemText(IDC_VERSION, sysverion);
}
BOOL AHome::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_proMem.SetRange(0, 100);
	m_proCPU.SetRange(0, 100);
	GetSysInfo();
	//刷新
	SetTimer(0x1008, 1000, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

int GetCpu()
{
	static LONGLONG nFree{}, nNt{}, nUser{};
	static LONGLONG nFreet{}, nNtt{}, nUsert{};
	static DOUBLE nHave{};
	GetSystemTimes((PFILETIME)&nFree, (PFILETIME)&nNt, (PFILETIME)&nUser);
	nHave = (DOUBLE)(nFree - nFreet) / (DOUBLE)(nNt - nNtt + nUser - nUsert);
	nFreet = nFree;
	nNtt = nNt;
	nUsert = nUser;
	return (100.0 - nHave * 100);
}
DWORD GetMem()
{
	MEMORYSTATUSEX nStatex;

	nStatex.dwLength = sizeof(nStatex);

	GlobalMemoryStatusEx(&nStatex);

	DOUBLE nAll = (DOUBLE)nStatex.ullTotalPhys / 1048576;
	DOUBLE nHave = (DOUBLE)nStatex.ullAvailPhys / 1048576;
	return (DWORD)((nAll - nHave) / nAll * 100);
}
void AHome::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent==0x1008)
	{
		int n = GetCpu();
		CString buffer;
		buffer.Format(L"%d%%", n);
		m_proCPU.SetPos(n);
		SetDlgItemText(IDC_CPUUSG, buffer);
		n = GetMem();
		m_proMem.SetPos(n);
		buffer.Format(L"%d%%", n);
		SetDlgItemText(IDC_MEMUSG2, buffer);

	}

	CDialogEx::OnTimer(nIDEvent);
}
void ClearMemory()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 ProcessInfo;
	ProcessInfo.dwSize = sizeof(ProcessInfo);
	int count = 0;
	//获取系统中第一个进程的信息  
	BOOL Status = Process32First(hProcessSnap, &ProcessInfo);
	while (Status)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE,
			ProcessInfo.th32ProcessID);
		if (hProcess)
		{
			//设置进程工作区大小
			SetProcessWorkingSetSize(hProcess, -1, -1);
			//尽可能多的将指定进程的页面从工作区移除
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		//获取下一个进程的信息  
		Status = Process32Next(hProcessSnap, &ProcessInfo);
	}
}

//内存优化
void AHome::OnBnClickedButton1()
{
	//获取清理前的内存信息
	MEMORYSTATUSEX memStatus = { sizeof(MEMORYSTATUSEX) };
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE preUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//开始清理内存
	ClearMemory();
	//获取清理后的内存信息
	GlobalMemoryStatusEx(&memStatus);
	DOUBLE afterUsedMem = (DOUBLE)(memStatus.ullTotalPhys - memStatus.ullAvailPhys) / 1024 / 1024;
	//输出清理信息
	CString m_szStatus;
	m_szStatus.Format(_T("内存清理完毕！本次清理 : %.2f MB"), preUsedMem - afterUsedMem);
	SetDlgItemText(IDC_MEMCLEAR, m_szStatus);

}
