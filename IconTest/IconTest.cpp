// IconTest.cpp : 定义应用程序的入口点。
// 文档
// 欢迎！
#define _CRT_SECURE_NO_WARNINGS
#define IDM_CHENGESRC 11461
#define IDM_PRINTSCREEN 11462
#define IDM_MUSCI 11460
#define IDM_OPENPIC 11463
#define IDM_CHANGEMUSCI 11464
#define IDM_INPT 11465
/*
*********************************************************
* 欢迎！												*
* 这个文件(指文件IconTest.h)非常重要！					*
* 您可以在全局(Or Other)中定义：MyNotify*<114514> Myn，	*
* 其中，"114514"是图标按下时的消息，众所周知，Windows是	*
* 事件驱动的操作系统QWQ									*
* 然后，通过某些手段获得任意窗口HWND (重要)，在任意位置	*
* 进行初始化：Myn = new MyNotify<114514>(您的HWND)。	*
*														*
* Tips 1:												*
* 两次初始化尖角括号"<>"中的数字一定是相同的正整数！	*
* Tips 2:												*
* 如果想有所反应，请在147行添加"ResNotify(*Myn)"，里面	*
* 。。。您自己看吧										*
*********************************************************
*/

#include <locale.h>  
#include <locale>  
#include<thread>
#include "framework.h"
#include "IconTest.h"
#include "audioclip.h"
#include "PrintScreen.h"
#include<commdlg.h>
#include<commctrl.h>
#include<fstream>
#include<mmsystem.h>
#include<format>

#pragma comment (lib,"winmm.lib")

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;  // 当前实例
WCHAR szTitle[MAX_LOADSTRING];// 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];// 主窗口类名
STARTUPINFO si;
PROCESS_INFORMATION pi;
// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);


const auto DeskTopWin = GetDesktopWindow();
MyNotify<1145>* Myn;
AudioClip* ac;
CTrayIcon* MyC;
HWND Iputs;
int hotkeyId = 1;

inline void NewWindowRun(char* RunPath) {
#pragma region MyRegion
	//LPWSTR szCommandLine = TEXT("NOTEPAD");//错误
	//STARTUPINFO si = { sizeof(si) };
	STARTUPINFOA si;
	auto runp = RunPath;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	si.dwFlags = STARTF_USESHOWWINDOW;  // 指定wShowWindow成员有效
	si.wShowWindow = TRUE;          // 此成员设为TRUE的话则显示新建进程的主窗口，
						// 为FALSE的话则不显示
	BOOL bRet = ::CreateProcessA(
		NULL,           // 不在此指定可执行文件的文件名
		runp,      // 命令行参数
		NULL,           // 默认进程安全性
		NULL,           // 默认线程安全性
		FALSE,          // 指定当前进程内的句柄不可以被子进程继承
		CREATE_NEW_CONSOLE, // 为新进程创建一个新的控制台窗口
		NULL,           // 使用本进程的环境变量
		NULL,           // 使用本进程的驱动器和目录
		&si,
		&pi);

	if (bRet)
	{
		// 既然我们不使用两个句柄，最好是立刻将它们关闭
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);

	}
#pragma endregion

}
inline char* GetF(LPSTR Filer = LPSTR("Exe文件(*.exe)\0 * .exe\0所有文件(*.*)\0 * .*\0"), LPSTR Title = (LPSTR)"选择WCH.exe文件QWQ", LPSTR norun() = [] {
	MessageBox(NULL, L"您选了个寂寞，现已改成System32 cmd", L"AZQWQ", MB_OK);
	return LPSTR("C:\\Windows\\System32\\cmd.exe");
	}
) {
	OPENFILENAMEA ofn;      // 公共对话框结构。   
	CHAR szFile[MAX_PATH]; // 保存获取文件名称的缓冲区。             
	// 初始化选择文件对话框。   
	ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
	ofn.lStructSize = sizeof(OPENFILENAMEA);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = Filer;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrTitle = Title;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);  
	// 显示打开选择文件对话框。   

	if (GetOpenFileNameA(&ofn))
		//显示选择的文件。   
		return(szFile);
	else
		return norun();
}
template<size_t Len = 30>
inline LPWSTR UserName() {
	WCHAR currentUser[Len] = { 0 };
	DWORD dwSize_currentUser = 256;
	GetUserName(
		currentUser,			// 接收当前登录用户的用户名
		&dwSize_currentUser		// 缓冲区大小
	);
	return currentUser;
}
inline void ShowInp() {

	if (Iputs) {
		wchar_t buffer[100];
		GetWindowText(Iputs, buffer, sizeof(buffer) / sizeof(buffer[0]));

		std::wofstream ofst(L"C:\\Users\\" + std::wstring(UserName()) + L"\\Documents\\MyFastSave\\" + std::to_wstring(clock()) + L".txt");
		ofst.imbue(std::locale("", std::locale::all ^ std::locale::numeric));
		ofst << buffer;
		ofst.close();
		DestroyWindow(Iputs), Iputs = 0;
	}
	else {
		HFONT hFont = CreateFont(
			-16/*高度*/, -8/*宽度*/, 0/*不用管*/, 0/*不用管*/, 400 /*一般这个值设为400*/,
			FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
			DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
			4,  //默认输出质量
			FF_DONTCARE,  //不指定字体族*/
			L"新宋体"  //字体名
		);
		Iputs = CreateWindow(TEXT("edit"), NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_BORDER |
			ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			20, 20, 550, 300,
			DeskTopWin, (HMENU)IDM_INPT, hInst, NULL);
		::SetWindowPos(Iputs, HWND_TOPMOST, 0, 0, 100, 100, SWP_NOMOVE | SWP_NOSIZE);
		::SetForegroundWindow(Iputs);
		SendMessage(
			Iputs,  //欲设置字体的控件句柄
			WM_SETFONT,  //消息名（消息类型）
			(WPARAM)hFont,  //字体句柄
			NULL  //传空值即可
		);
	}
}
inline void ShowList(HWND hWnd) {

	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu;
	hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, IDM_CHENGESRC, L"改程序地址");
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_STRING, IDM_PRINTSCREEN, L"截图(Ctrl+F1)");
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_STRING, IDM_MUSCI, TEXT("播放音乐"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_STRING, IDM_CHANGEMUSCI, TEXT("改音乐地址"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("退出程序"));
	AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
	TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, NULL, hWnd, NULL);
}


char RunSrc[MAX_PATH];
char MusSrc[MAX_PATH];
char RunCode[MAX_PATH];


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此处放置代码。

	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ICONTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ICONTEST));

	MSG msg;

	// 主消息循环:
	GetMessage(&msg, nullptr, 0, 0);
	Myn = new MyNotify<1145>(msg.hwnd);
	ac = new AudioClip();
	MyC = new CTrayIcon();
	MyC->CreateTray(msg.hwnd, LoadIcon(NULL, IDI_ERROR), IDM_OPENPIC, L"贴心地说");
	CreatedirOrUse();
	CreatedirOrUse(L"C:\\Users\\" + std::wstring(myUserName()) + L"\\Pictures\\PrintScreen");

	if (std::ifstream ifr("LBD.txt"); ifr.good()) {
		ifr >> RunSrc;
		ifr.close();
	}
	else {
		ifr.close();
		MessageBox(NULL, L"显示帮助：Ctrl+Alt+H", L"快捷键简介", MB_OK);
		MessageBox(NULL, L"左键托盘图标(Alt+L Or Alt+F5)，开启自定义程序\r\n右键托盘图标，呼出菜单\r\n中键托盘图标(Alt+F2 Or Alt+I)，开启速记\r\nAlt+F1 Or Alt+P，截图\r\nAlt+F3 Or Alt+M，播放音乐", L"使用简介",MB_OK);
		if (IDYES == MessageBox(NULL, L"完了，您，没设置左键单击启动的程序，可以在文件LBD.txt中设置，或在此右键托盘设置，现在设置吗", L"QWQWQWQW", MB_YESNO))
		{
			std::ofstream ofr("LBD.txt");
			strcpy(RunSrc, GetF());
			ofr << RunSrc;
			ofr.close();
		}
	}
	
	constexpr int SrePic = 121;
	constexpr int ShoInp = 122;
	constexpr int Led = 123;
	constexpr int Hlp = 124;
	constexpr int Plmu = 125;


	RegisterHotKey(NULL, SrePic, MOD_ALT, VK_F1);
	RegisterHotKey(NULL, SrePic, MOD_ALT, 'P');
	RegisterHotKey(NULL, ShoInp, MOD_ALT, VK_F2);
	RegisterHotKey(NULL, ShoInp, MOD_ALT, 'I');
	RegisterHotKey(NULL, Plmu, MOD_ALT, VK_F3);
	RegisterHotKey(NULL, Plmu, MOD_ALT, 'M');

	RegisterHotKey(NULL, Led, MOD_ALT, VK_F5);
	RegisterHotKey(NULL, Led, MOD_ALT, 'L');
	RegisterHotKey(NULL, Hlp, MOD_CONTROL | MOD_ALT, 'H');

	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (msg.message == WM_HOTKEY) {
			switch (msg.wParam) {
			case SrePic: {
				PrintScreen();
				MyC->ChangeTray(L"截图快乐！");
				break; 
			}
			case ShoInp: {
				ShowInp();
				break; }
			case Led: {
				NewWindowRun(RunSrc);
				break;
			}
			case Hlp: {
				MessageBox(NULL, L"左键托盘图标(Alt+L Or Alt+F5)，开启自定义程序\r\n右键托盘图标(Alt+L)，呼出菜单\r\n中键托盘图标(Alt+F2 Or Alt+I)，开启速记\r\n(Alt+F1 Or Alt+P)，截图\r\nAlt+F3 Or Alt+M，播放音乐", L"使用简介", MB_OK);
				break;
			}
			case Plmu:{
				if (std::ifstream("Mus.txt").good()) {
					std::ifstream("Mus.txt") >> MusSrc;
					ac->load(MusSrc);
					ac->play();
				}
				else {
					std::ofstream ofr("Mus.txt");
					strcpy(MusSrc, GetF(LPSTR("Mp3文件(*.mp3)\0 * .mp3\0所有文件(*.*)\0 * .*\0"), LPSTR("您找个mp3吧QWQ，另外中文目录是可耻的")));
					ofr << MusSrc;
					ofr.close();
				}
				break;
			}
			default:
				break;
			}
		}
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONTEST));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ICONTEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//函数: InitInstance(HINSTANCE, int)
//
//目标: 保存实例句柄并创建主窗口
//
//注释:
//
//  在此函数中，我们在全局变量中保存实例句柄并
//  创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	//ShowWindow(hWnd, nCmdShow);
	//UpdateWindow(hWnd);

	return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		ResNotify(*Myn) {
			switch (lParam) {
			case WM_LBUTTONDOWN:
				//MessageBox(NULL, TEXT("您单击左键"), TEXT("notify"), MB_ICONHAND);
				NewWindowRun(RunSrc);
				break;
			case WM_RBUTTONDOWN:
				POINT pt;
				GetCursorPos(&pt);
				HMENU hMenu;
				hMenu = CreatePopupMenu();
				AppendMenu(hMenu, MF_STRING, IDM_CHENGESRC, L"改程序地址");
				AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				AppendMenu(hMenu, MF_STRING, IDM_PRINTSCREEN, L"截图(Alt+F1)Or(Alt+P)");
				AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				AppendMenu(hMenu, MF_STRING, IDM_MUSCI, TEXT("播放音乐(Alt+F3)Or(Alt+M)"));
				AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				AppendMenu(hMenu, MF_STRING, IDM_CHANGEMUSCI, TEXT("改音乐地址"));
				AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("退出程序"));
				AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
				TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, NULL, hWnd, NULL);

				break;
			case WM_MBUTTONDOWN:
				ShowInp();
				break;
			}
		}
		break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_CHENGESRC: {
			if (IDYES != MessageBox(NULL, L"是吗？还是手贱？（是：改，否：不改", L"您。。。要改程序地址", MB_YESNO)) break;
			std::ofstream ofr("LBD.txt");
			strcpy(RunSrc, GetF());
			ofr << RunSrc;
			ofr.close();
			break;
		}
		case IDM_MUSCI: {
			if (std::ifstream("Mus.txt").good()) {
				std::ifstream("Mus.txt") >> MusSrc;
				ac->load(MusSrc);
				ac->play();
			}
			else {
				std::ofstream ofr("Mus.txt");
				strcpy(MusSrc, GetF(LPSTR("Mp3文件(*.mp3)\0 * .mp3\0所有文件(*.*)\0 * .*\0"), LPSTR("您找个mp3吧QWQ，另外中文目录是可耻的")));
				ofr << MusSrc;
				ofr.close();
			}
			break;
		}
		case IDM_PRINTSCREEN: {
			PrintScreen();
			MyC->ChangeTray(L"截图快乐！");
			break;
		}
		case IDM_CHANGEMUSCI: {
			std::ofstream ofr("Mus.txt");
			strcpy(MusSrc, GetF(LPSTR("Mp3文件(*.mp3)\0 * .mp3\0所有文件(*.*)\0 * .*\0"), LPSTR("您找个mp3吧QWQ，另外中文目录是可耻的")));
			ofr << MusSrc;
			ofr.close();
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
				   break;
	case WM_PAINT: {
		//int result = RegisterHotKey(this->GetHWND(), hotkeyId, MOD_ALT, 'C');
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		//HFONT hfon = CreateFont();

		EndPaint(hWnd, &ps);
	}
				 break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
