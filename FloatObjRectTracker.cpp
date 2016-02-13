#include "StdAfx.h"
#include "floatobjrecttracker.h"

CFloatObjRectTracker::CFloatObjRectTracker(void)
{
	m_rectReference.left = m_rectReference.top = m_rectReference.right = m_rectReference.bottom = 0;
	m_bKeepAspectRatio = false;
	m_bCanResize = true;
}

CFloatObjRectTracker::~CFloatObjRectTracker(void)
{
}

/*
void CFloatObjRectTracker::AdjustRect (int nHandle, LPRECT lpRect)
{
	if (m_rectReference == CRect (0, 0, 0, 0))
		return;

	TRACE ("AdjustRect (%d,%d)-(%d,%d)\n", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);

	// keep aspect ratio if edge handles are used
	if (0 <= nHandle && nHandle <= 3)
	{
		// has width or height changed more?
		if (m_rectLast.Width () - m_rect.Width () > m_rectLast.Height () - m_rect.Height ())
		{
			// width has changed more => adjust height
			m_rect.bottom = m_rect.top + (m_rect.Height () * m_rectReference.Width ()) / m_rectReference.Height ();
		}
		else
		{
			// height has changed more => adjust width
			m_rect.right = m_rect.left + (m_rect.Width () * m_rectReference.Height ()) / m_rectReference.Width ();
		}
	}

	CRectTracker::AdjustRect (nHandle, lpRect);
}*/

void CFloatObjRectTracker::SetRect (CRect r)
{
	m_rect = r;
	m_rectReference = r;
}

void CFloatObjRectTracker::AdjustRect (int nHandle, LPRECT lpRect)
{
	if (m_bKeepAspectRatio)
	{
		double Aspect = (double) m_rectReference.Width () / (double) m_rectReference.Height ();

		// Initialize the width.
		int nWidth = m_rect.right - m_rect.left;
		// If the width is smaller than minimum, make it the minimum.	
		if(nWidth < m_sizeMin.cx)
			nWidth = m_sizeMin.cx;
		
		// Initialize the height.
		int nHeight = m_rect.bottom - m_rect.top;
		// If the height is smaller than minimum, make it the minimum.
		if(nHeight < m_sizeMin.cy)
			nHeight = m_sizeMin.cy;

		// Set a center point for width.
		int x = m_rect.left + nWidth/2;
		// Set a center point for height.
		int y = m_rect.top + nHeight/2;
		
		// Derive a ratio which would be half the height if
		// the aspect ratio is restored using the current width.
		int nWidth_ratio = (int)(((double)nWidth/Aspect)/2);
		// Derive a ratio which would be half the width if
		// the aspect ratio is restored using the current height.
		int nHeight_ratio = (int)(((double)nHeight*Aspect)/2);

		switch(nHandle)
		{	
		case 0:	// TopLeft handle.
			m_rect.top = y - nWidth_ratio;
			m_rect.left = m_rect.right - (int) ((double) (m_rect.bottom - m_rect.top) * Aspect);
			break;
		
		case 1:	// TopRight handle.
			m_rect.top = y - nWidth_ratio;
			m_rect.right = m_rect.left + (int) ((double) (m_rect.bottom - m_rect.top) * Aspect);
			break;
		
		case 2:	// BottomRight handle.
			m_rect.right = x + nHeight_ratio;
			m_rect.bottom = m_rect.top + (int) ((double) (m_rect.right - m_rect.left) / Aspect);
			break;
		
		case 3:	// BottomLeft handle.
			m_rect.left = x - nHeight_ratio;
			m_rect.bottom = m_rect.top + (int) ((double) (m_rect.right - m_rect.left) / Aspect);
			break;
		}
	}
	else if (!m_bCanResize)
	{
		m_rect.right = m_rect.left + m_rectReference.Width ();
		m_rect.bottom = m_rect.top + m_rectReference.Height ();
	}

	CRectTracker::AdjustRect(nHandle, lpRect);
}