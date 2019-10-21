#pragma once
#include"CButtonPath.h"

// CClean 对话框

class CClean : public CDialogEx
{
	DECLARE_DYNAMIC(CClean)

public:
	CClean(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CClean();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CClean };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	CString m_filter;
	CString m_dir;
	CComboBox m_comB;
	HANDLE		 m_hScanThread;
	DWORD64 m_totalSize;
	CMenu m_menuFile;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButtonpath();
	CButtonPath m_buttPah;
	afx_msg void OnBnClickedButton3();
protected:
	afx_msg LRESULT OnMyMsgScanfile(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedAll();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClean32776();
	afx_msg void OnClean32777();
	afx_msg void OnClean32775();
};
