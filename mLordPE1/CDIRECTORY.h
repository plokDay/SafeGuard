#pragma once


// CDIRECTORY 对话框

class CDIRECTORY : public CDialogEx
{
	DECLARE_DYNAMIC(CDIRECTORY)

public:
	CDIRECTORY(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDIRECTORY();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECTORY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_ExportRVA;
	CString m_ExportSize;
	CString m_ImportRVA;
	CString m_ImportSize;
	CString m_ResourceRVA;
	CString m_ResourceSize;
	CString m_ExceptRVA;
	CString m_ExceptSize;
	CString m_SecureRVA;
	CString m_SecureSize;
	CString m_RelocRVA;
	CString m_RelocSize;
	CString m_DebugRVA;
	CString m_DebugSize;
	CString m_CopyRightRVA;
	CString m_CopyRightSize;
	CString m_GlobalRVA;
	CString m_GlobalSize;
	CString m_TLSRVA;
	CString m_TLSSize;
	CString m_LoadRVA;
	CString m_LoadSize;
	CString m_BoundRVA;
	CString m_BoundSize;
	CString m_ITARVA;
	CString m_ITASize;
	CString m_DelayRVA;
	CString m_DelaySize;
	CString m_COMRVA;
	CEdit m_COMSize;
	CString m_ReserveRVA;
	CString m_ReserveSize;
	virtual BOOL OnInitDialog();

public:
	CString m_COMSIZE;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
