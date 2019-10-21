#pragma once


// CService 对话框

class CService : public CDialogEx
{
	DECLARE_DYNAMIC(CService)

public:
	CService(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CService();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CService };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListServices;
	virtual BOOL OnInitDialog();
	BOOL GetServices();
	void UpDataService();
	CMenu m_menuService;
	const WCHAR* ServiceStatus[8] = {	//服务状态
	L" ",
	L"已停止",
	L"正在启动",
	L"正在停止",
	L"正在运行",
	L"即将继续",
	L"暂停中",
	L"已暂停",
	};

	const WCHAR* ServerStartType[6] = {	//服务启动类型
		L" ",
		L" ",
		L"自动",
		L"手动",
		L"禁用",
		L" "
	};
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnService32778();
	afx_msg void OnService32779();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();

	afx_msg void OnFilr32782();

};
