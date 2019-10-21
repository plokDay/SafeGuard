// CButtonPath.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CButtonPath.h"


// CButtonPath

IMPLEMENT_DYNAMIC(CButtonPath, CButton)

CButtonPath::CButtonPath()
{

}

CButtonPath::~CButtonPath()
{
}


BEGIN_MESSAGE_MAP(CButtonPath, CButton)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CButtonPath 消息处理程序




void CButtonPath::OnDropFiles(HDROP hDropInfo)
{
	//获取文件路径
	TCHAR  szPatch[MAX_PATH] = { 0 };
	DragQueryFile(hDropInfo, 0, szPatch, MAX_PATH);

	SetWindowText(szPatch);
	
	CButton::OnDropFiles(hDropInfo);
}





BOOL CButtonPath::PreTranslateMessage(MSG* pMsg)
{
	

	return CButton::PreTranslateMessage(pMsg);
}
