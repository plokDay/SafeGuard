#pragma once


// CSECTION 对话框

class CSECTION : public CDialogEx
{
	DECLARE_DYNAMIC(CSECTION)

public:
	CSECTION( CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSECTION();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lsCtr;
	virtual BOOL OnInitDialog();
private:
	
};
