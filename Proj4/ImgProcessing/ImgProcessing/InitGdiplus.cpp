#include "stdafx.h"
#include "InitGdiplus.h"



CInitGdiplus::CInitGdiplus(void)
{
	GdiplusStartup(&m_GdiplusToken, &gdiplusStartupInput, NULL);
}


CInitGdiplus::~CInitGdiplus(void)
{
	GdiplusShutdown(m_GdiplusToken);
}
