// DegreesStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "DegreesStatic.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDegreesStatic

CDegreesStatic::CDegreesStatic()
{
	m_dNeedleWidth = 2;
	m_dAngle = 0;
}

CDegreesStatic::~CDegreesStatic()
{
}


BEGIN_MESSAGE_MAP(CDegreesStatic, CStatic)
	//{{AFX_MSG_MAP(CDegreesStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDegreesStatic message handlers

BOOL CDegreesStatic::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= SS_NOTIFY;
	return CStatic::PreCreateWindow(cs);
}

void CDegreesStatic::PreSubclassWindow() 
{
	LONG lStyle = ::GetWindowLong (GetSafeHwnd (), GWL_STYLE);
	::SetWindowLong (GetSafeHwnd (), GWL_STYLE, lStyle | SS_NOTIFY);
	
	CStatic::PreSubclassWindow();
}

void CDegreesStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect (&rcClient);

	// get maximal square centered in rcClient.
	CRect r = rcClient;
	if (rcClient.Width () > rcClient.Height ())
	{
		r.left = (rcClient.Width () - rcClient.Height ()) / 2;
		r.right = r.left + rcClient.Height ();
	}
	else
	{
		r.top = (rcClient.Height () - rcClient.Width ()) / 2;
		r.bottom = r.top + rcClient.Width ();
	}

	r.DeflateRect (0, 0, 1, 1);

	// draw borders
	CPen penShadow (PS_SOLID, 1, ::GetSysColor (COLOR_3DSHADOW));
	CPen penInner (PS_SOLID, 1, IsWindowEnabled () ? ::GetSysColor (COLOR_3DHILIGHT) : ::GetSysColor (COLOR_BTNFACE));

	HBRUSH hOldBrush = (HBRUSH) ::SelectObject (dc.GetSafeHdc (), ::GetSysColorBrush (COLOR_3DSHADOW));
	CPen* pOldPen = dc.SelectObject (&penShadow);
	dc.Ellipse (r);
	r.OffsetRect (1, 1);
	::SelectObject (dc.GetSafeHdc (), ::GetSysColorBrush (IsWindowEnabled () ? COLOR_3DHILIGHT : COLOR_BTNFACE));
	dc.SelectObject (&penInner);
	dc.Ellipse (r);

	double radius = min (r.Width (), r.Height ()) / 2.0 - 2;

	// draw ticks
	CPen penTicks (PS_SOLID, 1, RGB (0x00, 0x00, 0xFF));
	dc.SelectObject (&penTicks);
	double s = radius - 3;
	double cos_phi, sin_phi;
	for (int i = 0; i < 12; i++)
	{
		cos_phi = cos (0.52359877559829887307710723054658 * i);
		sin_phi = sin (0.52359877559829887307710723054658 * i);
		dc.MoveTo ((int) (s * cos_phi) + r.CenterPoint ().x, (int) (s * sin_phi) + r.CenterPoint ().y);
		dc.LineTo ((int) (radius * cos_phi) + r.CenterPoint ().x, (int) (radius * sin_phi) + r.CenterPoint ().y);
	}

	// draw the needle
	::SelectObject (dc.GetSafeHdc (), ::GetSysColorBrush (IsWindowEnabled () ? COLOR_HIGHLIGHT : COLOR_3DSHADOW));
	CPen penNeedle (PS_SOLID, 1, ::GetSysColor (COLOR_HIGHLIGHT));
	dc.SelectObject (&penNeedle);

	double sinPhi = sin (m_dAngle);
	double cosPhi = cos (m_dAngle);
	POINT pt[5];
	pt[0] = r.CenterPoint ();
	pt[1].x = (int) (radius / 2 * cosPhi - m_dNeedleWidth * sinPhi) + r.CenterPoint ().x;
	pt[1].y = r.CenterPoint ().y - (int) (radius / 2 * sinPhi + m_dNeedleWidth * cosPhi);
	pt[2].x = (int) (radius * cosPhi) + r.CenterPoint ().x;
	pt[2].y = r.CenterPoint ().y - (int) (radius * sinPhi);
	pt[3].x = (int) (radius / 2 * cosPhi + m_dNeedleWidth * sinPhi) + r.CenterPoint ().x;
	pt[3].y = r.CenterPoint ().y - (int) (radius / 2 * sinPhi - m_dNeedleWidth * cosPhi);
	pt[4] = r.CenterPoint ();
	dc.Polygon (pt, 4);
//	dc.MoveTo (pt[0]);
//	dc.PolylineTo (pt, 5);

	CRect rcDot (r.CenterPoint () - CSize (2, 2), CSize (4, 4));
	dc.Ellipse (&rcDot);

	::SelectObject (dc.GetSafeHdc (), hOldBrush);
	dc.SelectObject (pOldPen);

	penInner.DeleteObject ();
	penShadow.DeleteObject ();
	penTicks.DeleteObject ();
	penNeedle.DeleteObject ();
}

void CDegreesStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	UpdateDisplay (point);
	if (GetParent () != NULL)
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), DSN_BUTTONUP), (LPARAM) GetSafeHwnd ());

	CStatic::OnLButtonUp(nFlags, point);
}

void CDegreesStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON)
		UpdateDisplay (point);

	CStatic::OnMouseMove(nFlags, point);
}

void CDegreesStatic::UpdateDisplay(CPoint pt)
{
	CRect r;
	GetClientRect (&r);
	m_dAngle = atan2 ((double) (r.Height () / 2 - pt.y), (double) (pt.x - r.Width () / 2));
	TRACE ("%f\n", m_dAngle);

	if (GetParent () != NULL)
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), DSN_UPDATE), (LPARAM) GetSafeHwnd ());

	if (::IsWindow (GetSafeHwnd ()))
		InvalidateRect (NULL);
}

void CDegreesStatic::SetAngle(double dAngle)
{
	m_dAngle = dAngle;
	if (::IsWindow (GetSafeHwnd ()))
		InvalidateRect (NULL);
}

void CDegreesStatic::SetDegAngle(int nAngle)
{
	m_dAngle = nAngle * 0.0017453292519943295769236907684886;
	if (::IsWindow (GetSafeHwnd ()))
		InvalidateRect (NULL);
}

int CDegreesStatic::GetDegAngle()
{
	int nAngle = (int) (m_dAngle * 572.95779513082320876798154814105);
	while (nAngle < 0)
		nAngle += 3600;
	return nAngle % 3600;
}
