#pragma once
struct imInfo{
	int posX;
	int posY;
	int sizeX;
	int sizeY;
};
HWND WINAPI CreateTrackbar(
	HWND hwndDlg,  // handle of dialog box (parent window)
	HINSTANCE hInst,
	imInfo info,
	HMENU ID,
	UINT iMin,     // minimum value in trackbar range 
	UINT iMax,     // maximum value in trackbar range 
	UINT iSelMin,  // minimum value in trackbar selection 
	UINT iSelMax,
	UINT init);
VOID WINAPI TBNotifications(
	HWND hWnd,
	WPARAM wParam,  // wParam of WM_HSCROLL message 
	HWND hwndTrack, // handle of trackbar window
	int &target,
	HWND hTextWnd
);
