#pragma once
#include <TlHelp32.h>
#include <vector>
#define MAX_PATH  500
typedef struct _VIRUSINFO
{
	TCHAR FileName[MAX_PATH]{0};
	TCHAR FileMd5[MAX_PATH]{0};
}VIRUSINFO, *PVIRUSINFO;
struct PROCESSINFO :PROCESSENTRY32
{
	double dwMemUsg;
	double dwCpuUsg;
	int nSafe=1;
	TCHAR szPath[MAX_PATH] = { 0 };
	TCHAR szUserName[MAX_PATH] = { 0 };
};
class CPEINFO
{
public:
	CPEINFO();
	~CPEINFO();
	//PIMAGE_NT_HEADERS32 pNtH;	//NTͷ
	//PIMAGE_NT_HEADERS64 pNtH64;	//NTͷ64
	PIMAGE_NT_HEADERS pNt=NULL;	//NTͷ

	IMAGE_FILE_HEADER* m_pFileHdr;		//�ļ�ͷ
	//PIMAGE_OPTIONAL_HEADER32 pOptH;//��ѡͷ
	//PIMAGE_OPTIONAL_HEADER64 pOptH64;//64λ�Ŀ�ѡPEͷ
	PIMAGE_SECTION_HEADER pSection=NULL;//����ͷ
	PIMAGE_DATA_DIRECTORY pDirectory=NULL;//����Ŀ¼
	PIMAGE_EXPORT_DIRECTORY pExport=NULL;// ����������
	PIMAGE_IMPORT_DESCRIPTOR pImport = NULL;
	PIMAGE_RESOURCE_DIRECTORY pResource = NULL;
	PIMAGE_BASE_RELOCATION pRelocal = NULL;
	PIMAGE_TLS_DIRECTORY pTLS = NULL;
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = NULL;
	unsigned char* buff;
	CString strNote;
	DWORD Rva2Foa(DWORD dwRva);
	CString GetSection(DWORD dwRva);
	bool findFiles(const TCHAR* dir, const TCHAR* filter, HWND hWnd, UINT uMsg);
	//PVIRUSINFO VirusLib;
	//PVIRUSINFO WhiteLib;
	std::vector<VIRUSINFO> WhiteLib;//���ذ�����
	std::vector<VIRUSINFO> VirusLib;//���ز�����

	//int numVirus;
	//int numWrite;
};

struct FILEINFO {
	DWORD dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	DWORD nFileSizeHigh;
	DWORD nFileSizeLow;
	TCHAR path[MAX_PATH];
};

char* TCHAR2char(const TCHAR* STR);
BOOL getAllProc(std::vector<PROCESSINFO>* procList, int tag, HWND hWnd, UINT uMsg);
bool SystemIsX32();
DWORD GetProcessPid(CString nProcessName);