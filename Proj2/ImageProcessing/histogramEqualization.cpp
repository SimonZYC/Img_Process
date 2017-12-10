#include"histogramEqualization.h"
#include "stdafx.h"
#include "openBMP.h"
#include "drawHistogram.h"

extern BITMAPINFOHEADER bi;
extern DWORD NumColors;//实际用到的颜色数 ，即调色板数组中的颜色个数
extern DWORD LineBytes;  //每一行的字节数
extern BITMAPFILEHEADER bf;
extern HGLOBAL  hImgData;

extern bool calcHistogram(int *originCount);
extern void drawHistogram(PAINTSTRUCT ps, int offsetXBase, int offsetYBase, int *originCount);
extern int *originCount;

HBITMAP hEBitmap = NULL;

double *normalCount = NULL;
double *cumDisFunc = NULL;
int *tempCount = NULL;

DWORD              OffBits, BufSize;
LPBITMAPINFOHEADER lpImgData;
LPSTR              lpPtr;
HLOCAL             hTempImgData;
LPBITMAPINFOHEADER lpTempImgData;
LPSTR              lpTempPtr;
HDC                hDc;
HFILE              hf;
LONG               x, y;
int                g, g1, g2, g3;

bool histogramEqualization();
void displayHEHistogram(HDC,PAINTSTRUCT);

bool histogramEqualization() {
	
	if (normalCount)
		free(normalCount);
	normalCount = (double *)malloc(sizeof(double)*NumColors);

	if (!originCount) {
		originCount = (int*)malloc(sizeof(int)*NumColors);
		memset(originCount, 0, sizeof(int)*NumColors);
		calcHistogram(originCount);
	}

	for (int i = 0; i < int(NumColors); i++) {
		normalCount[i] = double(originCount[i]*1.0 / (bi.biHeight*bi.biWidth));
	}//求像素的概率密度

	if (cumDisFunc)
		free(cumDisFunc);
	cumDisFunc = (double *)malloc(sizeof(double)*NumColors);
	memset(cumDisFunc, 0, sizeof(double)*NumColors);
	cumDisFunc[0] = normalCount[0];
	for (int i = 1; i < int(NumColors); i++) {
		cumDisFunc[i] = cumDisFunc[i - 1] + normalCount[i];
	}//求累积分布函数


	OffBits = bf.bfOffBits - sizeof(BITMAPFILEHEADER);//OffBits为BITMAPINFOHEADER结构长度加调色板的大小
	BufSize = bf.bfSize - sizeof(BITMAPFILEHEADER); //要开的缓冲区的大小

	if ((hTempImgData = LocalAlloc(LHND, BufSize)) == NULL)
	{
		//MessageBox(hWnd, "Error alloc memory!", "Error Message", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	lpImgData = (LPBITMAPINFOHEADER)GlobalLock(hImgData);
	lpTempImgData = (LPBITMAPINFOHEADER)LocalLock(hTempImgData);//拷贝头信息及位图数据

	memcpy(lpTempImgData, lpImgData, BufSize);

	unsigned char max = (unsigned char)*((char *)lpImgData + (BufSize - LineBytes - LineBytes) + 1),min = max;
	
	for (y = 1; y<bi.biHeight - 1; y++)
		for (x = 1; x < bi.biWidth - 1; x++) {
			lpPtr = (char *)lpImgData + (BufSize - LineBytes - y*LineBytes) + x;
			g = (unsigned char)*(lpPtr);
			if (max < g)
				max = g;
			else if (min > g)
				min = g;
		}//q求出最大、最小的像素值

	for (y = 1; y<bi.biHeight - 1; y++)
		for (x = 1; x < bi.biWidth - 1; x++) {
			lpPtr = (char *)lpImgData + (BufSize - LineBytes - y*LineBytes) + x;
			lpTempPtr = (char *)lpTempImgData + (BufSize - LineBytes - y*LineBytes) + x;

			g = (unsigned char)*(lpPtr);

			*lpTempPtr = (BYTE)(cumDisFunc[g]*(max-min)+min);

		}//

	return true;

}

void displayHEHistogram(HDC hDc, PAINTSTRUCT ps) {
	
	if (!histogramEqualization()) return;
	if (hEBitmap != NULL) {
		DeleteObject(hEBitmap);
		hEBitmap = NULL;
	}
	hEBitmap = CreateDIBitmap(hDc, (LPBITMAPINFOHEADER)lpTempImgData, (LONG)CBM_INIT,
		(LPSTR)lpTempImgData + sizeof(BITMAPINFOHEADER) + NumColors * sizeof(RGBQUAD),
		(LPBITMAPINFO)lpTempImgData, DIB_RGB_COLORS);

	if (hEBitmap)
	{
		HDC hMemDC = CreateCompatibleDC(hDc);
		if (hPalette)
		{
			SelectPalette(hDc, hPalette, FALSE);
			SelectPalette(hMemDC, hPalette, FALSE);
			RealizePalette(hDc);
		}
		SelectObject(hMemDC, hEBitmap);
		BitBlt(hDc, bi.biWidth*1.45 + 40, 5, bi.biWidth, bi.biHeight, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);

	}

	if (tempCount)
		free(tempCount);
	tempCount = (int *)malloc(sizeof(int)*NumColors);
	memset(tempCount, 0, sizeof(int)*NumColors);

	for (y = 1; y<bi.biHeight - 1; y++)
		for (x = 1; x < bi.biWidth - 1; x++) {
			lpPtr = (char *)lpTempImgData + (BufSize - LineBytes - y*LineBytes) + x;
			
			g = (unsigned char)*(lpPtr);

			tempCount[g]++;

		}//
	drawHistogram(ps,bi.biWidth + 20,bi.biHeight,tempCount);

}

