// MouseOverButton.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "MouseOverButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMouseOverButton

CMouseOverButton::CMouseOverButton()
{
	m_bTrackLeave = false;
	m_bButtonDown = false;

	m_hIcon = NULL;
}

CMouseOverButton::~CMouseOverButton()
{
}


BEGIN_MESSAGE_MAP(CMouseOverButton, CButton)
	//{{AFX_MSG_MAP(CMouseOverButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMouseOverButton message handlers

void CMouseOverButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTrackLeave)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (TRACKMOUSEEVENT);
		tme.hwndTrack = GetSafeHwnd ();
		tme.dwFlags = TME_LEAVE;

		_TrackMouseEvent (&tme);

		m_bTrackLeave = true;
		InvalidateRect (NULL);
	}
	
	CButton::OnMouseMove(nFlags, point);
}

LRESULT CMouseOverButton::OnMouseLeave (WPARAM wParam, LPARAM lParam)
{
	m_bTrackLeave = false;
	InvalidateRect (NULL);

	return S_OK;
}

void CMouseOverButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bButtonDown = true;
	InvalidateRect (NULL);

	CButton::OnLButtonDown(nFlags, point);
}

void CMouseOverButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bButtonDown = false;
	InvalidateRect (NULL);

	CButton::OnLButtonUp(nFlags, point);
}

void CMouseOverButton::SetIcon(HICON hIcon)
{
	m_hIcon = hIcon;
}

BOOL CMouseOverButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	return CButton::PreCreateWindow(cs);
}

void CMouseOverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle (lpDrawItemStruct->hDC);

	// paint icon centered on the control
	RECT* pRc = &lpDrawItemStruct->rcItem;

	pDC->FillSolidRect (pRc, ::GetSysColor (COLOR_3DFACE));

	if (m_hIcon == NULL)
		m_hIcon = (HICON) SendMessage (BM_GETIMAGE, IMAGE_ICON);

	ICONINFO iconinfo;
	::GetIconInfo (m_hIcon, &iconinfo);
	// (iconinfo.xHotspot, iconinfo.yHotspot) is the middle of the image for icons)

	::DrawIconEx (pDC->GetSafeHdc (),
		(pRc->left + pRc->right - (signed) iconinfo.xHotspot * 2) / 2,
		(pRc->bottom + pRc->top - (signed) iconinfo.yHotspot * 2) / 2,
		m_hIcon, iconinfo.xHotspot * 2, iconinfo.yHotspot * 2, 0, NULL, DI_NORMAL);
//	pDC->DrawIcon ((pRc->left + pRc->right - (signed) iconinfo.xHotspot * 2) / 2,
//		(pRc->bottom + pRc->top - (signed) iconinfo.yHotspot * 2) / 2, m_hIcon);

	// draw border if mouse is over control
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		pDC->Draw3dRect (pRc, ::GetSysColor (COLOR_3DSHADOW), ::GetSysColor (COLOR_3DHILIGHT));
	else if (m_bTrackLeave)
		pDC->Draw3dRect (pRc, ::GetSysColor (COLOR_3DHILIGHT), ::GetSysColor (COLOR_3DSHADOW));
}

void CMouseOverButton::PreSubclassWindow() 
{
	LONG lStyle = GetWindowLong (GetSafeHwnd (), GWL_STYLE);
	SetWindowLong (GetSafeHwnd (), GWL_STYLE, lStyle | BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}
