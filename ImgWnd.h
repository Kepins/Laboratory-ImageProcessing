#pragma once
#include "CDIB.h"
// CImgWnd

class CImgWnd : public CWnd
{
	DECLARE_DYNAMIC(CImgWnd)

public:
	CImgWnd();
	virtual ~CImgWnd();

	BOOL Create(const RECT& rect, CWnd*  pParentWnd, UINT nID);

protected:
	DECLARE_MESSAGE_MAP()
public:
	CDIB dib;
	afx_msg void OnPaint();
};
