#include "stdafx.h"
#include "MyImage.h"


MyImage::MyImage()
{
	border = 50;
	maxCG = 3;
	scale = 50;
}


MyImage::~MyImage()
{
	if (lpImgData)
		free(lpImgData);
	if (bmpInfo)
		delete bmpInfo;
	if (buffer)
		free(buffer);
}


bool MyImage::open(CString filePath)
{
	CFile file;
	if(file.Open(filePath, CFile::typeBinary | CFile::modeRead) == FALSE)
		return false;

	file.Read(&drImageHeader, sizeof(DrHeader));

	unsigned int bytePerPx = drImageHeader.bbp / 8;
	unsigned int imgSize = drImageHeader.height*drImageHeader.width;// *bytePerPx;

	if (!bmpInfo)
		free(bmpInfo);
	bmpInfo = (BITMAPINFO*)calloc(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD), 1);
	if (!bmpInfo)
		return false;
	bmpInfo->bmiHeader.biBitCount = 8;
	bmpInfo->bmiHeader.biCompression = BI_RGB;
	bmpInfo->bmiHeader.biHeight = -drImageHeader.height;
	bmpInfo->bmiHeader.biWidth = drImageHeader.width;
	bmpInfo->bmiHeader.biPlanes = 1;
	bmpInfo->bmiHeader.biSizeImage = 0;
	bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	//写入BMP图像

	initPalette();

	windowWidth = drImageHeader.maxVal;
	windowLevel = windowWidth / 2;

	readData(lpImgData, file, imgSize);
	
	if (buffer) {
		free(buffer);
		int lineByte = (drImageHeader.width + 3) / 4 * 4;
		buffer = (BYTE *)calloc(drImageHeader.height * lineByte, 1);
	}
		
	windowTech(lpImgData, buffer, 0, drImageHeader.maxVal);
	zoom(0.5);
	makeBorder(200);
	GetGrayIntegralImage(200);
	GetGrayIntegralImage2(200);

	localMeadVar();
	ACE();

	return true;
}


void MyImage::initPalette()
{
	for (int i = 0; i < 256; i++)
		bmpInfo->bmiColors[i].rgbRed = bmpInfo->bmiColors[i].rgbGreen = bmpInfo->bmiColors[i].rgbBlue = i;

}


bool MyImage::readData(unsigned short *&imgData, CFile &fp, unsigned int imgSize)
{
	if (!imgData)
		free(imgData);
	imgData = (unsigned short*)calloc(imgSize ,2);
	if (!imgData)
		return false;

	fp.Read(imgData, imgSize*2);
	return true;
	
}


bool MyImage::windowTech(unsigned short *sourceData, BYTE *&targetData, int low, int high)
{
	/*int width = bmpInfo->bmiHeader.biWidth;
	int height = (-bmpInfo->bmiHeader.biHeight);*/
	int width = drImageHeader.width;
	int height = drImageHeader.height;
	/*if (targetData != NULL)
		free(targetData);*/
	int lineByte = (width + 3) / 4 * 4;

	if(targetData == NULL)
	targetData = (BYTE *)calloc(height * lineByte, 1);

	if (targetData == NULL)
		return false;
	int windowWidth = high - low;
	int index = 0, index2 = 0;
	int diff = lineByte - width;
	for (unsigned int i = 0; i < height; i++)
		for (unsigned int j = 0; j < width; j++) {
			BYTE a;
			int origin = sourceData[index++];
			if (origin > high)
				a = 255;
			else if (origin < low)
				a = 0;
			else
			{
				a = ceil((origin - low)*255.0 / windowWidth);
			}
			/*BYTE a = ceil(double( origin* 255 * 1.0) / windowWidth);
			if (a > 255)
				a = 255;
			if (a < 0)
				a = 0;*/
			
			targetData[index2++] = a;
			if (j == width - 1)
				index2 += diff;
		}
	return true;
}


Bitmap* MyImage::getBitmap()
{
	//return bmp;
	return new Bitmap(bmpInfo, imgAfterACE);
}

void MyImage::windowTechInterface(int windowWidth, int windowLevel)
{
	int low = (windowLevel - windowWidth / 2 >= 0) ? (windowLevel - windowWidth / 2) : 0;
	int high = (windowLevel + windowWidth / 2 >= drImageHeader.maxVal) ? (drImageHeader.maxVal) : windowLevel + windowWidth / 2;
	windowTech(lpImgData, buffer, low, high);
	zoom(0.5);
	makeBorder(200);
	GetGrayIntegralImage(200);
	GetGrayIntegralImage2(200);

	localMeadVar();
	ACE();
}



bool MyImage::enhance()
{
	localMeadVar();
	ACE();
	return false;
}


void MyImage::makeBorder(int border)
{
	int width = bmpInfo->bmiHeader.biWidth;
	int height = -bmpInfo->bmiHeader.biHeight;
	if (imgWithBorder != NULL) {
		free(imgWithBorder);
	}
	imgWithBorder = (unsigned char *)malloc((width + 2 * border)*(height + 2 * border));

	int index = 0;
	for (int i = border; i < border + height; i++)
		for (int j = border; j < border + width; j++) {
			imgWithBorder[i*(width + border * 2) + j] = imgAfterZoom[index++];
		}//相应位置填入原像素

	for (int i = border; i < border + height; i++) {
		index = 0;
		for (int j = 0; j < border; j++) {
			imgWithBorder[i*(width + border * 2) + j] = imgWithBorder[i*(width + border * 2) + j + 2 * border - index];
			index += 2;
		}
	}//扩展左边界
	for (int i = border; i < border + height; i++) {
		index = 2;
		for (int j = border + width; j < 2 * border + width; j++) {
			imgWithBorder[i*(width + border * 2) + j] = imgWithBorder[i*(width + border * 2) + j - index];
			index += 2;
		}
	}//扩展右边界

	index = 0;
	for (int i = 0; i < border; i++) {
		for (int j = 0; j < 2 * border + width; j++) {
			imgWithBorder[i*(width + border * 2) + j] = imgWithBorder[(i + border * 2 - index)*(width + border * 2) + j];
		}
		index += 2;
	}//扩展上边界

	index = 2;
	for (int i = border + height; i < 2 * border + height; i++) {
		for (int j = 0; j < 2 * border + width; j++) {
			imgWithBorder[i*(width + border * 2) + j] = imgWithBorder[(i - index)*(width + border * 2) + j];
		}
		index += 2;
	}//扩展下边界
}


void MyImage::GetGrayIntegralImage(int border)
{
	int Width = bmpInfo->bmiHeader.biWidth + 2 * border;
	int Height = -bmpInfo->bmiHeader.biHeight + 2 * border;
	int Stride = Width * 1;
	if (Integral != NULL)
		free(Integral);
	Integral = (double *)malloc((Width + 1)*(Height + 1) * sizeof(double));
	memset(Integral, 0, (Width + 1) * sizeof(double));					//	第一行都为0
	for (int Y = 0; Y < Height; Y++)
	{
		unsigned char *LinePS = imgWithBorder + Y * Stride;
		double *LinePL = Integral + Y * (Width + 1) + 1;				//	上一行位置			
		double *LinePD = Integral + (Y + 1) * (Width + 1) + 1;			//	当前位置，注意每行的第一列的值都为0
		LinePD[-1] = 0;												//	第一列的值为0
		double Sum = 0;
		for (int X = 0; X < Width; X++)
		{
			Sum += LinePS[X];										//	行方向累加
			LinePD[X] = LinePL[X] + Sum;							//	更新积分图
		}
	}
}


void MyImage::GetGrayIntegralImage2(int border)
{
	int Width = bmpInfo->bmiHeader.biWidth + 2 * border;
	int Height = -bmpInfo->bmiHeader.biHeight + 2 * border;
	int Stride = Width * 1;
	if (Integral2 != NULL)
		free(Integral2);
	Integral2 = (double *)malloc((Width + 1)*(Height + 1) * sizeof(double));
	memset(Integral2, 0, (Width + 1) * sizeof(double));					//	第一行都为0
	for (int Y = 0; Y < Height; Y++)
	{
		unsigned char *LinePS = imgWithBorder + Y * Stride;
		double *LinePL = Integral2 + Y * (Width + 1) + 1;				//	上一行位置			
		double *LinePD = Integral2 + (Y + 1) * (Width + 1) + 1;			//	当前位置，注意每行的第一列的值都为0
		LinePD[-1] = 0;												//	第一列的值为0
		double Sum = 0;
		double aa = 0;
		for (int X = 0; X < Width; X++)
		{
			Sum += LinePS[X]*LinePS[X];										//	行方向累加
			LinePD[X] = LinePL[X] + Sum;							//	更新积分图
			aa = LinePD[X];
		}
		if (Sum < 0 || aa <0)
			MessageBox(NULL, NULL, 0,0);
	}
}


void MyImage::localMeadVar()
{
	int width = bmpInfo->bmiHeader.biWidth;
	int height = -bmpInfo->bmiHeader.biHeight;
	if (localMean != NULL)
		free(localMean);
	if (localVar != NULL)
		free(localVar);
	localMean = (double *)malloc(width*height * sizeof(double));
	localVar = (double *)malloc(width*height * sizeof(double));

	int width1 = width + 1 + 2 * 200;
	int N = (2 * border + 1)*(2 * border + 1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int i1 = i + 200 + 1;
			int j1 = j + 200 + 1;	//换算成积分图中对应点的坐标

			double sum1 = Integral[(i1 + border) * width1 + j1 + border] + Integral[(i1 - border - 1) * width1 + j1 - border - 1]
				- Integral[(i1 - border - 1) * width1 + j1 + border] - Integral[(i1 + border) * width1 + j1 - border - 1];
			double sum2 = Integral2[(i1 + border) * width1 + j1 + border] + Integral2[(i1 - border - 1) * width1 + j1 - border - 1]
				- Integral2[(i1 - border - 1) * width1 + j1 + border] - Integral2[(i1 + border) * width1 + j1 - border - 1];
			localMean[i*width + j] = sum1 * 1.0 / N;
			localVar[i*width + j] = ((sum2 * 1.0 - sum1 * sum1 * 1.0 / N) / ((N - 1)*1.0));
		}
	}
}


void MyImage::ACE()
{
	int width = bmpInfo->bmiHeader.biWidth;
	int height = -bmpInfo->bmiHeader.biHeight;
	int N = width * height;

	if (imgAfterACE != NULL)
		free(imgAfterACE);
	imgAfterACE = (BYTE *)malloc(N);

	/*int width1 = width + border * 2 + 1;
	int height1 = height + border * 2 + 1;*/
	int width1 = width + 200 * 2 + 1;
	int height1 = height + 200 * 2 + 1;
	/*double sum11 = Integral[(border + height)*width1 + border + width] + Integral[(border)*width1 + border] -
		Integral[(border + height)*width1 + border] - Integral[(border)*width1 + width + border];
	double sum22 = Integral2[(border + height)*width1 + border + width] + Integral2[(border)*width1 + border] -
		Integral2[(border + height)*width1 + border] - Integral2[(border)*width1 + width + border];*/
	double sum11 = Integral[(200 + height)*width1 + 200 + width] + Integral[(200)*width1 + 200] -
		Integral[(200 + height)*width1 + 200] - Integral[(200)*width1 + width + 200];
	double ax = Integral2[(200 + height)*width1 + 200 + width];
	double sum22 = Integral2[(200 + height)*width1 + 200 + width] + Integral2[(200)*width1 + 200] -
		Integral2[(200 + height)*width1 + 200] - Integral2[(200)*width1 + width + 200];
	double globalMean = sum11 * 1.0 / N;
	double globalStd = sqrt(/*double*/(sum22 - sum11 * sum11 / N) / (N));//仍利用积分图像

	int index = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double CG = globalStd * scale / 100 / sqrt(localVar[index] + 1);
			if (CG > maxCG)
				CG = maxCG;
			double temp = (localMean[index] + CG * (imgAfterZoom[index] - localMean[index]));//ACE算法公式
			if (temp < 0)
				temp = 0;
			if (temp > 255)
				temp = 255;
			imgAfterACE[index] = unsigned char(temp);
			index++;
		}
	}
}

void MyImage::setN(int n)
{
	border = n;
}
void MyImage::setScale(int n)
{
	scale = n;
}
void MyImage::setMaxCG(int cg)
{
	maxCG = cg;
}

bool MyImage::zoom(double zoomRatio)
{
	/*LONG oldHeight = abs(bmpInfo->bmiHeader.biHeight);
	LONG oldWidth = bmpInfo->bmiHeader.biWidth;
	LONG newHeight = abs(LONG(bmpInfo->bmiHeader.biHeight * zoomRatio - 0.5));
	LONG newWidth = LONG(bmpInfo->bmiHeader.biWidth * zoomRatio + 0.5);
	bmpInfo->bmiHeader.biHeight = -newHeight;
	bmpInfo->bmiHeader.biWidth = newWidth;

	if (imgAfterZoom != NULL)
		free(imgAfterZoom);
	imgAfterZoom = (unsigned char*)calloc(newHeight * newWidth, 2);
	if (!imgAfterZoom)
		return false;

	float reciprocal = (float)(1.0 / zoomRatio);

	for (int i = 0; i < newHeight; i++) {
		for (int j = 0; j < newWidth; j++) {
			DWORD i1 = (DWORD)(i * reciprocal + 0.5);
			DWORD j1 = (DWORD)(j * reciprocal + 0.5);
			if (i1 >= 0 && j1 >= 0 && i1 <= oldHeight && j1 <= oldWidth) {
				imgAfterZoom[i * newWidth + j] = buffer[i1 * oldWidth + j1];
			}
		}
	}
	return true;*/
	LONG oldHeight = abs(drImageHeader.height);
	LONG oldWidth = drImageHeader.width;
	LONG newHeight = abs(LONG(oldHeight * zoomRatio - 0.5));
	LONG newWidth = LONG(oldWidth * zoomRatio + 0.5);
	bmpInfo->bmiHeader.biHeight = -newHeight;
	bmpInfo->bmiHeader.biWidth = newWidth;

	if (imgAfterZoom != NULL)
		free(imgAfterZoom);
	imgAfterZoom = (unsigned char*)calloc(newHeight * newWidth, 2);
	if (!imgAfterZoom)
		return false;

	float reciprocal = (float)(1.0 / zoomRatio);

	for (int i = 0; i < newHeight; i++) {
		for (int j = 0; j < newWidth; j++) {
			DWORD i1 = (DWORD)(i * reciprocal + 0.5);
			DWORD j1 = (DWORD)(j * reciprocal + 0.5);
			if (i1 >= 0 && j1 >= 0 && i1 <= oldHeight && j1 <= oldWidth) {
				imgAfterZoom[i * newWidth + j] = buffer[i1 * oldWidth + j1];
			}
		}
	}
	return true;
}


void MyImage::enhance2()
{
}
