#pragma once

// CWindows 对话框

class CWindows : public CDialogEx
{
	DECLARE_DYNAMIC(CWindows)

public:
	CWindows(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWindows();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CWindows };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public:
	CListCtrl m_listW;
	std::vector<HWND> m_wnd;
	CImageList   m_imgList; // 图标链表
	virtual BOOL OnInitDialog();

};
