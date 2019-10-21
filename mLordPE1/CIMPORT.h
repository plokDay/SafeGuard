#pragma once


// CIMPORT 对话框

class CIMPORT : public CDialogEx
{
	DECLARE_DYNAMIC(CIMPORT)

public:
	CIMPORT(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CIMPORT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lsCtlImport;
	CListCtrl m_lsCtlFunc;
	virtual BOOL OnInitDialog();
	afx_msg void OnClickShowFunc(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_Check;
	afx_msg void OnBnClickedCheck1();
private:
	int m_nIdx = -1;
	PIMAGE_THUNK_DATA m_pFOAThunk = NULL;
	PIMAGE_THUNK_DATA m_pThunk = NULL;
	PIMAGE_THUNK_DATA m_pRVAThunk = NULL;
};
