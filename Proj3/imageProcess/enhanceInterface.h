#pragma once


extern double *localMean;	//��žֲ���ֵ
extern double *localVar;	//��žֲ���׼��
extern double *integral1;		//���ԭͼ�����ͼ��
extern double *integral2;		//���ԭͼ��ƽ���Ļ���ͼ��
extern unsigned char *result;	//�Աȶ���ߺ��ͼ��Ҷ�����

extern bool enhance(int,int,int);
extern void displayEnhanceImage(HDC hdc);
