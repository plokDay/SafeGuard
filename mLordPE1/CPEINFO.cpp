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

	//�����ַ����ĳ���

	int size = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, FALSE);

	//����һ�����ֽڵ��ַ�������

	char* str = new char[sizeof(char) * size];

	//��STRת��str

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
		// ���������̵�Ȩ��

		bFuncReturn = ::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);

		if (bFuncReturn == 0) // ʧ��
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
	// ��ʽ: (�����ں�ʱ�� + �����û�ʱ��) / (CPU�ں�ʱ�� + CPU�û�ʱ��)

	LARGE_INTEGER kernelTime = { 0 };
	LARGE_INTEGER userTime = { 0 };
	getProcessTime(dwPid, &kernelTime, &userTime);

	//			����ʱ��		�ں�ʱ��		�û�ʱ��
	FILETIME cpuIdleTime = {}, cpuKernelTime = {}, cpuUserTime = {};

	// ��ȡʱ��
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

				if (tag==2)//��������ɱ
				{
					CString processMD5 = md5FileValue(pi.szPath);
					for (DWORD i = 0; i < mpe.WhiteLib.size(); i++)//ѭ���������MD5�Ƚ�
					{
						if (mpe.WhiteLib[i].FileMd5 == processMD5)//���MD5�Ǻ�
						{
							pi.nSafe = 1;//��ȫ
						}
						else
						{
							for (DWORD i = 0; i < mpe.VirusLib.size(); i++)//ѭ���벡����Ƚ�
							{
								if (mpe.VirusLib[i].FileMd5 == processMD5)//���MD5�Ǻ�
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

// �����ļ�
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
		// �Ƿ���Ŀ¼
		if (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// �ݹ�����¼�Ŀ¼
			findFiles(path + "\\" + fData.cFileName, filter, hWnd, uMsg);
		}
		else {//������ļ�
			TCHAR notePath[MAX_PATH];
			_stprintf_s(notePath, _countof(notePath), _T("%s\\%s"), dir, fData.cFileName);
			mpe.strNote = notePath;
			if (filter==0)//������ɱ
			{
				
				_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);
 				CString nFileMd5 = md5FileValue(fInfo.path);//��ȡ�ļ�MD5
				//
  				for (DWORD i = 0; i < mpe.VirusLib.size(); i++)//ѭ���벡����MD5�Ƚ�
  				{
  					if (mpe.VirusLib[i].FileMd5== nFileMd5)//���MD5�Ǻ�
  					{
						// ���ļ���Ϣ���浽������
						memcpy(&fInfo, &fData, sizeof(FILEINFO) - sizeof(fInfo.path));
  						//InterlockedIncrement(&m_VirusNum);//��������+1
 						SendMessage(hWnd, uMsg, (WPARAM)&fInfo, 0);
 
  						break;
  					}
  				}
				//

			}
			else if (filter == L"getMD5")//����ļ���MD5
			{
				_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);
				wcscpy(fvirus.FileMd5, md5FileValue(fInfo.path).GetBuffer(MAX_PATH));
				wcscpy(fvirus.FileName,fData.cFileName);


				// ������ھ����Ϊ��, �����ݷ��͵�����
				if (hWnd)
				{
					SendMessage(hWnd, uMsg, (WPARAM)&fvirus, 0);
				}

			}
			else //�����ļ�
			{
				// ��ȡ�ļ���չ��
				TCHAR* pExtName = fData.cFileName + _tcslen(fData.cFileName);
				while (pExtName != fData.cFileName && *pExtName != '.')
					--pExtName;
				// �ж���չ���Ƿ���Ҫ���˳������ļ���չ��
				if (_tcsstr(filter, pExtName) != NULL) {
					// ���ļ���Ϣ���浽������
					memcpy(&fInfo, &fData, sizeof(FILEINFO) - sizeof(fInfo.path));
					// ƴ��������·��
					_stprintf_s(fInfo.path, _countof(fInfo.path), _T("%s\\%s"), dir, fData.cFileName);


					// ������ھ����Ϊ��, �����ݷ��͵�����
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
	//�ļ��������ڴ�������
	if (mpe.pNt->OptionalHeader.FileAlignment == mpe.pNt->OptionalHeader.SectionAlignment)
	{
		return dwRva;
	}
	DWORD offset = 0;
	//1.�ж����RVA�����ĸ�����
	
	DWORD dwSectionCount = pNt->FileHeader.NumberOfSections;

	for (int i = 0; i < dwSectionCount; ++i)
	{
		if (dwRva >= pSection[i].VirtualAddress
			&& dwRva < (pSection[i].SizeOfRawData + pSection[i].VirtualAddress))
		{
			//2.�����ҵ�����ݹ�ʽ����
			offset = dwRva - pSection[i].VirtualAddress + pSection[i].PointerToRawData;
		}
	}
	return offset;
}

CString CPEINFO::GetSection(DWORD dwRva)
{
	//1.�ж����RVA�����ĸ�����
	CString section;
	DWORD dwSectionCount = mpe.pNt->FileHeader.NumberOfSections;

	for (int i = 0; i < dwSectionCount; ++i)
	{
		if (dwRva >= mpe.pSection[i].VirtualAddress
			&& dwRva < (mpe.pSection[i].SizeOfRawData + mpe.pSection[i].VirtualAddress))
		{
			//2.�����ҵ�����ݹ�ʽ����
			section = (CString)mpe.pSection[i].Name;
		}
	}
	return section;
}
bool SystemIsX32() {
	SYSTEM_INFO si;
	// ��ȡϵͳ��Ϣ
	GetNativeSystemInfo(&si);
	// �ж�ϵͳ�ܹ�
	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64
		|| si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		// 64λϵͳ
		return false;
	}
	else {
		// 32λϵͳ
		return true;
	}
}