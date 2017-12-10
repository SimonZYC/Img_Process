#include "getIntegral.h"
#include "stdafx.h"


void GetGrayIntegralImage(unsigned char *Src, double *&Integral, int Width, int Height, int Stride)
{
	if (Integral != NULL)
		free(Integral);
	Integral = (double *)malloc((Width + 1)*(Height + 1) * sizeof(double));
	memset(Integral, 0, (Width + 1) * sizeof(double));					//	第一行都为0
	for (int Y = 0; Y < Height; Y++)
	{
		unsigned char *LinePS = Src + Y * Stride;
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

//图像平方的积分图像
void GetGrayIntegralImage2(unsigned char *Src, double *&Integral, int Width, int Height, int Stride)
{
	if (Integral != NULL)
		free(Integral);
	Integral = (double *)malloc((Width + 1)*(Height + 1) * sizeof(double));
	memset(Integral, 0, (Width + 1) * sizeof(double));					//	第一行都为0
	for (int Y = 0; Y < Height; Y++)
	{
		unsigned char *LinePS = Src + Y * Stride;
		double *LinePL = Integral + Y * (Width + 1) + 1;				//	上一行位置			
		double *LinePD = Integral + (Y + 1) * (Width + 1) + 1;			//	当前位置，注意每行的第一列的值都为0
		LinePD[-1] = 0;												//	第一列的值为0
		double Sum = 0;
		for (int X = 0; X < Width; X++)
		{
			Sum += LinePS[X]*LinePS[X];										//	行方向累加
			LinePD[X] = LinePL[X] + Sum;							//	更新积分图
		}
	}
}