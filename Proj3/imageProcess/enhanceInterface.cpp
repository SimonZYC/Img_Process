#include "stdafx.h"
#include "enhanceInterface.h"
#include "openImage.h"
#include "getIntegral.h"
#include "ACE.h"
#include "localMeanStd.h"
#include "makeBorder.h"

extern unsigned char *lpImgData; //���ԭͼ��Ҷ�ֵ������
extern BITMAPINFO bmpInfo;		//ͼ�������Ϣ

void makeBorder(unsigned char *src, unsigned char *&dst, int width, int height, int bor);	//ͼ��߽���չ

extern void GetGrayIntegralImage(unsigned char *Src, double *&Integral, int Width, int Height, int Stride);	//���ͼ��Ļ���ͼ��
extern void GetGrayIntegralImage2(unsigned char *Src, double *&Integral, int Width, int Height, int Stride);	//���ͼ��ƽ���Ļ���ͼ��
extern void localMeadVar(double *integral1, double *integral2, double *&localMean, double *&localVar, int width, int height, int bor);	//��ȡ�ֲ���ֵ�ͱ�׼��
extern int ACE(unsigned char *src, unsigned char *&result, int width, int height, int bor,int scale, int maxCG);

double *localMean = NULL;	//��žֲ���ֵ
double *localVar = NULL;	//��žֲ���׼��
double *integral1 = NULL;		//���ԭͼ�����ͼ��
double *integral2 = NULL;		//���ԭͼ��ƽ���Ļ���ͼ��
unsigned char *imgWithBorder = NULL;

unsigned char *result = NULL;	//�Աȶ���ߺ��ͼ��Ҷ�����
unsigned char *resultRGB = NULL;

bool isEnhance = false;
HBITMAP hCompatibleBitmap2 = NULL; //����BITMAP  
HBITMAP hOldBitmap2 = NULL;

void displayEnhanceImage(HDC hdc);


//�����ļ����µĶԱȶ���ǿ�Ľӿ�
bool enhance(int border, int scale, int maxCG) {
	int width = bmpInfo.bmiHeader.biWidth;
	int height = -bmpInfo.bmiHeader.biHeight;
	
	//border = 10;
	//scale = 50;
	//maxCG = 3;

	makeBorder(lpImgData, imgWithBorder, width, height, border);//��չ�߽�
	int widthWithBor = width + 2 * border;		//��չ��ͼ��Ŀ�
	int heightWithBor = height + 2 * border;	//��չ��ͼ��ĸ�
	
	if (imgWithBorder == NULL)
		printf("123");
	//�������ͼ��
	GetGrayIntegralImage(imgWithBorder, integral1, widthWithBor, heightWithBor, widthWithBor * sizeof(unsigned char));
	GetGrayIntegralImage2(imgWithBorder, integral2, widthWithBor, heightWithBor, widthWithBor * sizeof(unsigned char));

	//��ֲ���ֵ������
	localMeadVar(integral1, integral2, localMean, localVar, width, height, border);


	if (result != NULL)
		free(result);
	result = (unsigned char*)malloc(width*height * sizeof(unsigned char));//�洦����ͼ��
	//Ӧ���㷨���д���
	ACE(lpImgData, result, width, height, border,scale, maxCG);

	resultRGB = (BYTE *)malloc(width*height * 3);
	for (unsigned int i = 0; i < width*height; i++) {//д����ͼ���RGBֵ
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