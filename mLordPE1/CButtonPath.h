#pragma once


// CButtonPath

class CButtonPath : public CButton
{
	DECLARE_DYNAMIC(CButtonPath)

public:
	CButtonPath();
	virtual ~CButtonPath();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


