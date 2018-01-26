#pragma once
#include <GdiPlus.h>
using namespace Gdiplus;

class CInitGdiplus
{
public:
	CInitGdiplus(void);
	~CInitGdiplus(void);

private:
	ULONG_PTR m_GdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
};

