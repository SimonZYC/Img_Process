#include "stdafx.h"
#include <math.h>
#include "ACE.h"
#include "enhanceInterface.h"
#include<string>
using namespace std;

extern double *localMean;
extern double *localVar;
extern double *integral1;
extern double *integral2;

int ACE( unsigned char *src, unsigned char *&result, int width, int height, int bor,int scale, int maxCG) {
	//int sum = 0;
	//int sum21 = 0;
	//for(int i = 0; i < height; i ++)
	//	for (int j = 0; j < width; j++) {
	//		sum += src[i * width + j];
	//		sum21 += src[i * width + j] * src[i * width + j];
	//	}
	//double globalMean = sum*1.0 / (width*height);

	//double sum2 = 0.0;
	//for (int i = 0; i < height; i++)
	//	for (int j = 0; j < width; j++) {
	//		sum2 += (src[i * width + j] - globalMean) * (src[i * width + j] - globalMean);
	//	}
	//double globalStd = sqrt(sum2 / (width*height));//原始方法求解，验证结果
	
	int N = width*height;

	int width1 = width + bor * 2 + 1;
	int height1 = height + bor * 2 + 1;
	double sum11 = integral1[(bor + height)*width1 + bor + width] + integral1[(bor)*width1 + bor] -
		integral1[(bor + height)*width1 + bor] - integral1[(bor)*width1 + width + bor];
	double sum22 = integral2[(bor + height)*width1 + bor + width] + integral2[(bor)*width1 + bor] -
		integral2[(bor + height)*width1 + bor] - integral2[(bor)*width1 + width + bor];
	double globalMean = sum11*1.0 / N;
	double globalStd = sqrt(/*double*/(sum22 - sum11*sum11 / N) / (N ));//仍利用积分图像
	
	int index = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double CG = globalStd*scale/100 / sqrt(localVar[index]+1);
			if (CG > maxCG)
				CG = maxCG;
			double temp =(localMean[index] + CG*(src[index] - localMean[index]));//ACE算法公式
			if (temp < 0)
				temp = 0;
			if (temp > 255)
				temp = 255;
			result[index] = unsigned char(temp);
			index++;
		}
	}
	return 0;
}