#pragma once
#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")


template<size_t Len = 30>
inline LPWSTR myUserName() {
	WCHAR currentUser[Len] = { 0 };
	DWORD dwSize_currentUser = 256;
	GetUserName(
		currentUser,			// 接收当前登录用户的用户名
		&dwSize_currentUser		// 缓冲区大小
	);
	return currentUser;
}
inline void CreatedirOrUse(std::wstring folderPath = L"C:\\Users\\" + std::wstring(myUserName()) + L"\\Documents\\MyFastSave") {
		bool flag = CreateDirectory(folderPath.c_str(), NULL);
}
class GdiplusWrapper {
public:
	GdiplusWrapper() {
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	~GdiplusWrapper() {
		Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
			return -1;  // Failure

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == NULL)
			return -1;  // Failure

		Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}
		}

		free(pImageCodecInfo);
		return -1;  // Failure
	}

	void SaveImage(HBITMAP hBitmap, const WCHAR* filename, const WCHAR* format) {
		CLSID pngClsid;
		Gdiplus::Bitmap bitmap(hBitmap, NULL);
		GetEncoderClsid(format, &pngClsid);
		bitmap.Save(filename, &pngClsid);
	}

private:
	ULONG_PTR gdiplusToken;
};

template<double MagicNum = 1.5>/*我也不知道为什么要用这个数，但还必须得有*/
void PrintScreen(const WCHAR* SavePath = (L"C:\\Users\\"+(std::wstring(myUserName()))+L"\\Pictures\\PrintScreen\\"+std::to_wstring(clock())+L".jpg").c_str())
{
	HDC hdcScreen = ::GetDC(NULL);
	//HDC hdcScreen = CreateDC("DISPLAY", NULL, NULL, NULL);
	int nBitPerPixel = GetDeviceCaps(hdcScreen, BITSPIXEL);
	int nWidth = GetDeviceCaps(hdcScreen, HORZRES) * MagicNum;
	int nHeight = GetDeviceCaps(hdcScreen, VERTRES) * MagicNum;

	//建立一个屏幕设备环境句柄
	HDC hMemDC;
	HBITMAP hBitmap, hOldBitmap;
	hMemDC = CreateCompatibleDC(hdcScreen);
	hBitmap = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	//把屏幕设备描述表拷贝到内存设备描述表中
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY);

	// Initialize GDI+.
	GdiplusWrapper gdiplus;
	gdiplus.SaveImage(hBitmap, SavePath, L"image/jpeg");

	//释放资源
	DeleteDC(hdcScreen);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
}
