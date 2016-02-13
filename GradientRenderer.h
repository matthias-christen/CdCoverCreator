// GradientRenderer.h: interface for the CGradientRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADIENTRENDERER_H__A8A681F2_7D11_4993_A074_183AAC0BCC94__INCLUDED_)
#define AFX_GRADIENTRENDERER_H__A8A681F2_7D11_4993_A074_183AAC0BCC94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Gradient.h"

class CGradientRenderer  
{
public:
	static void DrawGradient (CDC* pDC, HANDLE hndPrinter, CRect r, int nAngle, COLORREF crStart, COLORREF crEnd, CPreviewDC* pDCPreview);
	static void DrawGradient (CDC* pDC, HANDLE hndPrinter, CRect r, LINEARGRADIENTDATA* pLgd, CPreviewDC* pDCPreview);
	
	CGradientRenderer();
	virtual ~CGradientRenderer();
};

#endif // !defined(AFX_GRADIENTRENDERER_H__A8A681F2_7D11_4993_A074_183AAC0BCC94__INCLUDED_)
