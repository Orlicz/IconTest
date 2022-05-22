#pragma once

#include "resource.h"

#define ResNotify(Obj) case ((Obj).MINMAXNotify) :
template<UINT WM_MINMAXNotify>struct MyNotify {
	static const UINT MINMAXNotify = WM_MINMAXNotify/*��զ֪������٣�������*/;
	MyNotify(HWND hWND, WCHAR* Path = (WCHAR*)L"C:\\Windows\\System32\\WindowsUpdateOK.ico", WCHAR* Title = (WCHAR*)L"���Ǳ���QWQ", UINT uID = 1) {
		NOTIFYICONDATA nID = {};//������������ʼ��
		nID.hIcon = (HICON)LoadImage(NULL, Path, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);//����������ͼ����Դ��ͼ����Ҫ��16*16����32*32���ص� 
		StrCpyW(nID.szTip, Title);//����ͼ�����ʾ�������������ᵯ������ʾ
		nID.hWnd = hWND;//����ͼ������Ĵ���
		//Ӧ�ó���ֻ��һ������ͼ��Ļ����������
		nID.uID = uID;
		//����ͼ������ͣ�����MSDN 
		nID.uFlags = NIF_GUID | NIF_ICON | NIF_MESSAGE | NIF_TIP;
		//����ͼ���������ϢID�����̵�������һ���Ϣ����ϢID������
		nID.uCallbackMessage = WM_MINMAXNotify;
		//֪ͨwindows���һ������ͼ�꣬��������֪����
		Shell_NotifyIcon(NIM_ADD, &nID);
	}
};

/*void ShowTrayIcon(void)
{
	//��ʾ����
	NOTIFYICONDATA nid;    //NOTIFYICONDATA�ṹ������ϵͳ������������ͼ�����Ϣ��
	//������ѡ���ͼ�ꡢ�ص���Ϣ����ʾ��Ϣ��ͼ���Ӧ�Ĵ��ڵ����ݡ�
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);         //���ֽ�Ϊ��λ������ṹ�Ĵ�С
	nid.hWnd = m_hWnd;          //��������ͼ��֪ͨ��Ϣ�Ĵ��ھ��
	nid.uID = IDR_MAINFRAME;    //Ӧ�ó�����ĸ�ͼ���ID��
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;     //���ø�ͼ�������
	nid.uCallbackMessage = WM_TRAYICON;             //Ӧ�ó��������ϢID�ţ�����Ϣ���ݸ�hWnd  
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME1));   //ͼ��ľ��
	wcscpy(nid.szInfoTitle, _T("��ʾ"));
	strTip.Format(_T("ϵͳ�����ˡ�����"));
	wcscpy(nid.szTip, strTip);   	//���ͣ����ͼ������ʾ����ʾ��Ϣ 
	Shell_NotifyIcon(NIM_ADD, &nid);	//�����������ͼ��de����
}*/


#pragma region MyRegion


class CTrayIcon
{
public:
	CTrayIcon() {};
	~CTrayIcon() {};

	BOOL CreateTray(HWND, HICON, UINT, LPCTSTR = L"���ĵ�˵");
	BOOL ChangeTray(LPCTSTR, LPCTSTR = L"���ĵ�˵", UINT = 3000);
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