// CdCoverCreator2View.cpp : implementation of the CCdCoverCreator2View class
//

#include "stdafx.h"
#include "CdCoverCreator2.h"

#include "CdCoverCreator2Doc.h"
#include "CdCoverCreator2View.h"

#include "MainFrm.h"
#include "CdCoverCreator2Doc.h"
#include "CdCoverCreator2View.h"

#include "LabelRenderer.h"
#include "GradientRenderer.h"
#include "RenderDC.h"
#include "resource.h"

#include "SelectLabelPrintDialog.h"
#include "SelectLabelsDlg.h"
#include "DlgImageProperties.h"
#include "DlgSetImageEditor.h"
#include "DlgBarcodeProperties.h"

#include "FloatingImage.h"
#include "FloatingText.h"
#include "FloatingBarcode.h"

#include <list>
#include <algorithm>

#include <math.h>
#include <winspool.h>
#include ".\cdcovercreator2view.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCdCoverCreator2View

IMPLEMENT_DYNCREATE(CCdCoverCreator2View, CScrollView)

BEGIN_MESSAGE_MAP(CCdCoverCreator2View, CScrollView)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()

	ON_COMMAND(ID_ZOOMIN, OnZoomin)
	ON_COMMAND(ID_ZOOMOUT, OnZoomout)
	ON_COMMAND(ID_SHOWINLAY, OnShowinlay)
	ON_COMMAND(ID_SHOWCOVER, OnShowcover)
	ON_COMMAND(ID_SHOWLABEL, OnShowlabel)
	ON_COMMAND(ID_SHOWBACK, OnShowback)
	ON_COMMAND(ID_SHOWBOOKLET, OnShowbooklet)

	ON_COMMAND(ID_TOOLS_ADDTEXT, OnToolAddText)
	ON_COMMAND(ID_TOOLS_ADDIMAGE, OnToolAddImage)
	ON_COMMAND(ID_TOOLS_ADDBARCODE, OnToolAddBarcode)

	ON_COMMAND_RANGE(ID__RAISE, ID__SENDTOBACK, OnFloatingObjectMenuPosition)
	ON_COMMAND(ID__DELETE, OnFloatingObjectMenuDelete)
	ON_COMMAND(ID__SELECTTRANSPARENTCOLOR, OnFloatingImageMenuSelectTransparentColor)
	ON_COMMAND(ID__ADJUSTCOLORS, OnFloatingImageMenuAdjustColors)
	ON_COMMAND(ID__EDITIMAGE, OnFloatingImageEdit)
	ON_COMMAND(ID__RELOADIMAGE, OnFloatingImageReload)
	ON_COMMAND_RANGE(ID_ALIGN_TOPLEFT, ID_ALIGN_BOTTOMRIGHT, OnFloatingImageAlign)
	ON_COMMAND(ID__STRETCHTOFULLCOVERSIZE, OnFloatingImageStretchToFit)
	ON_COMMAND(ID__BARCODEPROPERTIES, OnFloatingBarcodeProperties)

	ON_UPDATE_COMMAND_UI(ID_ZOOMIN, OnUpdateZoomin)
	ON_UPDATE_COMMAND_UI(ID_ZOOMOUT, OnUpdateZoomout)
	ON_UPDATE_COMMAND_UI(ID_SHOWINLAY, OnUpdateShowinlay)
	ON_UPDATE_COMMAND_UI(ID_SHOWCOVER, OnUpdateShowcover)
	ON_UPDATE_COMMAND_UI(ID_SHOWLABEL, OnUpdateShowlabel)
	ON_UPDATE_COMMAND_UI(ID_SHOWBACK, OnUpdateShowback)
	ON_UPDATE_COMMAND_UI(ID_SHOWBOOKLET, OnUpdateShowbooklet)

	ON_UPDATE_COMMAND_UI(ID_TOOLS_ADDTEXT, OnUpdateToolAddText)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ADDIMAGE, OnUpdateToolAddImage)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ADDBARCODE, OnUpdateToolAddBarcode)

	ON_CONTROL(AEN_ESCAPE, IDC_VIEWRICHEDIT, OnRichEditEscape)
	ON_CONTROL(EN_ESCAPE, IDC_VIEWTRACKEDIT, OnTrackEditEscape)
	ON_CONTROL(EN_UP, IDC_VIEWTRACKEDIT, OnTrackEditUp)
	ON_CONTROL(EN_DOWN, IDC_VIEWTRACKEDIT, OnTrackEditDown)
	ON_CONTROL(EN_NEXT, IDC_VIEWTRACKEDIT, OnTrackEditNext)
	ON_CONTROL(EN_PREV, IDC_VIEWTRACKEDIT, OnTrackEditPrev)

	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_LIGHTSCRIBE, OnUpdateFileLightscribe)
	ON_COMMAND(ID_FILE_SAVE_AS_IMAGE, &CCdCoverCreator2View::OnFileSaveAsImage)
END_MESSAGE_MAP()

// CCdCoverCreator2View construction/destruction

CCdCoverCreator2View::CCdCoverCreator2View()
{
	m_nHeight = 512;
	m_RenderType = Inlay;

	// initialize trackers
	m_trackerTitle.m_rect.left = m_trackerTitle.m_rect.top = m_trackerTitle.m_rect.right = m_trackerTitle.m_rect.bottom = 0;
	m_trackerTracks.m_rect.left = m_trackerTracks.m_rect.top = m_trackerTracks.m_rect.right = m_trackerTracks.m_rect.bottom = 0;
	m_trackerImage.m_rect.left = m_trackerImage.m_rect.top = m_trackerImage.m_rect.right = m_trackerImage.m_rect.bottom = 0;

	m_trackerTitle.m_nStyle = 0;
	m_trackerTracks.m_nStyle = 0;
	m_trackerImage.m_nStyle = 0;

	m_pPrevTrck = NULL;

	m_bIsEditingTitle = false;
	m_nEditingTrackNum = -1;
	m_nEditingTrackIdx = -1;

	m_pCurrentAddingObj = NULL;
	m_pCurrentContextMenuObj = NULL;
	m_pEditingFGO = NULL;

	m_bSettingTransparentColor = false;
	m_hPickCursor = AfxGetApp ()->LoadCursor (IDC_CURSOR_PICKER);
}

CCdCoverCreator2View::~CCdCoverCreator2View()
{
	m_Bitmap.DeleteObject ();
	::DestroyCursor (m_hPickCursor);
}

BOOL CCdCoverCreator2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CCdCoverCreator2View drawing

using namespace Gdiplus;

void CCdCoverCreator2View::OnDraw(CDC* pDC)
{
	CCdCoverCreator2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (m_RenderType == Booklet)
	{
	}
	else
	{
		//pDC->SetViewportOrg (20 - GetScrollPosition ().x, 20 - GetScrollPosition ().y);
		pDC->SetViewportOrg (-GetScrollPosition ().x, -GetScrollPosition ().y);

		/*
		CLabelRenderer renderer (*(pDoc->m_pTracks), pDoc->m_pBackground, *(pDoc->m_pStyle));
		CRenderDC rdc (pDC, m_nWidth);
		renderer.Render (rdc, m_RenderType);
		*/
		CDC memDC;
		memDC.CreateCompatibleDC (pDC);
		CBitmap* pOld = memDC.SelectObject (&m_Bitmap);

		CRect r;
		GetClientRect (&r);
		CSize sz = GetTotalSize ();
		r.right = max (r.right, sz.cx);
		r.bottom = max (r.bottom, sz.cy);

		pDC->BitBlt (0, 0, r.right, r.bottom, &memDC, 0, 0, SRCCOPY);

		memDC.SelectObject (pOld);
		memDC.DeleteDC ();

		// draw trackers
		CRenderDC rdc (pDC, m_nHeight, pDoc->m_pStyle);
		DrawTrackers (rdc);
	}

	// activate cover browser, if "Web" document has been created
	if (((CMainFrame*) AfxGetMainWnd ())->m_pActivateCoverBrowser != NULL)
	{
		((CMDIChildWnd*) ((CMainFrame*) AfxGetMainWnd ())->m_pActivateCoverBrowser)->MDIActivate ();
		((CMainFrame*) AfxGetMainWnd ())->m_pActivateCoverBrowser = NULL;
	}
}

void CCdCoverCreator2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;

	// TODO: calculate the total size of this view
	sizeTotal.cx = 680;
	sizeTotal.cy = 560;
	SetScrollSizes (MM_TEXT, sizeTotal);
}


// CCdCoverCreator2View printing

BOOL CCdCoverCreator2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	CWinApp* pApp = AfxGetApp ();
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	if (pApp->GetProfileInt ("Settings", "CoverMarginLeft", -1000) == -1000 ||
		pApp->GetProfileInt ("Settings", "CoverMarginTop", -1000) == -1000 ||
		pApp->GetProfileInt ("Settings", "CoverbackMarginLeft", -1000) == -1000 ||
		pApp->GetProfileInt ("Settings", "CoverbackMarginTop", -1000) == -1000 ||
		pApp->GetProfileInt ("Settings", "LabelMarginLeft", -1000) == -1000 ||
		pApp->GetProfileInt ("Settings", "LabelMarginTop", -1000) == -1000 ||
		pApp->GetProfileInt ("Settings", "InlayMarginLeft", -1000) == -1000 ||
		pApp->GetProfileInt ("Settings", "InlayMarginTop", -1000) == -1000)
	{
		// no page setup yet.
		// invoke setup routine
		((CMainFrame*) AfxGetMainWnd ())->OnFilePageSetup ();
	}

	int nPaperOrientation;
	
	if (pDoc->m_pStyle->GetMediumType () == MediumJewelCase || m_RenderType == Label)
	{
		switch (m_RenderType)
		{
		case Cover:
			nPaperOrientation = pApp->GetProfileInt ("Settings", "CoverPO", 0);
			break;
		case Back:
			nPaperOrientation = pApp->GetProfileInt ("Settings", "CoverbackPO", 1);
			break;
		case Label:
			nPaperOrientation = pApp->GetProfileInt ("Settings", "LabelPO", 0);

#ifdef PEROUTKA_FEDORISIN
			if (pInfo->m_bPreview)
			{
				// preview
				// show the label selector dialog
				m_dlgSelectLabels.ShowWindow (SW_SHOW);
				m_dlgSelectLabels.UpdateWindow ();

				CView* pView;
				for (POSITION pos = pDoc->GetFirstViewPosition (); pos != NULL; )
				{
					pView = pDoc->GetNextView (pos);
					if (pView->IsKindOf (RUNTIME_CLASS (CPreviewView)))
					{
						m_dlgSelectLabels.SetPreview (pView);
						break;
					}
				}
			}
			else
			{
				// printing
				// show the specialized printing dialog
				delete pInfo->m_pPD;
				pInfo->m_pPD = new CSelectLabelPrintDialog (FALSE, &m_dlgSelectLabels.m_LabelSelector);

				pInfo->m_pPD->m_pd.nMinPage = 1;
				pInfo->m_pPD->m_pd.nMaxPage = 1;
			}
#endif
			break;

		case Inlay:
			nPaperOrientation = pApp->GetProfileInt ("Settings", "InlayPO", 0);
			break;
		case Booklet:
			nPaperOrientation = 1;	// landscape
			break;
		}
	}
	else
		nPaperOrientation = 1;	// landscape orientation for slim cases, paper sleeves, and DVD boxes

	((CCdCoverCreator2App*) pApp)->SetPaperOrientation (nPaperOrientation);

	if (m_RenderType != Booklet)
		pInfo->SetMaxPage (1);

	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCdCoverCreator2View::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Get the printer handle
	::OpenPrinter (pInfo->m_pPD->GetDeviceName ().GetBuffer (-1), &m_hndPrinter, NULL);

	// Ensure the printer DC is in MM_TEXT mode
	pDC->SetMapMode (MM_TEXT);

	if (m_RenderType == Booklet)
		DoPreparePrintBooklet (pDC, pInfo);
}

void CCdCoverCreator2View::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView::OnPrepareDC(pDC, pInfo);

	if ((pInfo != NULL) && (m_RenderType == Booklet))
		pInfo->m_bContinuePrinting = pInfo->m_nCurPage <= (int) ceil ((m_vecBookletPages.size () + 2 - 1) / 4.0) * 2;
}

void CCdCoverCreator2View::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	CCdCoverCreator2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CSize szUnit (1000, 1000);	// 1 cm x 1 cm
	pDC->HIMETRICtoDP (&szUnit);

	int nLeft = pDC->GetDeviceCaps (PHYSICALOFFSETX);
	int nTop = pDC->GetDeviceCaps (PHYSICALOFFSETY);

	if (pDoc->m_pStyle->GetMediumType () == MediumJewelCase || m_RenderType == Label)
	{
#ifdef PEROUTKA_FEDORISIN
		if (m_RenderType != Label)
			DoPrint (pDC, pInfo, m_RenderType, szUnit, nLeft, nTop);
#else
		DoPrint (pDC, pInfo, m_RenderType, szUnit, nLeft, nTop);
#endif

		switch (m_RenderType)
		{
		case Cover:
			if ((RenderType) AfxGetApp ()->GetProfileInt ("Settings", "PrintCoverbackWith", -1) == Cover)
				DoPrint (pDC, pInfo, Back, szUnit, nLeft, nTop);
			if ((RenderType) AfxGetApp ()->GetProfileInt ("Settings", "PrintInlayWith", -1) == Cover)
				DoPrint (pDC, pInfo, Inlay, szUnit, nLeft, nTop);
			break;
		case Back:
			DoPrint (pDC, pInfo,
				(RenderType) AfxGetApp ()->GetProfileInt ("Settings", "PrintCoverbackWith", -1),
				szUnit, nLeft, nTop);
			break;
		case Inlay:
			DoPrint (pDC, pInfo,
				(RenderType) AfxGetApp ()->GetProfileInt ("Settings", "PrintInlayWith", -1),
				szUnit, nLeft, nTop);
			break;
		case Label:
#ifdef PEROUTKA_FEDORISIN
			{
				for (int i = 0; i < 3; i++)
					if (m_dlgSelectLabels.PrintLabel (i))
						DoPrint (pDC, pInfo, Label, szUnit, nLeft, nTop, i);
			}
#else
			if (AfxGetApp ()->GetProfileInt ("Settings", "2Labels", 0))
				DoPrint (pDC, pInfo, Label, szUnit, nLeft, nTop, 0, true);
#endif
			break;
		}
	}
	else if (pDoc->m_pStyle->GetMediumType () == MediumSlimCase)
	{
		DoPrintSlimCase (pDC, pInfo, Cover, szUnit, nLeft, nTop);
		DoPrintSlimCase (pDC, pInfo, Back, szUnit, nLeft, nTop);
		DoPrintSlimCase (pDC, pInfo, Inlay, szUnit, nLeft, nTop);
	}
	else if (pDoc->m_pStyle->GetMediumType () == MediumPaperSleeve)
	{
		DoPrintPaperSleeve (pDC, pInfo, Cover, szUnit, nLeft, nTop);
		DoPrintPaperSleeve (pDC, pInfo, Back, szUnit, nLeft, nTop);
	}
	else if (pDoc->m_pStyle->GetMediumType () == MediumDVDBox)
	{
		DoPrintDVDBox (pDC, pInfo, Cover, szUnit, nLeft, nTop);
		DoPrintDVDBox (pDC, pInfo, Inlay, szUnit, nLeft, nTop);
	}

#ifdef TESTING_VERSION
	CFont f;
	f.CreateFont (-200, 0, -450, 0, 700, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	CFont* pOldFont = (CFont*) pDC->SelectObject (&f);
	pDC->SetViewportOrg (0, 0);
	char szText[] = "UFTUJOH!WFSTJPO!PG!DeDpwfsDsfbups!3/3";
	for (int i = 0; i < strlen (szText); i++)
		szText[i]--;
	pDC->TextOut (nLeft + szUnit.cx * 5, nTop + szUnit.cy * 7, szText);
	pDC->SelectObject (pOldFont);
	f.DeleteObject ();
#endif
}

void CCdCoverCreator2View::DoPrint(CDC *pDC, CPrintInfo *pInfo, RenderType rt, CSize szUnit, int nLeft, int nTop, int nParam, bool bRevertPaper)
{
	if ((int) rt == -1)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CWinApp* pApp = AfxGetApp ();

	double dMarginLeft, dMarginTop;
	CRect* pRect;
	int nOrientation;

	switch (rt)
	{
	case Cover:
		dMarginLeft = pApp->GetProfileInt ("Settings", "CoverMarginLeft", 0) / 1000.0;
		dMarginTop = pApp->GetProfileInt ("Settings", "CoverMarginTop", 0) / 1000.0;
		nOrientation = pApp->GetProfileInt ("Settings", "CoverPO", 0);
		pRect = pDoc->m_pStyle->GetTotalCoverRect ();
		break;
	case Back:
		dMarginLeft = pApp->GetProfileInt ("Settings", "CoverbackMarginLeft", 0) / 1000.0;
		dMarginTop = pApp->GetProfileInt ("Settings", "CoverbackMarginTop", 0) / 1000.0;
		nOrientation = pApp->GetProfileInt ("Settings", "BackPO", 1);
		pRect = pDoc->m_pStyle->GetTotalCoverRect ();
		break;
	case Label:
#ifdef PEROUTKA_FEDORISIN
		switch (nParam)
		{
		case 0:	// 1st label
			dMarginLeft = 0.2;	// = 62-120/2 mm (diameter is always 120 mm)
			dMarginTop = 0.3;	// = 297 - (234+120/2)
			break;
		case 1:	// 2nd label
			dMarginLeft = 8.8;	// = 148-120/2
			dMarginTop = 8.85;	// 297 - (148.5+120/2)
			break;
		case 2:	// 3rd label
			dMarginLeft = 0.2;	// as in case 0
			dMarginTop = 17.4;	// 297 - (63+120/2)
			break;
		}
		nOrientation = 0;
#else
		dMarginLeft = pApp->GetProfileInt ("Settings", "LabelMarginLeft", 0) / 1000.0;
		dMarginTop = pApp->GetProfileInt ("Settings", "LabelMarginTop", 0) / 1000.0;
		nOrientation = pApp->GetProfileInt ("Settings", "LabelPO", 0);
#endif
		pRect = pDoc->m_pStyle->GetTotalLabelRect ();
		break;
	case Inlay:
		dMarginLeft = pApp->GetProfileInt ("Settings", "InlayMarginLeft", 0) / 1000.0;
		dMarginTop = pApp->GetProfileInt ("Settings", "InlayMarginTop", 0) / 1000.0;
		nOrientation = pApp->GetProfileInt ("Settings", "InlayPO", 0);
		pRect = pDoc->m_pStyle->GetTotalInlayRect ();
		break;
	case Booklet:
		nOrientation = pApp->GetProfileInt ("Settings", "BookletPO", 1);
		DoPrintBooklet (pDC, pInfo, szUnit, nLeft, nTop,
			pApp->GetProfileInt ("Settings", "BookletMarginLeft", 0) / 1000.0,
			pApp->GetProfileInt ("Settings", "BookletMarginTop", 0) / 1000.0, nOrientation);
		return;
	}

	int nOrgX = (int) (dMarginLeft * szUnit.cx) - nLeft;

	int nOrgY = bRevertPaper ?
		(int) ((((CCdCoverCreator2App*) pApp)->GetPaperLength (nOrientation) * szUnit.cy) - (pRect->Height () / 1000.0 + dMarginTop) * szUnit.cy) + nTop:
		(int) (dMarginTop * szUnit.cy) - nTop;

	pDC->SetViewportOrg (nOrgX, nOrgY);
	pDC->SetViewportExt (pInfo->m_rectDraw.Width () - nOrgX, pInfo->m_rectDraw.Height () - nOrgY);

	CLabelRenderer renderer (pDoc->m_pTracks, pDoc->m_pDataCDInfo, pDoc->m_pBackground,
		pDoc->m_listFloatingObjects, *(pDoc->m_pStyle));

	CRenderDC rdc (pDC, m_hndPrinter, (int) ((pDoc->m_pStyle->GetTotalCoverRect ()->Height () * szUnit.cy) / 1000), pDoc->m_pStyle);
	renderer.Render (rdc, rt, false, true, true, pApp->GetProfileInt ("Settings", "CutLines", 0) != 0);
}

void CCdCoverCreator2View::DoPrintSlimCase (CDC* pDC, CPrintInfo* pInfo, RenderType rt, CSize szUnit, int nLeft, int nTop)
{
	if ((int) rt == -1)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CWinApp* pApp = AfxGetApp ();

	double dMarginLeft, dMarginTop;

	CRect rectCover (*pDoc->m_pStyle->GetTotalCoverRect ());
	CRect rectInlay (*pDoc->m_pStyle->GetTotalInlayRect ());

	int nOrientation;

	switch (rt)
	{
	case Cover:
		dMarginLeft = 0.2 + (double) rectInlay.Width () / 1000.0;
		dMarginTop = 2;
		break;
	case Back:
		dMarginLeft = 0.2 + (double) rectInlay.Width () / 1000.0 + (double) rectCover.Width () / 1000.0;
		dMarginTop = 2;
		break;
	case Inlay:
		dMarginLeft = 0.2;
		dMarginTop = 2.2;
	}

	int nOrgX = (int) (dMarginLeft * szUnit.cx) - nLeft;
	int nOrgY = (int) (dMarginTop * szUnit.cy) - nTop;

	pDC->SetViewportOrg (nOrgX, nOrgY);
	pDC->SetViewportExt (pInfo->m_rectDraw.Width () - nOrgX, pInfo->m_rectDraw.Height () - nOrgY);

	CLabelRenderer renderer (pDoc->m_pTracks, pDoc->m_pDataCDInfo, pDoc->m_pBackground,
		pDoc->m_listFloatingObjects, *(pDoc->m_pStyle));

	CRenderDC rdc (pDC, m_hndPrinter, (int) ((rectCover.Height () * szUnit.cy) / 1000), pDoc->m_pStyle);
	renderer.Render (rdc, rt, false, true);
}

void CCdCoverCreator2View::DoPrintPaperSleeve (CDC* pDC, CPrintInfo* pInfo, RenderType rt, CSize szUnit, int nLeft, int nTop)
{
	if ((int) rt == -1)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CWinApp* pApp = AfxGetApp ();

	double dMarginLeft, dMarginTop;
	CRect* pRect = pDoc->m_pStyle->GetTotalCoverRect ();
	int nOrientation;

	switch (rt)
	{
	case Cover:
		dMarginLeft = 0.7 + (double) pRect->Width () / 1000.0;
		dMarginTop = 3;
		break;
	case Back:
		dMarginLeft = 0.7;
		dMarginTop = 3;
		break;
	}

	int nOrgX = (int) (dMarginLeft * szUnit.cx) - nLeft;
	int nOrgY = (int) (dMarginTop * szUnit.cy) - nTop;

	pDC->SetViewportOrg (nOrgX, nOrgY);
	pDC->SetViewportExt (pInfo->m_rectDraw.Width () - nOrgX, pInfo->m_rectDraw.Height () - nOrgY);

	CLabelRenderer renderer (pDoc->m_pTracks, pDoc->m_pDataCDInfo, pDoc->m_pBackground,
		pDoc->m_listFloatingObjects, *(pDoc->m_pStyle));

	CRenderDC rdc (pDC, m_hndPrinter, (int) (pRect->Height () * szUnit.cy / 1000), pDoc->m_pStyle);
	renderer.Render (rdc, rt, false, true);
}

void CCdCoverCreator2View::DoPrintDVDBox (CDC* pDC, CPrintInfo* pInfo, RenderType rt, CSize szUnit, int nLeft, int nTop)
{
	if ((int) rt == -1)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CWinApp* pApp = AfxGetApp ();

	double dMarginLeft = pApp->GetProfileInt ("Settings", "CoverMarginLeft", 0) / 1000.0;
	double dMarginTop = pApp->GetProfileInt ("Settings", "CoverMarginTop", 0) / 1000.0;
	CRect* pRect = pDoc->m_pStyle->GetTotalRect (rt);
	int nOrientation;

	if (rt == Cover)
		dMarginLeft += 14.3;

	int nOrgX = (int) (dMarginLeft * szUnit.cx) - nLeft;
	int nOrgY = (int) (dMarginTop * szUnit.cy) - nTop;

	pDC->SetViewportOrg (nOrgX, nOrgY);
	pDC->SetViewportExt (pInfo->m_rectDraw.Width () - nOrgX, pInfo->m_rectDraw.Height () - nOrgY);

	CLabelRenderer renderer (pDoc->m_pTracks, pDoc->m_pDataCDInfo, pDoc->m_pBackground,
		pDoc->m_listFloatingObjects, *(pDoc->m_pStyle));

	CRenderDC rdc (pDC, m_hndPrinter, (int) (pRect->Height () * szUnit.cy / 1000), pDoc->m_pStyle);
	renderer.Render (rdc, rt, false, true);

	// draw cut lines
	if (pApp->GetProfileInt ("Settings", "CutLines", 0) != 0 && rt == Inlay)
	{
		CPen pen (PS_SOLID, 1, RGB (0x00, 0x00, 0x00));
		CPen penDotted (PS_DOT, 1, RGB (0x00, 0x00, 0x00));

		CPen* pOldPen = (CPen*) pDC->SelectObject (&pen);
		CGdiObject* pOldBrush = pDC->SelectStockObject (HOLLOW_BRUSH);

		pRect = pDoc->m_pStyle->GetTotalInlayRect (rdc);
		CRect rectInlay (pRect);
		pRect = pDoc->m_pStyle->GetTotalCoverRect (rdc);
		pDC->Rectangle (0, 0, rectInlay.Width () + pRect->Width (), rectInlay.Height ());

		pDC->SelectObject (&penDotted);
		pRect = pDoc->m_pStyle->GetInlayRightSideRect (rdc);
		pDC->MoveTo (pRect->left, pRect->top);
		pDC->LineTo (pRect->left, pRect->bottom);
		pDC->MoveTo (pRect->right, pRect->top);
		pDC->LineTo (pRect->right, pRect->bottom);

		// clean up
		pDC->SelectObject (pOldBrush);
		pDC->SelectObject (pOldPen);
		pen.DeleteObject ();
		penDotted.DeleteObject ();
	}
}

void CCdCoverCreator2View::DoPreparePrintBooklet(CDC *pDC, CPrintInfo *pInfo)
{
	CStyleTemplate* pStyle = GetDocument ()->m_pStyle;

	m_vecBookletPages.clear ();
	m_vecBookletPages.reserve (20);
	m_vecBookletPages.push_back (0);	// beginning

	FORMATRANGE fr;
	int nHorzRes = pDC->GetDeviceCaps (HORZRES);
	int nVertRes = pDC->GetDeviceCaps (VERTRES);
	int nLogPixelsX = pDC->GetDeviceCaps (LOGPIXELSX);
	int nLogPixelsY = pDC->GetDeviceCaps (LOGPIXELSY);

	// Rendering to the same DC we are measuring.
	ZeroMemory (&fr, sizeof (fr));
	fr.hdc = fr.hdcTarget = pDC->GetSafeHdc ();
	
	// Set up the page
	fr.rcPage.left = fr.rcPage.top = 0;
	fr.rcPage.right = MulDiv (nHorzRes, 1440, nLogPixelsX);
	fr.rcPage.bottom = MulDiv (nVertRes, 1440, nLogPixelsY);

	// find out the page division
	////////////////////////////////////////////////////////////////

	// Set up margins. Booklet is 12 cm x 12 cm
	CRect* pRect = pStyle->GetTotalBookletRect ();

	CRect rcPage (fr.rcPage.left, fr.rcPage.top,
		fr.rcPage.left + (pRect->Width () / 2 / 1000.0 - pStyle->GetBookletMarginLeft () - pStyle->GetBookletMarginRight ()) / 2.54 * 1440,
		fr.rcPage.top + (pRect->Height () / 1000.0 - pStyle->GetBookletMarginTop () - pStyle->GetBookletMarginBottom ()) / 2.54 * 1440);

	// Default the range of text to print as the entire document.
	fr.chrg.cpMin = 0;
	fr.chrg.cpMax = -1;

	for (long lTextPrinted; ; )
	{
		fr.rc.left = rcPage.left;
		fr.rc.top = rcPage.top;
		fr.rc.right = rcPage.right;
		fr.rc.bottom = rcPage.bottom;

		lTextPrinted = m_wndRichEdit.FormatRange (&fr, false);

		// If there is more text to print, adjust the range of characters
		// to start printing at the first character of the next page.
		fr.chrg.cpMin = lTextPrinted;
		fr.chrg.cpMax = -1;

		if (m_vecBookletPages.back () >= lTextPrinted)
			break;

		m_vecBookletPages.push_back (lTextPrinted);
	}

	// Tell the control to release cached information.
	m_wndRichEdit.FormatRange (NULL, false);

	if (m_vecBookletPages.size () == 1)
		pInfo->SetMaxPage (1);	// no texts
	else
		pInfo->SetMaxPage ((int) ceil ((m_vecBookletPages.size () + 2 - 1) / 4.0) * 2);
}

void CCdCoverCreator2View::DoPrintBooklet(CDC *pDC, CPrintInfo *pInfo, CSize szUnit, int nLeft, int nTop, double dMarginLeft, double dMarginTop, int nOrientation)
{
	CStyleTemplate* pStyle = GetDocument ()->m_pStyle;
	CCdCoverCreator2App* pApp = (CCdCoverCreator2App*) AfxGetApp ();
	CRect* pRect = pStyle->GetTotalBookletRect ();

	// draw ticks
	CRect rcTotal (dMarginLeft * szUnit.cx - nLeft, dMarginTop * szUnit.cy - nTop,
		(dMarginLeft + pRect->Width () / 1000.0) * szUnit.cx - nLeft,
		(dMarginTop + pRect->Height () / 1000.0) * szUnit.cy - nTop);
	if (pInfo->m_nCurPage % 2 == 0)
	{
		if (nOrientation == 0)	// Portrait
		{
			rcTotal.bottom = (LONG) ((pApp->GetPaperLength (1) - dMarginTop) * szUnit.cy - nTop);
			rcTotal.top = (LONG) (rcTotal.bottom - pRect->Height () / 1000.0 * szUnit.cy);
		}
		else
		{
			rcTotal.right = (LONG) ((pApp->GetPaperLength (0) - dMarginLeft) * szUnit.cx - nLeft);
			rcTotal.left = (LONG) (rcTotal.right - pRect->Width () / 1000.0 * szUnit.cx);
		}
	}

	// top
	pDC->MoveTo (rcTotal.left - szUnit.cx / 2, rcTotal.top);
	pDC->LineTo (rcTotal.left - szUnit.cx / 4, rcTotal.top);
	pDC->MoveTo (rcTotal.left, rcTotal.top - szUnit.cy / 2);
	pDC->LineTo (rcTotal.left, rcTotal.top - szUnit.cy / 4);
	pDC->MoveTo ((rcTotal.left + rcTotal.right) / 2, rcTotal.top - szUnit.cy / 2);
	pDC->LineTo ((rcTotal.left + rcTotal.right) / 2, rcTotal.top - szUnit.cy / 4);
	pDC->MoveTo (rcTotal.right + szUnit.cx / 2, rcTotal.top);
	pDC->LineTo (rcTotal.right + szUnit.cx / 4, rcTotal.top);
	pDC->MoveTo (rcTotal.right, rcTotal.top - szUnit.cy / 2);
	pDC->LineTo (rcTotal.right, rcTotal.top - szUnit.cy / 4);
	// bottom
	pDC->MoveTo (rcTotal.left - szUnit.cx / 2, rcTotal.bottom);
	pDC->LineTo (rcTotal.left - szUnit.cx / 4, rcTotal.bottom);
	pDC->MoveTo (rcTotal.left, rcTotal.bottom + szUnit.cy / 2);
	pDC->LineTo (rcTotal.left, rcTotal.bottom + szUnit.cy / 4);
	pDC->MoveTo ((rcTotal.left + rcTotal.right) / 2, rcTotal.bottom + szUnit.cy / 2);
	pDC->LineTo ((rcTotal.left + rcTotal.right) / 2, rcTotal.bottom + szUnit.cy / 4);
	pDC->MoveTo (rcTotal.right + szUnit.cx / 2, rcTotal.bottom);
	pDC->LineTo (rcTotal.right + szUnit.cx / 4, rcTotal.bottom);
	pDC->MoveTo (rcTotal.right, rcTotal.bottom + szUnit.cy / 2);
	pDC->LineTo (rcTotal.right, rcTotal.bottom + szUnit.cy / 4);

	// end -- draw ticks

	if (pInfo->m_nCurPage == 1)
	{
		// first page: print cover and back

		CCdCoverCreator2Doc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		int nOrgX = (int) (dMarginLeft * szUnit.cx) - nLeft;
		int nOrgY = (int) (dMarginTop * szUnit.cy) - nTop;

		pDC->SetViewportOrg (nOrgX, nOrgY);
		pDC->SetViewportExt (pInfo->m_rectDraw.Width () - nOrgX, pInfo->m_rectDraw.Height () - nOrgY);

		CLabelRenderer renderer (pDoc->m_pTracks, pDoc->m_pDataCDInfo, pDoc->m_pBackground,
			pDoc->m_listFloatingObjects, *(pDoc->m_pStyle));

		CRenderDC rdc (pDC, (int) (pDoc->m_pStyle->GetTotalCoverRect ()->Height () * szUnit.cy / 1000), pDoc->m_pStyle);
		renderer.Render (rdc, Back, false, true, false);

		nOrgX += 12 * szUnit.cx;
		pDC->SetViewportOrg (nOrgX, nOrgY);
		pDC->SetViewportExt (pInfo->m_rectDraw.Width () - nOrgX, pInfo->m_rectDraw.Height () - nOrgY);
		renderer.Render (rdc, Cover, false, true, false);
	}
	else
	{
		int nTotalPages = (int) ceil ((m_vecBookletPages.size () + 2 - 1) / 4.0) * 4 - 2;
		// +2: cover and back are printed, too
		// -1: actual number of pages is .size()-1

		// page numbers
		if (pStyle->ShowBookletPageNums ())
		{
			CFont font;
			LOGFONT lf;
			memcpy (&lf, pStyle->GetBookletPageNumFont (), sizeof (LOGFONT));
			lf.lfHeight = -MulDiv (abs (lf.lfHeight), pDC->GetDeviceCaps (LOGPIXELSY), 72);
			font.CreateFontIndirect (&lf);
			CFont* pOldFont = (CFont*) pDC->SelectObject (&font);
			pDC->SetTextColor (pStyle->GetBookletPageNumColor ());

			// (if odd page, "revert")
			CRect rcPageNum ((dMarginLeft + pStyle->GetBookletMarginLeft ()) * szUnit.cx - nLeft,
				(dMarginTop + pRect->Height () / 1000.0 - 0.5) * szUnit.cy - pDC->GetTextExtent ("X").cy - nTop,
				(dMarginLeft + pRect->Width () / 1000.0 - pStyle->GetBookletMarginRight ()) * szUnit.cx - nLeft,
				(dMarginTop + pRect->Height () / 1000.0 - 0.5) * szUnit.cy - nTop);
			if (pInfo->m_nCurPage % 2 == 0)
			{
				if (nOrientation == 0)	// Portrait
				{
					rcPageNum.bottom = (LONG) ((pApp->GetPaperLength (1) - dMarginTop - 0.5) * szUnit.cy - nTop);
					rcPageNum.top = (LONG) (rcPageNum.bottom - pDC->GetTextExtent ("X").cy);
				}
				else
				{
					rcPageNum.right = (LONG) ((pApp->GetPaperLength (0) - dMarginLeft - pStyle->GetBookletMarginRight ()) * szUnit.cx - nLeft);
					rcPageNum.left = (LONG) (rcPageNum.right - (pRect->Width () / 1000.0 - pStyle->GetBookletMarginRight () - pStyle->GetBookletMarginLeft ()) * szUnit.cx);
				}
			}

			CRect rcPageNumLeft = rcPageNum;
			CRect rcPageNumRight = rcPageNum;
			rcPageNumLeft.right = (rcPageNum.left + rcPageNum.right) / 2 - (int) (pStyle->GetBookletMarginRight () * szUnit.cx);
			rcPageNumRight.left += rcPageNum.Width () / 2 + (int) (pStyle->GetBookletMarginLeft () * szUnit.cx);

			CString strPage;
			strPage.Format ("%d", pInfo->m_nCurPage % 2 == 0 ? pInfo->m_nCurPage : nTotalPages - pInfo->m_nCurPage + 3);
			pDC->DrawText (strPage, &rcPageNumLeft, /*DT_SINGLELINE | DT_TOP |*/
				(pStyle->GetBookletPageNumAlign () == 0 ? DT_LEFT : pStyle->GetBookletPageNumAlign () == 1 ? DT_CENTER : DT_RIGHT));

			strPage.Format ("%d", pInfo->m_nCurPage % 2 == 0 ? nTotalPages - pInfo->m_nCurPage + 3 : pInfo->m_nCurPage);
			pDC->DrawText (strPage, &rcPageNumRight, /*DT_SINGLELINE | DT_TOP |*/
				(pStyle->GetBookletPageNumAlign () == 0 ? DT_RIGHT : pStyle->GetBookletPageNumAlign () == 1 ? DT_CENTER : DT_LEFT));

			pDC->SelectObject (pOldFont);
		}

		// adjust DC because richedit doesn't do things like MFC
		if (pDC->GetDeviceCaps (TECHNOLOGY) != DT_METAFILE && pDC->m_hAttribDC != NULL)
		{
			::ScaleWindowExtEx (pDC->m_hDC,
				::GetDeviceCaps (pDC->m_hDC, LOGPIXELSX),
				::GetDeviceCaps (pDC->m_hAttribDC, LOGPIXELSX),
				::GetDeviceCaps (pDC->m_hDC, LOGPIXELSY),
				::GetDeviceCaps (pDC->m_hAttribDC, LOGPIXELSY), NULL);
		}

		FORMATRANGE fr;
		int nHorzRes = pDC->GetDeviceCaps (HORZRES);
		int nVertRes = pDC->GetDeviceCaps (VERTRES);
		int nLogPixelsX = pDC->GetDeviceCaps (LOGPIXELSX);
		int nLogPixelsY = pDC->GetDeviceCaps (LOGPIXELSY);

		// Rendering to the same DC we are printing.
		ZeroMemory (&fr, sizeof (fr));
		fr.hdc = fr.hdcTarget = pDC->GetSafeHdc ();
		
		// Set up the page
		fr.rcPage.left = fr.rcPage.top = 0;
		fr.rcPage.right = MulDiv (nHorzRes, 1440, nLogPixelsX);
		fr.rcPage.bottom = MulDiv (nVertRes, 1440, nLogPixelsY);

		int nRcTop, nRcBottom;

		// left page
		////////////////////////////////////////////////////////////////

		// Set up margins. Booklet is 12 cm x 12 cm (minus style margins)
		if (nOrientation == 0)	// Portrait
		{
			fr.rc.left = (LONG) ((dMarginLeft + pStyle->GetBookletMarginLeft ()) / 2.54 * 1440 - (nLeft * 1440 / 2.54 / szUnit.cx));
			fr.rc.right = (LONG) (fr.rc.left + (pRect->Width () / 2 / 1000.0 - pStyle->GetBookletMarginLeft () - pStyle->GetBookletMarginRight ()) / 2.54 * 1440);
			
			if (pInfo->m_nCurPage % 2 == 0)
			{
				nRcBottom = (int) ((pApp->GetPaperLength (1) - dMarginTop - pStyle->GetBookletMarginBottom ()) / 2.54 * 1440 - (nTop * 1440 / 2.54 / szUnit.cy));
				nRcTop = (int) (nRcBottom - (pRect->Height () / 1000.0 - pStyle->GetBookletMarginBottom () - pStyle->GetBookletMarginTop ()) / 2.54 * 1440);
			}
			else
			{
				nRcTop = (int) ((dMarginTop + pStyle->GetBookletMarginTop ()) / 2.54 * 1440 - nTop * 1440 / 2.54 / szUnit.cy);
				nRcBottom = (int) (nRcTop + (pRect->Height () / 1000.0 - pStyle->GetBookletMarginTop () - pStyle->GetBookletMarginBottom ()) / 2.54 * 1440);
			}
		}
		else	// Landscape
		{
			nRcTop = (int) ((dMarginTop + pStyle->GetBookletMarginTop ()) / 2.54 * 1440 - nTop * 1440 / 2.54 / szUnit.cy);
			nRcBottom = (int) (nRcTop + (pRect->Height () / 1000.0 - pStyle->GetBookletMarginTop () - pStyle->GetBookletMarginBottom ()) / 2.54 * 1440);
			
			if (pInfo->m_nCurPage % 2 == 0)
			{
				fr.rc.right = (LONG) ((pApp->GetPaperLength (0) - dMarginLeft - pStyle->GetBookletMarginRight ()) / 2.54 * 1440 - (nLeft * 1440 / 2.54 / szUnit.cx));
				fr.rc.left = (LONG) (fr.rc.right - (pRect->Width () / 1000.0 - pStyle->GetBookletMarginRight () - pStyle->GetBookletMarginLeft ()) / 2.54 * 1440);
			}
			else
				fr.rc.left = (LONG) ((dMarginLeft + pStyle->GetBookletMarginLeft ()) / 2.54 * 1440 - (nLeft * 1440 / 2.54 / szUnit.cx));

			fr.rc.right = (LONG) (fr.rc.left + (pRect->Width () / 2 / 1000.0 - pStyle->GetBookletMarginLeft () - pStyle->GetBookletMarginRight ()) / 2.54 * 1440);
		}

		bool bDoPrint = true;

		if (pInfo->m_nCurPage % 2 == 0)
		{
			if ((pInfo->m_nCurPage - 1 >= m_vecBookletPages.size ()) || (pInfo->m_nCurPage - 2 >= m_vecBookletPages.size ()))
				bDoPrint = false;
			else
			{
				fr.chrg.cpMin = m_vecBookletPages.at (pInfo->m_nCurPage - 2);
				fr.chrg.cpMax = m_vecBookletPages.at (pInfo->m_nCurPage - 1);
			}
		}
		else
		{
			if ((nTotalPages - pInfo->m_nCurPage + 1 >= m_vecBookletPages.size ()) || (nTotalPages - pInfo->m_nCurPage + 2 >= m_vecBookletPages.size ()))
				bDoPrint = false;
			else
			{
				fr.chrg.cpMin = m_vecBookletPages.at (nTotalPages - pInfo->m_nCurPage + 1);
				fr.chrg.cpMax = m_vecBookletPages.at (nTotalPages - pInfo->m_nCurPage + 2);
			}
		}

		if (bDoPrint)
		{
			fr.rc.top = nRcTop;
			fr.rc.bottom = nRcBottom;

			m_wndRichEdit.FormatRange (&fr);
			m_wndRichEdit.DisplayBand (&fr.rc);
		}

		// right page
		////////////////////////////////////////////////////////////////

		fr.rc.left = (LONG) (fr.rc.right + (pStyle->GetBookletMarginRight () + pStyle->GetBookletMarginLeft ()) / 2.54 * 1440);
		fr.rc.right = (LONG) (fr.rc.left + (pRect->Width () / 2 / 1000.0 - pStyle->GetBookletMarginLeft () - pStyle->GetBookletMarginRight ()) / 2.54 * 1440);

		bDoPrint = true;

		if (pInfo->m_nCurPage % 2 == 0)
		{
			if ((nTotalPages - pInfo->m_nCurPage + 1 >= m_vecBookletPages.size ()) || (nTotalPages - pInfo->m_nCurPage + 2 >= m_vecBookletPages.size ()))
				bDoPrint = false;
			else
			{
				fr.chrg.cpMin = m_vecBookletPages.at (nTotalPages - pInfo->m_nCurPage + 1);
				fr.chrg.cpMax = m_vecBookletPages.at (nTotalPages - pInfo->m_nCurPage + 2);
			}
		}
		else
		{
			if ((pInfo->m_nCurPage - 1 >= m_vecBookletPages.size ()) || (pInfo->m_nCurPage - 2 >= m_vecBookletPages.size ()))
				bDoPrint = false;
			else
			{
				fr.chrg.cpMin = m_vecBookletPages.at (pInfo->m_nCurPage - 2);
				fr.chrg.cpMax = m_vecBookletPages.at (pInfo->m_nCurPage - 1);
			}
		}

		if (bDoPrint)
		{
			fr.rc.top = nRcTop;
			fr.rc.bottom = nRcBottom;

			m_wndRichEdit.FormatRange (&fr);
			m_wndRichEdit.DisplayBand (&fr.rc);
		}
	}
}

void CCdCoverCreator2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	::ClosePrinter (m_hndPrinter);

#ifdef PEROUTKA_FEDORISIN
	// hide the label selector
	m_dlgSelectLabels.ShowWindow (SW_HIDE);
#endif
}


// CCdCoverCreator2View diagnostics

#ifdef _DEBUG
void CCdCoverCreator2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCdCoverCreator2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CCdCoverCreator2Doc* CCdCoverCreator2View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCdCoverCreator2Doc)));
	return (CCdCoverCreator2Doc*)m_pDocument;
}
#endif //_DEBUG


// CCdCoverCreator2View message handlers

void CCdCoverCreator2View::OnZoomin() 
{
	m_nHeight = (int) (m_nHeight * 1.4142135623730950488016887242097);	// sqrt(2)
	AdjustScrollSizes ();
	OnUpdate (this, 0, NULL);
}

void CCdCoverCreator2View::OnZoomout() 
{
	m_nHeight = (int) (m_nHeight / 1.4142135623730950488016887242097);
	AdjustScrollSizes ();
	OnUpdate (this, 0, NULL);
}

void CCdCoverCreator2View::OnUpdateZoomin(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable ((m_nHeight <= 1024) && (m_RenderType != Booklet));
}

void CCdCoverCreator2View::OnUpdateZoomout(CCmdUI* pCmdUI)
{
	pCmdUI->Enable ((m_nHeight > 64) && (m_RenderType != Booklet));
}

void CCdCoverCreator2View::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	GetDocument ()->Properties (GetDocument ()->m_pTracks == NULL ? DataCD : AudioCD);
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CCdCoverCreator2View::OnContextMenu(CWnd*, CPoint point)
{
	// popup menu for a floating object?
	if (m_pPrevTrck != NULL)
	{
		CCdCoverCreator2Doc* pDoc = GetDocument ();
		for (std::list<CFloatingGraphicsObject*>::iterator it = pDoc->m_listFloatingObjects[m_RenderType].begin (); it != pDoc->m_listFloatingObjects[m_RenderType].end (); it++)
			if ((*it)->GetTracker () == m_pPrevTrck)
			{
				m_pCurrentContextMenuObj = *it;
				(*it)->ContextMenu (this);
				return;
			}
	}

	m_pCurrentContextMenuObj = NULL;

	// no floating object is selected => standard popup menu

	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect (rect);
		ClientToScreen (rect);

		point = rect.TopLeft ();
		point.Offset (5, 5);
	}

	BCMenu menu;
	VERIFY (menu.LoadMenu (IDR_POPUP_LABELITVIEW));
	menu.LoadToolbar (IDR_MAINFRAME);

	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle () & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent ();

	menu.GetSubMenu (0)->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);

	menu.DestroyMenu ();
}

void CCdCoverCreator2View::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	if (bActivate)
	{
		GetDocument ()->DisplayStyle ();

		GetDocument ()->DisplayMediumType ();
		GetDocument ()->SetMediumType (GetDocument ()->m_pStyle->GetMediumType ());
		
		((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (
			(m_RenderType == Booklet) || m_bIsEditingTitle ? &m_wndRichEdit : NULL);
	}

	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CCdCoverCreator2View::OnShowinlay() 
{
	ShowRenderType (Inlay);
}

void CCdCoverCreator2View::OnUpdateShowinlay(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (GetDocument ()->m_pStyle->m_bHasRenderType[Inlay]);
	pCmdUI->SetRadio (m_RenderType == Inlay);
}

void CCdCoverCreator2View::OnShowcover() 
{
	ShowRenderType (Cover);
}

void CCdCoverCreator2View::OnUpdateShowcover(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (GetDocument ()->m_pStyle->m_bHasRenderType[Cover]);
	pCmdUI->SetRadio (m_RenderType == Cover);
}

void CCdCoverCreator2View::OnShowback() 
{
	ShowRenderType (Back);
}

void CCdCoverCreator2View::OnUpdateShowback(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (GetDocument ()->m_pStyle->m_bHasRenderType[Back]);
	pCmdUI->SetRadio (m_RenderType == Back);	
}

void CCdCoverCreator2View::OnShowlabel() 
{
	ShowRenderType (Label);
}

void CCdCoverCreator2View::OnUpdateShowlabel(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (GetDocument ()->m_pStyle->m_bHasRenderType[Label]);
	pCmdUI->SetRadio (m_RenderType == Label);	
}

void CCdCoverCreator2View::OnShowbooklet() 
{
	ShowRenderType (Booklet);
}

void CCdCoverCreator2View::OnUpdateShowbooklet(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (GetDocument ()->m_pStyle->m_bHasRenderType[Booklet]);
	pCmdUI->SetRadio (m_RenderType == Booklet);	
}

void CCdCoverCreator2View::ShowRenderType(RenderType rt)
{
	bool bModified = false;

	if (m_bIsEditingTitle)
	{
		// end title editing
		m_bIsEditingTitle = false;

		if (GetDocument ()->SetRTFTitle (m_wndRichEdit.GetRTF (), m_RenderType))
			bModified = true;
	}
	if (m_nEditingTrackIdx >= 0)
	{
		m_wndTrackEdit.ShowWindow (SW_HIDE);
		if (GetDocument ()->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx))
			bModified = true;
		
		m_nEditingTrackNum = -1;
		m_nEditingTrackIdx = -1;
	}

	if (m_RenderType == Booklet)
	{
		// save the booklet's content
		GetDocument ()->m_strBookletRTFText = m_wndRichEdit.GetRTF ();
	}

	// is this render type active at all?
	if (!GetDocument ()->m_pStyle->m_bHasRenderType[rt])
	{
		MessageBeep (0);
		return;
	}

	if (rt == Booklet)
	{
		// restore the booklet's content
		SetRTF (GetDocument ()->m_strBookletRTFText);

		CRect r;
		GetClientRect (&r);
		m_wndRichEdit.SetWindowPos (NULL, 0, 0, r.Width (), r.Height (), SWP_NOZORDER);
	}

	m_RenderType = rt;
	
	m_wndRichEdit.ShowWindow (rt == Booklet ? SW_SHOW : SW_HIDE);
	if (rt == Booklet)
		m_wndRichEdit.SetFocus ();

	AdjustScrollSizes ();
	OnUpdate (this, 0, NULL);

	CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd ();
	pMainFrame->UpdateStyleDlgs (GetDocument ()->m_pStyle, GetDocument ()->m_pBackground[rt], rt);
	pMainFrame->m_wndRTFToolbar.SetRTFControl ((rt == Booklet) || m_bIsEditingTitle ? &m_wndRichEdit : NULL);

	if (bModified)
		GetDocument ()->CheckPoint ();
}

void CCdCoverCreator2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// 'Tab' toggles between views
	switch (nChar)
	{
		// scrolling
	case VK_UP:
		ScrollToPosition (CPoint (GetScrollPos (SB_HORZ), GetScrollPos (SB_VERT) - 5));
		break;
	case VK_DOWN:
		ScrollToPosition (CPoint (GetScrollPos (SB_HORZ), GetScrollPos (SB_VERT) + 5));
		break;
	case VK_LEFT:
		ScrollToPosition (CPoint (GetScrollPos (SB_HORZ) - 5, GetScrollPos (SB_VERT)));
		break;
	case VK_RIGHT:
		ScrollToPosition (CPoint (GetScrollPos (SB_HORZ) + 5, GetScrollPos (SB_VERT)));
		break;
	case VK_PRIOR:
		ScrollToPosition (CPoint (GetScrollPos (SB_HORZ), GetScrollPos (SB_VERT) - 50));
		break;
	case VK_NEXT:
		ScrollToPosition (CPoint (GetScrollPos (SB_HORZ), GetScrollPos (SB_VERT) + 50));
		break;
	case VK_HOME:
		ScrollToPosition (CPoint (GetScrollPos (SB_HORZ), GetScrollPos (SB_VERT)));
		break;
	case VK_END:
		{
			int nMin, nMax;
			GetScrollRange (SB_HORZ, &nMin, &nMax);
			ScrollToPosition (CPoint (nMax, GetScrollPos (SB_VERT)));
		}
		break;

		// switching between render types
	case '\t':
		int n = ::GetKeyState (VK_SHIFT) & 0x8000 ? NUM_RENDER_TYPES - 1 : 1;
		m_RenderType = (RenderType) ((m_RenderType + n) % NUM_RENDER_TYPES);

		if (m_RenderType == Booklet)
		{
			m_wndRichEdit.ShowWindow (SW_SHOW);
			AdjustScrollSizes ();
			((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (&m_wndRichEdit);
		}
		else
		{
			m_wndRichEdit.ShowWindow (SW_HIDE);
			OnUpdate (this, 0, NULL);
			AdjustScrollSizes ();
			((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (NULL);
		}

		((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (
			GetDocument ()->m_pStyle, GetDocument ()->m_pBackground[m_RenderType], m_RenderType);

		break;
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CCdCoverCreator2View::OnEraseBkgnd(CDC* pDC) 
{
	/*
	CRect r;
	GetClientRect (&r);
//	pDC->FillSolidRect (&r, RGB (0xF0, 0xF0, 0xFF));
	CGradientRenderer::DrawGradient (pDC, r, -450, RGB (0xF0, 0xF0, 0xFF), RGB (0xA0, 0xA0, 0xAA), NULL);
	*/
	return TRUE;
}

void CCdCoverCreator2View::AdjustScrollSizes()
{
	CClientDC dc (this);
	CRenderDC rdc (&dc, m_nHeight, GetDocument ()->m_pStyle);
	
	CRect r;
	switch (m_RenderType)
	{
	case Cover:
	case Back:
		r = *GetDocument ()->m_pStyle->GetTotalCoverRect (rdc);
		break;
	case Label:
		r = *GetDocument ()->m_pStyle->GetTotalLabelRect (rdc);
		break;
	case Inlay:
		r = *GetDocument ()->m_pStyle->GetTotalInlayRect (rdc);
		break;
	case Booklet:
		r.left = r.top = 0;
		r.right = r.bottom = 50;
		break;
	}
	
	CSize sz (r.Width () + 40, r.Height () + 40);
	SetScrollSizes (MM_TEXT, sz);
}

RenderType CCdCoverCreator2View::GetRenderType()
{
	return m_RenderType;
}

BOOL CCdCoverCreator2View::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bSettingTransparentColor)
	{
		::SetCursor (m_hPickCursor);
		return TRUE;
	}
	else if (pWnd == this)
	{
		CCdCoverCreator2Doc* pDoc = GetDocument ();
		for (std::list<CFloatingGraphicsObject*>::iterator it = pDoc->m_listFloatingObjects[m_RenderType].begin (); it != pDoc->m_listFloatingObjects[m_RenderType].end (); it++)
			if ((*it)->SetCursor (pWnd, nHitTest))
				return TRUE;

		if (m_trackerTitle.m_nStyle != 0)
			if (m_trackerTitle.SetCursor (this, nHitTest))
				return TRUE;
		if (m_trackerTracks.m_nStyle != 0)
			if (m_trackerTracks.SetCursor (this, nHitTest))
				return TRUE;
		if (m_trackerImage.m_nStyle != 0)
			if (m_trackerImage.SetCursor (this, nHitTest))
				return TRUE;
	}
	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CCdCoverCreator2View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();
	bool bTrck = false;
	bool bTrackingFloatingObject = false;
	bool bEndEditingTitle = false;

	CPoint ptScrollPos = GetScrollPosition ();
	CSize sizeOffset (20 - ptScrollPos.x, 20 - ptScrollPos.y);

	// Are we adding a new floating object?
	if (m_pCurrentAddingObj != NULL)
	{
		AddToolObject (m_pCurrentAddingObj, point);
		goto __end;
	}

	// Are we setting a transparent color for an image?
	if (m_bSettingTransparentColor)
	{
		POINT pt;
		::GetCursorPos (&pt);

		HDC hdc = ::GetDC (NULL);
		(reinterpret_cast<CFloatingImage*> (m_pCurrentContextMenuObj))->SetTransparentColor (::GetPixel (hdc, pt.x, pt.y));
		::ReleaseDC (NULL, hdc);

		m_bSettingTransparentColor = false;

		ReleaseCapture ();

		// update image
		pDoc->UpdateAllViews (NULL);
		pDoc->CheckPoint ();
		goto __end;
	}

	// None of all this...
	// Regular procedure

	//////////////////////////////////////////////////////////////////////////
	// Hit tests

	// floating objects
	for (std::list<CFloatingGraphicsObject*>::reverse_iterator it = pDoc->m_listFloatingObjects[m_RenderType].rbegin (); it != pDoc->m_listFloatingObjects[m_RenderType].rend (); it++)
	{
		if ((*it)->HitTest (point) > 0)
		{
			// unselect the previous tracker, if it is not us
			if (m_pPrevTrck != NULL && m_pPrevTrck != (*it)->GetTracker ())
				m_pPrevTrck->m_nStyle = 0;

			// end editing of the previous tracker
			if (m_pEditingFGO != NULL)
			{
				m_pEditingFGO->EndEdit (&m_wndRichEdit);
				pDoc->CheckPoint ();
			}

			// enable editing if this tracker has been selected previously
			CRect rcEdit = (*it)->GetTracker ()->m_rect;
			rcEdit.DeflateRect (5, 5, 5, 5);	// enable resizing (user must click in the deflated rect to enable editing)

			if ((*it)->GetTracker ()->m_nStyle != 0 && rcEdit.PtInRect (point))
			{
				m_pEditingFGO = *it;
				(*it)->Edit (&m_wndRichEdit);
			}

			// we are the current tracker
			bTrackingFloatingObject = true;
			(*it)->GetTracker ()->m_nStyle = CRectTracker::solidLine | CRectTracker::resizeInside;
			m_pPrevTrck = (*it)->GetTracker ();

			break;
		}
	}

	if (!bTrackingFloatingObject)
	{
		// end editing of floating objects
		if (m_pEditingFGO != NULL)
		{
			m_pEditingFGO->EndEdit (&m_wndRichEdit);
			pDoc->CheckPoint ();
		}
		m_pEditingFGO = NULL;

		if (pDoc->m_pStyle->GetDisplayTitles (m_RenderType))
		{
			if (m_trackerTitle.HitTest (point) > 0)
			{
				if (m_pPrevTrck != NULL)
					m_pPrevTrck->m_nStyle = 0;

				if ((m_RenderType != Label) || (m_RenderType == Label && !pDoc->m_pStyle->IsLabelTitleRound ()))
				{
					CRect rcEdit = m_trackerTitle.m_rect;// - ptScrollPos;
					rcEdit.DeflateRect (5, 5, 5, 5);	// enable resizing

					// if second click in this tracker - show RTF editor
					if ((m_pPrevTrck == &m_trackerTitle) &&	rcEdit.PtInRect (point))
					{
						m_bIsEditingTitle = true;

						m_wndRichEdit.MoveWindow (&m_trackerTitle.m_rect);
						SetRTF (pDoc->GetRTFTitle (m_RenderType));
						((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (&m_wndRichEdit);
						
						m_wndRichEdit.ShowWindow (SW_SHOW);
						m_wndRichEdit.SetFocus ();
					}
				}
				if (m_RenderType == Label && pDoc->m_pStyle->IsLabelTitleRound ())
					MessageBeep (0);

				bTrck = true;
				m_trackerTitle.m_nStyle = CRectTracker::solidLine | CRectTracker::resizeInside;

				m_pPrevTrck = &m_trackerTitle;
			}
			else
			{
				// end editing mode
				if (m_bIsEditingTitle)
				{
					m_bIsEditingTitle = false;
					bEndEditingTitle = true;
					m_wndRichEdit.ShowWindow (SW_HIDE);
					((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (NULL);
					if (pDoc->SetRTFTitle (m_wndRichEdit.GetRTF (), m_RenderType))
						pDoc->CheckPoint ();	// title has changed
				}
			}
		}

		if (pDoc->m_pStyle->GetDisplayTrackGroups (m_RenderType))
		{
			if (m_trackerTracks.HitTest (point) > 0)
			{
				if (m_pPrevTrck != NULL)
					m_pPrevTrck->m_nStyle = 0;

				if ((m_RenderType == Cover) || (m_RenderType == Back) || (m_RenderType == Inlay))
				{
					CRect rcEdit = m_trackerTracks.m_rect - ptScrollPos;
					rcEdit.DeflateRect (5, 5, 5, 5);	// enable resizing
				
					// if second click in this tracker - show tracks text editor
					if (pDoc->m_pTracks != NULL)
					{
						if ((m_pPrevTrck == &m_trackerTracks) && rcEdit.PtInRect (point))
						{
							if (m_RenderType == Cover)
								MessageBeep (0);
							else
							{
								// save previous text
								if (m_nEditingTrackIdx >= 0)
								{
									pDoc->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx);
									pDoc->UpdateAllViews (NULL);
								}

								CClientDC dc (this);
								CRenderDC rdc (&dc, m_nHeight, pDoc->m_pStyle);

								CRect r;
								CString strText;
								LOGFONT lf;
								COLORREF col;
								DWORD dwAlign;

								if (pDoc->m_pTracks->GetTrackRectFromPoint (
									point - CPoint (20, 20) + GetScrollPosition (),
									rdc, m_RenderType, *(pDoc->m_pStyle),
									r, strText, m_nEditingTrackNum, m_nEditingTrackIdx, &lf, col, dwAlign))
								{
									switch (m_nEditingTrackIdx)
									{
									case -1:	// no hit: stop editing
									case 0:		// tracks num hit. Do not allow track num to be edited
										m_wndTrackEdit.ShowWindow (SW_HIDE);

										m_nEditingTrackIdx = -1;
										m_nEditingTrackNum = -1;

										break;

									default:
										r.OffsetRect (20 - ptScrollPos.x, 20 - ptScrollPos.y);
										//r.OffsetRect (20, 20);
										r.right += 2;
										m_wndTrackEdit.MoveWindow (&r);
										m_wndTrackEdit.SetText (strText);
										m_wndTrackEdit.SetSel (0, -1);
										m_wndTrackEdit.SetAppearance (col, &lf, dwAlign);
										
										m_wndTrackEdit.ShowWindow (SW_SHOW);
										m_wndTrackEdit.SetFocus ();
									}
								}
							}
						}
					}
				}
				else
					MessageBeep (0);

				bTrck = true;
				m_trackerTracks.m_nStyle = CRectTracker::solidLine | CRectTracker::resizeInside;

				m_pPrevTrck = &m_trackerTracks;
			}
			else
			{
				// end editing mode
				if (m_nEditingTrackIdx >= 0)
				{
					m_wndTrackEdit.ShowWindow (SW_HIDE);
					
					m_nEditingTrackNum = -1;
					m_nEditingTrackIdx = -1;

					if (pDoc->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx))
					{
						pDoc->UpdateAllViews (NULL);
						pDoc->CheckPoint ();	// text has changed
					}
				}
			}
		}

		if (m_trackerImage.HitTest (point) > 0)
		{
			if (m_pPrevTrck != NULL)
				m_pPrevTrck->m_nStyle = 0;

			bTrck = true;
			m_trackerImage.m_nStyle = CRectTracker::solidLine | CRectTracker::resizeInside;

			m_pPrevTrck = &m_trackerImage;
		}
	}

	// If none of the trackers has been clicked...
	if (!(bTrck || bTrackingFloatingObject) && (m_pPrevTrck != NULL))
	{
		// unmark the previous tracker
		m_pPrevTrck->m_nStyle = 0;
		m_pPrevTrck = NULL;

		// update
		bTrck = true;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// Tracking

	if (!m_bIsEditingTitle)
	{
		CWindowDC dc (this);
		CRenderDC rdc (&dc, m_nHeight, pDoc->m_pStyle);

		// floating objects
		bTrackingFloatingObject = false;
		for (std::list<CFloatingGraphicsObject*>::reverse_iterator it = pDoc->m_listFloatingObjects[m_RenderType].rbegin (); it != pDoc->m_listFloatingObjects[m_RenderType].rend (); it++)
		{
			bTrackingFloatingObject = bTrackingFloatingObject || (*it)->Track (this, point, m_RenderType, rdc, pDoc->m_pStyle, sizeOffset);
			if (bTrackingFloatingObject)
				break;
		}

		if (!bTrackingFloatingObject)
		{
			if (m_trackerTitle.Track (this, point))
			{
				bTrck = true;

				m_trackerTitle.m_rect.OffsetRect (ptScrollPos.x - 20, ptScrollPos.y - 20);	// due to vieport org at (20, 20)
				//m_trackerTitle.m_rect.OffsetRect (-20, -20);
				pDoc->m_pStyle->SetTitleRect (m_trackerTitle.m_rect, m_RenderType, rdc);

				// end editing mode
				if (m_bIsEditingTitle)
				{
					m_bIsEditingTitle = false;
					m_wndRichEdit.ShowWindow (SW_HIDE);
					((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (NULL);
					pDoc->SetRTFTitle (m_wndRichEdit.GetRTF (), m_RenderType);
				}

				pDoc->CheckPoint ();
				((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (pDoc->m_pStyle, pDoc->m_pBackground[m_RenderType], m_RenderType);
			}

			if ((m_RenderType != Label) && !bEndEditingTitle)
				if (m_trackerTracks.Track (this, point))
				{
					bTrck = true;
					m_trackerTracks.m_rect.OffsetRect (ptScrollPos.x - 20, ptScrollPos.y - 20);	// due to vieport org at (20, 20)

					pDoc->m_pStyle->SetTrackgroupAlign (m_RenderType, 0, 0);
					pDoc->m_pStyle->SetTrackRect (m_trackerTracks.m_rect, m_RenderType, rdc);

					// end editing mode
					m_wndTrackEdit.ShowWindow (SW_HIDE);
					m_nEditingTrackNum = -1;
					m_nEditingTrackIdx = -1;

					if (pDoc->m_pTracks != NULL)
						pDoc->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx);

					pDoc->CheckPoint ();
					((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (pDoc->m_pStyle, pDoc->m_pBackground[m_RenderType], m_RenderType);
				}

			if (m_trackerImage.Track (this, point))
			{
				bTrck = true;
			}
		}
	}

	if (bTrck || bTrackingFloatingObject)
		OnUpdate (NULL, NULL, NULL);
	
__end:
	CScrollView::OnLButtonDown(nFlags, point);
}

void CCdCoverCreator2View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CScrollView::OnLButtonUp(nFlags, point);
}

void CCdCoverCreator2View::DrawTrackers(CRenderDC &rdc)
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	switch (m_RenderType)
	{
	case Cover:
		m_trackerTitle.m_rect = *(pDoc->m_pStyle->GetCoverTitleRect (rdc));
		m_trackerTracks.m_rect = *(pDoc->m_pStyle->GetCoverTrackRect (rdc));
		break;
	case Back:
		m_trackerTitle.m_rect = *(pDoc->m_pStyle->GetCoverTitleRect (rdc, Back));
		m_trackerTracks.m_rect = *(pDoc->m_pStyle->GetBackTrackRect (rdc));
		break;
	case Inlay:
		m_trackerTitle.m_rect = *(pDoc->m_pStyle->GetInlayTitleRect (rdc));
		m_trackerTracks.m_rect = *(pDoc->m_pStyle->GetInlayTrackRect (rdc));
		break;
	case Label:
		m_trackerTitle.m_rect = *(pDoc->m_pStyle->GetLabelTitleRect (rdc));
		m_trackerTracks.m_rect = *(pDoc->m_pStyle->GetLabelTrackRect (rdc));
		break;
	}

	// viewport org is at (20, 20)
	CSize sizeOffset (20 - GetScrollPosition ().x, 20 - GetScrollPosition ().y);

	m_trackerTitle.m_rect.OffsetRect (sizeOffset);
	m_trackerTracks.m_rect.OffsetRect (sizeOffset);
	m_trackerImage.m_rect.OffsetRect (sizeOffset);

	m_trackerTitle.Draw (rdc.m_pDC);
	m_trackerTracks.Draw (rdc.m_pDC);
	m_trackerImage.Draw (rdc.m_pDC);

	for (std::list<CFloatingGraphicsObject*>::iterator it = pDoc->m_listFloatingObjects[m_RenderType].begin (); it != pDoc->m_listFloatingObjects[m_RenderType].end (); it++)
		(*it)->DrawTracker (rdc, m_RenderType, pDoc->m_pStyle, sizeOffset);
}

int CCdCoverCreator2View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_wndTrackEdit.Create (WS_CHILD | ES_AUTOHSCROLL | ES_MULTILINE | ES_NOHIDESEL,
		CRect (0, 0, 0, 0), this, IDC_VIEWTRACKEDIT);

	m_wndRichEdit.Create (WS_CHILD | WS_VSCROLL |
		/*ES_AUTOHSCROLL |*/ ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_WANTRETURN,
		CRect (0, 0, 0, 0), this, IDC_VIEWRICHEDIT);

	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT);
	cf.dwMask = CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_COLOR | CFM_BACKCOLOR;
	_tcscpy (cf.szFaceName, "Tahoma");
	cf.yHeight = 10 * 20;
	cf.dwEffects = CFE_AUTOCOLOR | CFE_AUTOBACKCOLOR;
	//cf.crTextColor = ::GetSysColor (COLOR_WINDOWTEXT);
	//cf.crBackColor = ::GetSysColor (COLOR_WINDOW);
	m_wndRichEdit.SetDefaultCharFormat (cf);

	m_wndRichEdit.LimitText (2147483647);
	
#ifdef PEROUTKA_FEDORISIN
	// create the label selector dialog
	m_dlgSelectLabels.Create (IDD_DIALOG_SELECTLABELS, AfxGetMainWnd ());

	// set the window position
	CRect r;
	::GetWindowRect (((CMainFrame*) AfxGetMainWnd ())->m_hWndMDIClient, &r);
	m_dlgSelectLabels.SetWindowPos (NULL, r.right - 150, r.top + 50, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	// initial label to print
	m_dlgSelectLabels.m_LabelSelector.InternalSetState (1);
#endif

	return 0;
}

void CCdCoverCreator2View::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	if (m_RenderType == Booklet)
		m_wndRichEdit.SetWindowPos (NULL, 0, 0, cx, cy, SWP_NOZORDER);
}

void CCdCoverCreator2View::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	m_Bitmap.DeleteObject ();

	CWindowDC dc (this);
	CDC memDC;
	memDC.CreateCompatibleDC (&dc);
	CRenderDC rdc (&memDC, m_nHeight, pDoc->m_pStyle);

//	CRect r (0, 0, ::GetSystemMetrics (SM_CXFULLSCREEN), ::GetSystemMetrics (SM_CYFULLSCREEN));
	CRect r (0, 0, 1500, 1500);	// max zoom + viewpoint org offset + ...
	m_Bitmap.CreateCompatibleBitmap (&dc, r.Width (), r.Height ());
	CBitmap* pOld = memDC.SelectObject (&m_Bitmap);

	CGradientRenderer::DrawGradient (&memDC, NULL, r, -450, RGB (0xF0, 0xF0, 0xFF), RGB (0x70, 0x70, 0x77), NULL);

	CLabelRenderer renderer (pDoc->m_pTracks, pDoc->m_pDataCDInfo, pDoc->m_pBackground,
		pDoc->m_listFloatingObjects, *(pDoc->m_pStyle));

	memDC.SetViewportOrg (20, 20);
	renderer.Render (rdc, m_RenderType);

	memDC.SelectObject (pOld);
	memDC.DeleteDC ();

	AdjustScrollSizes ();
	InvalidateRect (NULL);
}

void CCdCoverCreator2View::OnDestroy() 
{
	((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (NULL);	
	CScrollView::OnDestroy();
}

void CCdCoverCreator2View::OnRichEditEscape ()
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	if (m_bIsEditingTitle)
	{
		m_wndRichEdit.ShowWindow (SW_HIDE);
		((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (NULL);
		m_bIsEditingTitle = false;

		if (GetDocument ()->SetRTFTitle (m_wndRichEdit.GetRTF (), m_RenderType))
		{
			pDoc->UpdateAllViews (NULL);
			pDoc->CheckPoint ();	// title has changed
		}
	}
	else
	{
		for (std::list<CFloatingGraphicsObject*>::iterator it = pDoc->m_listFloatingObjects[m_RenderType].begin (); it != pDoc->m_listFloatingObjects[m_RenderType].end (); it++)
			if ((*it)->GetTracker () == m_pPrevTrck)
			{
				(*it)->EndEdit (&m_wndRichEdit);
				pDoc->UpdateAllViews (NULL);
				pDoc->CheckPoint ();
			}
	}
}

void CCdCoverCreator2View::OnTrackEditEscape ()
{
	// end editing mode
	if (m_nEditingTrackIdx >= 0)
	{
		m_wndTrackEdit.ShowWindow (SW_HIDE);
		
		m_nEditingTrackNum = -1;
		m_nEditingTrackIdx = -1;

		if (GetDocument ()->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx))
		{
			GetDocument ()->UpdateAllViews (NULL);
			GetDocument ()->CheckPoint ();	// text has changed
		}
	}
}

void CCdCoverCreator2View::OnTrackEditUp ()
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	// save previous text
	if (m_nEditingTrackIdx >= 0)
		if (pDoc->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx))
		{
			pDoc->UpdateAllViews (NULL);
			pDoc->CheckPoint ();	// text has changed
		}

	CClientDC dc (this);
	CRenderDC rdc (&dc, m_nHeight, pDoc->m_pStyle);

	CRect r (-1, -1, -1, -1);
	CString strText;
	LOGFONT lf;
	COLORREF col;
	DWORD dwAlign;

	m_nEditingTrackNum--;
	if (m_nEditingTrackNum < 0)
		m_nEditingTrackNum = 0;

	if (pDoc->m_pTracks->GetTrackRectFromIdx (m_nEditingTrackNum, m_nEditingTrackIdx, 
		rdc, m_RenderType, *(pDoc->m_pStyle), r, strText, &lf, col, dwAlign))
	{
		r.OffsetRect (20 - GetScrollPosition ().x, 20 - GetScrollPosition ().y);
		r.right += 2;
		m_wndTrackEdit.MoveWindow (&r);
		m_wndTrackEdit.SetText (strText);
		m_wndTrackEdit.SetSel (0, -1);
		m_wndTrackEdit.SetAppearance (col, &lf, dwAlign);
		
		m_wndTrackEdit.ShowWindow (SW_SHOW);
		m_wndTrackEdit.SetFocus ();
	}
}

void CCdCoverCreator2View::OnTrackEditDown ()
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	// save previous text
	if (m_nEditingTrackIdx >= 0)
		if (pDoc->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx))
		{
			pDoc->UpdateAllViews (NULL);
			pDoc->CheckPoint ();	// text has changed
		}

	CClientDC dc (this);
	CRenderDC rdc (&dc, m_nHeight, pDoc->m_pStyle);

	CRect r (-1, -1, -1, -1);
	CString strText;
	LOGFONT lf;
	COLORREF col;
	DWORD dwAlign;

	m_nEditingTrackNum++;
	if (m_nEditingTrackNum >= pDoc->m_pTracks->GetNumTracks ())
		m_nEditingTrackNum = pDoc->m_pTracks->GetNumTracks () - 1;

	if (pDoc->m_pTracks->GetTrackRectFromIdx (m_nEditingTrackNum, m_nEditingTrackIdx, 
		rdc, m_RenderType, *(pDoc->m_pStyle), r, strText, &lf, col, dwAlign))
	{
		r.OffsetRect (20 - GetScrollPosition ().x, 20 - GetScrollPosition ().y);
		r.right += 2;
		m_wndTrackEdit.MoveWindow (&r);
		m_wndTrackEdit.SetText (strText);
		m_wndTrackEdit.SetSel (0, -1);
		m_wndTrackEdit.SetAppearance (col, &lf, dwAlign);
		
		m_wndTrackEdit.ShowWindow (SW_SHOW);
		m_wndTrackEdit.SetFocus ();
	}
}

void CCdCoverCreator2View::OnTrackEditNext ()
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	// save previous text
	if (m_nEditingTrackIdx >= 0)
		if (pDoc->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx))
		{
			pDoc->UpdateAllViews (NULL);
			pDoc->CheckPoint ();
		}

	CClientDC dc (this);
	CRenderDC rdc (&dc, m_nHeight, pDoc->m_pStyle);

	CRect r (-1, -1, -1, -1);
	CString strText;
	LOGFONT lf;
	COLORREF col;
	DWORD dwAlign;

	int nPos = pDoc->m_pStyle->GetTrackPositionById (m_nEditingTrackIdx) + 1;
	if (nPos == 4)
	{
		m_nEditingTrackNum++;
		if (m_nEditingTrackNum >= pDoc->m_pTracks->GetNumTracks ())
			m_nEditingTrackNum = 0;
		nPos = 1;
	}
	for (int i = 1; i < 4; i++)
		if (pDoc->m_pStyle->GetTrackPositionById (i) == nPos)
		{
			m_nEditingTrackIdx = i;
			break;
		}

	if (pDoc->m_pTracks->GetTrackRectFromIdx (m_nEditingTrackNum, m_nEditingTrackIdx, 
		rdc, m_RenderType, *(pDoc->m_pStyle), r, strText, &lf, col, dwAlign))
	{
		r.OffsetRect (20 - GetScrollPosition ().x, 20 - GetScrollPosition ().y);
		r.right += 2;
		m_wndTrackEdit.MoveWindow (&r);
		m_wndTrackEdit.SetText (strText);
		m_wndTrackEdit.SetSel (0, -1);
		m_wndTrackEdit.SetAppearance (col, &lf, dwAlign);
		
		m_wndTrackEdit.ShowWindow (SW_SHOW);
		m_wndTrackEdit.SetFocus ();
	}
}

void CCdCoverCreator2View::OnTrackEditPrev ()
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	// save previous text
	if (m_nEditingTrackIdx >= 0)
		if (pDoc->m_pTracks->SetTrackText (m_wndTrackEdit.GetText (), m_nEditingTrackNum, m_nEditingTrackIdx))
		{
			// text has changed
			pDoc->UpdateAllViews (NULL);
			pDoc->CheckPoint ();
		}

	CClientDC dc (this);
	CRenderDC rdc (&dc, m_nHeight, pDoc->m_pStyle);

	CRect r (-1, -1, -1, -1);
	CString strText;
	LOGFONT lf;
	COLORREF col;
	DWORD dwAlign;

	int nPos = pDoc->m_pStyle->GetTrackPositionById (m_nEditingTrackIdx) - 1;
	if (nPos == 0)
	{
		m_nEditingTrackNum--;
		if (m_nEditingTrackNum < 0)
			m_nEditingTrackNum = pDoc->m_pTracks->GetNumTracks () - 1;
		nPos = 3;
	}
	for (int i = 1; i < 4; i++)
		if (pDoc->m_pStyle->GetTrackPositionById (i) == nPos)
		{
			m_nEditingTrackIdx = i;
			break;
		}

	if (pDoc->m_pTracks->GetTrackRectFromIdx (m_nEditingTrackNum, m_nEditingTrackIdx, 
		rdc, m_RenderType, *(pDoc->m_pStyle),	r, strText, &lf, col, dwAlign))
	{
		r.OffsetRect (20 - GetScrollPosition ().x, 20 - GetScrollPosition ().y);
		r.right += 2;
		m_wndTrackEdit.MoveWindow (&r);
		m_wndTrackEdit.SetText (strText);
		m_wndTrackEdit.SetSel (0, -1);
		m_wndTrackEdit.SetAppearance (col, &lf, dwAlign);
		
		m_wndTrackEdit.ShowWindow (SW_SHOW);
		m_wndTrackEdit.SetFocus ();
	}
}

void CCdCoverCreator2View::OnToolAddText ()
{
	if (m_pCurrentAddingObj != NULL)
		delete m_pCurrentAddingObj;

	m_pCurrentAddingObj = new CFloatingText ();
}

void CCdCoverCreator2View::OnToolAddImage ()
{
	if (m_pCurrentAddingObj != NULL)
		delete m_pCurrentAddingObj;

	m_pCurrentAddingObj = new CFloatingImage ();
}

void CCdCoverCreator2View::OnToolAddBarcode ()
{
	if (m_pCurrentAddingObj != NULL)
		delete m_pCurrentAddingObj;

	m_pCurrentAddingObj = new CFloatingBarCode ();
}

void CCdCoverCreator2View::OnUpdateToolAddText (CCmdUI* pCmdUI)
{
	bool bIsChecked = false;
	if (m_pCurrentAddingObj != NULL)
		bIsChecked = m_pCurrentAddingObj->IsKindOf (RUNTIME_CLASS (CFloatingText));

	pCmdUI->SetCheck (bIsChecked);
}

void CCdCoverCreator2View::OnUpdateToolAddImage (CCmdUI* pCmdUI)
{
	bool bIsChecked = false;
	if (m_pCurrentAddingObj != NULL)
		bIsChecked = m_pCurrentAddingObj->IsKindOf (RUNTIME_CLASS (CFloatingImage));

	pCmdUI->SetCheck (bIsChecked);
}

void CCdCoverCreator2View::OnUpdateToolAddBarcode (CCmdUI* pCmdUI)
{
	bool bIsChecked = false;
	if (m_pCurrentAddingObj != NULL)
		bIsChecked = m_pCurrentAddingObj->IsKindOf (RUNTIME_CLASS (CFloatingBarCode));

	pCmdUI->SetCheck (bIsChecked);
}

void CCdCoverCreator2View::AddToolObject (CFloatingGraphicsObject* pObj, CPoint pt)
{
	CCdCoverCreator2Doc* pDoc = GetDocument ();

	CWindowDC dc (this);
	CRenderDC rdc (&dc, NULL, m_nHeight, pDoc->m_pStyle);

	if (pObj->Add (this, pt, m_RenderType, rdc, GetDocument ()->m_pStyle, &m_wndRichEdit, CSize (20 - GetScrollPosition ().x, 20 - GetScrollPosition ().y)))
	{
		if (m_pPrevTrck != NULL)
		{
			OnRichEditEscape ();		// end editing for previous tracker
			m_pPrevTrck->m_nStyle = 0;	// deselect previous tracker
		}

		m_pEditingFGO = pObj;
		m_pPrevTrck = pObj->GetTracker ();

		// add to the objects list and update the views
		pDoc->m_listFloatingObjects[m_RenderType].push_back (pObj);
		pDoc->UpdateAllViews (NULL);
		pDoc->CheckPoint ();
	}
	else
		delete pObj;

	m_pCurrentAddingObj = NULL;
}

void CCdCoverCreator2View::OnFloatingObjectMenuPosition (UINT nId)
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument ();

	std::list<CFloatingGraphicsObject*>* pList = &pDoc->m_listFloatingObjects[m_RenderType];
	std::list<CFloatingGraphicsObject*>::iterator it;

	switch (nId)
	{
	case ID__RAISE:
		it = std::find (pList->begin (), pList->end (), m_pCurrentContextMenuObj);
		if (it != pList->end ())
		{
			std::list<CFloatingGraphicsObject*>::iterator it1 = it;
			if (++it1 != pList->end ())
			{
				it = pList->erase (it);
				it++;
				pList->insert (it, m_pCurrentContextMenuObj);
			}
		}
		break;

	case ID__LOWER:
		it = std::find (pList->begin (), pList->end (), m_pCurrentContextMenuObj);
		if (it != pList->end () && it != pList->begin ())
		{
			it = pList->erase (it);
			pList->insert (--it, m_pCurrentContextMenuObj);
		}
		break;

	case ID__BRINGTOFRONT:
		pDoc->m_listFloatingObjects[m_RenderType].remove (m_pCurrentContextMenuObj);
		pDoc->m_listFloatingObjects[m_RenderType].push_back (m_pCurrentContextMenuObj);
		break;

	case ID__SENDTOBACK:
		pDoc->m_listFloatingObjects[m_RenderType].remove (m_pCurrentContextMenuObj);
		pDoc->m_listFloatingObjects[m_RenderType].push_front (m_pCurrentContextMenuObj);
		break;

	default:
		return;
	}

	pDoc->UpdateAllViews (NULL);
	pDoc->CheckPoint ();
}

/**
 * Delete a floating object
 */
void CCdCoverCreator2View::OnFloatingObjectMenuDelete ()
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument ();

	// remove the objects
	pDoc->m_listFloatingObjects[m_RenderType].remove (m_pCurrentContextMenuObj);

	if (m_pEditingFGO == m_pCurrentContextMenuObj)
		m_pEditingFGO = NULL;

	delete m_pCurrentContextMenuObj;
	m_pCurrentContextMenuObj = NULL;

	// update the views
	pDoc->UpdateAllViews (NULL);
	pDoc->CheckPoint ();
}

/**
 * Sets the transparent color of the currently selected picture
 */
void CCdCoverCreator2View::OnFloatingImageMenuSelectTransparentColor ()
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	m_bSettingTransparentColor = true;
	SetCapture ();
	SetCursor (m_hPickCursor);
}

void CCdCoverCreator2View::OnFloatingImageMenuAdjustColors ()
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument ();
	CPicture* pPic = ((CFloatingImage*) m_pCurrentContextMenuObj)->GetPicture ();

	CDlgImageProperties dlg;
	dlg.m_pDocument = pDoc;
	dlg.SetPicture (pPic);

	if (dlg.DoModal () == IDOK)
		pDoc->CheckPoint ();

	// NOTE: all the Set stuff is done in the dialog
}

void CCdCoverCreator2View::OnFloatingImageEdit ()
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	CPicture* pPic = ((CFloatingImage*) m_pCurrentContextMenuObj)->GetPicture ();
	if (pPic == NULL)
		return;

	CString strAppPath = AfxGetApp ()->GetProfileString ("ImageEditing", "app", "");
	if (strAppPath.IsEmpty ())
	{
		CDlgSetImageEditor dlg;
		if (dlg.DoModal () == IDOK)
		{
			strAppPath = dlg.m_strAppName;
			if (strAppPath.IsEmpty ())
				return;
		}
	}

	WinExec (strAppPath + " \"" + pPic->GetFilename () + "\"", SW_SHOW);
}

void CCdCoverCreator2View::OnFloatingImageReload ()
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	CPicture* pPic = ((CFloatingImage*) m_pCurrentContextMenuObj)->GetPicture ();
	if (pPic != NULL)
	{
		pPic->Reload ();
		GetDocument ()->UpdateAllViews (NULL);
	}
}

void CCdCoverCreator2View::OnFloatingImageAlign (UINT nId)
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument ();
	CStyleTemplate* pStyle = pDoc->m_pStyle;

	double dLeft = m_RenderType == Inlay ? pStyle->GetInlaySideWidth () : 0;
	double dRight = ((double) pStyle->GetTotalRect (m_RenderType)->Width () / 1000.0) - m_pCurrentContextMenuObj->GetWidth () - dLeft;
	double dXMiddle = ((double) pStyle->GetTotalRect (m_RenderType)->Width () / 1000.0 - m_pCurrentContextMenuObj->GetWidth ()) / 2.0;
	double dYMiddle = ((double) pStyle->GetTotalRect (m_RenderType)->Height () / 1000.0 - m_pCurrentContextMenuObj->GetHeight ()) / 2.0;
	double dBottom = ((double) pStyle->GetTotalRect (m_RenderType)->Height () / 1000.0) - m_pCurrentContextMenuObj->GetHeight ();

	switch (nId)
	{
	case ID_ALIGN_TOPLEFT:
		m_pCurrentContextMenuObj->SetTopLeft (dLeft, 0);
		break;
	case ID_ALIGN_TOPMIDDLE:
		m_pCurrentContextMenuObj->SetTopLeft (dXMiddle, 0);
		break;
	case ID_ALIGN_TOPRIGHT:
		m_pCurrentContextMenuObj->SetTopLeft (dRight, 0);
		break;

	case ID_ALIGN_MIDDLELEFT:
		m_pCurrentContextMenuObj->SetTopLeft (dLeft, dYMiddle);
		break;
	case ID_ALIGN_MIDDLEMIDDLE:
		m_pCurrentContextMenuObj->SetTopLeft (dXMiddle, dYMiddle);
		break;
	case ID_ALIGN_MIDDLERIGHT:
		m_pCurrentContextMenuObj->SetTopLeft (dRight, dYMiddle);
		break;

	case ID_ALIGN_BOTTOMLEFT:
		m_pCurrentContextMenuObj->SetTopLeft (dLeft, dBottom);
		break;
	case ID_ALIGN_BOTTOMMIDDLE:
		m_pCurrentContextMenuObj->SetTopLeft (dXMiddle, dBottom);
		break;
	case ID_ALIGN_BOTTOMRIGHT:
		m_pCurrentContextMenuObj->SetTopLeft (dRight, dBottom);
		break;

	default:
		return;
	}

	// update the views
	pDoc->UpdateAllViews (NULL);
	pDoc->CheckPoint ();
}

void CCdCoverCreator2View::OnFloatingImageStretchToFit ()
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument ();
	CStyleTemplate* pStyle = pDoc->m_pStyle;

	m_pCurrentContextMenuObj->SetTopLeft (0, 0);
	m_pCurrentContextMenuObj->SetSize (
		(double) pStyle->GetTotalRect (m_RenderType)->Width () / 1000.0,
		(double) pStyle->GetTotalRect (m_RenderType)->Height () / 1000.0);

	// update the views
	pDoc->UpdateAllViews (NULL);
	pDoc->CheckPoint ();
}

void CCdCoverCreator2View::OnFloatingBarcodeProperties ()
{
	if (m_pCurrentContextMenuObj == NULL)
		return;

	CDlgBarcodeProperties dlg (((CFloatingBarCode*) m_pCurrentContextMenuObj));
	if (dlg.DoModal ())
	{
			CCdCoverCreator2Doc* pDoc = GetDocument ();

		// update the views
		pDoc->UpdateAllViews (NULL);
		pDoc->CheckPoint ();
	}
}

void CCdCoverCreator2View::SetRTF (CString strRTF)
{
	// if the string is empty, replace it by a default empty RTF
	if (strRTF.IsEmpty ())
		strRTF = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2055{\\fonttbl{\\f0\\fswiss\\fcharset0 Tahoma;}}\\viewkind4\\uc1\\pard\\f0\\fs20\\par}";
	
	// set the RTF to the editor
	m_wndRichEdit.SetRTF (strRTF);
}

void CCdCoverCreator2View::OnUpdateFileLightscribe(CCmdUI *pCmdUI)
{
	pCmdUI->Enable (m_RenderType == Label);
}

int CCdCoverCreator2View::GetEncoderClsid (const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;		// number of image encoders
	UINT  size = 0;		// size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize (&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*) (malloc (size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders (num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
		if (wcscmp (pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free (pImageCodecInfo);
			return j;  // Success
		}

	free (pImageCodecInfo);
	return -1;  // Failure
}

void CCdCoverCreator2View::OnFileSaveAsImage()
{
	if ((int) m_RenderType == -1)
		return;

	CCdCoverCreator2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CWinApp* pApp = AfxGetApp ();
	CRect* pRect;

	switch (m_RenderType)
	{
	case Cover:
		pRect = pDoc->m_pStyle->GetTotalCoverRect ();
		break;
	case Back:
		pRect = pDoc->m_pStyle->GetTotalCoverRect ();
		break;
	case Label:
		pRect = pDoc->m_pStyle->GetTotalLabelRect ();
		break;
	case Inlay:
		pRect = pDoc->m_pStyle->GetTotalInlayRect ();
		break;
	case Booklet:
		return;
	}

	CFileDialog dlg (FALSE, _T ("*.jpg"), _T ("*.jpg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "JPEG Images (*.jpg)", AfxGetMainWnd ());
	if (dlg.DoModal () == IDOK)
	{
		// create an image, select it into a Graphics and create a CDC from it
		Bitmap image (pRect->Width () / 5, pRect->Height () / 5, PixelFormat32bppARGB);
		Graphics graphics (&image);
		SolidBrush brush (Color::White);
		graphics.FillRectangle (&brush, 0, 0, image.GetWidth (), image.GetHeight ());
		CDC* pDC = CDC::FromHandle (graphics.GetHDC ());

		// create the renderer and render
		CLabelRenderer renderer (pDoc->m_pTracks, pDoc->m_pDataCDInfo, pDoc->m_pBackground,
			pDoc->m_listFloatingObjects, *(pDoc->m_pStyle));

		CRenderDC rdc (pDC, pDoc->m_pStyle->GetTotalCoverRect ()->Height () / 5, pDoc->m_pStyle);
		renderer.Render (rdc, m_RenderType, false, true, false, false);

		graphics.ReleaseHDC ((HDC) *pDC);

		// save the image
		BSTR bstrFilenameJPEG = dlg.GetPathName ().AllocSysString ();

		CLSID clsidJpeg;
		GetEncoderClsid (L"image/jpeg", &clsidJpeg);
		image.Save (bstrFilenameJPEG, &clsidJpeg);
		
		::SysFreeString (bstrFilenameJPEG);
	}
}
