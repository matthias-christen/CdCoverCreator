#include "StdAfx.h"
#include "gradient.h"

#include <math.h>

double normalize_rad (double rad)
{
	while (rad > PI)
		rad -= 2 * PI;
	while (rad < -PI)
		rad += 2 * PI;

	return rad;
}

int normalize_deg (int nDeg)
{
	while (nDeg > 3600)
		nDeg -= 3600;
	while (nDeg < 0)
		nDeg += 3600;

	return nDeg;
}

double deg2rad (int nDeg)
{
	return normalize_rad ((double) nDeg * PI / 1800.0);
}

int rad2deg (double rad)
{
	return normalize_deg ((int) (rad * 1800.0 / PI));
}

bool compare_lineargradientdata (LINEARGRADIENTDATA* pLgd1, LINEARGRADIENTDATA* pLgd2)
{
	if (pLgd1->type != pLgd2->type)
		return false;

	if (pLgd1->dAngle != pLgd2->dAngle)
		return false;

	if (pLgd1->nNumColors != pLgd2->nNumColors)
		return false;
	for (int i = 0; i < pLgd1->nNumColors; i++)
	{
		if (pLgd1->colors[i] != pLgd2->colors[i])
			return false;
		if (pLgd1->positions[i] != pLgd2->positions[i])
			return false;
	}

	return true;
}

CGradient::CGradient(void)
{
}

CGradient::~CGradient(void)
{
}

Gdiplus::LinearGradientBrush* CGradient::CreateLinearGradientBrush (CRect rectGradient, double dAngle, Gdiplus::Color* colors, float* positions, int nNumColors)
{
	CPoint ptCenter = rectGradient.CenterPoint ();

	// calculate the points
	CPoint ptRefPoint;
	bool bIsInUpperHalf;

	// in which quadrant is dAngle?
	if (0 <= dAngle && dAngle < PI / 2)
	{
		// first quadrant
		ptRefPoint.x = rectGradient.right;
		ptRefPoint.y = rectGradient.top;
		bIsInUpperHalf = true;
	}
	else if (PI / 2 <= dAngle && dAngle <= PI)
	{
		// second quadrant
		ptRefPoint.x = rectGradient.left;
		ptRefPoint.y = rectGradient.top;
		bIsInUpperHalf = true;
	}
	else if (-PI < dAngle && dAngle < -PI / 2)
	{
		// third quadrand
		ptRefPoint.x = rectGradient.right;
		ptRefPoint.y = rectGradient.top;
		bIsInUpperHalf = false;
	}
	else
	{
		// forth quadrant
		ptRefPoint.x = rectGradient.left;
		ptRefPoint.y = rectGradient.top;
		bIsInUpperHalf = false;
	}

	double sinPhi = sin (dAngle);
	double cosPhi = cos (dAngle);

	// intersect the two lines
	//		f: center point + s * (cos phi, sin phi)
	//		g: ref point + t * (-sin phi, cos phi)
	// f, g are orthogonal; the intersection point is the starting point
	// for the gradient.
	double t = (ptRefPoint.x - ptCenter.x) * sinPhi + (ptRefPoint.y - ptCenter.y) * cosPhi;	
	// compute the first point (= intersection point)
	Gdiplus::Point pt1 ((int) (ptRefPoint.x - t * sinPhi), (int) (ptRefPoint.y - t * cosPhi));
	// pt2 lies opposite of pt1 respective to the center point
	Gdiplus::Point pt2 (2 * ptCenter.x - pt1.X, 2 * ptCenter.y - pt1.Y);

	Gdiplus::LinearGradientBrush* pBrush = new Gdiplus::LinearGradientBrush (
		bIsInUpperHalf ? pt1 : pt2, bIsInUpperHalf ? pt2 : pt1, Gdiplus::Color::Black, Gdiplus::Color::Black);
	pBrush->SetInterpolationColors (colors, positions, nNumColors);

	return pBrush;
}

Gdiplus::LinearGradientBrush* CGradient::CreateLinearGradientBrush (CRect rectGradient, double dAngle, COLORREF* colors, float* positions, int nNumColors)
{
	Gdiplus::Color* cols = new Gdiplus::Color[nNumColors];
	for (int i = 0; i < nNumColors; i++)
		cols[i] = Gdiplus::Color (GetRValue (colors[i]), GetGValue (colors[i]), GetBValue (colors[i]));
	Gdiplus::LinearGradientBrush* pBrush = CGradient::CreateLinearGradientBrush (rectGradient, dAngle, cols, positions, nNumColors);
	delete cols;

	return pBrush;
}

Gdiplus::LinearGradientBrush* CGradient::CreateLinearGradientBrush (CRect rectGradient, double dAngle, COLORREF colStart, COLORREF colEnd)
{
	COLORREF colors[2];
	float positions[2];

	colors[0] = colStart;
	colors[1] = colEnd;

	positions[0] = 0.0;
	positions[1] = 1.0;

	return CGradient::CreateLinearGradientBrush (rectGradient, dAngle, colors, positions, 2);
}

Gdiplus::Brush* CGradient::CreateGradientBrush (CRect rectGradient, double dAngle, Gdiplus::Color* colors, float* positions, int nNumColors, GradientType type, Gdiplus::GraphicsPath** ppPath)
{
	if (ppPath != NULL)
		*ppPath = NULL;

	// if this is a linear gradient, create a linear gradient brush
	if (type == Linear)
		return CGradient::CreateLinearGradientBrush (rectGradient, dAngle, colors, positions, nNumColors);

	// otherwise create a path gradient brush
	Gdiplus::GraphicsPath* pPath = new Gdiplus::GraphicsPath;
	switch (type)
	{
	case Radial:
		pPath->AddEllipse (rectGradient.left, rectGradient.top, rectGradient.Width (), rectGradient.Height ());
		break;
	case Rectangular:
		{
			Gdiplus::Point pt[4];
			pt[0].X = pt[3].X = rectGradient.left;
			pt[0].Y = pt[1].Y = rectGradient.top;
			pt[1].X = pt[2].X = rectGradient.right;
			pt[2].Y = pt[3].Y = rectGradient.bottom;
			pPath->AddPolygon (pt, 4);
		}
		break;
	case Diamond:
		{
			Gdiplus::Point pt[4];
			pt[0].X = pt[2].X = (rectGradient.left + rectGradient.right) / 2;
			pt[0].Y = rectGradient.top;
			pt[1].X = rectGradient.right;
			pt[1].Y = pt[3].Y = (rectGradient.top + rectGradient.bottom) / 2;
			pt[2].Y = rectGradient.bottom;
			pt[3].X = rectGradient.left;
			pPath->AddPolygon (pt, 4);
		}
		break;
	case Triangular:
		{
			Gdiplus::Point pt[3];
			pt[0].X = pt[2].X = (rectGradient.left + rectGradient.right) / 2;
			pt[0].Y = rectGradient.top;
			pt[1].X = rectGradient.right;
			pt[1].Y = pt[2].Y = rectGradient.bottom;
			pt[2].X = rectGradient.left;
			pPath->AddPolygon (pt, 3);
		}
		break;
	case Radial_TopLeft:
		{
			int w = (int) (rectGradient.Width () * 1.4142135623730950488016887242097);
			int h = (int) (rectGradient.Height () * 1.4142135623730950488016887242097);
			pPath->AddEllipse (rectGradient.left - w, rectGradient.top - h,	2 * w, 2 * h);
		}
		break;
	case Wheel:
		{
			Gdiplus::Point* pPt = new Gdiplus::Point[nNumColors + 3];
			Gdiplus::Color* pColors = new Gdiplus::Color[nNumColors + 3];

			float fOldPosition = 0;
			int i = 0;
			for (int j = 0; j < nNumColors; i++, j++)
			{
				if (positions[j] < 0.25)
				{
					pPt[i].X = (int) (rectGradient.left + rectGradient.Width () * positions[j] * 4);
					pPt[i].Y = rectGradient.top;
				}
				else if (positions[j] < 0.5)
				{
					if (fOldPosition < 0.25)
					{
						// old point is on the previous side of the rectangle.
						// add an extra point in the edge
						pPt[i].X = rectGradient.right;
						pPt[i].Y = rectGradient.top;
						double f = (0.25 - fOldPosition) / (positions[j] - fOldPosition);
						pColors[i].SetFromCOLORREF (RGB (
							(int) (colors[j - 1].GetR () + f * (colors[j].GetR () - colors[j - 1].GetR ())),
							(int) (colors[j - 1].GetG () + f * (colors[j].GetG () - colors[j - 1].GetG ())),
							(int) (colors[j - 1].GetB () + f * (colors[j].GetB () - colors[j - 1].GetB ()))));
						i++;
					}

					pPt[i].X = rectGradient.right;
					pPt[i].Y = (int) (rectGradient.top + rectGradient.Height () * (positions[j] * 4 - 1)); 
				}
				else if (positions[j] < 0.75)
				{
					if (fOldPosition < 0.5)
					{
						// old point is on the previous side of the rectangle.
						// add an extra point in the edge
						pPt[i].X = rectGradient.right;
						pPt[i].Y = rectGradient.bottom;
						double f = (0.5 - fOldPosition) / (positions[j] - fOldPosition);
						pColors[i].SetFromCOLORREF (RGB (
							(int) (colors[j - 1].GetR () + f * (colors[j].GetR () - colors[j - 1].GetR ())),
							(int) (colors[j - 1].GetG () + f * (colors[j].GetG () - colors[j - 1].GetG ())),
							(int) (colors[j - 1].GetB () + f * (colors[j].GetB () - colors[j - 1].GetB ()))));
						i++;
					}

					pPt[i].X = (int) (rectGradient.right - rectGradient.Width () * (positions[j] * 4 - 2));
					pPt[i].Y = rectGradient.bottom; 
				}
				else
				{
					if (fOldPosition < 0.75)
					{
						// old point is on the previous side of the rectangle.
						// add an extra point in the edge
						pPt[i].X = rectGradient.left;
						pPt[i].Y = rectGradient.bottom;
						double f = (0.75 - fOldPosition) / (positions[j] - fOldPosition);
						pColors[i].SetFromCOLORREF (RGB (
							(int) (colors[j - 1].GetR () + f * (colors[j].GetR () - colors[j - 1].GetR ())),
							(int) (colors[j - 1].GetG () + f * (colors[j].GetG () - colors[j - 1].GetG ())),
							(int) (colors[j - 1].GetB () + f * (colors[j].GetB () - colors[j - 1].GetB ()))));
						i++;
					}

					pPt[i].X = rectGradient.left;
					pPt[i].Y = (int) (rectGradient.bottom - rectGradient.Height () * (positions[j] * 4 - 3)); 
				}

				pColors[i] = positions[j] == 1 ? colors[0] : colors[j];
				fOldPosition = positions[j];
			}

			pPath->AddLines (pPt, nNumColors + 3);
			delete[] pPt;

			Gdiplus::PathGradientBrush* pBrush = new Gdiplus::PathGradientBrush (pPath);
			INT nNumCols = nNumColors + 3;
			pBrush->SetSurroundColors (pColors, &nNumCols);
			pBrush->SetCenterPoint (Gdiplus::Point ((rectGradient.left + rectGradient.right) / 2, (rectGradient.top + rectGradient.bottom) / 2));
			pBrush->SetCenterColor (colors[nNumColors - 1]);

			if (ppPath != NULL)
				*ppPath = pPath;
			else
				delete pPath;
			delete[] pColors;

			return pBrush;
		}
		break;	// NOTE: the brush is created in the above lines!
	}

	// create the brush
	Gdiplus::PathGradientBrush* pBrush = new Gdiplus::PathGradientBrush (pPath);
	pBrush->SetInterpolationColors (colors, positions, nNumColors);

	if (ppPath != NULL)
		*ppPath = pPath;
	else
		delete pPath;

	return pBrush;
}

Gdiplus::Brush* CGradient::CreateGradientBrush (CRect rectGradient, LINEARGRADIENTDATA lgd, Gdiplus::GraphicsPath** ppPath)
{
		// set the colors
	Gdiplus::Color* pColors = new Gdiplus::Color[lgd.nNumColors];
	for (int i = 0; i < lgd.nNumColors; i++)
		pColors[i].SetFromCOLORREF (lgd.colors[i]);

	Gdiplus::Brush* pBrsh = CreateGradientBrush (rectGradient, lgd.dAngle, pColors, lgd.positions, lgd.nNumColors, lgd.type, ppPath);

	delete[] pColors;
	return pBrsh;
}