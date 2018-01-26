#pragma once
#include <gdiplus.h>
using namespace Gdiplus;

struct DrHeader {
	WORD width;
	WORD height;
	WORD bbp;
	WORD isSign;
	WORD maxVal;
	WORD reserved[3];
};

class MyImage
{
private:
	DrHeader drImageHeader;
	unsigned short *lpImgData = NULL;
	BITMAPINFO *bmpInfo = NULL;
	BYTE *buffer = NULL;
	//Bitmap *bmp = NULL;

	int windowWidth;
	int windowLevel;

	BYTE* imgWithBorder = NULL;
	BYTE* imgAfterZoom = NULL;
	BYTE* imgAfterACE = NULL;
	int border;
	double *Integral = NULL;
	double *Integral2 = NULL;
	double *localMean; 
	double *localVar;

	int scale;
	int maxCG;

public:
	MyImage();
	~MyImage();
	bool isOpen = false;
	bool open(CString);
	void initPalette();
	bool readData(unsigned short *&, CFile &, unsigned int);
	bool windowTech(unsigned short *sourceData, BYTE *&targetData, int low, int high);
	void windowTechInterface(int windowWidth, int windowLevel);
	Bitmap* getBitmap();
	bool enhance();
	void makeBorder(int);
	void GetGrayIntegralImage(int);
	void GetGrayIntegralImage2(int);
	void localMeadVar();
	void ACE();

	void setN(int);
	void setScale(int);
	void setMaxCG(int);
	bool zoom(double);
	void enhance2();
};

