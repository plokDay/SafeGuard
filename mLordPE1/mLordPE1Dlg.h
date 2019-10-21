
// mLordPE1Dlg.h: 头文件
//

#pragma once

// CmLordPE1Dlg 对话框
class CmLordPE1Dlg : public CDialogEx
{
// 构造
public:
	CmLordPE1Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MLORDPE1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatusBar  m_bar;
	CTabCtrl m_tabCtl;
	std::vector<CDialogEx*> m_tabSubWnd;
	void addTabWnd(const CString& title, CDialogEx* pSubWnd, UINT uId);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	bool isHide=true;
	// 查询权限
	BOOL QueryPrivileges();
	// 提升权限
	afx_msg void OnBnClickedButton1();


	void CmLordPE1Dlg::mpower(int n);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
