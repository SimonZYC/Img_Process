#pragma once
#include"stdafx.h"

extern bool openImage(HWND hWnd);//�ṩ������ӿ�
extern bool openBmpImage(HWND hWnd, char strFilename[]);//��ȡBMP��Ҫ�ĳ���
extern bool openFile();//�����Ի���ѡ���ļ�

extern void displayBMP(HDC);//��Ϣ��Ӧ�л���ͼ��

					 
extern BITMAPFILEHEADER bf;
extern BITMAPINFOHEADER bi;
extern HPALETTE hPalette;
extern HBITMAP  hBitmap;
extern HGLOBAL  hImgData;
extern char strFilename[MAX_PATH];//���ڽ����ļ���
extern DWORD NumColors;//ʵ���õ�����ɫ�� ������ɫ�������е���ɫ����
extern DWORD LineBytes;  //ÿһ�е��ֽ���