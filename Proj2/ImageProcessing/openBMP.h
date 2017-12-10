#pragma once
#include"stdafx.h"

extern bool openImage(HWND hWnd);//提供主程序接口
extern bool openBmpImage(HWND hWnd, char strFilename[]);//读取BMP主要的程序
extern bool openFile();//弹出对话框选择文件

extern void displayBMP(HDC);//消息响应中绘制图像

					 
extern BITMAPFILEHEADER bf;
extern BITMAPINFOHEADER bi;
extern HPALETTE hPalette;
extern HBITMAP  hBitmap;
extern HGLOBAL  hImgData;
extern char strFilename[MAX_PATH];//用于接收文件名
extern DWORD NumColors;//实际用到的颜色数 ，即调色板数组中的颜色个数
extern DWORD LineBytes;  //每一行的字节数