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