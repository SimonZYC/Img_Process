#include"drawHistogram.h"
#include "openBMP.h"
#include"stdafx.h"
//用到其他文件中的全局变量
extern BITMAPINFOHEADER bi;
extern DWORD NumColors;//实际用到的颜色数 ，即调色板数组中的颜色个数
extern DWORD LineBytes;  //每一行的字节数
extern BITMAPFILEHEADER bf;
extern HGLOBAL  hImgData;

//int max = 0;
//int* count = NULL;

int *originCount = NULL;

void drawHistogram(PAINTSTRUCT);
void drawHistogram(PAINTSTRUCT ps, int offsetXBase, int offsetYBase, int *originCount);
bool calcHistogram(int *originCount);
void displayHis(PAINTSTRUCT ps);

void drawHistogram(PAINTSTRUCT ps, int offsetXBase, int offsetYBase, int *inputCount) {
	if (!inputCount)
		return;

	// 创建钢笔  
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	// 把笔选到DC中  
	SelectObject(ps.hdc, pen);
	// 设定线段的起点
	//int offsetXBase = bi.biWidth + 80;
	//int offsetYBase = bi.biHeight + 5;
	MoveToEx(ps.hdc, offsetXBase, offsetYBase, NULL);

	// 绘制坐标轴  绘制坐标轴箭头 
	LineTo(ps.hdc, offsetXBase + 0.45*bi.biWidth, offsetYBase);

	LineTo(ps.hdc, offsetXBase + 0.45*bi.biWidth - 5, offsetYBase - 5);
	MoveToEx(ps.hdc, offsetXBase + 0.45*bi.biWidth, offsetYBase, NULL);
	LineTo(ps.hdc, offsetXBase + 0.45*bi.biWidth - 5, offsetYBase + 5);

	MoveToEx(ps.hdc, offsetXBase, offsetYBase, NULL);
	LineTo(ps.hdc, offsetXBase, offsetYBase - 0.45*bi.biHeight);
	LineTo(ps.hdc, offsetXBase - 5, offsetYBase - 0.45*bi.biHeight + 5);
	MoveToEx(ps.hdc, offsetXBase, offsetYBase - 0.45*bi.biHeight, NULL);
	LineTo(ps.hdc, offsetXBase + 5, offsetYBase - 0.45*bi.biHeight + 5);


	int maxGray = inputCount[0];
	for (int i = 1; i<int(NumColors); i++) {
		if (maxGray < inputCount[i])
			maxGray = inputCount[i];
	}//求得最大的灰度值，为绘图做准备

	double step = 0.45*bi.biWidth * 0.9 / NumColors;
	for (int i = 0; i < (int)(NumColors); i++) {
		MoveToEx(ps.hdc, offsetXBase + i*step, offsetYBase, NULL);
		LineTo(ps.hdc, double(offsetXBase + i * step), offsetYBase - double(inputCount[i]*1.0/maxGray)*bi.biHeight*0.45*0.9);
	}//画直方图，按照每个灰度值一条线一条线画

	 //删去钢笔
	DeleteObject(pen);
}

bool calcHistogram(int *originCount) {
	if (!originCount) return false;

	DWORD              OffBits, BufSize;
	LPBITMAPINFOHEADER lpImgData;
	LPSTR              lpPtr;
	LONG               x, y;
	int                g;

	OffBits = bf.bfOffBits - sizeof(BITMAPFILEHEADER);//OffBits为BITMAPINFOHEADER结构长度加调色板的大小
	BufSize = bf.bfSize - sizeof(BITMAPFILEHEADER); //要开的缓冲区的大小

	lpImgData = (LPBITMAPINFOHEADER)GlobalLock(hImgData);

	for (y = 1; y<bi.biHeight - 1; y++)
		for (x = 1; x < bi.biWidth - 1; x++) {
			lpPtr = (char *)lpImgData + (BufSize - LineBytes - y*LineBytes) + x;
			g = (unsigned char)*(lpPtr);
			originCount[g]++;
		}//统计灰度值分布

	return true;
}

void displayHis(PAINTSTRUCT ps) {
	if (originCount)
		free(originCount);
	originCount = (int *)malloc(sizeof(int)*NumColors);
	memset(originCount, 0, sizeof(int)*NumColors);

	if (calcHistogram(originCount)) {
		drawHistogram(ps, bi.biWidth + 20, bi.biHeight*0.45 + 5, originCount);
	}

}