#pragma once


// AHome 对话框

class AHome : public CDialogEx
{
	DECLARE_DYNAMIC(AHome)

public:
	AHome(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AHome();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AHOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CProgressCtrl m_proCPU;
	CProgressCtrl m_proMem;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void GetSysInfo();
	afx_msg void OnBnClickedButton1();
};
