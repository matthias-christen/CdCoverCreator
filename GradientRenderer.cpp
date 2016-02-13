// GradientRenderer.cpp: implementation of the CGradientRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "GradientRenderer.h"
#include "BitmapPrinter.h"

#include <math.h>

#include <Gdiplus.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define PI 3.1415926535897932384626433832795

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGradientRenderer::CGradientRenderer()
{
}

CGradientRenderer::~CGradientRenderer()
{
}

using namespace Gdiplus;

/**
 *	Draws a gradient filling the rectangle specified by r
 *	in direction of nAngle (which is specified in tenth of degrees)
 *	using a linear shading from color crStart to crEnd
 */
void CGradientRenderer::DrawGradient(CDC* pDC, HANDLE hndPrinter, CRect r, int nAngle, COLORREF crStart, COLORREF crEnd, CPreviewDC* pDCPreview)
{
	Graphics graphics (pDC->GetSafeHdc (), hndPrinter);
	graphics.SetPageUnit (UnitPixel);
	graphics.SetSmoothingMode (SmoothingModeHighQuality);
	graphics.SetInterpolationMode (InterpolationModeHighQualityBicubic);
	graphics.SetPixelOffsetMode (PixelOffsetModeHighQuality);

	Brush* pBrush = CGradient::CreateLinearGradientBrush (r, deg2rad (nAngle), crStart, crEnd);
	graphics.FillRectangle (pBrush, r.left, r.top, r.Width (), r.Height ());
}

void CGradientRenderer::DrawGradient (CDC* pDC, HANDLE hndPrinter, CRect r, LINEARGRADIENTDATA* pLgd, CPreviewDC* pDCPreview)
{
	// normalize angle
	pLgd->dAngle = normalize_rad (pLgd->dAngle);

	Graphics graphics (pDC->GetSafeHdc (), hndPrinter);
	graphics.SetPageUnit (UnitPixel);
	graphics.SetSmoothingMode (SmoothingModeHighQuality);
	graphics.SetInterpolationMode (InterpolationModeHighQualityBicubic);
	graphics.SetPixelOffsetMode (PixelOffsetModeHighQuality);

	if ((pLgd->type != Linear) && (pLgd->type != Rectangular) && (pLgd->type != Radial_TopLeft))
	{
		Color color;
		color.SetFromCOLORREF (pLgd->colors[0]);
		Rect rect (r.left, r.top, r.Width (), r.Height ());
		LinearGradientBrush brush (rect, color, color, 0.0f);
		graphics.FillRectangle (&brush, rect);
	}

	GraphicsPath* pPath = NULL;
	Brush* pBrush = CGradient::CreateGradientBrush (r, *pLgd, &pPath);

	if (pPath == NULL)
		graphics.FillRectangle (pBrush, r.left, r.top, r.Width (), r.Height ());
	else
	{
		if (pLgd->type == Radial_TopLeft)
			graphics.FillRectangle (pBrush, r.left, r.top, r.Width (), r.Height ());
		else
			graphics.FillPath (pBrush, pPath);

		delete pPath;
	}

	delete pBrush;
}