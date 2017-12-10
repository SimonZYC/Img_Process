#include "stdafx.h"
#include "enhanceInterface.h"
#include "openImage.h"
#include "getIntegral.h"
#include "ACE.h"
#include "localMeanStd.h"
#include "makeBorder.h"

extern unsigned char *lpImgData; //存放原图像灰度值的数组
extern BITMAPINFO bmpInfo;		//图像各种信息

void makeBorder(unsigned char *src, unsigned char *&dst, int width, int height, int bor);	//图像边界扩展

extern void GetGrayIntegralImage(unsigned char *Src, double *&Integral, int Width, int Height, int Stride);	//获得图像的积分图像
extern void GetGrayIntegralImage2(unsigned char *Src, double *&Integral, int Width, int Height, int Stride);	//获得图像平方的积分图像
extern void localMeadVar(double *integral1, double *integral2, double *&localMean, double *&localVar, int width, int height, int bor);	//获取局部均值和标准差
extern int ACE(unsigned char *src, unsigned char *&result, int width, int height, int bor,int scale, int maxCG);

double *localMean = NULL;	//存放局部均值
double *localVar = NULL;	//存放局部标准差
double *integral1 = NULL;		//存放原图像积分图像
double *integral2 = NULL;		//存放原图像平方的积分图像
unsigned char *imgWithBorder = NULL;

unsigned char *result = NULL;	//对比度提高后的图像灰度数组
unsigned char *resultRGB = NULL;

bool isEnhance = false;
HBITMAP hCompatibleBitmap2 = NULL; //兼容BITMAP  
HBITMAP hOldBitmap2 = NULL;

void displayEnhanceImage(HDC hdc);


//给主文件留下的对比度增强的接口
bool enhance(int border, int scale, int maxCG) {
	int width = bmpInfo.bmiHeader.biWidth;
	int height = -bmpInfo.bmiHeader.biHeight;
	
	//border = 10;
	//scale = 50;
	//maxCG = 3;

	makeBorder(lpImgData, imgWithBorder, width, height, border);//扩展边界
	int widthWithBor = width + 2 * border;		//扩展后图像的宽
	int heightWithBor = height + 2 * border;	//扩展后图像的高
	
	if (imgWithBorder == NULL)
		printf("123");
	//计算积分图像
	GetGrayIntegralImage(imgWithBorder, integral1, widthWithBor, heightWithBor, widthWithBor * sizeof(unsigned char));
	GetGrayIntegralImage2(imgWithBorder, integral2, widthWithBor, heightWithBor, widthWithBor * sizeof(unsigned char));

	//求局部均值、方差
	localMeadVar(integral1, integral2, localMean, localVar, width, height, border);


	if (result != NULL)
		free(result);
	result = (unsigned char*)malloc(width*height * sizeof(unsigned char));//存处理后的图像
	//应用算法进行处理
	ACE(lpImgData, result, width, height, border,scale, maxCG);

	resultRGB = (BYTE *)malloc(width*height * 3);
	for (unsigned int i = 0; i < width*height; i++) {//写入新图像的RGB值
		//OutputDebugString((LPCWSTR(result[i])));
		resultRGB[i * 3] = result[i];
		resultRGB[i * 3 + 1] = result[i];
		resultRGB[i * 3 + 2] = result[i];
	}

	isEnhance = true;
	return true;
}


void displayEnhanceImage(HDC hdc) {
	if (isEnhance)
	{
		int width = bmpInfo.bmiHeader.biWidth;
		int height = -bmpInfo.bmiHeader.biHeight;
		HDC hMemDC = CreateCompatibleDC(hdc);
		hCompatibleBitmap2 = CreateCompatibleBitmap(hdc,width, height);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC, hCompatibleBitmap2);

		SetDIBits(hdc, hCompatibleBitmap2, 0, height, resultRGB, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS);
		BitBlt(hdc, 20+width, 60, width, height, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);

	}
}