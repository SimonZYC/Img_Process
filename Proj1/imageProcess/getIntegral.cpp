#include "getIntegral.h"
#include "stdafx.h"


void GetGrayIntegralImage(unsigned char *Src, double *&Integral, int Width, int Height, int Stride)
{
	if (Integral != NULL)
		free(Integral);
	Integral = (double *)malloc((Width + 1)*(Height + 1) * sizeof(double));
	memset(Integral, 0, (Width + 1) * sizeof(double));					//	��һ�ж�Ϊ0
	for (int Y = 0; Y < Height; Y++)
	{
		unsigned char *LinePS = Src + Y * Stride;
		double *LinePL = Integral + Y * (Width + 1) + 1;				//	��һ��λ��			
		double *LinePD = Integral + (Y + 1) * (Width + 1) + 1;			//	��ǰλ�ã�ע��ÿ�еĵ�һ�е�ֵ��Ϊ0
		LinePD[-1] = 0;												//	��һ�е�ֵΪ0
		double Sum = 0;
		for (int X = 0; X < Width; X++)
		{
			Sum += LinePS[X];										//	�з����ۼ�
			LinePD[X] = LinePL[X] + Sum;							//	���»���ͼ
		}
	}
}

//ͼ��ƽ���Ļ���ͼ��
void GetGrayIntegralImage2(unsigned char *Src, double *&Integral, int Width, int Height, int Stride)
{
	if (Integral != NULL)
		free(Integral);
	Integral = (double *)malloc((Width + 1)*(Height + 1) * sizeof(double));
	memset(Integral, 0, (Width + 1) * sizeof(double));					//	��һ�ж�Ϊ0
	for (int Y = 0; Y < Height; Y++)
	{
		unsigned char *LinePS = Src + Y * Stride;
		double *LinePL = Integral + Y * (Width + 1) + 1;				//	��һ��λ��			
		double *LinePD = Integral + (Y + 1) * (Width + 1) + 1;			//	��ǰλ�ã�ע��ÿ�еĵ�һ�е�ֵ��Ϊ0
		LinePD[-1] = 0;												//	��һ�е�ֵΪ0
		double Sum = 0;
		for (int X = 0; X < Width; X++)
		{
			Sum += LinePS[X]*LinePS[X];										//	�з����ۼ�
			LinePD[X] = LinePL[X] + Sum;							//	���»���ͼ
		}
	}
}