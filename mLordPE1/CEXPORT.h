#pragma once


// CEXPORT 对话框

class CEXPORT : public CDialogEx
{
	DECLARE_DYNAMIC(CEXPORT)

public:
	CEXPORT(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CEXPORT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_Base;
	CString m_Foa;
	CString m_AddressOfFunctions;
	CString m_NumberOfFunctions;
	CString m_AddressOfOridinals;
	CString m_NumberOfNames;
	CString m_AddressOfNames;
	CListCtrl m_lsCtl;
	virtual BOOL OnInitDialog();
};
