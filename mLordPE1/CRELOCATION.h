#pragma once


// CRELOCATION 对话框

class CRELOCATION : public CDialogEx
{
	DECLARE_DYNAMIC(CRELOCATION)

public:
	CRELOCATION(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRELOCATION();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RELOCATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_LsCtlSection;
	CListCtrl m_LsCtlModule;
	afx_msg void OnLvnItemchangedList3(NMHDR *pNMHDR, LRESULT *pResult);
};
