#pragma once
#include"stdafx.h"

extern bool openImage(HWND hWnd);//提供主程序接口

extern bool openFile();//弹出对话框选择文件


extern void displayMyImage(HDC);

extern bool openMyImage(HWND hWnd, char strFilename[]);

					 
extern char strFilename[MAX_PATH];//用于接收文件名
extern unsigned char *lpImgData; //存放原图像灰度值的数组
extern BITMAPINFO bmpInfo;	//图像各种信息

