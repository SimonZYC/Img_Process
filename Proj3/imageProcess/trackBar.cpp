#include"stdafx.h"
#include "trackBar.h"
//#include"resource.h"
#include "imageProcess.h"

extern int border;
extern int scale;
extern int maxCG;
extern bool imgExist;

// CreateTrackbar - creates and initializes a trackbar. 
// 
// Global variable
//     g_hinst - instance handle
//
HWND WINAPI CreateTrackbar(
	HWND hWnd,  // handle of dialog box (parent window)
	HINSTANCE hInst,
	imInfo info,
	HMENU ID,
	UINT iMin,     // minimum value in trackbar range 
	UINT iMax,     // maximum value in trackbar range 
	UINT iSelMin,  // minimum value in trackbar selection 
	UINT iSelMax,
	UINT init)  // maximum value in trackbar selection 
{

	//InitCommonControls(); // loads common control's DLL 

	HWND hwndTrack = CreateWindowEx(
		0,                               // no extended styles 
		TRACKBAR_CLASS,                  // class name 
		LPCWSTR("Trackbar Control"),              // title (caption) 
		WS_CHILD |
		WS_VISIBLE |
		TBS_AUTOTICKS |
		TBS_ENABLESELRANGE,              // style 
		info.posX, info.posY,                          // position 
		info.sizeX, info.sizeY,                         // size 
		hWnd,                         // parent window 
		ID,                     // control identifier 
		hInst,                         // instance 
		NULL                             // no WM_CREATE parameter 
	);

	SendMessage(hwndTrack, TBM_SETRANGE,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)MAKELONG(iMin, iMax));  // min. & max. positions

	SendMessage(hwndTrack, TBM_SETPAGESIZE,
		0, (LPARAM)4);                  // new page size 

	SendMessage(hwndTrack, TBM_SETSEL,
		(WPARAM)FALSE,                  // redraw flag 
		(LPARAM)MAKELONG(iSelMin, iSelMax));

	SendMessage(hwndTrack, TBM_SETPOS,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)init);

	SetFocus(hwndTrack);

	return hwndTrack;
}


// TBNotifications - handles trackbar notifications received 
// in the wParam parameter of WM_HSCROLL. This function simply 
// ensures that the slider remains within the selection range. 

VOID WINAPI TBNotifications(
	HWND hWnd,
	WPARAM wParam,  // wParam of WM_HSCROLL message 
	HWND hwndTrack, // handle of trackbar window
	int &target,
	HWND hTextWnd
	)
{
	DWORD dwPos;    // current position of slider 

	dwPos = SendMessage(hwndTrack, TBM_GETPOS, 0, 0);
	TCHAR szTextBuf[20];
	wsprintf(szTextBuf, TEXT("%d"), dwPos);
	SetWindowText(hTextWnd, szTextBuf);
	RECT rec;
	GetWindowRect(hTextWnd, &rec);
	ScreenToClient(hWnd,(LPPOINT)&rec);
	InvalidateRect(hWnd, &rec, TRUE);
	//UpdateWindow(hTextWnd);
	switch (LOWORD(wParam)) {

	case TB_ENDTRACK:
		//MessageBox(NULL, TEXT("123"), TEXT("123"), NULL);
		dwPos = SendMessage(hwndTrack, TBM_GETPOS, 0, 0);

		target = dwPos;
		
		if (imgExist) {
			enhance(border, scale, maxCG);
			InvalidateRect(hWnd, NULL, TRUE);
		}

		break;

	default:

		break;
	}
}