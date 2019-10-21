#include "stdafx.h"
#include "Md5.h"
#include <windows.h>
#include "psapi.h"
#include <winternl.h>
CPEINFO::CPEINFO()
{
}


CPEINFO::~CPEINFO()
{
}



DWORD GetProcessPid(CString nProcessName)
{
	PROCESSENTRY32 nPT;
	nPT.dwSize = sizeof(nPT);
	HANDLE nSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	BOOL nRet = Process32First(nSnapShot, &nPT);
	while (nRet)
	{
		if (nProcessName == CString(nPT.szExeFile))
		{
			return nPT.th32ProcessID;
		}
		nRet = Process32Next(nSnapShot, &nPT);
	}
	return 0;
}
char* TCHAR2char(const TCHAR* STR)

{

	//返回字符串的长度

	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, FALSE);

	//申请一个多字节的字符串变量

	char* str = new char[sizeof(char) * size];

	//将STR转成str

	WideCharToMultiByte(CP_ACP, 0, STR, -1, str, size, NULL, FALSE);

	return str;

}
void GetProcessUserName(HANDLE hProcess, TCHAR* strUserName)
{
	HANDLE hToken = NULL;
	BOOL bFuncReturn = FALSE;

	PTOKEN_USER pToken_User = NULL;
	DWORD dwTokenUser = 0;
	TCHAR szAccName[MAX_PATH] = { 0 };
	TCHAR szDomainName[MAX_PATH] = { 0 };
	HANDLE hProcessToken = NULL;

	if (hProcess != NULL)
	{
		// 提升本进程的权限

		bFuncReturn = ::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);

		if (bFuncReturn == 0) // 失败
		{
			return;
		}

		if (hToken != NULL)
		{
			::GetTokenInformation(hToken, TokenUser, NULL, 0L, &dwTokenUser);

			if (dwTokenUser > 0)
			{
				pToken_User = (PTOKEN_USER)::GlobalAlloc(GPTR, dwTokenUser);
			}

			if (pToken_User != NULL)
			{
				bFuncReturn = ::GetTokenInformation(hToken, TokenUser, pToken_User, dwTokenUser, &dwTokenUser);
			}

			if (bFuncReturn != FALSE && pToken_User != NULL)
			{
				SID_NAME_USE eUse = SidTypeUnknown;

				DWORD dwAccName = 0L;
				DWORD dwDomainName = 0L;

				PSID  pSid = pToken_User->User.Sid;

				bFuncReturn = ::LookupAccountSid(NULL, pSid, NULL, &dwAccName,
					NULL, &dwDomainName, &eUse);
				if (dwAccName > 0 && dwAccName < MAX_PATH && dwDomainName>0 && dwDomainName <= MAX_PATH)
				{
					bFuncReturn = ::LookupAccountSid(NULL, pSid, szAccName, &dwAccName,
						szDomainName, &dwDomainName, &eUse);
				}

				if (bFuncReturn != 0)
					wcsncpy_s(strUserName, MAX_PATH, szAccName, MAX_PATH);
			}
		}
	}

	if (pToken_User != NULL)
	{
		::GlobalFree(pToken_User);
	}

	if (hToken != NULL)
	{
		::CloseHandle(hToken);
	}


}
double getProcessMemoryUsage(DWORD dwPid)
{
	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		dwPid);
	double usage = 0;
	if (hProc != NULL) {

		PROCESS_MEMORY_COUNTERS pmc = { sizeof(PROCESS_MEMORY_COUNTERS) };;
		GetProcessMemoryInfo(hProc, &pmc, sizeof(pmc));
		usage = pmc.WorkingSetSize;
		CloseHandle(hProc);
	}
	return usage / 1024;
}
void getProcessTime(DWORD dwPid, LARGE_INTEGER* kernelTIme, LARGE_INTEGER* userTime)
{
	typedef struct _KERNEL_USERTEXTIMES
	{
		LARGE_INTEGER CreateTime;
		LARGE_INTEGER ExitTime;
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
	} KERNEL_USERTEXTIMES, *PKERNEL_USERTEXTIMES;
	KERNEL_USERTEXTIMES time;
#define ProcessTimes  4

	HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	if (hProc == NULL) {
		return;
	}
	decltype(NtQueryInformationProcess)* fnNtQueryInformationProcess;
	fnNtQueryInformationProcess = (decltype(NtQueryInformationProcess)*)
		(GetProcAddress(GetModuleHandle(_T("ntdll.dll")), "NtQueryInformationProcess"));

	DWORD read = 0;
	fnNtQueryInformationProcess(hProc,
		(PROCESSINFOCLASS)ProcessTimes,
		&time,
		sizeof(time),
		&read);
	*kernelTIme = time.KernelTime;
	*userTime = time.UserTime;
	CloseHandle(hProc);
}
double	FILETIME2Double(const _FILETIME&	fileTime)
{
	return	double(fileTime.dwHighDateTime*4.294967296e9) + double(fileTime.dwLowDateTime);
}
double getProcessCpuUsage(DWORD dwPid)
{
	// 公式: (进程内核时间 + 进程用户时间) / (CPU内核时间 + CPU用户时间)

	LARGE_INTEGER kernelTime = { 0 };
	LARGE_INTEGER userTime = { 0 };
	getProcessTime(dwPid, &kernelTime, &userTime);

	//			空闲时间		内核时间		用户时间
	FILETIME cpuIdleTime = {}, cpuKernelTime = {}, cpuUserTime = {};

	// 获取时间
	GetSystemTimes((FILETIME*)&cpuIdleTime, (FILETIME*)&cpuKernelTime, (FILETIME*)&cpuUserTime);

	double proc = FILETIME2Double(*(FILETIME*)&kernelTime) + FILETIME2Double(*(FILETIME*)&userTime);
	double cpu = FILETIME2Double(cpuKernelTime) + FILETIME2Double(cpuUserTime);
	return proc / cpu * 100.0;
}
BOOL getAllProc(std::vector<PROCESSINFO>* procList,int tag, HWND hWnd, UINT uMsg)
{
	int nRes = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		nRes = 1;
		PROCESSENTRY32 proc = { sizeof(PROCESSENTRY32) };
		Process32First(hSnap, &proc);
		
		do
		{
			nRes++;
			HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, proc.th32ProcessID);
			PROCESSINFO pi;
			memcpy(&pi, &proc, sizeof(proc));


			if (hProc == NULL) {

				pi.dwMemUsg = 0;
				pi.dwCpuUsg = 0;
				wcsncpy_s(pi.szPath, L"--", 2);
				wcsncpy_s(pi.szUserName, L"--", 2);
			}
			else {
				GetProcessUserName(hProc, pi.szUserName);
				GetModuleFileNameEx(hProc, 0, pi.szPath, MAX_PATH);
				CloseHandle(hProc);
				mpe.strNote = pi.szExeFile;
				pi.dwMemUsg = getProcessMemoryUsage(pi.th32ProcessID);
				pi.dwCpuUsg = getProcessCpuUsage(pi.th32ProcessID);

				if (tag==2)//白名单查杀
				{
					CString processMD5 = md5FileValue(pi.szPath);
					for (DWORD i = 0; i < mpe.WhiteLib.size(); i++)//循环与白名单MD5比较
					{
						if (mpe.WhiteLib[i].FileMd5 == processMD5)//如果MD5吻合
						{
							pi.nSafe = 1;//安全
						}
						else
						{
							for (DWORD i = 0; i < mpe.VirusLib.size(); i++)//循环与病毒库比较
							{
								if (mpe.VirusLib[i].FileMd5 == processMD5)//如果MD5吻合
								{
									pi.nSafe = 0;

								}
								else pi.nSafe = 1;
							}
						}
					}
					SendMessage(hWnd, uMsg, (WPARAM)&pi, 0);
				}
			}
			procList->push_back(pi);
			
			
		} while (Process32Next(hSnap, &proc));
	}
	return nRes;
}

// 查找文件
bool CPEINFO::findFiles(const TCHAR* dir, const TCHAR* filter, HWND hWnd, UINT uMsg)
{
	CString path = dir;
	WIN32_FIND_DATA fData = { 0 };
	HANDLE hFind = FindFirstFile(path + _T("\\*"), &fData);
	VIRUSINFO fvirus = { 0 };

	FILEINFO fInfo = { 0 };
	
	if (hFind == INVALID_HANDLE_VALUE) {

		return false;
	}
	do
	{
		if (_tcscmp(fData.cFileName, _T(".")) == 0 || _tcscmp(fData.cFileName, _T("..")) == 0) {
			continue;
		}
		// 是否是目录
		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// 递归遍历下级目录
			findFiles(path + "\\" + fData.cFileName, filter, hWnd, uMsg);
		}
		else {//如果是文件
			TCHAR notePath[MAX_PATH];
			_stprintf_s(notePath, _countof(notePath), _T("%s\\%s"), dir, fData.cFileName);
			mpe.strNote = notePath;
			if (filter==0)//病毒查杀
			{
				
				_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);
 				CString nFileMd5 = md5FileValue(fInfo.path);//获取文件MD5
				//
  				for (DWORD i = 0; i < mpe.VirusLib.size(); i++)//循环与病毒库MD5比较
  				{
  					if (mpe.VirusLib[i].FileMd5== nFileMd5)//如果MD5吻合
  					{
						// 将文件信息保存到数组中
						memcpy(&fInfo, &fData, sizeof(FILEINFO) - sizeof(fInfo.path));
  						//InterlockedIncrement(&m_VirusNum);//病毒数量+1
 						SendMessage(hWnd, uMsg, (WPARAM)&fInfo, 0);
 
  						break;
  					}
  				}
				//

			}
			else if (filter == L"getMD5")//获得文件的MD5
			{
				_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);
				wcscpy(fvirus.FileMd5, md5FileValue(fInfo.path).GetBuffer(MAX_PATH));
				wcscpy(fvirus.FileName,fData.cFileName);


				// 如果窗口句柄不为空, 则将数据发送到窗口
				if (hWnd)
				{
					SendMessage(hWnd, uMsg, (WPARAM)&fvirus, 0);
				}

			}
			else //查找文件
			{
				// 获取文件扩展名
				TCHAR* pExtName = fData.cFileName + _tcslen(fData.cFileName);
				while (pExtName != fData.cFileName && *pExtName != '.')
					--pExtName;
				// 判断扩展名是否是要过滤出来的文件扩展名
				if (_tcsstr(filter, pExtName) != NULL) {
					// 将文件信息保存到数组中
					memcpy(&fInfo, &fData, sizeof(FILEINFO) - sizeof(fInfo.path));
					// 拼接完整的路径
					_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);


					// 如果窗口句柄不为空, 则将数据发送到窗口
					if (hWnd)
					{
						SendMessage(hWnd, uMsg, (WPARAM)&fInfo, 0); 
					}
				}
			}
		}
	} while (FindNextFile(hFind, &fData));
	return true;
}
DWORD CPEINFO::Rva2Foa(DWORD dwRva)
{
	//文件对齐与内存对齐相等
	if (mpe.pNt->OptionalHeader.FileAlignment == mpe.pNt->OptionalHeader.SectionAlignment)
	{
		return dwRva;
	}
	DWORD offset = 0;
	//1.判断这个RVA落在哪个区段
	
	DWORD dwSectionCount = pNt->FileHeader.NumberOfSections;

	for (int i = 0; i < dwSectionCount; ++i)
	{
		if (dwRva >= pSection[i].VirtualAddress
			&& dwRva < (pSection[i].SizeOfRawData + pSection[i].VirtualAddress))
		{
			//2.遍历找到后根据公式计算
			offset = dwRva - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	return offset;
}

CString CPEINFO::GetSection(DWORD dwRva)
{
	//1.判断这个RVA落在哪个区段
	CString section;
	DWORD dwSectionCount = mpe.pNt->FileHeader.NumberOfSections;

	for (int i = 0; i < dwSectionCount; ++i)
	{
		if (dwRva >= mpe.pSection[i].VirtualAddress
			&& dwRva < (mpe.pSection[i].SizeOfRawData + mpe.pSection[i].VirtualAddress))
		{
			//2.遍历找到后根据公式计算
			section = (CString)mpe.pSection[i].Name;
		}
	}
	return section;
}
bool SystemIsX32() {
	SYSTEM_INFO si;
	// 获取系统信息
	GetNativeSystemInfo(&si);
	// 判断系统架构
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64
		|| si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		// 64位系统
		return false;
	}
	else {
		// 32位系统
		return true;
	}
}