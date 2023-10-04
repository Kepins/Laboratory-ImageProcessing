#pragma once

class CDIB {
public:
	BITMAPFILEHEADER* bitmapFileHeader;
	BITMAPINFOHEADER* bitmapInfoHeader;
	LPVOID palleteContent = nullptr;
	LPVOID bitmapContent = nullptr;

	LPVOID fileContent = nullptr;

	bool LoadDIB(CString path);
	bool PaintDIB(HDC context, CRect target, CRect source);
	bool CreateGreateScaleDIB(CRect size, int xPPM, int yPPM);
	bool GetPixel1(int x, int y);
	BYTE GetPixel8(int x, int y);
	RGBTRIPLE GetPixel24(int x, int y);
	bool SetPixel8(int x, int y, BYTE val);
	bool SaveDIB(CString path);

	~CDIB();
};
