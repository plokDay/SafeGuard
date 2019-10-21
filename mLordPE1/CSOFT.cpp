// CSOFT.cpp: 实现文件
//

#include "stdafx.h"
#include "mLordPE1.h"
#include "CSOFT.h"
#include "afxdialogex.h"
//#include "minwinbase.h"

// CSOFT 对话框

IMPLEMENT_DYNAMIC(CSOFT, CDialogEx)

CSOFT::CSOFT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOFT, pParent)
{
	
}

CSOFT::~CSOFT()
{
}

void CSOFT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listSoft);
}


BEGIN_MESSAGE_MAP(CSOFT, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSOFT::OnNMRClickList1)
	ON_COMMAND(ID_SOFT_32781, &CSOFT::OnSoft32781)
END_MESSAGE_MAP()


// CSOFT 消息处理程序


BOOL CSOFT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listSoft.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES);
	m_listSoft.InsertColumn(0, TEXT("软件名称"), 0, 280);
	m_listSoft.InsertColumn(1, TEXT("出品公司"), 0, 100);
	m_listSoft.InsertColumn(2, TEXT("版本"), 0, 100);
	m_listSoft.InsertColumn(3, TEXT("卸载目录"), 0, 300);
	m_menuSoft.LoadMenu(IDR_MENU3);

	GetSoft();
	for (int i=0;i<m_vecSoft.size();++i)
	{
		m_listSoft.InsertItem(i, m_vecSoft[i].szSoftName);
		m_listSoft.SetItemText(i, 1, m_vecSoft[i].szSoftPub);
		m_listSoft.SetItemText(i, 2, m_vecSoft[i].szSoftVer);
		m_listSoft.SetItemText(i, 3, m_vecSoft[i].szSoftUniPath);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSOFT::GetSoft()
{
	CString subkey;
	if (SystemIsX32())subkey = subKey32;
	else subkey = subkey64;
	//打开存在注册表子键
	LONG lRet = RegOpenKeyEx(RootKey, subkey, 0, KEY_READ | KEY_WOW64_64KEY, &hkResult);
	DWORD dwIndex = 0; CString strBuffer;
	CString strMidReg;
	HKEY hSubKey = NULL;//子键句柄
	WCHAR szNewKeyName[MAX_PATH] = { 0 };
	DWORD dwKeyLen = MAXBYTE;
	WCHAR szBuffer[MAX_PATH] = { 0 };
	DWORD dwType = REG_DWORD | REG_EXPAND_SZ | REG_SZ;
	DWORD dwNameLen = MAXBYTE;
	DWORD dwSoftSize = 0;

	if (lRet == ERROR_SUCCESS)
	{
		//枚举软件Uninstall目录子键
		while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hkResult, dwIndex, szNewKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
		{

			strBuffer.Format(L"%s", szNewKeyName);
			if (!strBuffer.IsEmpty())
			{
				//格式化拼接子键
				strMidReg = subkey + L"\\" + strBuffer;
				//打开单个软件子键
				if (ERROR_SUCCESS == RegOpenKeyEx(RootKey, strMidReg, 0, KEY_READ | KEY_WOW64_64KEY, &hSubKey))
				{
					//获取信息
					SOFTWARE SoftInfo = { 0 };

					//名称
					RegQueryValueEx(hSubKey, L"DisplayName", 0, &dwType, (LPBYTE)SoftInfo.szSoftName, &dwNameLen);
					dwNameLen = MAXBYTE;	//需要恢复此值

					//厂商
					RegQueryValueEx(hSubKey, L"Publisher", 0, &dwType, (LPBYTE)SoftInfo.szSoftPub, &dwNameLen);
					dwNameLen = MAXBYTE;

					//日期
					RegQueryValueEx(hSubKey, L"InstallDate", 0, &dwType, (LPBYTE)SoftInfo.szSoftDate, &dwNameLen);
					dwNameLen = MAXBYTE;

					//版本
					RegQueryValueEx(hSubKey, L"DisplayVersion", 0, &dwType, (LPBYTE)SoftInfo.szSoftVer, &dwNameLen);
					dwNameLen = MAXBYTE;

					//安装路径
					RegQueryValueEx(hSubKey, L"InstallLocation", 0, &dwType, (LPBYTE)SoftInfo.szSoftInsPath, &dwNameLen);
					dwNameLen = MAXBYTE;

					//卸载路径
					RegQueryValueEx(hSubKey, L"UninstallString", 0, &dwType, (LPBYTE)SoftInfo.szSoftUniPath, &dwNameLen);
					dwNameLen = MAXBYTE;

					//大小为DWORD值
					RegQueryValueEx(hSubKey, L"EstimatedSize", 0, &dwType, (LPBYTE)&dwSoftSize, &dwNameLen);
					dwNameLen = MAXBYTE;

					//转换文件大小
					CString strSize;
					if (dwSoftSize > 0 && dwSoftSize < 1024)
					{
						strSize.Format(L"%dKB", dwSoftSize);
					}
					else if (dwSoftSize > 1024 && dwSoftSize < 1024 * 1024)
					{
						strSize.Format(L"%dMB", dwSoftSize / 1024);
					}
					else if (dwSoftSize > 1024 * 1024)
					{
						strSize.Format(L"%dGB", dwSoftSize / 1024 / 1024);
					}
					else
					{
						strSize = L" ";
					}
					memcpy_s(SoftInfo.szSoftSize, (strSize.GetLength() + 1) * sizeof(WCHAR),
						strSize.GetBuffer(), (strSize.GetLength() + 1) * sizeof(WCHAR));

					CString strName = SoftInfo.szSoftName;
					if (!strName.IsEmpty())
					{
						m_vecSoft.push_back(SoftInfo);
					}
				}
				dwIndex++;
				dwKeyLen = MAXBYTE;
				dwNameLen = MAXBYTE;
				dwSoftSize = 0;
				ZeroMemory(szNewKeyName, MAX_PATH);
			}

		}

	}
}


void CSOFT::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu* pSubMenu = m_menuSoft.GetSubMenu(0);
	CPoint pos;
	GetCursorPos(&pos);
	pSubMenu->TrackPopupMenu(0, pos.x, pos.y, this);
	*pResult = 0;
}

//卸载
void CSOFT::OnSoft32781()
{
	int index = (int)m_listSoft.GetFirstSelectedItemPosition();
	if (index == 0) {
		return;
	}
	index -= 1;


	CString strPath = m_listSoft.GetItemText(index, 3);// 第1列是pid
	OFSTRUCT ofstruct;
	ShellExecute(NULL, _T("open"), strPath, NULL, NULL, SW_SHOW);
}
