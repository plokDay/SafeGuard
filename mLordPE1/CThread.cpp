// CThread.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CThread.h"
#include "afxdialogex.h"


// CThread 对话框

IMPLEMENT_DYNAMIC(CThread, CDialogEx)

CThread::CThread(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CThread, pParent)
{

}

CThread::~CThread()
{
}

void CThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listT);
}


BEGIN_MESSAGE_MAP(CThread, CDialogEx)
END_MESSAGE_MAP()


// CThread 消息处理程序

void GetThreadState(CString &nState, DWORD nThreadid)
{
	HANDLE nHandle = OpenThread(PROCESS_ALL_ACCESS, FALSE, nThreadid);

	DWORD WaitCode;
	WaitCode = WaitForSingleObject(nHandle, 0);

	CloseHandle(nHandle);

	switch (WaitCode)
	{
	case 258:
		nState = "正在运行";
		return;
	case -1:
		nState = "已销毁";
		return;
	}
	nState = "已结束";
}
BOOL CThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(L"遍历线程");
	m_listT.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listT.InsertColumn(0, L"索引", 0, 50);
	m_listT.InsertColumn(1, L"线程ID", 0, 120);
	m_listT.InsertColumn(2, L"线程状态", 0, 120);
	m_listT.InsertColumn(3, L"线程优先级", 0, 120);

	int nIdx = 0;
	CString buffer;
	for (auto&i : m_thread)
	{
		buffer.Format(L"%d", nIdx);
		m_listT.InsertItem(nIdx, buffer);
		buffer.Format(_T("%d"), i.th32ThreadID);
		m_listT.SetItemText(nIdx,1, buffer);

		GetThreadState(buffer, i.th32ThreadID);
		m_listT.SetItemText(nIdx, 2, buffer);
		buffer.Format(_T("%d"), i.tpBasePri);
		m_listT.SetItemText(nIdx, 3, buffer);
		nIdx++;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
