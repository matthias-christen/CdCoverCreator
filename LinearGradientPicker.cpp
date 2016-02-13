// LinearGradientPicker.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "LinearGradientPicker.h"

#include "Gradient.h"

#include <Gdiplus.h>
#include <math.h>

#define PI 3.1415926535897932384626433832795

#define BLENDPICKER_MARGIN 10
#define BLENDPICKER_HEIGHT 8
#define TICK_YOFFSET 4
#define TICK_WIDTH2 4
#define TICK_HEIGHT 6

// CLinearGradientPicker

IMPLEMENT_DYNAMIC(CLinearGradientPicker, CWnd)

using namespace Gdiplus;

CLinearGradientPicker::CLinearGradientPicker ()
{
	m_nNumColors = 0;
	m_type = Linear;
	m_nCurrentTick = -1;
	m_bMovedTick = false;
}

CLinearGradientPicker::~CLinearGradientPicker()
{
}

void CLinearGradientPicker::SetData (LINEARGRADIENTDATA data)
{
	SetAngle (data.dAngle);
	SetType (data.type);
	SetTicks (data.colors, data.positions, data.nNumColors);
}

void CLinearGradientPicker::GetData (LINEARGRADIENTDATA* pData)
{
	pData->dAngle = m_dAngle;
	pData->nNumColors = m_nNumColors;
	pData->type = m_type;

	for (int i = 0; i < m_nNumColors; i++)
	{
		pData->colors[i] = GetColor (i);
		pData->positions[i] = m_positions[i];
	}
}

void CLinearGradientPicker::SetAngle (double dAngle)
{
	m_dAngle = normalize_rad (dAngle);

	// redraw
	InvalidateRect (NULL, FALSE);
}

void CLinearGradientPicker::SetAngle (int nAngle)
{
	SetAngle ((double) nAngle * PI / 1800.0);
}

void CLinearGradientPicker::SetTicks (COLORREF* colors, float* positions, int nNumTicks)
{
	m_nNumColors = min (nNumTicks, MAX_TICKS);
	for (int i = 0; i < m_nNumColors; i++)
	{
		m_colors[i] = Gdiplus::Color (GetRValue (colors[i]), GetGValue (colors[i]), GetBValue (colors[i]));
		m_positions[i] = positions[i];
	}

	InvalidateRect (NULL, FALSE);
}

void CLinearGradientPicker::SetTick (COLORREF color, float position)
{
	InsertTick (color, position);
	InvalidateRect (NULL, FALSE);
}

void CLinearGradientPicker::Reset ()
{
	m_nNumColors = 0;
	m_type = Linear;
	m_nCurrentTick = -1;
	m_bMovedTick = false;

	InvalidateRect (NULL, FALSE);
}

void CLinearGradientPicker::EndSelection (UINT nMessage)
{
	LINEARGRADIENTDATA* pLgd = new LINEARGRADIENTDATA;

	pLgd->nNumColors = m_nNumColors;
	for (int i = 0; i < m_nNumColors; i++)
	{
		pLgd->colors[i] = GetTickColor (i);
		pLgd->positions[i] = m_positions[i];
	}

	pLgd->dAngle = m_dAngle;
	pLgd->type = m_type;

//	m_pParent->SendMessage (nMessage, 0, (LPARAM) pLgd);
	DestroyWindow ();
}

BEGIN_MESSAGE_MAP(CLinearGradientPicker, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(CPN_SELENDOK, OnColorChanged)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND_RANGE (ID_GRADIENT_LINEAR, ID_GRADIENT_LINEAR+100, OnGradientMenu)
END_MESSAGE_MAP()


// CLinearGradientPicker message handlers

BOOL CLinearGradientPicker::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= SS_NOTIFY;
	return CStatic::PreCreateWindow(cs);
}

void CLinearGradientPicker::PreSubclassWindow() 
{
	LONG lStyle = ::GetWindowLong (GetSafeHwnd (), GWL_STYLE);
	::SetWindowLong (GetSafeHwnd (), GWL_STYLE, lStyle | SS_NOTIFY);
	
	CStatic::PreSubclassWindow();
}

void CLinearGradientPicker::OnPaint()
{
	CPaintDC dc (this); // device context for painting

	CRect rect;
	GetClientRect (&rect);

	Graphics graphics (dc.GetSafeHdc ());

//	CRect rectGradient (0, 0, rect.right, max (rect.bottom - BLENDPICKER_HEIGHT - TICK_HEIGHT - 2, rect.top + (rect.Height () / 4) * 3));
	CRect rectGradient (0, 0, rect.right, rect.bottom - BLENDPICKER_HEIGHT + TICK_YOFFSET - TICK_HEIGHT - 2);
	CPoint ptCenter = rectGradient.CenterPoint ();

	// fill the background if the gradient is non-rectangular
	if (m_type != Linear && m_type != Rectangular)
	{
		SolidBrush brush (m_colors[0]);
		graphics.FillRectangle (&brush, rectGradient.left, rectGradient.top, rectGradient.Width (), rectGradient.Height ());
	}

	// draw the gradient
	Brush* pBrush = CGradient::CreateGradientBrush (rectGradient, m_dAngle, m_colors, m_positions, m_nNumColors, m_type);
	graphics.FillRectangle (pBrush, rectGradient.left, rectGradient.top, rectGradient.Width (), rectGradient.Height ());
	delete pBrush;

	// draw the blend selector
	Rect rectBlendSelector (BLENDPICKER_MARGIN, rectGradient.bottom + 1, rect.Width () - 2 * BLENDPICKER_MARGIN, BLENDPICKER_HEIGHT);
	LinearGradientBrush brush (rectBlendSelector, m_colors[0], m_colors[1], 0);
	brush.SetInterpolationColors (m_colors, m_positions, m_nNumColors);
	graphics.FillRectangle (&brush, rectBlendSelector);
	Pen pen (Color::Black);
	graphics.DrawRectangle (&pen, rectBlendSelector);

	// white rectangle below the gradient for the ticks
	SolidBrush brushWhite (Color::White);
	graphics.FillRectangle (&brushWhite, rectBlendSelector.X, rectBlendSelector.Y + rectBlendSelector.Height,
		rectBlendSelector.Width, TICK_HEIGHT - 1);

	// draw the color ticks
	for (int i = 0; i < m_nNumColors; i++)
		DrawTick (&graphics, i, rect);

	// draw the gradient type selector button
	CPen penSelector (PS_SOLID, 1, RGB (0x70, 0x80, 0xff));
	CPen* pPenOld = (CPen*) dc.SelectObject (&penSelector);
	CBrush* pBrushOld = (CBrush*) dc.SelectStockObject (HOLLOW_BRUSH);
	dc.RoundRect (rectGradient.right - 20, rectGradient.top + 1, rectGradient.right - 2, rectGradient.bottom - 2, 5, 5);
	CPen penSelector1 (PS_SOLID, 1, RGB (0xcc, 0xdd, 0xff));
	dc.SelectObject (&penSelector1);
	dc.RoundRect (rectGradient.right - 19, rectGradient.top + 2, rectGradient.right - 1, rectGradient.bottom - 1, 5, 5);
	POINT pts[3] = {
		{rectGradient.right - 15, (rectGradient.top + rectGradient.bottom) / 2 - 3},
		{rectGradient.right - 7, (rectGradient.top + rectGradient.bottom) / 2 - 3},
		{rectGradient.right - 11, (rectGradient.top + rectGradient.bottom) / 2 + 3}
	};
	CBrush brushSelector (RGB (0xaa, 0xbb, 0xff));
	dc.SelectObject (&brushSelector);
	dc.Polygon (pts, 3);
	dc.SelectObject (pPenOld);
	dc.SelectObject (pBrushOld);
}

////////////////////////////////////////////////////////////////////////////////
// Tick methods

// Insert a tick taking into account that the positions array must be sorted
void CLinearGradientPicker::InsertTick (COLORREF color, float fPosition)
{
	// still place for another tick?
	if (m_nNumColors >= MAX_TICKS)
		return;
	if (fPosition < 0.0 || fPosition > 1.0)
		return;

	// determine insertion position

	if (fPosition == 0.0)
		m_nCurrentTick = 0;
	else if (fPosition == 1.0)
		m_nCurrentTick = m_nNumColors;
	
	for (int i = 0; i < m_nNumColors; i++)
		if (m_positions[i] > fPosition)
		{
			m_nCurrentTick = i;
			break;
		}

	// move all ticks one to the right
	for (int i = m_nNumColors - 1; i >= m_nCurrentTick; i--)
	{
		m_colors[i + 1] = m_colors[i];
		m_positions[i + 1] = m_positions[i];
	}

	// new tick
	m_colors[m_nCurrentTick] = Gdiplus::Color (GetRValue (color), GetGValue (color), GetBValue (color));
	m_positions[m_nCurrentTick] = fPosition;

	m_nNumColors++;
}

void CLinearGradientPicker::DeleteTick (int nIdx)
{
	// move all ticks at the right of the tick to delete one to the left
	m_nNumColors--;
	for (int i = nIdx; i < m_nNumColors; i++)
	{
		m_colors[i] = m_colors[i + 1];
		m_positions[i] = m_positions[i + 1];
	}
}

void CLinearGradientPicker::DrawTick (Graphics* pGraphics, int nTickIdx, CRect& rectPicker)
{
	SolidBrush brush (m_colors[nTickIdx]);
	CPoint pt = GetTickPosition (nTickIdx, rectPicker);
	Point points[3] = { Point (pt.x - TICK_WIDTH2, pt.y + TICK_HEIGHT), Point (pt.x, pt.y), Point (pt.x + TICK_WIDTH2, pt.y + TICK_HEIGHT) };
	pGraphics->FillPolygon (&brush, points, 3);
	Pen pen (Color::Black);
	pGraphics->DrawPolygon (&pen, points, 3);
}

bool CLinearGradientPicker::HitTestTick (CPoint point, int nTickIdx, CRect& rectPicker)
{
	CPoint pt = GetTickPosition (nTickIdx, rectPicker);
	CRect r (pt.x - TICK_WIDTH2, pt.y, pt.x + TICK_WIDTH2, pt.y + TICK_HEIGHT);
	return r.PtInRect (point);
}

COLORREF CLinearGradientPicker::GetTickColor (int nTickIdx)
{
	return RGB (m_colors[nTickIdx].GetRed (), m_colors[nTickIdx].GetGreen (), m_colors[nTickIdx].GetBlue ());
}

CPoint CLinearGradientPicker::GetTickPosition (int nTickIdx, CRect& rectPicker)
{
	return CPoint (BLENDPICKER_MARGIN + (int) ((rectPicker.Width () - 2 * BLENDPICKER_MARGIN) * m_positions[nTickIdx]), rectPicker.bottom - 1 - BLENDPICKER_HEIGHT + TICK_YOFFSET);
}

////////////////////////////////////////////////////////////////////////////////
// Message handlers

void CLinearGradientPicker::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect (&rect);
	CRect rectGradient (0, 0, rect.right, rect.bottom - BLENDPICKER_HEIGHT + TICK_YOFFSET - TICK_HEIGHT - 2);
	CRect rectSelectorBtn (rectGradient.right - 20, rectGradient.top + 2, rectGradient.right - 2, rectGradient.bottom - 2);
	CPoint ptCenter = rectGradient.CenterPoint ();

	CRect rectBlendPicker (BLENDPICKER_MARGIN - TICK_WIDTH2, rectGradient.bottom + 1, rect.right - BLENDPICKER_MARGIN + TICK_WIDTH2, rectGradient.bottom + 1 + BLENDPICKER_HEIGHT + TICK_HEIGHT);
	
	m_bMovedTick = false;

	if (rectSelectorBtn.PtInRect (point))
	{
		CPoint pt (rectGradient.right, rectGradient.bottom);
		ClientToScreen (&pt);

		CMenu menu;
		menu.LoadMenu (IDR_MENU_GRADIENT);

		menu.CheckMenuItem (ID_GRADIENT_LINEAR, MF_BYCOMMAND | (m_type == Linear ? MF_CHECKED : MF_UNCHECKED));
		menu.CheckMenuItem (ID_GRADIENT_RADIAL, MF_BYCOMMAND | (m_type == Radial ? MF_CHECKED : MF_UNCHECKED));
		menu.CheckMenuItem (ID_GRADIENT_RECTANGULAR, MF_BYCOMMAND | (m_type == Rectangular ? MF_CHECKED : MF_UNCHECKED));
		menu.CheckMenuItem (ID_GRADIENT_DIAMOND, MF_BYCOMMAND | (m_type == Diamond ? MF_CHECKED : MF_UNCHECKED));
		menu.CheckMenuItem (ID_GRADIENT_TRIANGULAR, MF_BYCOMMAND | (m_type == Triangular ? MF_CHECKED : MF_UNCHECKED));
		menu.CheckMenuItem (ID_GRADIENT_RADIALTOP, MF_BYCOMMAND | (m_type == Radial_TopLeft ? MF_CHECKED : MF_UNCHECKED));
		menu.CheckMenuItem (ID_GRADIENT_WHEEL, MF_BYCOMMAND | (m_type == Wheel ? MF_CHECKED : MF_UNCHECKED));

		menu.GetSubMenu (0)->TrackPopupMenu (TPM_RIGHTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}
	else if (rectBlendPicker.PtInRect (point))
	{
		SetCapture ();

		// clicked on a tick?
		bool bTickHit = false;
		for (int i = 0; i < m_nNumColors; i++)
			if (HitTestTick (point, i, rect))
			{
				// thick number i has been hit.
				m_nCurrentTick = i;
				bTickHit = true;
				break;
			}

		// no tick has been hit. Add a new tick!
		if (!bTickHit)
			InsertTick (0x000000, (point.x - BLENDPICKER_MARGIN) / (float) (rect.Width () - 2 * BLENDPICKER_MARGIN));
	}

	InvalidateRect (NULL, FALSE);

	CWnd::OnLButtonDown(nFlags, point);
}

void CLinearGradientPicker::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture ();

	if (m_nCurrentTick != -1 && !m_bMovedTick)
	{
		CPoint pt = point;
		ClientToScreen (&pt);
		new CColourPopup (pt, GetTickColor (m_nCurrentTick), this, NULL, "Custom");
	}

	// send update message to parent
	if (m_nCurrentTick != -1 && m_bMovedTick && GetParent () != NULL)
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), LGPN_UPDATE), (LPARAM) GetSafeHwnd ());

//	m_nCurrentTick = -1;
	m_bMovedTick = false;
	InvalidateRect (NULL, FALSE);

	CWnd::OnLButtonUp(nFlags, point);
}

void CLinearGradientPicker::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect (&rect);

	// Deletes a ticker, if clicked on
	for (int i = 1; i < m_nNumColors - 1; i++)	// note: first and last ticks can't be deleted
		if (HitTestTick (point, i, rect))
		{
			DeleteTick (i);

			InvalidateRect (NULL, FALSE);

			// update message for parent
			if (GetParent () != NULL)
				GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), LGPN_UPDATE), (LPARAM) GetSafeHwnd ());

			break;
		}

	CWnd::OnRButtonDown(nFlags, point);
}

void CLinearGradientPicker::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON)
	{
		// moving a tick?
		if (1 <= m_nCurrentTick && m_nCurrentTick < m_nNumColors) // note: first and last ticks can't be moved
		{
			CRect rect;
			GetClientRect (&rect);

			float fTempPos = (point.x - BLENDPICKER_MARGIN) / (float) (rect.Width () - 2 * BLENDPICKER_MARGIN);
			if (m_positions[m_nCurrentTick - 1] < fTempPos && fTempPos < m_positions[m_nCurrentTick + 1])
				m_positions[m_nCurrentTick] = fTempPos;

			m_bMovedTick = true;
			InvalidateRect (NULL, FALSE);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CLinearGradientPicker::OnColorChanged (WPARAM wParam, LPARAM lParam)
{
	if (m_nCurrentTick < 0 || m_nCurrentTick >= MAX_TICKS)
		return 0;

	m_colors[m_nCurrentTick] = Color (GetRValue (wParam), GetGValue (wParam), GetBValue (wParam));

	// update message for parent
	if (GetParent () != NULL)
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), LGPN_UPDATE), (LPARAM) GetSafeHwnd ());

	InvalidateRect (NULL, FALSE);

	return 0;
}

void CLinearGradientPicker::OnGradientMenu (UINT nId)
{
	switch (nId)
	{
	case ID_GRADIENT_LINEAR:
		m_type = Linear;
		break;
	case ID_GRADIENT_RADIAL:
		m_type = Radial;
		break;
	case ID_GRADIENT_RECTANGULAR:
		m_type = Rectangular;
		break;
	case ID_GRADIENT_DIAMOND:
		m_type = Diamond;
		break;
	case ID_GRADIENT_TRIANGULAR:
		m_type = Triangular;
		break;
	case ID_GRADIENT_RADIALTOP:
		m_type = Radial_TopLeft;
		break;
	case ID_GRADIENT_WHEEL:
		m_type = Wheel;
		break;
	}

	if (GetParent () != NULL)
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), LGPN_UPDATE), (LPARAM) GetSafeHwnd ());

	InvalidateRect (NULL, FALSE);	
}