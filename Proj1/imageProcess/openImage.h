#pragma once
#include"stdafx.h"

extern bool openImage(HWND hWnd);//�ṩ������ӿ�

extern bool openFile();//�����Ի���ѡ���ļ�


extern void displayMyImage(HDC);

extern bool openMyImage(HWND hWnd, char strFilename[]);

					 
extern char strFilename[MAX_PATH];//���ڽ����ļ���
extern unsigned char *lpImgData; //���ԭͼ��Ҷ�ֵ������
extern BITMAPINFO bmpInfo;	//ͼ�������Ϣ

