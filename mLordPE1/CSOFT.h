#pragma once


// CSOFT 对话框
typedef struct _SOFTWARE
{
	WCHAR szSoftName[50];
	WCHAR szSoftVer[50];
	WCHAR szSoftDate[20];
	WCHAR szSoftSize[MAX_PATH];
	WCHAR szSoftInsPath[MAX_PATH];
	WCHAR szSoftUniPath[MAX_PATH];
	WCHAR szSoftPub[50];
	WCHAR szSoftIco[MAX_PATH];
}SOFTWARE, *PSOFTWARE;
class CSOFT : public CDialogEx
{
	DECLARE_DYNAMIC(CSOFT)

public:
	CSOFT(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSOFT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOFT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listSoft;
	std::vector<SOFTWARE> m_vecSoft;
	HKEY RootKey = HKEY_LOCAL_MACHINE;//主键
	CString subKey32 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	CString subkey64 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	HKEY hkResult;//接收将要打开的键的句柄
	CMenu m_menuSoft;
	void GetSoft();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSoft32781();
};
