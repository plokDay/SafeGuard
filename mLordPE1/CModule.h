#pragma once


// CModule 对话框

class CModule : public CDialogEx
{
	DECLARE_DYNAMIC(CModule)

public:
	CModule(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CModule();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CModule };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listM;
	std::vector<MODULEENTRY32> mmodule;
};
