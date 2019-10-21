#pragma once


// CRVATOFOA 对话框

class CRVATOFOA : public CDialogEx
{
	DECLARE_DYNAMIC(CRVATOFOA)

public:
	CRVATOFOA(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRVATOFOA();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RVATOFOA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_VA;
	CString m_RVA;
	CString m_FOA;
	CString m_Section;
	
	afx_msg void OnBnClickedButton1();
	
	
	virtual BOOL OnInitDialog();
};
