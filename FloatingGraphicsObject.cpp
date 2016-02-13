#include "StdAfx.h"
#include "floatinggraphicsobject.h"

IMPLEMENT_DYNCREATE(CFloatingGraphicsObject, CObject)

CFloatingGraphicsObject::CFloatingGraphicsObject(void)
{
	m_tracker.m_nStyle = 0;
	m_nContextMenuID = 0;
}

CFloatingGraphicsObject::~CFloatingGraphicsObject(void)
{
}

void CFloatingGraphicsObject::Render (CRenderDC rdc, RenderType rt, CStyleTemplate* pStyle, bool bIsPreviewing, bool bIsPrinting)
{
	// compute the rendering rect
	CRect r (
		pStyle->MapX (m_dLeft * 1000, rdc, rt),
		pStyle->MapY (m_dTop * 1000, rdc, rt),
		pStyle->MapX ((m_dLeft + m_dWidth) * 1000, rdc, rt),
		pStyle->MapY ((m_dTop + m_dHeight) * 1000, rdc, rt));

	// render the object
	Render (rdc, rt, r, bIsPrinting);
}

void CFloatingGraphicsObject::Render (CRenderDC rdc, RenderType rt, CRect rectRender, bool bIsPrinting)
{
	AfxMessageBox ("CFloatingGraphicsObject::Render must be overridden by subclasses!");
}

void CFloatingGraphicsObject::DrawTracker (CRenderDC rdc, RenderType rt, CStyleTemplate* pStyle, CSize sizeOffset)
{
	// compute the rendering rect
	CRect r (
		pStyle->MapX (m_dLeft * 1000, rdc, rt),
		pStyle->MapY (m_dTop * 1000, rdc, rt),
		pStyle->MapX ((m_dLeft + m_dWidth) * 1000, rdc, rt),
		pStyle->MapY ((m_dTop + m_dHeight) * 1000, rdc, rt));
	r.OffsetRect (sizeOffset);

	// draw the tracker
	m_tracker.SetRect (r);
	m_tracker.Draw (rdc.m_pDC);
}

void CFloatingGraphicsObject::Deselect ()
{
	m_tracker.m_nStyle = 0;
}

bool CFloatingGraphicsObject::Add (CWnd* pWnd, CPoint pt, RenderType rt, CRenderDC rdc, CStyleTemplate* pStyle, CAutoRichEditCtrl* pEditCtrl, CSize sizeOffset)
{
	if (m_tracker.TrackRubberBand (pWnd, pt))
	{
		m_dLeft = pStyle->MapXr (m_tracker.m_rect.left - sizeOffset.cx, rdc, rt);
		m_dTop = pStyle->MapYr (m_tracker.m_rect.top - sizeOffset.cy, rdc, rt);
		m_dWidth = pStyle->MapXr (m_tracker.m_rect.right - sizeOffset.cx, rdc, rt) - m_dLeft;
		m_dHeight = pStyle->MapYr (m_tracker.m_rect.bottom - sizeOffset.cy, rdc, rt) - m_dTop;	

		Edit (pEditCtrl);

		// a new drawing object is selected
		m_tracker.m_nStyle = CRectTracker::solidLine | CRectTracker::resizeInside;
		return true;
	}

	return false;
}

int CFloatingGraphicsObject::HitTest (CPoint pt)
{
	return m_tracker.HitTest (pt);
}

bool CFloatingGraphicsObject::Track (CWnd* pWnd, CPoint pt, RenderType rt, CRenderDC rdc, CStyleTemplate* pStyle, CSize sizeOffset)
{
	bool bHitObject = m_tracker.HitTest (pt) != CRectTracker::hitNothing;

	if (m_tracker.m_nStyle == 0)	// not selected
	{
		if (!bHitObject)
			return false;

		// select the tracker
		//m_tracker.m_nStyle = CRectTracker::solidLine | CRectTracker::resizeInside;
	}

	bool bResult = m_tracker.Track (pWnd, pt) == TRUE;

	if (bResult)
	{
		m_dLeft = pStyle->MapXr (m_tracker.m_rect.left - sizeOffset.cx, rdc, rt);
		m_dTop = pStyle->MapYr (m_tracker.m_rect.top - sizeOffset.cy, rdc, rt);
		m_dWidth = pStyle->MapXr (m_tracker.m_rect.right - sizeOffset.cx, rdc, rt) - m_dLeft;
		m_dHeight = pStyle->MapYr (m_tracker.m_rect.bottom - sizeOffset.cy, rdc, rt) - m_dTop;	
	}

	return bHitObject || bResult;
}

bool CFloatingGraphicsObject::SetCursor (CWnd* pWnd, UINT nHitTest)
{
	if (m_tracker.m_nStyle == 0)	// not selected
		return false;

	return m_tracker.SetCursor (pWnd, nHitTest) != FALSE;
}

bool CFloatingGraphicsObject::Edit (CAutoRichEditCtrl* pEditCtrl)
{
	return false;
}

void CFloatingGraphicsObject::EndEdit (CAutoRichEditCtrl* pEditCtrl)
{
}

bool CFloatingGraphicsObject::ContextMenu (CView* pView)
{
	if (m_tracker.m_nStyle == 0)
		return false;

	CPoint pt;
	GetCursorPos (&pt);

	CMenu menu;
	menu.LoadMenu (m_nContextMenuID);
	menu.GetSubMenu (0)->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, pView);
}

void CFloatingGraphicsObject::Serialize (CArchive& ar)
{
	if (ar.IsStoring ())
	{
		ar << m_dLeft << m_dTop << m_dWidth << m_dHeight;
	}
	else
	{
		ar >> m_dLeft >> m_dTop >> m_dWidth >> m_dHeight;
	}
}