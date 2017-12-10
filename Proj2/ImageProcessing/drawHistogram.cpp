#include"drawHistogram.h"
#include "openBMP.h"
#include"stdafx.h"
//�õ������ļ��е�ȫ�ֱ���
extern BITMAPINFOHEADER bi;
extern DWORD NumColors;//ʵ���õ�����ɫ�� ������ɫ�������е���ɫ����
extern DWORD LineBytes;  //ÿһ�е��ֽ���
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

	// �����ֱ�  
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	// �ѱ�ѡ��DC��  
	SelectObject(ps.hdc, pen);
	// �趨�߶ε����
	//int offsetXBase = bi.biWidth + 80;
	//int offsetYBase = bi.biHeight + 5;
	MoveToEx(ps.hdc, offsetXBase, offsetYBase, NULL);

	// ����������  �����������ͷ 
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
	}//������ĻҶ�ֵ��Ϊ��ͼ��׼��

	double step = 0.45*bi.biWidth * 0.9 / NumColors;
	for (int i = 0; i < (int)(NumColors); i++) {
		MoveToEx(ps.hdc, offsetXBase + i*step, offsetYBase, NULL);
		LineTo(ps.hdc, double(offsetXBase + i * step), offsetYBase - double(inputCount[i]*1.0/maxGray)*bi.biHeight*0.45*0.9);
	}//��ֱ��ͼ������ÿ���Ҷ�ֵһ����һ���߻�

	 //ɾȥ�ֱ�
	DeleteObject(pen);
}

bool calcHistogram(int *originCount) {
	if (!originCount) return false;

	DWORD              OffBits, BufSize;
	LPBITMAPINFOHEADER lpImgData;
	LPSTR              lpPtr;
	LONG               x, y;
	int                g;

	OffBits = bf.bfOffBits - sizeof(BITMAPFILEHEADER);//OffBitsΪBITMAPINFOHEADER�ṹ���ȼӵ�ɫ��Ĵ�С
	BufSize = bf.bfSize - sizeof(BITMAPFILEHEADER); //Ҫ���Ļ������Ĵ�С

	lpImgData = (LPBITMAPINFOHEADER)GlobalLock(hImgData);

	for (y = 1; y<bi.biHeight - 1; y++)
		for (x = 1; x < bi.biWidth - 1; x++) {
			lpPtr = (char *)lpImgData + (BufSize - LineBytes - y*LineBytes) + x;
			g = (unsigned char)*(lpPtr);
			originCount[g]++;
		}//ͳ�ƻҶ�ֵ�ֲ�

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