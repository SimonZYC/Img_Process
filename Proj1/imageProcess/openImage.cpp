#include"openImage.h"
#include"stdafx.h"
#include<errno.h>

BITMAPINFO bmpInfo;
BYTE *buffer = NULL;
HBITMAP hCompatibleBitmap = NULL; //����BITMAP  
HBITMAP hOldBitmap = NULL;
bool isOpen = false;
unsigned char *lpImgData = NULL;


char strFilename[MAX_PATH] = {};//���ڽ����ļ���

bool openImage(HWND hWnd);//�ṩ������ӿ�

bool openFile();//�����Ի���ѡ���ļ�

void displayMyImage(HDC);//��Ϣ��Ӧ�л���ͼ��

bool openMyImage(HWND hWnd, char strFilename[]);

bool openImage(HWND hWnd) {

	if (lpImgData != NULL) {
		free(lpImgData);
		lpImgData = NULL;
	}
	if (buffer != NULL) {
		free(buffer);
		buffer = NULL;
	}
	if (openFile()) {
		if (openMyImage(hWnd,strFilename)) {

			return TRUE;
		}
	}//�ɹ������ػ�����
}

bool openFile() {
	OPENFILENAME ofn = { 0 };
	//TCHAR strFilename[MAX_PATH] = { 0 };//���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);//�ṹ���С
	ofn.hwndOwner = NULL;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
	ofn.lpstrFilter = TEXT("�Զ���ͼ��\0*.dr*\0�����ļ�\0*.*\0\0");//���ù���
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

struct myImage {
	WORD width;
	WORD height;
	WORD bbp;
	WORD isSign;
	WORD maxVal;
	WORD reserved[3];
}drImageHeader;

void displayMyImage(HDC hdc) {
	if (isOpen)
	{
		HDC hMemDC = CreateCompatibleDC(hdc);
		hCompatibleBitmap = CreateCompatibleBitmap(hdc, drImageHeader.width, drImageHeader.height);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hCompatibleBitmap);
		
		SetDIBits(hdc, hCompatibleBitmap, 0, drImageHeader.height, buffer, (BITMAPINFO*)&bmpInfo, DIB_RGB_COLORS);
		BitBlt(hdc, 5, 60, drImageHeader.width, drImageHeader.height, hMemDC, 0, 0, SRCCOPY);
		DeleteDC(hMemDC);

	}
}

bool openMyImage(HWND hWnd, char strFilename[]) {
	FILE *fp;
	errno_t err = _wfopen_s(&fp, (LPWSTR)(strFilename), (wchar_t*)("r\0b\0"));
	//fopen_s(&fp, "Image02.dr", "rb");
	if (fp == NULL) {
		MessageBox(NULL, LPWSTR(err), LPWSTR("123"), 0);
		return FALSE; //���ļ����󣬷���
	}

	fread(&drImageHeader, sizeof(myImage), 1, fp);
	unsigned int bytePerPx = drImageHeader.bbp / 8;
	unsigned int imgSize = drImageHeader.height*drImageHeader.width*bytePerPx;

	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biHeight = -drImageHeader.height;
	bmpInfo.bmiHeader.biWidth = drImageHeader.width;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);	//д��BMPͼ��

	lpImgData = (unsigned char*)malloc(imgSize);

	buffer = (BYTE *)malloc(imgSize * 3);
	fread(lpImgData, imgSize, 1, fp);
	for (unsigned int i = 0; i < imgSize; i++) {
		buffer[i * 3] = lpImgData[i];
		buffer[i * 3 + 1] = lpImgData[i];
		buffer[i * 3 + 2] = lpImgData[i];		//RGBֵ�����ڻҶ�ֵ
	}
	isOpen = true;

	return true;
}
