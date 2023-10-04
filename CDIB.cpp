#include "stdafx.h"
#include "CDIB.h"

bool CDIB::LoadDIB(CString path) {
	CFile f;
	f.Open(path, CFile::modeRead);

	BITMAPFILEHEADER tmp_bitmapfileheader;
	f.Read(&tmp_bitmapfileheader, sizeof(BITMAPFILEHEADER));


	if (fileContent) {
		HeapFree(GetProcessHeap(), 0, fileContent);
		fileContent = nullptr;
	}
	
	fileContent = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, tmp_bitmapfileheader.bfSize);
	f.SeekToBegin();
	f.Read(fileContent, tmp_bitmapfileheader.bfSize);
	f.Close();


	bitmapFileHeader = (BITMAPFILEHEADER*)fileContent;
	bitmapInfoHeader = (BITMAPINFOHEADER*)((BYTE*)fileContent + sizeof(BITMAPFILEHEADER));
	palleteContent = (BYTE*)fileContent + +sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bitmapContent = (BYTE*)fileContent + bitmapFileHeader->bfOffBits;

	return true;
}

bool CDIB::PaintDIB(HDC context, CRect target, CRect source) {
	SetStretchBltMode(context, COLORONCOLOR);
	
	double ratio_target = (double)target.bottom / target.right;
	double ratio_source = (double)source.bottom / source.right;

	if (ratio_target > ratio_source) {
		StretchDIBits(context, target.left, target.top, target.right, target.bottom*ratio_source, source.left, source.top, source.right, source.bottom, bitmapContent, (BITMAPINFO*)bitmapInfoHeader, DIB_RGB_COLORS, SRCCOPY);
	}
	else {
		StretchDIBits(context, target.left, target.top, target.right/ratio_source, target.bottom, source.left, source.top, source.right, source.bottom, bitmapContent, (BITMAPINFO*)bitmapInfoHeader, DIB_RGB_COLORS, SRCCOPY);
	}
	
	return true;
}


bool CDIB::CreateGreateScaleDIB(CRect size, int xPPM, int yPPM) {

	if (fileContent) {
		HeapFree(GetProcessHeap(), 0, fileContent);
		fileContent = nullptr;
	}

	size_t pallete_size = 256 * sizeof(RGBQUAD);
	size_t bitmap_size = ((8 * size.right + 31) / 32) * 4 * size.bottom;

	size_t bmp_size = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + pallete_size + bitmap_size;

	fileContent = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bmp_size);
	bitmapFileHeader = (BITMAPFILEHEADER*)fileContent;
	bitmapInfoHeader = (BITMAPINFOHEADER*)((BYTE*)fileContent + sizeof(BITMAPFILEHEADER));
	palleteContent = (BYTE*)fileContent + +sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bitmapContent = (BYTE*)fileContent + bmp_size - bitmap_size;

	bitmapFileHeader->bfType = 0x4D42;
	bitmapFileHeader->bfReserved1 = 0;
	bitmapFileHeader->bfReserved2 = 0;
	bitmapFileHeader->bfSize = bmp_size;
	bitmapFileHeader->bfOffBits = bmp_size - bitmap_size;

	bitmapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader->biWidth = size.right;
	bitmapInfoHeader->biHeight = size.bottom;
	bitmapInfoHeader->biPlanes = 1;
	bitmapInfoHeader->biBitCount = 8;
	bitmapInfoHeader->biCompression = 0;
	bitmapInfoHeader->biSizeImage = bitmap_size;
	bitmapInfoHeader->biXPelsPerMeter = xPPM;
	bitmapInfoHeader->biYPelsPerMeter = yPPM;
	bitmapInfoHeader->biClrUsed = 256;
	bitmapInfoHeader->biClrImportant = 0;

	for (int i = 0; i < 256; i++) {
		RGBQUAD* color = (RGBQUAD*)palleteContent + i;
		color->rgbRed = i;
		color->rgbGreen = i;
		color->rgbBlue = i;
		color->rgbReserved = 0;
	}

	return true;
}

bool CDIB::GetPixel1(int x, int y) {
	int bit_num = (y * bitmapInfoHeader->biWidth + x);
	int byte_num = bit_num / 8;
	int bit_in_byte = bit_num % 8;

	BYTE byte = *((BYTE*)bitmapContent + byte_num);

	return true;
}

BYTE CDIB::GetPixel8(int x, int y) {
	int bytes_row = (8 * bitmapInfoHeader->biWidth + 31) / 32 * 4;

	BYTE* pixel = (BYTE*)bitmapContent + y * bytes_row + x;

	return *pixel;
}

RGBTRIPLE CDIB::GetPixel24(int x, int y) {
	int bytes_row = (24 * bitmapInfoHeader->biWidth + 31) / 32 * 4;

	RGBTRIPLE* pixel = (RGBTRIPLE*)((BYTE*)bitmapContent + y * bytes_row + x * sizeof(RGBTRIPLE));

	return *pixel;
}

bool CDIB::SetPixel8(int x, int y, BYTE val) {
	int bytes_row = (8 * bitmapInfoHeader->biWidth + 31)/32 * 4;

	BYTE* pixel = (BYTE*)bitmapContent + y * bytes_row + x;
	*pixel = val;

	return true;
}

bool CDIB::SaveDIB(CString path) {
	if (!fileContent) {
		return false;
	}
	CFile f;
	f.Open(path, CFile::modeWrite | CFile::modeCreate);
	f.Write(fileContent, bitmapFileHeader->bfSize);
	f.Close();

	return true;
}

CDIB::~CDIB() {
	if (fileContent) {
		HeapFree(GetProcessHeap(), 0, fileContent);
		fileContent = nullptr;
	}
}
