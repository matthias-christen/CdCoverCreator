// STWPreviewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "STWPreviewWnd.h"

#include "LabelRenderer.h"
#include "RenderDC.h"
#include "StyleTemplateWizard.h"
#include "StyleTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTWPreviewWnd

CSTWPreviewWnd::CSTWPreviewWnd()
{
	m_tracks.SetTitle ("César Franck");
//	m_tracks.SetSubtitle ("H. von Karajan · R. Muti");
	m_tracks.Add (new CTrack (-1, "", "Symphony in D minor", ""));
	m_tracks.Add (new CTrack (1, "20:32", "I: Lento - Allegro non troppo", ""));
	m_tracks.Add (new CTrack (2, "10:24", "II: Allegretto - Poco più lento - Tempo I", ""));
	m_tracks.Add (new CTrack (3, "11:11", "III: Allegro non troppo", ""));
	m_tracks.Add (new CTrack (-1, "", "", ""));
	m_tracks.Add (new CTrack (4, "15:46", "Symphonic Variations", "Piano: A. Weissenberg"));
	m_tracks.Add (new CTrack (-1, "", "", ""));
	m_tracks.Add (new CTrack (5, "15:21", "Le Chasseur maudit", "Ricardo Muti"));

	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		m_pBkgrnd[i] = new CBackgroundInfo ((RenderType) i);
}

CSTWPreviewWnd::~CSTWPreviewWnd()
{
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		delete m_pBkgrnd[i];
}

BEGIN_MESSAGE_MAP(CSTWPreviewWnd, CWnd)
	//{{AFX_MSG_MAP(CSTWPreviewWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSTWPreviewWnd message handlers

void CSTWPreviewWnd::OnPaint() 
{
	CPaintDC dc (this); // device context for painting
	CRect rect;
	GetClientRect (rect);
	rect.DeflateRect (5, 5, 5, 5);

	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	if (pTmp != NULL)
	{
		CRenderDC rdc (&dc, (rect.Width () / 5) * 4, pTmp);

		CLabelRenderer renderer (&m_tracks, NULL,
			reinterpret_cast<CBackgroundInfo**> (&m_pBkgrnd), NULL, *pTmp);

		m_yCover = 5;
		m_yLabel = m_yCover + rdc.GetHeight (Cover) + 10;
		m_yInlay = m_yLabel + rdc.GetHeight (Label) + 10;

		dc.SetViewportOrg ((rect.Width () - rdc.GetWidth (Cover)) / 2 + 5, m_yCover);
		renderer.Render (rdc, Cover);

		dc.SetViewportOrg ((rect.Width () - rdc.GetWidth (Label)) / 2 + 5, m_yLabel);
		renderer.Render (rdc, Label);

		dc.SetViewportOrg (5, m_yInlay);
		renderer.Render (rdc, Inlay);

		dc.SetViewportOrg (0, 0);

		// marker
		switch (((CStyleTemplateWizard*) GetParent ())->GetActiveIndex ())
		{
		case 0:	// select template
		case 1:	// select title fonts
		case 2:	// select track fonts
		case 3:	// select background colors
			rect.InflateRect (2, 2);
			Mark (&dc, rect);
			break;
		case 4:	// cover layout
			Mark (&dc, CRect (rect.left, m_yCover, rect.right, m_yCover + rdc.GetHeight (Cover)));
			break;
		case 5:	// label layout
			Mark (&dc, CRect (rect.left, m_yLabel, rect.right, m_yLabel + rdc.GetHeight (Label)));
			break;
		case 6:	// inlay layout
			Mark (&dc, CRect (rect.left, m_yInlay, rect.right, m_yInlay + rdc.GetHeight (Inlay)));
			break;
		case 7:	// track layout
			Mark (&dc, CRect (rect.left + 10, m_yInlay + 20, rect.right - 3, m_yInlay + rdc.GetHeight (Inlay) - 5));
			break;
		}
	}
}

BOOL CSTWPreviewWnd::OnEraseBkgnd(CDC* pDC) 
{
	// Use the same background color as that of the dialog
	//  (property sheet).

	Mark (pDC, m_rcCurMark, true);
	return TRUE;
}

void CSTWPreviewWnd::Mark(CDC *pDC, CRect r, bool bErase)
{
	m_rcCurMark = r;

	CPen pen (PS_SOLID, 1, bErase ? GetSysColor (COLOR_3DFACE) : RGB (0x6B, 0x8B, 0xBD));
	CPen* pOldPen = pDC->SelectObject (&pen);
	pDC->SelectStockObject (HOLLOW_BRUSH);
	pDC->Rectangle (&r);

	CPen penShadow (PS_GEOMETRIC | PS_ENDCAP_SQUARE, 2, bErase ? GetSysColor (COLOR_3DFACE) : RGB (0x51, 0x70, 0xA7));
	pDC->SelectObject (&penShadow);
	pDC->MoveTo (r.right + 1, r.top + 1);
	POINT pt[2] = {r.right + 1, r.bottom, r.left + 1, r.bottom};
	pDC->PolylineTo (pt, 2);

	pDC->SelectObject (pOldPen);
	pen.DeleteObject ();
	penShadow.DeleteObject ();
}

void CSTWPreviewWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (point.y > m_yInlay)
		((CStyleTemplateWizard*) GetParent ())->SetActivePage (6);
	else if (point.y > m_yLabel)
		((CStyleTemplateWizard*) GetParent ())->SetActivePage (5);
	else
		((CStyleTemplateWizard*) GetParent ())->SetActivePage (4);

	CWnd::OnLButtonDown(nFlags, point);
}
