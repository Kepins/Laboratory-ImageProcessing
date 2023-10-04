// ImgWnd.cpp : implementation file
//

#include "stdafx.h"
#include "PO1.h"
#include "ImgWnd.h"


// CImgWnd

IMPLEMENT_DYNAMIC(CImgWnd, CWnd)

CImgWnd::CImgWnd()
{

}

CImgWnd::~CImgWnd()
{
}


BEGIN_MESSAGE_MAP(CImgWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CImgWnd message handlers

BOOL CImgWnd::Create(const RECT& rect, CWnd*  pParentWnd, UINT nID)
{
	BOOL bRes;

	bRes = CWnd::Create(NULL, NULL,
		WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL, rect, pParentWnd, nID, NULL);

	ShowScrollBar(SB_BOTH, FALSE);

	return bRes;
}


void CImgWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// Do not call CWnd::OnPaint() for painting messages
	HDC context = dc.GetSafeHdc();

	CRect r;
	GetClientRect(r);

	//test
	/*dib.CreateGreateScaleDIB(CRect(0, 0, 700, 800), 2835, 2835);

	for (int i = 0; i < 700; i++)
	{
		dib.SetPixel8(i, i, 0xFF);
	}*/


	if (dib.fileContent) {
		// l, t, r, b
		CRect dib_rect(0, 0, dib.bitmapInfoHeader->biWidth, dib.bitmapInfoHeader->biHeight);
		dib.PaintDIB(context, r, dib_rect);
	}
}
