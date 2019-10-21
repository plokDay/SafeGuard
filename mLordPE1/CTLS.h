#pragma once


// CTLS 对话框

class CTLS : public CDialogEx
{
	DECLARE_DYNAMIC(CTLS)

public:
	CTLS(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CTLS();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_StartAddressOfRawData;
	CString m_EndAddressOfRawData;
	CString m_AddressOfIndex;
	CString m_AddressOfCallBacks;
	CString m_SizeOfZeroFill;
	CString m_Characteristics;
};
