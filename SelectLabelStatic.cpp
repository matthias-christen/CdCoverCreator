// SelectLabelStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "SelectLabelStatic.h"


// CSelectLabelStatic

IMPLEMENT_DYNAMIC(CSelectLabelStatic, CStatic)
CSelectLabelStatic::CSelectLabelStatic()
{
	m_nState = 0;
}

CSelectLabelStatic::~CSelectLabelStatic()
{
}


BEGIN_MESSAGE_MAP(CSelectLabelStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CSelectLabelStatic message handlers


BOOL CSelectLabelStatic::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= SS_NOTIFY;
	return CStatic::PreCreateWindow(cs);
}

void CSelectLabelStatic::PreSubclassWindow()
{
	LONG lStyle = ::GetWindowLong (GetSafeHwnd (), GWL_STYLE);
	::SetWindowLong (GetSafeHwnd (), GWL_STYLE, lStyle | SS_NOTIFY);

	CStatic::PreSubclassWindow();
}

void CSelectLabelStatic::SetState (int nState)
{
	m_nState = nState;
	InvalidateRect (NULL, FALSE);

	if (GetParent () != NULL)
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), SLN_STATECHANGED), (LPARAM) GetSafeHwnd ());
}

#define MARGIN 2

void CSelectLabelStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect r;
	GetClientRect (&r);

	// r.Width : r.Height = 1/sqrt(2)
	int nTmp = r.bottom;	// bottom = height, since top = 0
	r.bottom = (int) (r.right * 1.414213562);	// right = width, since left = 0
	if (r.bottom > nTmp)
	{
		r.bottom = nTmp;
		r.right = (int) (r.bottom / 1.414213562);
	}

	dc.SelectStockObject (WHITE_BRUSH);
	dc.SelectStockObject (BLACK_PEN);
	dc.Rectangle (&r);

	int nRadius = (int) ((r.Width () - MARGIN) * 0.56) / 2;

	CRect rectLabel;

	// draw labels
	DrawLabel (&dc, CPoint (MARGIN + nRadius, MARGIN + nRadius), nRadius, m_nState & 1);
	DrawLabel (&dc, CPoint (r.right - MARGIN - nRadius, r.bottom / 2), nRadius, m_nState & 2);
	DrawLabel (&dc, CPoint (MARGIN + nRadius, r.bottom - MARGIN - nRadius), nRadius, m_nState & 4);
}

void CSelectLabelStatic::DrawLabel (CDC* pDC, CPoint ptCenter, int nRadius, bool bSelected)
{
	CBrush brush1 (bSelected ? GetSysColor (COLOR_HIGHLIGHT) : RGB (0xff, 0xff, 0xff));
	CPen pen (PS_SOLID, 1, GetSysColor (bSelected ? COLOR_WINDOWTEXT : COLOR_INACTIVEBORDER));
	CBrush* pOldBrush = (CBrush*) pDC->SelectObject (&brush1);
	CPen* pOldPen = (CPen*) pDC->SelectObject (&pen);
	pDC->Ellipse (ptCenter.x - nRadius, ptCenter.y - nRadius, ptCenter.x + nRadius, ptCenter.y + nRadius);

	int nSmlRd = (int) (nRadius * 0.31);
	pDC->SelectStockObject (WHITE_BRUSH);
	pDC->Ellipse (ptCenter.x - nSmlRd, ptCenter.y - nSmlRd, ptCenter.x + nSmlRd, ptCenter.y + nSmlRd);

	pDC->SelectObject (pOldBrush);
	pDC->SelectObject (pOldPen);
}

void CSelectLabelStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect r;
	GetClientRect (&r);

	// r.Width : r.Height = 1/sqrt(2)
	int nTmp = r.bottom;	// bottom = height, since top = 0
	r.bottom = (int) (r.right * 1.414213562);	// right = width, since left = 0
	if (r.bottom > nTmp)
	{
		r.bottom = nTmp;
		r.right = (int) (r.bottom / 1.414213562);
	}

	int nDiameter = (int) ((r.Width () - MARGIN) * 0.56);

	// is the point in any of the labels?
	CRect rectLabel1 (MARGIN, MARGIN, MARGIN + nDiameter, MARGIN + nDiameter);
	CRect rectLabel2 (r.right - MARGIN - nDiameter, (r.bottom - nDiameter) / 2, r.right - MARGIN, (r.bottom + nDiameter) / 2);
	CRect rectLabel3 (MARGIN, r.bottom - MARGIN - nDiameter, MARGIN + nDiameter, r.bottom - MARGIN);

	if (rectLabel1.PtInRect (point))
		m_nState ^= 1;
	else if (rectLabel2.PtInRect (point))
		m_nState ^= 2;
	else if (rectLabel3.PtInRect (point))
		m_nState ^= 4;

	InvalidateRect (NULL, FALSE);

	if (GetParent () != NULL)
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), SLN_STATECHANGED), (LPARAM) GetSafeHwnd ());

	CStatic::OnLButtonDown(nFlags, point);
}
