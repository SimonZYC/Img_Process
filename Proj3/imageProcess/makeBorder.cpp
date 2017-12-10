#include"stdafx.h"
#include "makeBorder.h"

void makeBorder(unsigned char *src, unsigned char *&dst,int width, int height, int bor) {
	if (dst != NULL) {
		free(dst);
	}
	dst = (unsigned char *)malloc((width + 2 * bor)*(height + 2 * bor));

	int index = 0;
	for(int i = bor; i < bor + height; i++)
		for (int j = bor; j < bor + width; j++) {
			dst[i*(width + bor*2) + j] = src[index++];
		}//��Ӧλ������ԭ����

	for (int i = bor; i < bor + height; i++) {
		index = 0;
		for (int j = 0; j < bor; j++) {
			dst[i*(width + bor * 2) + j] = dst[i*(width + bor * 2) + j + 2 * bor - index];
			index+=2;
		}
	}//��չ��߽�
	for (int i = bor; i < bor + height; i++) {
		index = 2;
		for (int j = bor + width; j < 2*bor + width; j++) {
			dst[i*(width + bor * 2) + j] = dst[i*(width + bor * 2) + j - index];
			index+=2;
		}
	}//��չ�ұ߽�

	index = 0;
	for (int i = 0; i < bor; i++) {
		for (int j = 0; j < 2 * bor + width; j++) {
			dst[i*(width + bor * 2) + j] = dst[(i + bor * 2 - index)*(width + bor * 2) + j];
		}
		index += 2;
	}//��չ�ϱ߽�

	index = 2;
	for (int i = bor + height; i < 2 * bor + height; i++) {
		for (int j = 0; j < 2 * bor + width; j++) {
			dst[i*(width + bor * 2) + j] = dst[(i - index)*(width + bor * 2) + j];
		}
		index += 2;
	}//��չ�±߽�
}