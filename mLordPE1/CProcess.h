#pragma once

class CProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CProcess)

public:
	CProcess(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CProcess();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CProcess };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:

	HANDLE hFile;
	TCHAR DLLPATH[MAX_PATH];
	CListCtrl m_list;
	CMenu m_menu;
	std::vector<PROCESSINFO> m_procList;

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void WINAPI RefreshPro();
	
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFilr32771();
	afx_msg void OnFilr32772();
	afx_msg void OnFilr32773();
	afx_msg void OnFilr32780();
	afx_msg void OnFilr32782();
};
