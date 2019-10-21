#pragma once


// CLORDPE 对话框

class CLORDPE : public CDialogEx
{
	DECLARE_DYNAMIC(CLORDPE)

public:
	CLORDPE(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLORDPE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LORDPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	
	bool InitPE(TCHAR *path);
	bool IsPE(unsigned char* pbuff);

	PIMAGE_DOS_HEADER GetDosHeader();
	PIMAGE_NT_HEADERS GetNTHeaders();
	PIMAGE_SECTION_HEADER GetSectionHeader();
	PIMAGE_EXPORT_DIRECTORY GetExportDirectory();
	PIMAGE_IMPORT_DESCRIPTOR GetImportDescriptor();
	PIMAGE_RESOURCE_DIRECTORY GetResourceDirectory();
	PIMAGE_BASE_RELOCATION GetRelocaltion();
	PIMAGE_TLS_DIRECTORY GetTLSDirectory();
	PIMAGE_DELAYLOAD_DESCRIPTOR GetDelayLoadDirectory();
	

	void ShowInfo();

private:
	TCHAR* m_path;
	DWORD m_size;
	unsigned char* m_buff;
public:
	CString m_AddressOfEntryPoint;
	CString m_ImageBase;
	CString m_TimeDateStamp;
	CString m_NumberOfSections;
	CString m_SubSystem;
	CString m_SizeOfImage;
	CString m_SectionAlignment;
	CString m_FileAlignment;
	CString m_BaseOfCode;
	CString m_BaseOfData;
	CString m_SizeOfOptionalHeader;
	CString m_SizeOfHeaders;
	CString m_DllCharacteristics;
	CString m_CheckSum;
	CString m_Magic;
	CString m_NumberOfRvaAndSizes;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	CButton m_CButPath;
};
