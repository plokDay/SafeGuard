#pragma once


// CDELAY 对话框

class CDELAY : public CDialogEx
{
	DECLARE_DYNAMIC(CDELAY)

public:
	CDELAY(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDELAY();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DELAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lsCtlDll;
	CListCtrl m_lsCtlFunc;

	
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
};
