#pragma once


extern double *localMean;	//存放局部均值
extern double *localVar;	//存放局部标准差
extern double *integral1;		//存放原图像积分图像
extern double *integral2;		//存放原图像平方的积分图像
extern unsigned char *result;	//对比度提高后的图像灰度数组

extern bool enhance(int,int,int);
extern void displayEnhanceImage(HDC hdc);
