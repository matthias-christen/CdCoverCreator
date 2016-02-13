// LeftPanelDlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "LeftPanelDlgBar.h"

#include "MainFrm.h"
#include "CdCoverCreator2Doc.h"
#include "CdCoverCreator2View.h"

#include "GradientRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftPanelDlgBar dialog


CLeftPanelDlgBar::CLeftPanelDlgBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CLeftPanelDlgBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLastDoc = NULL;
	m_bRedraw = true;

#ifdef PEROUTKA_FEDORISIN
	m_bmpLogo.LoadBitmap (IDB_BITMAP_AITEC);
#endif
}

CLeftPanelDlgBar::~CLeftPanelDlgBar ()
{
#ifdef PEROUTKA_FEDORISIN
	m_bmpLogo.DeleteObject ();
#endif

	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		m_Bitmap[i].DeleteObject ();
}

void CLeftPanelDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftPanelDlgBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

CSize CLeftPanelDlgBar::CalcFixedLayout (BOOL bStretch, BOOL bHorz)
{
	// adjust the left panel
	CRect rectMDIClient;
	
	CWnd* pMainWnd = AfxGetApp ()->m_pMainWnd;
	if (pMainWnd != NULL)
		::GetClientRect (((CMainFrame*) pMainWnd)->m_hWndMDIClient, &rectMDIClient);
	else
		rectMDIClient.top = 0, rectMDIClient.bottom = 700;

	return CSize (148, rectMDIClient.Height ());
}

void CLeftPanelDlgBar::Update (bool bRepaintBackground)
{
	m_bRedraw = true;
	InvalidateRect (NULL, bRepaintBackground ? TRUE : FALSE);
}

BEGIN_MESSAGE_MAP(CLeftPanelDlgBar, CDialogBar)
	//{{AFX_MSG_MAP(CLeftPanelDlgBar)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftPanelDlgBar message handlers

void CLeftPanelDlgBar::OnPaint() 
{
	CPaintDC dc (this); // device context for painting

	CDC dcMem;
	dcMem.CreateCompatibleDC (&dc);
	CBrush brush (::GetSysColor (COLOR_3DFACE));

	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();

	CRect r;
	GetClientRect (&r);

	if (pChild == NULL)
	{
		dc.FillRect (&r, &brush);
		goto __end;
	}

	CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
	if (pDoc == NULL)
	{
		// look whether the last doc is still valid
		POSITION pos = AfxGetApp ()->GetFirstDocTemplatePosition ();
		CDocTemplate* pDocTmpl = AfxGetApp ()->GetNextDocTemplate (pos);
		bool bIsValid = false;
		for (pos = pDocTmpl->GetFirstDocPosition (); pos; )
			if (pDocTmpl->GetNextDoc (pos) == m_pLastDoc)
			{
				bIsValid = true;
				break;
			}

		if (bIsValid)
			pDoc = m_pLastDoc;
		else
			m_pLastDoc = NULL;
	}
	else
		m_pLastDoc = pDoc;

	if (pDoc == NULL)
	{
		dc.FillRect (&r, &brush);
		goto __end;
	}

	CRect rect;
	GetClientRect (rect);
	rect.DeflateRect (5, 5, 5, 5);

	if (pDoc->m_pStyle != NULL)
	{
		CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pChild->GetActiveView ();
		if (pView == NULL)
			goto __end;

		int nHeight = (rect.Width () / 5) * 4;
		CRenderDC rdc (&dc, nHeight, pView->GetDocument ()->m_pStyle);

		m_y[Cover] = 5;
		m_y[Back] = m_y[Cover] + rdc.GetHeight (Cover) + 10;
		m_y[Inlay] = m_y[Back] + rdc.GetHeight (Back) + 10;
		m_y[Label] = m_y[Inlay] + rdc.GetHeight (Inlay) + 10;
		m_y[Booklet] = m_y[Label] + rdc.GetHeight (Label) + 10;

		if (m_bRedraw)
		{
			CLabelRenderer renderer (pDoc->m_pTracks, pDoc->m_pDataCDInfo,
				reinterpret_cast<CBackgroundInfo**> (&(pDoc->m_pBackground)),
				pDoc->m_listFloatingObjects, *(pDoc->m_pStyle));

			for (int i = 0; i < NUM_RENDER_TYPES - 1; i++)
			{
				if ((RenderType) i == Booklet)
					continue;

				CRect rectBmp (0, 0, rdc.GetWidth ((RenderType) i) + 5, /*nHeight*/(m_y[i + 1] - m_y[i]) + 5);

				m_Bitmap[i].DeleteObject ();
				m_Bitmap[i].CreateCompatibleBitmap (&dc, rectBmp.right, rectBmp.bottom);

				// render
				dcMem.SelectObject (&m_Bitmap[i]);
				dcMem.FillRect (rectBmp, &brush);

				renderer.Render (CRenderDC (&dcMem, nHeight, pDoc->m_pStyle), (RenderType) i, true);
			}

			m_bRedraw = false;
		}

		// draw the bitmaps
		int x[] = {
			(rect.Width () - rdc.GetWidth (Cover)) / 2 + 5,
			(rect.Width () - rdc.GetWidth (Back)) / 2 + 5,
			(rect.Width () - rdc.GetWidth (Inlay)) / 2 + 5,
			(rect.Width () - rdc.GetWidth (Label)) / 2 + 5,
			(rect.Width () - rdc.GetWidth (Cover)) / 2 + 5
		};

		dc.SetViewportOrg (x[Booklet], m_y[Booklet]);
		DrawBooklet (&dc, rdc.GetWidth (Cover), rdc.GetWidth (Cover) / 2, pDoc, pView->GetRenderType ());
		dc.SetViewportOrg (0, 0);

		CBrush* pOldBrush = dc.GetCurrentBrush ();

		for (int i = 0; i < NUM_RENDER_TYPES; i++)
		{
			CRect rectArea (rect.left, m_y[i], rect.right,
				m_y[i] + ((RenderType) i == Booklet ? rdc.GetWidth (Cover) / 2 : rdc.GetHeight ((RenderType) i)));

			if ((RenderType) i != Booklet)
			{
				dcMem.SelectObject (m_Bitmap[i]);

				if (pDoc->m_pStyle->m_bHasRenderType[i])
					dc.SelectStockObject (WHITE_BRUSH);
				else
				{
					dc.SetTextColor (RGB (0xff, 0xff, 0xff));
					dc.SetBkColor (RGB (0x7f, 0x7f, 0x7f));
					dc.SelectObject (dc.GetHalftoneBrush ());
				}

				//dc.BitBlt (x[i], m_y[i], rdc.GetWidth ((RenderType) i) + 5, /*nHeight*/(m_y[i + 1] - m_y[i]) + 5, &dcMem, 0, 0, MERGECOPY);
				dc.BitBlt (x[i], m_y[i], rectArea.Width (), rectArea.Height () + 4, &dcMem, 0, 0, MERGECOPY);
			}

			// marker
			Mark (&dc, rectArea, pView->GetRenderType () != (RenderType) i);
		}

		dc.SelectObject (pOldBrush);
	}

__end:

#ifdef PEROUTKA_FEDORISIN
	CBitmap* pOldBmp = (CBitmap*) dcMem.SelectObject (m_bmpLogo);
	dc.FillSolidRect (0, r.bottom - 32, 148, 32, RGB (0xff, 0xff, 0xff));
	dc.BitBlt ((148 - 80) / 2, r.bottom - 32, 80, 32, &dcMem, 0, 0, SRCCOPY);
//	dcMem.SelectObject (pOldBmp);
#endif

	dcMem.DeleteDC ();
	brush.DeleteObject ();
}

BOOL CLeftPanelDlgBar::OnEraseBkgnd(CDC* pDC) 
{
	// Use the same background color as that of the dialog
	Mark (pDC, m_rcCurMark, true);

	if (GetDockingFrame () == NULL)
		return TRUE;
	return CDialogBar::OnEraseBkgnd(pDC);
}

void CLeftPanelDlgBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
	if (pChild == NULL)
		return;

	CCdCoverCreator2View* pView = NULL;
	CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();

	if (pDoc == NULL)
	{
		if (IsLastDocValid ())
		{
			// if the last doc is valid, get its view and activate it
			for (POSITION pos = m_pLastDoc->GetFirstViewPosition (); pos != NULL; )
			{
				CView* pV = m_pLastDoc->GetNextView (pos);
				if (pV != NULL)
					if (pV->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
					{
						pView = (CCdCoverCreator2View*) pV;
						break;
					}
			}

			if (pView != NULL)
				((CMDIChildWnd*) pView->GetParent ())->MDIActivate ();
		}
	}
	else
	{
		CView* pV = pChild->GetActiveView ();
		if (pV != NULL)
			if (pV->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
				pView = (CCdCoverCreator2View*) pV;
		m_pLastDoc = pDoc;
	}

	if (pView == NULL)
		return;

	CRect rect;
	GetClientRect (rect);
	rect.DeflateRect (5, 5, 5, 5);

	CWindowDC dc (this);
	CRenderDC rdc (&dc, rect.Width (), pView->GetDocument ()->m_pStyle);

	bool bShow = false;

	if (CRect (rect.left, m_y[Cover], rect.right, m_y[Cover] + rdc.GetHeight (Cover)).PtInRect (point))
		pView->OnShowcover (), bShow = true;
	else if (CRect (rect.left, m_y[Back], rect.right, m_y[Back] + rdc.GetHeight (Back)).PtInRect (point))
		pView->OnShowback (), bShow = true;
	else if (CRect (rect.left, m_y[Inlay], rect.right, m_y[Inlay] + rdc.GetHeight (Inlay)).PtInRect (point))
		pView->OnShowinlay (), bShow = true;
	else if (CRect (rect.left, m_y[Label], rect.right, m_y[Label] + rdc.GetHeight (Label)).PtInRect (point))
		pView->OnShowlabel (), bShow = true;
	else if (CRect (rect.left, m_y[Booklet], rect.right, m_y[Booklet] + rdc.GetWidth (Cover) / 2).PtInRect (point))
		pView->OnShowbooklet (), bShow = true;

	if (bShow)
		InvalidateRect (NULL, false);
	else	
		CDialogBar::OnLButtonDown(nFlags, point);
}

void CLeftPanelDlgBar::Mark(CDC *pDC, CRect r, bool bErase)
{
	m_rcCurMark = r;

	CPen pen (PS_SOLID, 1, bErase ? GetSysColor (COLOR_3DFACE) : RGB (0x6B, 0x8B, 0xBD));
	CPen* pOldPen = pDC->SelectObject (&pen);
	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject (HOLLOW_BRUSH);

	// adjust marker rectangle
	r.left--;
	r.top--;
	r.right--;
	r.bottom += 3;

	// draw
	pDC->Rectangle (&r);

	// draw shade
	CPen penShadow (PS_GEOMETRIC | PS_ENDCAP_SQUARE, 2, bErase ? GetSysColor (COLOR_3DFACE) : RGB (0x51, 0x70, 0xA7));
	pDC->SelectObject (&penShadow);
	pDC->MoveTo (r.right + 1, r.top + 1);
	POINT pt[2] = {r.right + 1, r.bottom, r.left + 1, r.bottom};
	pDC->PolylineTo (pt, 2);

	// clean up
	pDC->SelectObject (pOldPen);
	pen.DeleteObject ();
	penShadow.DeleteObject ();

	pDC->SelectObject (pOldBrush);
}

bool CLeftPanelDlgBar::IsLastDocValid()
{
	// look whether the last doc is still valid
	POSITION pos = AfxGetApp ()->GetFirstDocTemplatePosition ();
	CDocTemplate* pDocTmpl = AfxGetApp ()->GetNextDocTemplate (pos);
	for (pos = pDocTmpl->GetFirstDocPosition (); pos; )
		if (pDocTmpl->GetNextDoc (pos) == m_pLastDoc)
			return true;

	return false;
}

void CLeftPanelDlgBar::DrawBooklet(CDC *pDC, int w, int h, CCdCoverCreator2Doc* pDoc, RenderType rt)
{
	CRect r (0, 0, w, h);
	CGradientRenderer::DrawGradient (pDC, NULL, r, -450, RGB (0xF0, 0xF0, 0xFF), RGB (0xA0, 0xA0, 0xAA), NULL);

	// border
	CBrush* pOldBrush = (CBrush*) pDC->SelectStockObject (NULL_BRUSH);
	CPen pen (PS_SOLID, 1, pDoc->m_pStyle->GetGradientData ((RenderType) min (NUM_RENDER_TYPES, max (rt, 0)))->colors[0]);
	CPen* pOldPen = (CPen*) pDC->SelectObject (&pen);

	pDC->Rectangle (&r);
	pDC->MoveTo ((r.left + r.right) / 2, r.top);
	pDC->LineTo ((r.left + r.right) / 2, r.bottom);

	// lines
	CPen penLines (PS_SOLID, 1, RGB (0x75, 0x75, 0x75));
	pOldPen = (CPen*) pDC->SelectObject (&penLines);

	for (int i = r.top + 4; i < r.bottom - 4; i += 3)
	{
		pDC->MoveTo (r.left + 3, i);
		pDC->LineTo (r.left + (i * 59) % ((r.left + r.right) / 2 - 3), i);

		pDC->MoveTo ((r.left + r.right) / 2 + 3, i);
		pDC->LineTo ((r.left + r.right) / 2 + (i * 51) % ((r.left + r.right) / 2 - 3), i);
	}

	pDC->SelectObject (pOldPen);

	// shade
	CPen penShade (PS_SOLID, 2, RGB (0x60, 0x60, 0x60));
	pOldPen = (CPen*) pDC->SelectObject (&penShade);

	pDC->MoveTo (r.right + 1, r.top + 2);
	pDC->LineTo (r.right + 1, r.bottom + 1);
	pDC->LineTo (r.left + 2, r.bottom + 1);

	pDC->SelectObject (pOldPen);
	pDC->SelectObject (pOldBrush);
}
