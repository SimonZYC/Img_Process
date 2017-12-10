#pragma once
#include"stdafx.h"
extern void drawHistogram(PAINTSTRUCT ps, int offsetXBase, int offsetYBase, int *originCount);
extern bool calcHistogram(int *);
extern void displayHis(PAINTSTRUCT ps);

extern int *originCount;
