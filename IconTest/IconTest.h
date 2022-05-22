#pragma once

#include "resource.h"

#define ResNotify(Obj) case ((Obj).MINMAXNotify) :
template<UINT WM_MINMAXNotify>struct MyNotify {
	static const UINT MINMAXNotify = WM_MINMAXNotify/*我咋知道填多少，您试试*/;
	MyNotify(HWND hWND, WCHAR* Path = (WCHAR*)L"C:\\Windows\\System32\\WindowsUpdateOK.ico", WCHAR* Title = (WCHAR*)L"这是标题QWQ", UINT uID = 1) {
		NOTIFYICONDATA nID = {};//声明变量并初始化
		nID.hIcon = (HICON)LoadImage(NULL, Path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);//关联的托盘图标资源，图标需要是16*16或者32*32像素的 
		StrCpyW(nID.szTip, Title);//托盘图标的提示语，即鼠标放在上面会弹出的提示
		nID.hWnd = hWND;//托盘图标关联的窗口
		//应用程序只有一个托盘图标的话可以随便设
		nID.uID = uID;
		//托盘图标的类型，详解见MSDN 
		nID.uFlags = NIF_GUID | NIF_ICON | NIF_MESSAGE | NIF_TIP;
		//托盘图标关联的消息ID，托盘的左击和右击消息的消息ID就是它
		nID.uCallbackMessage = WM_MINMAXNotify;
		//通知windows添加一个托盘图标，看参数就知道啦
		Shell_NotifyIcon(NIM_ADD, &nID);
	}
};

/*void ShowTrayIcon(void)
{
	//显示托盘
	NOTIFYICONDATA nid;    //NOTIFYICONDATA结构包含了系统用来处理托盘图标的信息，
	//它包括选择的图标、回调消息、提示消息和图标对应的窗口等内容。
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);         //以字节为单位的这个结构的大小
	nid.hWnd = m_hWnd;          //接收托盘图标通知消息的窗口句柄
	nid.uID = IDR_MAINFRAME;    //应用程序定义的该图标的ID号
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;     //设置该图标的属性
	nid.uCallbackMessage = WM_TRAYICON;             //应用程序定义的消息ID号，此消息传递给hWnd  
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME1));   //图标的句柄
	wcscpy(nid.szInfoTitle, _T("提示"));
	strTip.Format(_T("系统崩溃了。。。"));
	wcscpy(nid.szTip, strTip);   	//鼠标停留在图标上显示的提示信息 
	Shell_NotifyIcon(NIM_ADD, &nid);	//在托盘区添加图标de函数
}*/


#pragma region MyRegion


class CTrayIcon
{
public:
	CTrayIcon() {};
	~CTrayIcon() {};

	BOOL CreateTray(HWND, HICON, UINT, LPCTSTR = L"贴心地说");
	BOOL ChangeTray(LPCTSTR, LPCTSTR = L"贴心地说", UINT = 3000);
	BOOL DeleteTray();

private:
	ULONGLONG GetVersion(LPCTSTR);
	DWORD GetSize_NotifyIconData();

	NOTIFYICONDATA m_Notify;
};
ULONGLONG CTrayIcon::GetVersion(LPCTSTR lpszDllName)
{
	HINSTANCE hinstDll;
	ULONGLONG dwVersion = 0;

	hinstDll = LoadLibrary(lpszDllName);

	if (hinstDll)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, "DllGetVersion");

		if (pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			HRESULT hr;

			ZeroMemory(&dvi, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);

			hr = (*pDllGetVersion)(&dvi);

			if (SUCCEEDED(hr))
			{
				dwVersion = MAKEDLLVERULL(dvi.dwMajorVersion, dvi.dwMinorVersion,
					dvi.dwBuildNumber, dvi.dwPlatformID);
			}
		}
		FreeLibrary(hinstDll);
	}
	return dwVersion;
}

DWORD CTrayIcon::GetSize_NotifyIconData()
{
	TCHAR lpszDllName[128];

	GetWindowsDirectory(lpszDllName, _TRUNCATE);
	_tcsncat_s(lpszDllName, _T("\\System32\\Shell32.dll"), _TRUNCATE);

	ULONGLONG NIDdllVer = GetVersion(lpszDllName);

	// before windows 2000
	if (NIDdllVer < MAKEDLLVERULL(5, 0, 0, 0)) {
		return NOTIFYICONDATA_V1_SIZE;
	}
	// Windows 2000
	else if (NIDdllVer < MAKEDLLVERULL(6, 0, 0, 0)) {
		return NOTIFYICONDATA_V2_SIZE;
	}
	// Windows XP or 2003
	else if (NIDdllVer < MAKEDLLVERULL(6, 0, 6000, 0)) {
		return NOTIFYICONDATA_V3_SIZE;
	}
	// Windows Vista and later
	else {
		return sizeof(NOTIFYICONDATA);
	}
}
BOOL CTrayIcon::CreateTray(HWND hWnd, HICON hIcon, UINT uCallbackMessage, LPCTSTR szTitle)
{
	m_Notify.cbSize = GetSize_NotifyIconData();
	m_Notify.hIcon = hIcon;
	m_Notify.hWnd = hWnd;
	m_Notify.uCallbackMessage = uCallbackMessage;
	m_Notify.uVersion = NOTIFYICON_VERSION;
	m_Notify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	m_Notify.uID = 1;

	_tcscpy_s(m_Notify.szTip, szTitle);

	return Shell_NotifyIcon(NIM_ADD, &m_Notify);
}
BOOL CTrayIcon::ChangeTray(LPCTSTR msg, LPCTSTR title, UINT uTimeout)
{
	m_Notify.uFlags = NIF_INFO;
	m_Notify.dwInfoFlags = NIIF_NONE;
	m_Notify.szInfoTitle;
	m_Notify.uTimeout = uTimeout;

	_tcscpy_s(m_Notify.szInfo, msg);
	_tcscpy_s(m_Notify.szInfoTitle, title);

	return Shell_NotifyIcon(NIM_MODIFY, &m_Notify);
}
BOOL CTrayIcon::DeleteTray() {
	return Shell_NotifyIcon(NIM_DELETE, &m_Notify);
}

#pragma endregion