// TextRenderer.cpp: implementation of the CTextRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "TextRenderer.h"

#include <math.h>
#include <Gdiplus.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextRenderer::CTextRenderer()
{
}

CTextRenderer::~CTextRenderer()
{
}

// 180/pi
#define a180_PI 572.95779513082320876798154814105

using namespace Gdiplus;

void CTextRenderer::DrawRoundText(CDC* pDC, HANDLE hndPrinter, CRect rect, bool bFlip, CString strText, double dMaxSweepAngle)
{
	Graphics graphics (pDC->GetSafeHdc (), hndPrinter);

	graphics.SetPageUnit (UnitPixel);
	graphics.SetSmoothingMode (SmoothingModeHighQuality);
	graphics.SetInterpolationMode (InterpolationModeHighQualityBicubic);
	graphics.SetPixelOffsetMode (PixelOffsetModeHighQuality);

	// prepare the font characteristics
	LOGFONT lf;
	pDC->GetCurrentFont ()->GetLogFont (&lf);
	BSTR bstrFontFace = CString (lf.lfFaceName).AllocSysString ();
	FontFamily ff (bstrFontFace);
	::SysFreeString (bstrFontFace);

	StringFormat sf;
	Point p (0, 0);

	// add the text to the path
	BSTR bstrText = strText.AllocSysString ();
	GraphicsPath path;
	path.AddString (bstrText, lstrlenW (bstrText), &ff,
		((lf.lfWeight > 600) ? FontStyleBold : FontStyleRegular) |
		(lf.lfItalic ? FontStyleItalic : FontStyleRegular) |
		(lf.lfUnderline ? FontStyleUnderline : FontStyleRegular) |
		(lf.lfStrikeOut ? FontStyleStrikeout : FontStyleRegular),
		abs (lf.lfHeight), p, &sf);

	::SysFreeString (bstrText);

	int nPointsCount = path.GetPointCount ();
	Point* points = new Point[nPointsCount];
	path.GetPathPoints (points, nPointsCount);

	Rect rectBounds;
	path.GetBounds (&rectBounds);

	// mirror the points at the vertical middle line
	if (bFlip)
		for (int i = 0; i < nPointsCount; i++)
			points[i].Y = rectBounds.Height - points[i].Y;

	double r, theta;

	double R2 = rect.Width () / 2;
	double R1 = R2 - rectBounds.Height;
	double Alpha = min ((double) rectBounds.Width / R1, dMaxSweepAngle);
	double Alpha0 = (PI + Alpha) / 2;

	// transform the points
	int nFlip = bFlip ? -1 : 1;
	for (int i = 0; i < nPointsCount; i++)
	{
		r = (1 - points[i].Y / (double) rectBounds.Height) * R2 + points[i].Y / (double) rectBounds.Height * R1;
		theta = Alpha0 - points[i].X / (double) rectBounds.Width * Alpha;

		points[i].X = (INT) (r * cos (theta) + rect.CenterPoint ().x);
		points[i].Y = (INT) (rect.CenterPoint ().y - nFlip * r * sin (theta));
	}

	BYTE* types = new BYTE[nPointsCount];
	path.GetPathTypes (types, nPointsCount);

	GraphicsPath path1 (points, types, nPointsCount);

	delete points;
	delete types;

	Color c;
	c.SetFromCOLORREF (pDC->GetTextColor ());
	SolidBrush brush (c);
	graphics.FillPath (&brush, &path1);

	return;



/*
	CFont* pOldFont = pDC->GetCurrentFont ();
	LOGFONT lf;
	pOldFont->GetLogFont (&lf);

	// get the char's width
	int n = strText.GetLength ();
	CSize szExtent;
	int* arrExtents = new int[n + 1];
	::GetTextExtentExPoint (pDC->GetSafeHdc (), strText, n, 0, NULL, &arrExtents[1], &szExtent);
	arrExtents[0] = 0;

	// center of circle
	int xm = (rect.left + rect.right) / 2;
	int ym = (rect.top + rect.bottom) / 2;
	// radius
	int r = min (rect.Width (), rect.Height ()) / 2;
	// radius (of inner circle)
	int ri = r - szExtent.cy;

	double phi, phi_old;

	CFont f;
	if (nStartAngle > nEndAngle)
	{
		// calculate starting angle
		switch (nAlignment)
		{
		case DT_LEFT:
			phi = nStartAngle / a180_PI;
			break;
		case DT_CENTER:
			phi = ((nEndAngle - nStartAngle) / a180_PI - szExtent.cx / (double) ri) / 2;
			break;
		case DT_RIGHT:
			phi = szExtent.cx / (double) ri - nEndAngle / a180_PI;
		}

		for (int i = 0; i < n; i++)
		{
			// calculate angle
			phi_old = phi;
			phi += (arrExtents[i + 1] - arrExtents[i]) / (double) ri;

			// text output
			lf.lfEscapement = 2700 - (int) ((phi + phi_old) / 2 * a180_PI);
			f.CreateFontIndirect (&lf);
			pOldFont = pDC->SelectObject (&f);
			pDC->TextOut ((int) (r * cos (phi_old) + xm), (int) (r * sin (phi_old) + ym), strText.Mid (i, 1));
//			pDC->SelectObject (pOldFont);	// ASSERTs !!?! (is a temp gdi object)
			f.DeleteObject ();
		}
	}
	else
	{
		double r2 = (r + ri) / 2.0;

		// calculate starting angle
		switch (nAlignment)
		{
		case DT_LEFT:
			phi = nStartAngle / a180_PI;
			break;
		case DT_CENTER:
			phi = PI - ((nEndAngle - nStartAngle) / a180_PI - szExtent.cx / r2) / 2;
			break;
		case DT_RIGHT:
			phi = szExtent.cx / r2 - nEndAngle / a180_PI;
		}

		for (int i = 0; i < n; i++)
		{
			// calcualte angle
			phi_old = phi;
			phi -= (arrExtents[i + 1] - arrExtents[i]) / r2;

			// text output
			lf.lfEscapement = 900 - (int) ((phi + phi_old) / 2 * a180_PI);
			f.CreateFontIndirect (&lf);
			pOldFont = pDC->SelectObject (&f);
			pDC->TextOut ((int) (ri * cos (phi_old) + xm), (int) (ri * sin (phi_old) + ym), strText.Mid (i, 1));
//			pDC->SelectObject (pOldFont);	// ASSERTs !!?! (is a temp gdi object)
			f.DeleteObject ();
		}
	}
	//pDC->SelectObject (pOldFont);

	delete arrExtents;
	*/
}

#define MAXSIZE 180

void CTextRenderer::DrawEllipticText(CDC *pDC, CRect rect, int nStartAngle, CString strText)
{
	LOGFONT lf;
	CFont* pOldFont = pDC->GetCurrentFont ();
	pOldFont->GetLogFont (&lf);

	// get char width's
	int n = strText.GetLength ();
	CSize szExtent;
	int* arrExtents = new int[n + 1];
	::GetTextExtentExPoint (pDC->GetSafeHdc (), strText, n, 0, NULL, &arrExtents[1], &szExtent);
	arrExtents[0] = 0;

	// center of ellipse
	int xm = (rect.left + rect.right) / 2;
	int ym = (rect.top + rect.bottom) / 2;
	// half axes
	int a = rect.Width () / 2;
	int b = rect.Height () / 2;

	// starting angle
	double phi = nStartAngle / a180_PI;
	// points on ellipse
	double x, y;
	int w;

	// initialize approximation array
	double dDeltaW[MAXSIZE/2 + 1];
	int i = 0;
	for (double theta = 0; i <= MAXSIZE/2; theta += PI / MAXSIZE, i++)
		dDeltaW[i] = _hypot (
			a * (cos (theta) - cos (theta + PI / MAXSIZE)),
			b * (sin (theta) - sin (theta + PI / MAXSIZE)));

	CFont f;
	for (i = 0; i < n; i++)
	{
		x = a * cos (phi);
		y = b * sin (phi);
		w = arrExtents[i + 1] - arrExtents[i];

		// find nearest angle that matches the width of the char
		double dw = 0;
		int idx = (int) (phi * MAXSIZE / PI);
		int tmp;
		int j = 0;
		for ( ; dw < w; j++)
		{
			tmp = (idx + j) % MAXSIZE;
			dw += dDeltaW[tmp >= MAXSIZE/2 ? MAXSIZE - tmp : tmp];
		}
		phi += j * PI / MAXSIZE;

		// Text output
		lf.lfEscapement = 1800 + (int) (atan2 (x * b * b, y * a * a) * a180_PI);
		f.CreateFontIndirect (&lf);
		pDC->SelectObject (&f);
		pDC->TextOut ((int) (x + xm), (int) (y + ym), strText.Mid (i, 1));
		f.DeleteObject ();
	}

	pDC->SelectObject (pOldFont);

	delete arrExtents;
}
