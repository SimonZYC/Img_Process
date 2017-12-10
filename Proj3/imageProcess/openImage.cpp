#include"openImage.h"
#include"stdafx.h"
#include<errno.h>

BITMAPINFO bmpInfo;
BYTE *buffer = NULL;
HBITMAP hCompatibleBitmap = NULL; //兼容BITMAP  
HBITMAP hOldBitmap = NULL;
bool isOpen = false;
unsigned char *lpImgData = NULL;


char strFilename[MAX_PATH] = {};//用于接收文件名

bool openImage(HWND hWnd);//提供主程序接口

bool openFile();//弹出对话框选择文件

void displayMyImage(HDC);//消息响应中绘制图像

bool openMyImage(HWND hWnd, char strFilename[]);

bool openImage(HWND hWnd) {

	if (lpImgData != NULL) {
		free(lpImgData);
		lpImgData = NULL;
	}
	if (buffer != NULL) {
		free(buffer);
		buffer = NULL;
	}
	if (openFile()) {
		if (openMyImage(hWnd,strFilename)) {

			return TRUE;
		}
	}//成功，则重画窗口
}

bool openFile() {
	OPENFILENAME ofn = { 0 };
	//TCHAR strFilename[MAX_PATH] = { 0 };//用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);//结构体大小
	ofn.hwndOwner = NULL;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
	ofn.lpstrFilter = TEXT("自定义图像\0*.dr*\0所有文件\0*.*\0\0");//设置过滤
	ofn.nFilterIndex = 1;//过滤器索引
	ofn.lpstrFile = LPWSTR(strFilename);//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(strFilename);//缓冲区长度
	ofn.lpstrInitialDir = NULL;//初始目录为默认
	ofn.lpstrTitle = TEXT("请选择一个文件");//使用系统默认标题留空即可
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//文件、目录必须存在，隐藏只读选项
	if (GetOpenFileName(&ofn))
	{
		MessageBox(NULL, LPWSTR(strFilename), TEXT("选择的文件"), 0);
		return true;
	}
	else {
		return false;
	}
}

struct myImage {
	WORD width;
	WORD height;
	WORD bbp;
	WORD isSign;
	WORD maxVal;
	WORD reserved[3];
}drImageHeader;

void displayMyImage(HDC hdc) {
	if (isOpen)
	{
		HDC hMemDC = CreateCompatibleDC(hdc);
		hCompatibleBitmap = CreateCompatibleBitmap(hdc, drImageHeader.width, drImageHeader.height);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hCompatibleBitmap);
		
		SetDIBits(hdc, hCompatibleBitmap, 0, drImageHeader.height, buffer, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS);
		BitBlt(hdc, 5, 60, drImageHeader.width, drImageHeader.height, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);

	}
}

bool openMyImage(HWND hWnd, char strFilename[]) {
	FILE *fp;
	errno_t err = _wfopen_s(&fp, (LPWSTR)(strFilename), (wchar_t*)("r\0b\0"));
	//fopen_s(&fp, "Image02.dr", "rb");
	if (fp == NULL) {
		MessageBox(NULL, LPWSTR(err), LPWSTR("123"), 0);
		return FALSE; //打开文件错误，返回
	}

	fread(&drImageHeader, sizeof(myImage), 1, fp);
	unsigned int bytePerPx = drImageHeader.bbp / 8;
	unsigned int imgSize = drImageHeader.height*drImageHeader.width*bytePerPx;

	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biHeight = -drImageHeader.height;
	bmpInfo.bmiHeader.biWidth = drImageHeader.width;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	//写入BMP图像

	lpImgData = (unsigned char*)malloc(imgSize);

	buffer = (BYTE *)malloc(imgSize * 3);
	fread(lpImgData, imgSize, 1, fp);
	for (unsigned int i = 0; i < imgSize; i++) {
		buffer[i * 3] = lpImgData[i];
		buffer[i * 3 + 1] = lpImgData[i];
		buffer[i * 3 + 2] = lpImgData[i];		//RGB值都等于灰度值
	}
	isOpen = true;

	return true;
}
