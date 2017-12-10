#include"openBMP.h"
#include"stdafx.h"
#include<errno.h>

BITMAPFILEHEADER bf;
BITMAPINFOHEADER bi;
HPALETTE hPalette = NULL;
HBITMAP  hBitmap = NULL;
HGLOBAL  hImgData = NULL;
DWORD NumColors;//ʵ���õ�����ɫ�� ������ɫ�������е���ɫ����
DWORD LineBytes;  //ÿһ�е��ֽ���

char strFilename[MAX_PATH] = {};//���ڽ����ļ���

bool openImage(HWND hWnd);//�ṩ������ӿ�
bool openBmpImage(HWND hWnd, char strFilename[]);//��ȡBMP��Ҫ�ĳ���
bool openFile();//�����Ի���ѡ���ļ�

void displayBMP(HDC);//��Ϣ��Ӧ�л���ͼ��

bool openImage(HWND hWnd) {
	if (hBitmap != NULL) {
		DeleteObject(hBitmap);
		hBitmap = NULL;
	}
	if (hPalette != NULL) {
		DeleteObject(hPalette);
		hPalette = NULL;
	}
	if (hImgData != NULL) {
		GlobalUnlock(hImgData);
		GlobalFree(hImgData);
		hImgData = NULL;
	}
	if (openFile()) {
		if (openBmpImage(hWnd,strFilename)) {
			InvalidateRect(hWnd, NULL, TRUE);
			return TRUE;
		}
	}//�ɹ������ػ�����
}
bool openBmpImage(HWND hWnd, char strFilename[])
{
	LPBITMAPINFOHEADER lpImgData;//ָ��BITMAPINFOHEADER�ṹ��ָ��
	LOGPALETTE *pPal; //ָ���߼���ɫ��ṹ��ָ��
	LPRGBQUAD lpRGB; //ָ��RGBQUAD�ṹ��ָ��
	HPALETTE hPrevPalette = NULL; //���������豸��ԭ���ĵ�ɫ��
	HDC hDc; //�豸���
	HLOCAL hPal; //�洢��ɫ��ľֲ��ڴ���

	DWORD ImgSize;   //ʵ�ʵ�ͼ������ռ�õ��ֽ���
	
	DWORD i;

	NumColors = 0;

	//HANDLE file 
	FILE *fp;
	errno_t err = _wfopen_s(&fp, (LPWSTR)(strFilename), (wchar_t*)("r\0b\0"));
	if (fp == NULL) {
		MessageBox(NULL, LPWSTR(err), LPWSTR("123"), 0);
		return FALSE; //���ļ����󣬷���
	}
	fread((LPSTR)&bf, sizeof(BITMAPFILEHEADER), 1, fp);//��BITMAPFILEHEADER�ṹ���ļ��ж�������д��bf��

	fread((LPSTR)&bi, sizeof(BITMAPINFOHEADER), 1, fp);//��BITMAPINFOHEADER�ṹ���ļ��ж�������д��bi��

	LineBytes = (DWORD)WIDTHBYTES(bi.biWidth*bi.biBitCount);//LineBytesΪÿһ�е��ֽ���

	ImgSize = (DWORD)LineBytes*bi.biHeight;//ImgSizeΪʵ�ʵ�ͼ������ռ�õ��ֽ���

	if (bi.biClrUsed != 0) {//���bi.biClrUsed��Ϊ�㣬��Ϊʵ���õ�����ɫ��
		NumColors = (DWORD)bi.biClrUsed;//NumColorsΪʵ���õ�����ɫ�� ������ɫ�������е���ɫ����
	}
	else { //�����õ�����ɫ��Ϊ2biBitCount��
		switch (bi.biBitCount) {
			
		case 1:
			NumColors = 2;
			break;
		case 4:
			NumColors = 16;
			break;
		case 8:
			NumColors = 256;
			break;
		case 24:
			NumColors = 0; //�������ɫͼ��û�õ���ɫ��
			break;
		default: //��������������ɫ������Ϊ����
			MessageBox(hWnd, LPCWSTR("Invalid color numbers!"), LPCWSTR("Error Message"), MB_OK | MB_ICONEXCLAMATION);
			fclose(fp);
			return FALSE; //�ر��ļ�������FALSE
		}
	}
	

	if (bf.bfOffBits != (DWORD)(NumColors * sizeof(RGBQUAD) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))) {
		//�������ƫ������ʵ��ƫ����������һ������ɫ������
		MessageBox(hWnd, LPCWSTR("Invalid color numbers!"), LPCWSTR("Error Message"), MB_OK | MB_ICONEXCLAMATION);
		fclose(fp);
		return FALSE; //�ر��ļ�������FALSE
	}

	bf.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + NumColors * sizeof(RGBQUAD) + ImgSize;

	//�����ڴ棬��СΪBITMAPINFOHEADER�ṹ���ȼӵ�ɫ��+ʵ��λͼ
	if ((hImgData = GlobalAlloc(GHND, (DWORD)(sizeof(BITMAPINFOHEADER) + NumColors * sizeof(RGBQUAD) + ImgSize))) == NULL) {
		//�����ڴ����
		MessageBox(hWnd, LPCWSTR("Error alloc memory!"), LPCWSTR("ErrorMessage"), MB_OK | MB_ICONEXCLAMATION);
		fclose(fp);
		return FALSE; //�ر��ļ�������FALSE
	}

	lpImgData = (LPBITMAPINFOHEADER)GlobalLock(hImgData);//ָ��lpImgDataָ����ڴ���

	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);//�ļ�ָ�����¶�λ��BITMAPINFOHEADER��ʼ��

	fread((char *)lpImgData, (long)sizeof(BITMAPINFOHEADER) + (long)NumColors * sizeof(RGBQUAD) + ImgSize, 1, fp);//���ļ����ݶ���lpImgData


	fclose(fp);//�ر��ļ�

	if (NumColors != 0) { //NumColors��Ϊ�㣬˵���õ��˵�ɫ��
						  //Ϊ�߼���ɫ�����ֲ��ڴ棬��СΪ�߼���ɫ��ṹ���ȼ�NumColors��PALETTENTRY
		hPal = LocalAlloc(LHND, sizeof(LOGPALETTE) + NumColors * sizeof(PALETTEENTRY));

		pPal = (LOGPALETTE *)LocalLock(hPal);//ָ��pPalָ����ڴ���

		pPal->palNumEntries = NumColors;//��д�߼���ɫ��ṹ��ͷ

		pPal->palVersion = 0x300;

		lpRGB = (LPRGBQUAD)((LPSTR)lpImgData + (DWORD)sizeof(BITMAPINFOHEADER));//lpRGBָ����ǵ�ɫ�忪ʼ��λ��

																				//��дÿһ��
		for (i = 0; i < NumColors; i++) {
			pPal->palPalEntry[i].peRed = lpRGB->rgbRed;

			pPal->palPalEntry[i].peGreen = lpRGB->rgbGreen;

			pPal->palPalEntry[i].peBlue = lpRGB->rgbBlue;

			pPal->palPalEntry[i].peFlags = (BYTE)0;

			lpRGB++; //ָ���Ƶ���һ��

		}
		hPalette = CreatePalette(pPal);//�����߼���ɫ�壬hPalette��һ��ȫ�ֱ���

		LocalUnlock(hPal);//�ͷžֲ��ڴ�

		LocalFree(hPal);
	}

	hDc = GetDC(hWnd);//����豸�����ľ��

	if (hPalette) { //����ղŲ������߼���ɫ��
		hPrevPalette = SelectPalette(hDc, hPalette, FALSE);//���µ��߼���ɫ��ѡ��DC�����ɵ��߼���ɫ���������� hPrevPalette

		RealizePalette(hDc);
	}

	hBitmap = CreateDIBitmap(hDc, (LPBITMAPINFOHEADER)lpImgData,
		(LONG)CBM_INIT,
		(LPSTR)lpImgData + sizeof(BITMAPINFOHEADER) + NumColors * sizeof(RGBQUAD),
		(LPBITMAPINFO)lpImgData, DIB_RGB_COLORS);//����λͼ���

	if (hPalette && hPrevPalette) {
		SelectPalette(hDc, hPrevPalette, FALSE);
		RealizePalette(hDc);
	}//��ԭ���ĵ�ɫ��(����еĻ�)ѡ���豸�����ľ��

	ReleaseDC(hWnd, hDc); //�ͷ��豸������

	GlobalUnlock(hImgData); //�����ڴ���

	HMENU showHistogram = GetSubMenu(GetMenu(hWnd),1);
	EnableMenuItem(showHistogram, 0, MF_BYPOSITION | MF_ENABLED);//�ò˵��С���ʾֱ��ͼ����ѡ��ɵ����

	HMENU histoEqual = GetSubMenu(GetMenu(hWnd), 1);
	EnableMenuItem(showHistogram, 1, MF_BYPOSITION | MF_ENABLED);//�ò˵��С�ֱ��ͼ���⻯����ѡ��ɵ����

	return TRUE; //�ɹ�����
}

bool openFile() {
	OPENFILENAME ofn = { 0 };
	//TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
	ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
	ofn.lpstrFilter = TEXT("bmpͼ��\0*.bmp*\0�����ļ�\0*.*\0\0");//���ù���
	ofn.nFilterIndex = 1;//����������
	ofn.lpstrFile = LPWSTR(strFilename);//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile = sizeof(strFilename);//����������
	ofn.lpstrInitialDir = NULL;//��ʼĿ¼ΪĬ��
	ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�");//ʹ��ϵͳĬ�ϱ������ռ���
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;//�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
	if (GetOpenFileName(&ofn))
	{
		MessageBox(NULL, LPWSTR(strFilename), TEXT("ѡ����ļ�"), 0);
		return true;
	}
	else {
		return false;
	}
}


void displayBMP( HDC hdc){
	if (hBitmap)
	{
		HDC hMemDC = CreateCompatibleDC(hdc);
		if (hPalette)
		{
			SelectPalette(hdc, hPalette, FALSE);
			SelectPalette(hMemDC, hPalette, FALSE);
			RealizePalette(hdc);
		}
		SelectObject(hMemDC, hBitmap);
		BitBlt(hdc, 5, 5, bi.biWidth, bi.biHeight, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);
		
	}
}