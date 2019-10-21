#pragma once


// CHomePage 对话框

class CHomePage : public CDialogEx
{
	DECLARE_DYNAMIC(CHomePage)

public:
	CHomePage(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CHomePage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Home };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_comBVirus;
	CListCtrl m_lsCtlVirus;
	afx_msg void OnCbnSelchangeCombo1();
	CMenu m_menuVirus;
	CString m_pathVirus = L"";
public:
	afx_msg void OnBnClickedButton1();
protected:
	afx_msg LRESULT OnMyMsgScanvirus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMyMsgScanmd5(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_staticPath;
	BOOL m_IsScaning = FALSE;//是否正在扫描
	std::vector<HANDLE> vecHThread;
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClean32776();
	afx_msg void OnClean32777();
	void pushVirusLib();
	
protected:
	afx_msg LRESULT OnMyMsgScanwhite(WPARAM wParam, LPARAM lParam);
public:
	CString m_response;
	
	afx_msg void OnClose();
	afx_msg void OnClean32775();
};
