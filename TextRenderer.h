// TextRenderer.h: interface for the CTextRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTRENDERER_H__C629F88F_0A54_4F51_AA07_B9CE2E8F3321__INCLUDED_)
#define AFX_TEXTRENDERER_H__C629F88F_0A54_4F51_AA07_B9CE2E8F3321__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PI 3.1415926535897932384626433832795

class CTextRenderer  
{
public:
	static void DrawEllipticText(CDC* pDC, CRect rect, int nStartAngle, CString strText);
	static void DrawRoundText (CDC* pDC, HANDLE hndPrinter, CRect rect, bool bFlip, CString strText, double dMaxSweepAngle = 2 * PI - 0.2);
	CTextRenderer();
	virtual ~CTextRenderer();

};

#endif // !defined(AFX_TEXTRENDERER_H__C629F88F_0A54_4F51_AA07_B9CE2E8F3321__INCLUDED_)
