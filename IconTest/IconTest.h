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