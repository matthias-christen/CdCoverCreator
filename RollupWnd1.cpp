// RollupWnd1.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "MainFrm.h"
#include "RollupWnd1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef WM_NCMOUSELEAVE
	#define WM_NCMOUSELEAVE 0x02A2
#endif

#ifndef TME_NONCLIENT
	#define TME_NONCLIENT 0x0010
#endif

/////////////////////////////////////////////////////////////////////////////
// CRollupWnd dialog


CRollupWnd::CRollupWnd(UINT nId, CWnd* pParent /*=NULL*/)
	: CDialog(nId, pParent)
{
	//{{AFX_DATA_INIT(CRollupWnd)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bTrackLeave = false;
	m_bIsExpanded = true;
	m_bIsExpanding = false;
	m_bCollapseBtnClicked = false;
	m_nIdTimer = 0;

	m_fntCloseUpBtn.CreateFont (8, 0, 0, 0, 0, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, "Marlett");
}

CRollupWnd::~CRollupWnd ()
{
	m_fntCloseUpBtn.DeleteObject ();
}

void CRollupWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRollupWnd)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRollupWnd, CDialog)
	//{{AFX_MSG_MAP(CRollupWnd)
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_NCMOUSEMOVE()
	ON_WM_CLOSE()
	ON_WM_NCPAINT()
	ON_WM_ACTIVATE()
	ON_WM_NCACTIVATE()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCLBUTTONUP()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_NCMOUSELEAVE, OnNcMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRollupWnd message handlers

BOOL CRollupWnd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LONG lExStyle = GetWindowLong (GetSafeHwnd (), GWL_EXSTYLE);
	SetWindowLong (GetSafeHwnd (), GWL_EXSTYLE, lExStyle | WS_EX_TOOLWINDOW);

	GetWindowRect (&m_rectSize);
	Collapse ();
	
	return TRUE;
}

BOOL CRollupWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CDialog::PreCreateWindow(cs);
}

void CRollupWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTrackLeave)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (TRACKMOUSEEVENT);
		tme.hwndTrack = GetSafeHwnd ();
		tme.dwFlags = TME_LEAVE;

		_TrackMouseEvent (&tme);

		m_bTrackLeave = true;
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

LRESULT CRollupWnd::OnMouseLeave (WPARAM wParam, LPARAM lParam)
{
	m_bTrackLeave = false;

	CRect r;
	GetWindowRect (&r);
	CPoint pt;
	GetCursorPos (&pt);
	if (!r.PtInRect (pt))
	{
		AfxGetMainWnd ()->SetFocus ();

		if (m_nIdTimer == 0)
			m_nIdTimer = SetTimer (1, 1000, NULL);
	}

	return S_OK;
}

void CRollupWnd::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	if (!m_bTrackLeave)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (TRACKMOUSEEVENT);
		tme.hwndTrack = GetSafeHwnd ();
		tme.dwFlags = TME_NONCLIENT | TME_LEAVE;

		_TrackMouseEvent (&tme);

		m_bTrackLeave = true;
	}

	if (!m_bCollapseBtnClicked)
		Expand ();

	CDialog::OnNcMouseMove(nHitTest, point);
}

LRESULT CRollupWnd::OnNcMouseLeave (WPARAM wParam, LPARAM lParam)
{
	m_bTrackLeave = false;

	CRect r;
	GetWindowRect (&r);
	CPoint pt;
	GetCursorPos (&pt);
	if (!r.PtInRect (pt))
	{
		AfxGetMainWnd ()->SetFocus ();
		m_bCollapseBtnClicked = false;

		if (m_nIdTimer == 0)
			m_nIdTimer = SetTimer (1, 1000, NULL);
	}

	return S_OK;
}

void CRollupWnd::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	if (DrawCloseupButton (point))
	{
		m_bCollapseBtnClicked = true;
		Collapse ();
	}
	else
		Expand ();

	CDialog::OnNcLButtonDown(nHitTest, point);
}

void CRollupWnd::OnTimer(UINT nIDEvent) 
{
	CRect r;
	GetWindowRect (&r);
	CPoint pt;
	GetCursorPos (&pt);

	if (!r.PtInRect (pt))
	{
		KillTimer (m_nIdTimer);
		m_nIdTimer = 0;
		Collapse ();
	}
	else
	{
		if (m_bIsExpanding)
		{
			SetWindowPos (NULL, 0, 0, m_rectSize.Width (), m_rectSize.Height (), SWP_NOZORDER | SWP_NOMOVE);
			KillTimer (m_nIdTimer);
			m_nIdTimer = 0;
			m_bIsExpanded = true;
		}
	}

	m_bIsExpanding = false;

	CDialog::OnTimer(nIDEvent);
}

void CRollupWnd::Expand()
{
	if (m_bIsExpanded)
		return;

	m_bIsExpanding = true;

	// set the timer. If the mouse is still on the window after the timer time
	// has elapsed, unfold the window.
	m_nIdTimer = SetTimer (1, 300, NULL);
}

void CRollupWnd::Collapse()
{
	if (!m_bIsExpanded)
		return;

	SetWindowPos (NULL, 0, 0, m_rectSize.Width (),
		::GetSystemMetrics (SM_CYSMCAPTION) + 2 * ::GetSystemMetrics (SM_CYDLGFRAME), SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

	if (AfxGetMainWnd () != NULL)
		if (::IsWindow (AfxGetMainWnd ()->GetSafeHwnd ()))
			AfxGetMainWnd ()->SetFocus ();

	m_bIsExpanded = false;
}

void CRollupWnd::OnClose() 
{
	ShowWindow (SW_HIDE);
}

/**
 * Call this method to re-position the window, typically after resizing
 * or moving the parent window.
 */
void CRollupWnd::Reposition()
{
	CMainFrame* pParentWnd = (CMainFrame*) GetParent ();
	if (pParentWnd == NULL)
		return;

	CRect rectParent;
	::GetWindowRect (pParentWnd->m_hWndMDIClient, &rectParent);

	int x, y;

	if (m_nArea <= 2)	// upper edge
		y = m_ptRelCoords.y + rectParent.top;
	else	// lower edge
		y = m_ptRelCoords.y + rectParent.bottom;

	if (m_nArea % 2)	// left edge
		x = m_ptRelCoords.x + rectParent.left;
	else	// right edge
		x = m_ptRelCoords.x + rectParent.right;

	SetWindowPos (NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void CRollupWnd::CalcRelPosition()
{
	// Check where this window is: at the right, left or in the middle
	// of the parent window
	// Areas:
	//
	//   1 | 2       1,2 relative to upper edge,
	//  ---+---      1,3 relative to left edge, etc.
	//   3 | 4 
	//
	
	if (!::IsWindow (m_hWnd))
		return;

	CMainFrame* pParentWnd = (CMainFrame*) GetParent ();
	if (pParentWnd == NULL)
		return;

	CRect rectParent, rect;
	::GetWindowRect (pParentWnd->m_hWndMDIClient, &rectParent);
	GetWindowRect (&rect);

	// x coord
	if (abs (rect.left - rectParent.left) < abs (rect.right - rectParent.right))
	{
		m_nArea = 1;
		m_ptRelCoords.x = rect.left - rectParent.left;
	}
	else
	{
		m_nArea = 2;
		m_ptRelCoords.x = rect.left - rectParent.right;
	}

	// y coord
	if (abs (rect.top - rectParent.top) < abs (rect.bottom - rectParent.bottom))
		m_ptRelCoords.y = rect.top - rectParent.top;
	else
	{
		m_nArea += 2;
		m_ptRelCoords.y = rect.top - rectParent.bottom;
	}
}

void CRollupWnd::OnNcPaint() 
{
	CDialog::OnNcPaint ();
	DrawCloseupButton ();
}

void CRollupWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	CPoint pt;
	::GetCursorPos (&pt);
	DrawCloseupButton (pt);
}

BOOL CRollupWnd::OnNcActivate(BOOL bActive) 
{
	return CDialog::OnNcActivate(bActive);
	DrawCloseupButton ();	
}

bool CRollupWnd::DrawCloseupButton(CPoint pt)
{
	CWindowDC dc (this);

	CRect r;
	GetWindowRect (&r);
	ScreenToClient (&r);
	//CRect r = CalculateCloseupButtonRect ();

	CPoint ptOffset (-::GetSystemMetrics (SM_CXDLGFRAME), ::GetSystemMetrics (SM_CYDLGFRAME) + ::GetSystemMetrics (SM_CYSMCAPTION));
	r.OffsetRect (ptOffset);

	r.right -= ::GetSystemMetrics (SM_CXSMSIZE) - ::GetSystemMetrics (SM_CXEDGE);
	r.left = r.right - ::GetSystemMetrics (SM_CXSMSIZE) + ::GetSystemMetrics (SM_CXEDGE);
	r.top += ::GetSystemMetrics (SM_CYDLGFRAME);
	int n = (::GetSystemMetrics (SM_CYSMCAPTION) - ::GetSystemMetrics (SM_CYSMSIZE) + 2 * ::GetSystemMetrics (SM_CYEDGE)) / 2;
	TRACE ("SM_CYSMCAPTION: %d, CM_CYSMSIZE: %d\n", ::GetSystemMetrics (SM_CYSMCAPTION), ::GetSystemMetrics (SM_CYSMSIZE));
	r.top += n;
	r.bottom = r.top + ::GetSystemMetrics (SM_CYSMSIZE) - 2 * ::GetSystemMetrics (SM_CYEDGE);

 	CPoint point = pt;
	ScreenToClient (&point);
	point.x += ::GetSystemMetrics (SM_CXDLGFRAME);
	point.y += ptOffset.y;

	if (r.PtInRect (point))
		SetCapture ();
	else
		ReleaseCapture ();

	dc.DrawFrameControl (&r, DFC_BUTTON,
		DFCS_ADJUSTRECT | DFCS_BUTTONPUSH | (r.PtInRect (point) ? DFCS_PUSHED : 0));

	CFont* pOldFont = (CFont*) dc.SelectObject (&m_fntCloseUpBtn);
	dc.SetBkMode (TRANSPARENT);
	dc.DrawText ("5", &r, DT_CENTER | DT_VCENTER);
	dc.SelectObject (pOldFont);

	return r.PtInRect (point) ? true : false;
}

void CRollupWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	CDialog::OnNcLButtonDblClk(nHitTest, point);
	if (DrawCloseupButton (point))
		Collapse ();
}

void CRollupWnd::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	DrawCloseupButton ();
	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CRollupWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	DrawCloseupButton ();
	CDialog::OnLButtonUp(nFlags, point);
}

// calculate the rectangular area of the caption at the offset
CRect CRollupWnd::CalculateCloseupButtonRect ()
{
	CRect rectWindow;
	CSize szFrameSize = GetFrameSize ();
	CSize szCaptionSize = GetCaptionSize ();
	GetWindowRect (&rectWindow);

	CRect rectCaption;
	rectCaption.left = rectWindow.right - (szCaptionSize.cx * 2 + szFrameSize.cx);
	rectCaption.top = rectWindow.top + szFrameSize.cy;
	rectCaption.right = rectCaption.left + szCaptionSize.cx;
	rectCaption.bottom = rectCaption.top + szCaptionSize.cy;

	// check for partially covered up menu, and compensate
	CRect rectMenu;
	CMenu* pmnuWindow = GetMenu ();
	if (pmnuWindow != NULL)
	{
		GetMenuItemRect (GetSafeHwnd (), pmnuWindow->GetSafeHmenu (), 0, &rectMenu);
		if(rectMenu.bottom != 0)	// if it is not a child window
		{
			rectCaption.bottom = rectMenu.top - 1;
			rectCaption.top = rectCaption.bottom - szCaptionSize.cy;
		}
	}

	bool bToClient = true;
	if (bToClient)
	{
		CPoint ptTopLeft = rectCaption.TopLeft ();
		CPoint ptBottomRight = rectCaption.BottomRight ();

		ScreenToClient (&ptTopLeft);
		ScreenToClient (&ptBottomRight);
		rectCaption.left = ptTopLeft.x;
		rectCaption.top = ptTopLeft.y;
		rectCaption.right = ptBottomRight.x;
		rectCaption.bottom = ptBottomRight.y;
	}

	return rectCaption;
}

// gets the width of the window's frame
CSize CRollupWnd::GetFrameSize () const
{
	CSize szFrameSize;
	if (GetWindowLong (GetSafeHwnd (), GWL_STYLE) & WS_THICKFRAME)
	{
		// resizing frame
		szFrameSize.cx = GetSystemMetrics (SM_CXSIZEFRAME);
		szFrameSize.cy = GetSystemMetrics (SM_CYSIZEFRAME);
	}
	else
	{
		// non resizing frame
		szFrameSize.cx = GetSystemMetrics (SM_CXFIXEDFRAME);
		szFrameSize.cy = GetSystemMetrics (SM_CYFIXEDFRAME);
	}

	return szFrameSize;
}

// get the size of a caption for the current window
CSize CRollupWnd::GetCaptionSize () const
{
	CSize szCaptionSize;
	if(GetWindowLong (GetSafeHwnd (), GWL_STYLE) & WS_EX_TOOLWINDOW)
	{
		// toolwindow
		szCaptionSize.cx = GetSystemMetrics (SM_CXSMSIZE);
		szCaptionSize.cy = GetSystemMetrics (SM_CYSMSIZE);
	}
	else
	{
		// appwindow
		szCaptionSize.cx = GetSystemMetrics (SM_CXSIZE);
		szCaptionSize.cy = GetSystemMetrics (SM_CYSIZE);
	}

	return szCaptionSize;
}