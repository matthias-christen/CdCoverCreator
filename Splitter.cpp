// Splitter.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "Splitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitter

CSplitter::CSplitter()
{
}

CSplitter::~CSplitter()
{
}


BEGIN_MESSAGE_MAP(CSplitter, CStatic)
	//{{AFX_MSG_MAP(CSplitter)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitter message handlers

void CSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture ();

	GetClientRect (&m_rectTracker);
	m_rectTracker.right = 3;	
	OnInvertTracker (m_rectTracker);

	CStatic::OnLButtonDown(nFlags, point);
}

void CSplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture ();
	OnInvertTracker (m_rectTracker);

	GetParent ()->SendMessage (SPLIT_RESIZE, m_rectTracker.left, 0);
	
	CStatic::OnLButtonUp(nFlags, point);
}

void CSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags == MK_LBUTTON)
	{
		if (m_rectTracker.left != point.x)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.OffsetRect(point.x - m_rectTracker.left, 0);
			OnInvertTracker(m_rectTracker);
		}
	}
	
	CStatic::OnMouseMove(nFlags, point);
}

void CSplitter::OnInvertTracker(const CRect &rect)
{
	// pat-blt without clip children on
	CDC* pDC = GetDC();
	// invert the brush pattern (looks just like frame window sizing)
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	if (hOldBrush != NULL)
		SelectObject(pDC->m_hDC, hOldBrush);
	ReleaseDC(pDC);
}
