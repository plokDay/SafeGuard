#pragma once


// CRESOURCE 对话框

class CRESOURCE : public CDialogEx
{
	DECLARE_DYNAMIC(CRESOURCE)

public:
	CRESOURCE(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRESOURCE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESOURCE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeCtl;
	virtual BOOL OnInitDialog();
	CString m_NumName;
	CString m_NumID;
	HTREEITEM hRoot = NULL;
	HTREEITEM hChild = NULL;

	CString m_NumName2;
	CString m_NumID2;
	CString m_ResRVA;
	CString m_ResFOA;
	CString m_ResSize;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
