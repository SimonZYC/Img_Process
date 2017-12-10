#include "stdafx.h"
#include "getIntegral.h"
#include<math.h>
#include "localMeanStd.h"

void localMeadVar(double *integral1, double *integral2,double *&localMean, double *&localVar, int width, int height, int bor) {
	if (localMean != NULL)
		free(localMean);
	if (localVar != NULL)
		free(localVar);
	localMean = (double *)malloc(width*height * sizeof(double));
	localVar = (double *)malloc(width*height * sizeof(double));

	int width1 = width + 1 + 2 * bor;
	int N = (2 * bor + 1)*(2 * bor + 1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int i1 = i + bor + 1;
			int j1 = j + bor + 1;	//换算成积分图中对应点的坐标
			
			double sum1 = integral1[(i1 + bor) * width1 + j1 + bor] + integral1[(i1 - bor - 1) * width1 + j1 - bor - 1]
				- integral1[(i1 - bor - 1) * width1 + j1 + bor] - integral1[(i1 + bor) * width1 + j1 - bor - 1];
			double sum2 = integral2[(i1 + bor) * width1 + j1 + bor] + integral2[(i1 - bor - 1) * width1 + j1 - bor - 1]
				- integral2[(i1 - bor - 1) * width1 + j1 + bor] - integral2[(i1 + bor) * width1 + j1 - bor - 1];
			localMean[i*width + j] = sum1*1.0 / N;
			localVar[i*width + j] = ((sum2 * 1.0 - sum1 * sum1 * 1.0 / N) / ((N - 1)*1.0));
		}
	}
}
