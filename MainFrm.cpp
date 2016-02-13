// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CdCoverCreator2.h"

#include "MainFrm.h"
#include "CdCoverCreator2Doc.h"
#include "CdCoverCreator2View.h"

#include "StyleTemplateWizard.h"
#include "PageSetupDlg.h"

#include "WebCdCoversBrowser1.h"
#include "LyricsBrowser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_STYLETEMPLATE, OnStyletemplate)
	ON_WM_CLOSE()
	ON_WM_DROPFILES()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_COVERBROWSER, OnViewCoverbrowser)
	ON_COMMAND(ID_VIEW_STYLES, OnViewStyles)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STYLES, OnUpdateViewStyles)
	ON_COMMAND(ID_VIEW_LEFTPANEL, OnViewLeftpanel)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LEFTPANEL, OnUpdateViewLeftpanel)
	ON_COMMAND(ID_VIEW_TEXTTOOLBAR, OnViewTextToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TEXTTOOLBAR, OnUpdateViewTextToolbar)
	ON_COMMAND(ID_VIEW_LYRICSBROWSER, OnViewLyricsbrowser)
	ON_COMMAND(ID_RTFF_BOLD, OnRtffBold)
	ON_COMMAND(ID_RTFF_ITALIC, OnRtffItalic)
	ON_COMMAND(ID_RTFF_UNDERLINE, OnRtffUnderline)
	ON_COMMAND(ID_RTFF_UNDERLINE_DOT, OnRtffUnderlineDot)
	ON_COMMAND(ID_RTFF_STRIKETHRU, OnRtffStrikethru)
	ON_COMMAND(ID_RTFF_ALIGNLEFT, OnRtffAlignleft)
	ON_COMMAND(ID_RTFF_ALIGNCENTER, OnRtffAligncenter)
	ON_COMMAND(ID_RTFF_ALIGNRIGHT, OnRtffAlignright)
	ON_COMMAND(ID_RTFF_SUP, OnRtffSup)
	ON_COMMAND(ID_RTFF_SUB, OnRtffSub)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_COMMAND(ID_RTFF_BORDER, OnRtffBorder)
	ON_COMMAND(ID_RTFF_PAGEBREAK, OnRtffPagebreak)
	ON_COMMAND(ID_FILE_PAGE_SETUP, OnFilePageSetup)
	ON_COMMAND(ID_RTFF_BULLETS, OnRtffBullets)
	ON_COMMAND_RANGE(ID_RTFF_BORDER_LEFT, ID_RTFF_BORDER_OUTSIDE, OnRtffBorderStyle)
	ON_COMMAND_RANGE(ID_RTFF_BULLETS_NONE, ID_RTFF_BULLETS_ROMUPPER, OnRtffBulletsStyle)

	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_WINDOW_NEXTWINDOW, OnWindowNextwindow)
	ON_COMMAND(ID_WINDOW_PREVIOUSWINDOW, OnWindowPreviouswindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEXTWINDOW, OnUpdateWindowNextwindow)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_PREVIOUSWINDOW, OnUpdateWindowPreviouswindow)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_LIGHTSCRIBE_PANE,
	ID_LIGHTSCRIBE_PROGRESS,
	//ID_LIGHTSCRIBE_TIME_LEFT,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	PrepareSysImageList ();

	m_bInitPosition = true;

	m_dwLastWindowPosChange = 0;
	m_bTimerSet = false;

	m_pActivateCoverBrowser = NULL;
	m_bmCoverBrowserMode = BrowserSearch;

	m_pWebCdCovers = NULL;
}

CMainFrame::~CMainFrame()
{
	delete m_pWndRollup;

//	m_menu.DestroyMenu ();
//	m_menuDefault.DestroyMenu ();

	m_ilSystemImageList.Detach ();
	//delete m_pBmpSystemImageList;

	if (m_pWebCdCovers != NULL)
		delete m_pWebCdCovers;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
//	
	if (!m_wndToolBar.Create (this, AFX_IDW_TOOLBAR) ||
		!m_wndToolBar.LoadToolBar (IDR_MAINFRAME, AILS_NEW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndToolBarWindow.Create (this, AFX_IDW_TOOLBAR) ||
		!m_wndToolBarWindow.LoadToolBar (IDR_TOOLBAR_WINDOW, AILS_NEW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndDlgBar.Create (this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndRTFToolbar.CreateBar (this))
	//if (!m_wndRTFToolBar.CreateEx (this) || !m_wndRTFToolBar.LoadToolBar (IDR_TOOLBAR_RTFFORMAT, AILS_NEW))
	{
		TRACE0 ("Failed to create RTF toolbar\n");
		return -1;
	}

	if (!m_wndDlgBarSelectMedium.Create (this, IDD_DIALOGBAR_MEDIUM, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR + 1))
	{
		TRACE0("Failed to create medium dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndMenuBar.Create (this) ||
		!m_wndMenuBar.LoadMenuBar (IDR_LABELSTYPE, AILS_NEW))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
	
	m_wndMenuBar.LoadToolBar (IDR_MAINFRAME);
	m_wndMenuBar.LoadToolBar (IDR_TOOLBAR_NEW);

	// toolbar creation
	/////////////////////////////////////////////////////////
/*
	if (!m_wndToolBar.CreateEx (this) || !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndDlgBar.Create (this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndRTFToolbar.CreateBar (this))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndDlgBarSelectMedium.Create (this, IDD_DIALOGBAR_MEDIUM, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR + 1))
	{
		TRACE0("Failed to create medium dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create (this) ||
		!m_wndReBar.AddBar (&m_wndToolBar) ||
		!m_wndReBar.AddBar (&m_wndDlgBar) ||
		!m_wndReBar.AddBar (&m_wndRTFToolbar) ||
		!m_wndReBar.AddBar (&m_wndDlgBarSelectMedium))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
*/

	if (!m_wndReBar.Create (this, RBS_BANDBORDERS,
		WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | CBRS_ALIGN_TOP)
		|| !m_wndReBar.AddBar (&m_wndMenuBar)
		|| !m_wndReBar.AddBar (&m_wndToolBarWindow)
		|| !m_wndReBar.AddBar (&m_wndToolBar, NULL, (CBitmap*) NULL, RBBS_BREAK | RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP)
		|| !m_wndReBar.AddBar (&m_wndDlgBarSelectMedium)
//		|| !m_wndReBar.AddBar (&m_wndToolBarTools, NULL, (CBitmap*) NULL, RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP)
		|| !m_wndReBar.AddBar (&m_wndRTFToolbar, NULL, (CBitmap*) NULL, RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP)
		|| !m_wndReBar.AddBar (&m_wndDlgBar)
		)
    {
        TRACE0("Failed to create rebar\n");
        return -1;      // fail to create
    }

	m_wndReBar.GetReBarCtrl ().MinimizeBand (0);
	m_wndDlgBar.UpdateData (false);

	if (!m_wndStatusBar.Create (this) ||
		!m_wndStatusBar.SetIndicators (indicators, sizeof (indicators) / sizeof (UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// clear status texts and create progress bar
	m_wndStatusBar.SetPaneText (1, "");
	m_wndStatusBar.SetPaneText (2, "");
	//m_wndStatusBar.SetPaneText (3, "");
	m_wndProgress.Create (WS_CHILD | PBS_SMOOTH, CRect (), &m_wndStatusBar, ID_LIGHTSCRIBE_PROGRESS);

	if (!m_wndLeftPanel.Create (this, IDD_LEFTPANEL, CBRS_LEFT, 101))
	{
		TRACE0("Failed to create left panel bar\n");
		return -1;      // fail to create
	}

	CString strCaption;
	strCaption.LoadString (IDS_LEFTPANELCAPTION);
	m_wndLeftPanel.SetWindowText (strCaption);

	EnableDocking (CBRS_ALIGN_ANY);
	m_wndLeftPanel.EnableDocking (CBRS_ALIGN_ANY);
	DockControlBar (&m_wndLeftPanel, AFX_IDW_DOCKBAR_LEFT);

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle (m_wndToolBar.GetBarStyle () |	CBRS_TOOLTIPS | CBRS_FLYBY);

	//m_wndRTFToolbar.SetBarStyle (m_wndRTFToolbar.GetStyle () | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndRTFToolbar.SetBarStyle (CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_ALIGN_TOP);
	m_wndRTFToolbar.SetRTFControl (NULL);

	// Toolbar Arrows
//	m_wndToolBar.GetToolBarCtrl ().SetExtendedStyle (TBSTYLE_EX_DRAWDDARROWS);

	TBBUTTONINFO tbi;
	tbi.dwMask = TBIF_STYLE;
	tbi.cbSize = sizeof (TBBUTTONINFO);

	m_wndToolBar.GetToolBarCtrl ().GetButtonInfo (ID_FILE_NEW, &tbi);
	tbi.fsStyle |= TBSTYLE_DROPDOWN;
	m_wndToolBar.GetToolBarCtrl ().SetButtonInfo (ID_FILE_NEW, &tbi);

	m_wndToolBar.GetToolBarCtrl ().GetButtonInfo (ID_FILE_OPEN, &tbi);
	tbi.fsStyle |= TBSTYLE_DROPDOWN;
	m_wndToolBar.GetToolBarCtrl ().SetButtonInfo (ID_FILE_OPEN, &tbi);

	m_wndReBar.GetReBarCtrl ().MaximizeBand (0);

	// rollup window
	/////////////////////////////////////////////////////////

	m_pWndRollup = new CRollupWnd (CStyleDlgGradient::IDD, NULL);
	if (!m_pWndRollup->Create (IDD_DIALOG_ROLLUP, this))
	{
		TRACE0 ("Failed to create style rollup\n");
		return -1;
	}

	CRect r;
	m_pWndRollup->GetClientRect (&r);
	r.DeflateRect (2, 2, 2, 2);
	r.bottom = 300;
	m_wndRollupCtrl.Create (WS_VISIBLE | WS_CHILD, r, m_pWndRollup, 2);

	// Add some pages

	// Measurements
	m_wndStyleMeasures.Create (IDD_DIALOG_STYLEMEASURES, &m_wndRollupCtrl);
	strCaption.LoadString (IDS_DLGSTYLE_TITLES);
	m_wndRollupCtrl.InsertPage (strCaption, &m_wndStyleMeasures, false);

	// Track gourping measurements
	m_wndStyleTrackgroupings.Create (IDD_DIALOG_STYLETRACKGROUPINGS, &m_wndRollupCtrl);
	strCaption.LoadString (IDS_DLGSTYLE_TRACKGROUPINGS);
	m_wndRollupCtrl.InsertPage (strCaption, &m_wndStyleTrackgroupings, false);

	// Tracks
	m_wndStyleTracks.Create (IDD_DIALOG_STYLETRACKS, &m_wndRollupCtrl);
	strCaption.LoadString (IDS_DLGSTYLE_TRACKS);
	m_wndRollupCtrl.InsertPage (strCaption, &m_wndStyleTracks, false);

	// Spines
	m_wndStyleSpines.Create (IDD_DIALOG_STYLESPINES, &m_wndRollupCtrl);
	strCaption.LoadString (IDS_DLGSTYLE_SPINES);
	m_wndRollupCtrl.InsertPage (strCaption, &m_wndStyleSpines, false);

	// Fonts
	m_wndStyleFonts.Create (IDD_DIALOG_STYLEFONTS, &m_wndRollupCtrl);
	strCaption.LoadString (IDS_DLGSTYLE_FONTS);
	m_wndRollupCtrl.InsertPage (strCaption, &m_wndStyleFonts, false);

	// Gradient
	m_wndStyleGradient.Create (IDD_DIALOG_STYLE, &m_wndRollupCtrl);
	strCaption.LoadString (IDS_DLGSTYLE_GRADIENT);
	m_wndRollupCtrl.InsertPage (strCaption, &m_wndStyleGradient, false);

	// Bk Images
	m_wndStyleBkImg.Create (IDD_DIALOG_STYLEBKIMG, &m_wndRollupCtrl);
	strCaption.LoadString (IDS_DLGSTYLE_BKIMGS);
	m_wndRollupCtrl.InsertPage (strCaption, &m_wndStyleBkImg, false);

	// Alpha textures
//	m_wndStyleAlpha.Create (IDD_DIALOG_STYLEALPHA, &m_wndRollupCtrl);
//	strCaption.LoadString (IDS_DLGSTYLE_ALPHA);
//	m_wndRollupCtrl.InsertPage (strCaption, &m_wndStyleAlpha, false);

	UpdateStyleDlgs (NULL, NULL, (RenderType) -1);

//	m_pWndRollup->UpdateWindow ();
//	m_pWndRollup->ShowWindow (SW_SHOW);
	// (show only after positioning)

	// init Twain
	m_twain.InitTwain (m_hWnd);

//	OnViewCoverbrowser ();

	m_wndDlgBar.ListStyles ();

	GetMenu ()->DestroyMenu ();
	SetMenu (NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.x = 0;
	cs.y = 0;
	cs.cx = ::GetSystemMetrics (SM_CXMAXIMIZED) - 2 * ::GetSystemMetrics (SM_CXSIZEFRAME);
	cs.cy = ::GetSystemMetrics (SM_CYMAXIMIZED) - 2 * ::GetSystemMetrics (SM_CYSIZEFRAME);

	return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_MENU)
        m_wndMenuBar.SetPrefix(TRUE);
    else if (pMsg->message == WM_KEYUP && pMsg->wParam == VK_MENU)
        m_wndMenuBar.SetPrefix(FALSE);

	m_twain.ProcessMessage (*pMsg);
	return __super::PreTranslateMessage(pMsg);
//    return CFrameWnd::PreTranslateMessage(pMsg);
}

LRESULT CMainFrame::OnMenuChar (UINT nChar, UINT nFlags, CMenu* pMenu)
{
    if (m_wndMenuBar.OpenMenu(nChar))
        return -1;

    return CFrameWnd::OnMenuChar(nChar, nFlags, pMenu);
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
    if (nID == SC_KEYMENU && m_wndMenuBar.OnKeyMenu(lParam))
        return;

    CFrameWnd::OnSysCommand(nID, lParam);
}

void CMainFrame::OnActivate (UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
    m_wndMenuBar.Activate(nState != WA_INACTIVE);

    CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    m_wndMenuBar.UpdateSettings();

    CFrameWnd::OnSettingChange(uFlags, lpszSection);
}

using namespace Gdiplus;

void CMainFrame::PrepareSysImageList ()
{
	SHFILEINFO sfi;
	memset (&sfi, 0, sizeof (sfi));
	HIMAGELIST hil = reinterpret_cast<HIMAGELIST> (
		SHGetFileInfo ("C:\\", 0, &sfi, sizeof (sfi), SHGFI_SYSICONINDEX));
	if (hil)
		m_ilSystemImageList.Attach (hil);

/*
	Bitmap bmp (m_ilSystemImageList.GetImageCount () * 32 * 2, 32, PixelFormat32bppARGB);
	Graphics* pGraphics = Graphics::FromImage (&bmp);

	for (int i = 0; i < m_ilSystemImageList.GetImageCount () * 2; i++)
	{
		HICON hIcon = m_ilSystemImageList.ExtractIcon (i);
		Bitmap* pIcon = Bitmap::FromHICON (hIcon);
		pGraphics->DrawImage (pIcon, i * 32, 0, 32, 32);
		delete pIcon;
	}
	delete pGraphics;
	
	m_pBmpSystemImageList = bmp.Clone (0, 0, bmp.GetWidth (), bmp.GetHeight (), PixelFormat32bppARGB);
*/
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnStyletemplate() 
{
	CStyleTemplateWizard propSheet;
	propSheet.m_strCurrentStyleName = m_wndDlgBar.GetCurSel ();
	propSheet.DoModal();

	// update documents
	POSITION pos = AfxGetApp ()->GetFirstDocTemplatePosition ();
	CDocTemplate* pDocTmpl = AfxGetApp ()->GetNextDocTemplate (pos);

	pos = pDocTmpl->GetFirstDocPosition ();
	while (pos)
		((CCdCoverCreator2Doc*) pDocTmpl->GetNextDoc (pos))->UpdateStyle ();

	// update combo box with style of current active document
	CMDIChildWnd* pChild = MDIGetActive ();
	if (pChild != NULL)
	{
		CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
		if (pDoc != NULL)
		{
			ListStyles (pDoc->m_pStyle ? pDoc->m_pStyle->GetName () : "");
			m_wndDlgBarSelectMedium.SetMediumType (pDoc->m_pStyle->GetMediumType ());
		}
	}
}

void CMainFrame::OnFilePageSetup() 
{
	CPageSetupDlg dlg;
	CWinApp* pApp = AfxGetApp ();

	dlg.m_strCoverLabelType = pApp->GetProfileString ("Settings", "Cover");
	dlg.m_strCoverbackLabelType = pApp->GetProfileString ("Settings", "Coverback");
	dlg.m_strLabelLabelType = pApp->GetProfileString ("Settings", "Label");
	dlg.m_strInlayLabelType = pApp->GetProfileString ("Settings", "Inlay");
	dlg.m_rtRenderCoverbackWith = (RenderType) pApp->GetProfileInt ("Settings", "PrintCoverbackWith", -1);
	dlg.m_rtRenderInlayWith = (RenderType) pApp->GetProfileInt ("Settings", "PrintInlayWith", -1);
	dlg.m_dCoverMarginLeft = pApp->GetProfileInt ("Settings", "CoverMarginLeft", 0) / 1000.0;
	dlg.m_dCoverMarginTop = pApp->GetProfileInt ("Settings", "CoverMarginTop", 0) / 1000.0;
	dlg.m_dCoverBackMarginLeft = pApp->GetProfileInt ("Settings", "CoverbackMarginLeft", 0) / 1000.0;
	dlg.m_dCoverBackMarginTop = pApp->GetProfileInt ("Settings", "CoverbackMarginTop", 0) / 1000.0;
	dlg.m_dLabelMarginLeft = pApp->GetProfileInt ("Settings", "LabelMarginLeft", 0) / 1000.0;
	dlg.m_dLabelMarginTop = pApp->GetProfileInt ("Settings", "LabelMarginTop", 0) / 1000.0;
	dlg.m_dInlayMarginLeft = pApp->GetProfileInt ("Settings", "InlayMarginLeft", 0) / 1000.0;
	dlg.m_dInlayMarginTop = pApp->GetProfileInt ("Settings", "InlayMarginTop", 0) / 1000.0;
	dlg.m_dBookletMarginLeft = pApp->GetProfileInt ("Settings", "BookletMarginLeft", 0) / 1000.0;
	dlg.m_dBookletMarginTop = pApp->GetProfileInt ("Settings", "BookletMarginTop", 0) / 1000.0;
	dlg.m_nCoverPO = pApp->GetProfileInt ("Settings", "CoverPO", 0);
	dlg.m_nCoverbackPO = pApp->GetProfileInt ("Settings", "CoverbackPO", 1);
	dlg.m_nLabelPO = pApp->GetProfileInt ("Settings", "LabelPO", 0);
	dlg.m_nInlayPO = pApp->GetProfileInt ("Settings", "InlayPO", 0);
	dlg.m_nBookletPO = pApp->GetProfileInt ("Settings", "BookletPO", 1);
	dlg.m_b2Labels = pApp->GetProfileInt ("Settings", "2Labels", 0);
	dlg.m_nIncludeCutLines = pApp->GetProfileInt ("Settings", "CutLines", 0);

	if (dlg.DoModal () == IDOK)
	{
		pApp->WriteProfileString ("Settings", "Cover", dlg.m_strCoverLabelType);
		pApp->WriteProfileString ("Settings", "Coverback", dlg.m_strCoverbackLabelType);
		pApp->WriteProfileString ("Settings", "Label", dlg.m_strLabelLabelType);
		pApp->WriteProfileString ("Settings", "Inlay", dlg.m_strInlayLabelType);
		pApp->WriteProfileInt ("Settings", "PrintCoverbackWith", (int) dlg.m_rtRenderCoverbackWith);
		pApp->WriteProfileInt ("Settings", "PrintInlayWith", (int) dlg.m_rtRenderInlayWith);
		pApp->WriteProfileInt ("Settings", "CoverMarginLeft", (int) (dlg.m_dCoverMarginLeft * 1000));
		pApp->WriteProfileInt ("Settings", "CoverMarginTop", (int) (dlg.m_dCoverMarginTop * 1000));
		pApp->WriteProfileInt ("Settings", "CoverbackMarginLeft", (int) (dlg.m_dCoverBackMarginLeft * 1000));
		pApp->WriteProfileInt ("Settings", "CoverbackMarginTop", (int) (dlg.m_dCoverBackMarginTop * 1000));
		pApp->WriteProfileInt ("Settings", "LabelMarginLeft", (int) (dlg.m_dLabelMarginLeft * 1000));
		pApp->WriteProfileInt ("Settings", "LabelMarginTop", (int) (dlg.m_dLabelMarginTop * 1000));
		pApp->WriteProfileInt ("Settings", "InlayMarginLeft", (int) (dlg.m_dInlayMarginLeft * 1000));
		pApp->WriteProfileInt ("Settings", "InlayMarginTop", (int) (dlg.m_dInlayMarginTop * 1000));
		pApp->WriteProfileInt ("Settings", "BookletMarginLeft", (int) (dlg.m_dBookletMarginLeft * 1000));
		pApp->WriteProfileInt ("Settings", "BookletMarginTop", (int) (dlg.m_dBookletMarginTop * 1000));
		pApp->WriteProfileInt ("Settings", "CoverPO", dlg.m_nCoverPO);
		pApp->WriteProfileInt ("Settings", "CoverbackPO", dlg.m_nCoverbackPO);
		pApp->WriteProfileInt ("Settings", "LabelPO", dlg.m_nLabelPO);
		pApp->WriteProfileInt ("Settings", "InlayPO", dlg.m_nInlayPO);
		pApp->WriteProfileInt ("Settings", "BookletPO", dlg.m_nBookletPO);
		pApp->WriteProfileInt ("Settings", "2Labels", dlg.m_b2Labels);
		pApp->WriteProfileInt ("Settings", "CutLines", dlg.m_nIncludeCutLines);
	}
}

void CMainFrame::OnClose() 
{
	m_twain.ReleaseTwain ();	
	CMDIFrameWnd::OnClose();
}

//BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
//{
//	m_twain.ProcessMessage (*pMsg);	
//	return CMDIFrameWnd::PreTranslateMessage(pMsg);
//}

CWebCDCovers* CMainFrame::GetWebCDCovers()
{
	if (m_pWebCdCovers == NULL)
		m_pWebCdCovers = new CWebCDCovers ();

	return m_pWebCdCovers;
}

void CMainFrame::OnDropFiles(HDROP hDropInfo) 
{
	POINT pt;
	DragQueryPoint (hDropInfo, &pt);
	ClientToScreen (&pt);

	RECT r;
	for (HWND hwndChild = ::GetWindow (m_hWndMDIClient, GW_CHILD); hwndChild != NULL; hwndChild = ::GetWindow (hwndChild, GW_HWNDNEXT))
	{
		::GetWindowRect (hwndChild, &r);
		if (::PtInRect (&r, pt))
		{
			// dropped on a view
			// if this is an image, set the bk image
			char szFile[MAX_PATH + 1];
			DragQueryFile (hDropInfo, 0, szFile, MAX_PATH);
			LPSTR lpszExtension = strrchr (szFile, '.') + 1;
			if ((stricmp (lpszExtension, "jpg") == 0) ||
				(stricmp (lpszExtension, "jpeg") == 0) ||
				(stricmp (lpszExtension, "gif") == 0) ||
				(stricmp (lpszExtension, "bmp") == 0))
			{
				CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) ((CMDIChildWnd*) CWnd::FromHandle (hwndChild))->GetActiveDocument ();
				if (pDoc != NULL)
				{
					CString strFile = szFile;
					
					CString strPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetAlphaTextureDir ();
					if (strFile.Left (strPath.GetLength ()) == strPath)
					{
						// this is an alpha texture
						pDoc->SetBkAlphaFilename (strFile);
						return;
					}

					POSITION pos = pDoc->GetFirstViewPosition ();
					CCdCoverCreator2View* pView = (CCdCoverCreator2View*)  pDoc->GetNextView (pos);

					strPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetTextureDir ();
					if (strFile.Left (strPath.GetLength ()) == strPath)
					{
						// this is a texture: set "tile"
						pDoc->m_pBackground[pView->GetRenderType ()]->SetAlignmentMode (BIAM_TILE);
					}
					else
					{
						// if background is set to "tiled", set to center-center
						if (pDoc->m_pBackground[pView->GetRenderType ()]->GetAlignmentMode () == BIAM_TILE)
							pDoc->m_pBackground[pView->GetRenderType ()]->SetAlignmentMode (BIAM_CENTERCENTER);
					}

					pDoc->SetBkImageFilename (szFile);
				}
				return;
			}
		}
	}
	
	CMDIFrameWnd::OnDropFiles(hDropInfo);
}

BOOL CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (((LPNMHDR) lParam)->code == TBN_DROPDOWN)
	{
		LPNMTOOLBAR pNmTb = (LPNMTOOLBAR) lParam;

		switch (pNmTb->iItem)
		{
		case ID_FILE_NEW:
			{
				RECT r;
				m_wndToolBar.GetItemRect (m_wndToolBar.CommandToIndex (pNmTb->iItem), &r);
				m_wndToolBar.ClientToScreen (&r);

				m_wndMenuBar.TrackPopup (0, 0, CPoint (r.left, r.bottom));
			}
			return false;

		case ID_FILE_OPEN:
			{
				RECT r;
				m_wndToolBar.GetItemRect (m_wndToolBar.CommandToIndex (pNmTb->iItem), &r);
				m_wndToolBar.ClientToScreen (&r);

				CMenu menu;
				menu.LoadMenu (IDR_MENU_OPEN);
				CMenu* pPopup = menu.GetSubMenu (0);
				UINT nIdx = 2;
				UINT nId = ID_FILE_MRU_FILE1;
				((CCdCoverCreator2App*) AfxGetApp ())->FillRecentFileMenu (pPopup, nIdx, nId);
				pPopup->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, r.left, r.bottom, this);
			}
			return false;

		case ID_RTFF_BULLETS:
			m_wndRTFToolbar.OnBulletsArrow ();
			return false;

		case ID_RTFF_BORDER:
			m_wndRTFToolbar.OnBorderArrow ();
			return false;
		}
	}
	
	return CMDIFrameWnd::OnNotify(wParam, lParam, pResult);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	if (!m_bInitPosition && !m_bTimerSet)
	{
		m_pWndRollup->CalcRelPosition ();
		m_nTimerId = SetTimer (2, 100, NULL);
		m_bTimerSet = true;
	}

	// call base class method
	CMDIFrameWnd::OnSize(nType, cx, cy);

	CRect rectMDIClient;
	::GetWindowRect (m_hWndMDIClient, &rectMDIClient);

	CRect r;
	if (m_bInitPosition)
	{
		// set the initial position
		CPoint ptTopRight (rectMDIClient.right, rectMDIClient.top);

		m_pWndRollup->GetWindowRect (&r);
		m_pWndRollup->SetWindowPos (NULL, ptTopRight.x - r.Width () - 5, ptTopRight.y + 5, 0, 0,
			SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW);
		m_pWndRollup->CalcRelPosition ();

		m_bInitPosition = false;
	}

	m_dwLastWindowPosChange = GetTickCount ();

	m_wndStatusBar.GetItemRect (2, &r);
	m_wndProgress.MoveWindow (&r);
}

void CMainFrame::OnMove(int x, int y) 
{
	if (!m_bTimerSet)
	{
		m_pWndRollup->CalcRelPosition ();
		m_nTimerId = SetTimer (2, 100, NULL);
		m_bTimerSet = true;
	}

	CMDIFrameWnd::OnMove(x, y);	

	m_dwLastWindowPosChange = GetTickCount ();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	if (GetTickCount () - m_dwLastWindowPosChange > 100)
	{
		KillTimer (m_nTimerId);
		m_bTimerSet = false;

		m_pWndRollup->Reposition ();
	}
	
	CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnViewCoverbrowser() 
{
	CWebCdCoversBrowser* pCb = (CWebCdCoversBrowser*)
		CreateNewChild (RUNTIME_CLASS (CWebCdCoversBrowser), IDR_COVERBROWSER);

	// switch to the desired page (any caller of this method can set the
	// m_bmCoverBrowserMode member variable to the needed value)
	pCb->SetMode (m_bmCoverBrowserMode);

	// reset to standard
	m_bmCoverBrowserMode = BrowserSearch;

	m_pActivateCoverBrowser = pCb;
}

void CMainFrame::OnViewLyricsbrowser() 
{
	CreateNewChild (RUNTIME_CLASS (CLyricsBrowser), IDR_LYRICSBROWSER);
}

void CMainFrame::OnViewStyles() 
{
	m_pWndRollup->ShowWindow (m_pWndRollup->IsWindowVisible () ? SW_HIDE : SW_SHOW);
}

void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_pWndRollup->IsWindowVisible ());	
}

void CMainFrame::OnViewLeftpanel() 
{
	ShowControlBar (&m_wndLeftPanel, !m_wndLeftPanel.IsWindowVisible (), FALSE);
}

void CMainFrame::OnUpdateViewLeftpanel(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndLeftPanel.IsWindowVisible ());	
}

void CMainFrame::OnViewTextToolbar ()
{
	ShowControlBar (&m_wndRTFToolbar, !m_wndRTFToolbar.IsWindowVisible (), FALSE);
}

void CMainFrame::OnUpdateViewTextToolbar (CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck (m_wndRTFToolbar.IsWindowVisible ());	
}

void CMainFrame::UpdateStyleDlgs(CStyleTemplate* pStyle, CBackgroundInfo* pBk, RenderType rt)
{
	m_wndStyleMeasures.SetTemplate (pStyle, rt);
	m_wndStyleTrackgroupings.SetTemplate (pStyle, rt);
	m_wndStyleTracks.SetTemplate (pStyle);
	m_wndStyleSpines.SetTemplate (pStyle, rt);
	m_wndStyleFonts.SetTemplate (pStyle, rt);
	m_wndStyleGradient.SetTemplate (pStyle, rt);
	m_wndStyleBkImg.SetBackground (pBk, rt);
//	m_wndStyleAlpha.SetBackgroundAlpha (pBk == NULL ? NULL : pBk->m_pAlphaTexture);
	m_wndLeftPanel.Update ();
}

void CMainFrame::OnRtffBold() 
{
	m_wndRTFToolbar.OnBold ();	
}

void CMainFrame::OnRtffItalic() 
{
	m_wndRTFToolbar.OnItalic ();
}

void CMainFrame::OnRtffUnderline() 
{
	m_wndRTFToolbar.OnUnderline ();
}

void CMainFrame::OnRtffUnderlineDot() 
{
	m_wndRTFToolbar.OnUnderlineDot ();
}
/*
void CMainFrame::OnRtffUnderlineDouble() 
{
	m_wndRTFToolbar.OnUnderlineDouble ();	
}*/

void CMainFrame::OnRtffStrikethru() 
{
	m_wndRTFToolbar.OnStrikethru ();
}

void CMainFrame::OnRtffAlignleft() 
{
	m_wndRTFToolbar.OnAlignleft ();
}

void CMainFrame::OnRtffAligncenter() 
{
	m_wndRTFToolbar.OnAligncenter ();	
}

void CMainFrame::OnRtffAlignright() 
{
	m_wndRTFToolbar.OnAlignright ();
}

void CMainFrame::OnRtffSup() 
{
	m_wndRTFToolbar.OnSuperScript ();
}

void CMainFrame::OnRtffSub() 
{
	m_wndRTFToolbar.OnSubScript ();	
}

void CMainFrame::OnRtffBullets() 
{
	m_wndRTFToolbar.OnBullets ();	
}

void CMainFrame::OnRtffBorder() 
{
	m_wndRTFToolbar.OnBorder ();	
}

void CMainFrame::OnRtffPagebreak() 
{
	m_wndRTFToolbar.OnPagebreak ();	
}

void CMainFrame::OnEditUndo() 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		m_wndRTFToolbar.GetRTFControl ()->Undo ();
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				if (pView->m_wndTrackEdit.IsWindowVisible ())
					pView->m_wndTrackEdit.Undo ();
				else
				{
					pView->GetDocument ()->Undo ();
					pView->GetDocument ()->UpdateAllViews (NULL);
				}
			}
		}
	}
}

void CMainFrame::OnEditRedo() 
{
	CMDIChildWnd* pChild = MDIGetActive ();
	if (pChild != NULL)
	{
		CView* pAView = pChild->GetActiveView ();
		if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
		{
			CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
			pView->GetDocument ()->Redo ();
			pView->GetDocument ()->UpdateAllViews (NULL);
		}
	}
	//m_wndRTFToolbar.GetRTFControl ()->???	
}

void CMainFrame::OnEditCut() 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		m_wndRTFToolbar.GetRTFControl ()->Cut ();
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				pView->m_wndTrackEdit.Cut ();
			}
		}
	}
}

void CMainFrame::OnEditCopy() 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		m_wndRTFToolbar.GetRTFControl ()->Copy ();
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				pView->m_wndTrackEdit.Copy ();
			}
		}
	}
}

void CMainFrame::OnEditPaste() 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		m_wndRTFToolbar.GetRTFControl ()->Paste ();
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				pView->m_wndTrackEdit.Paste ();
			}
		}
	}
}

void CMainFrame::OnEditClear() 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		m_wndRTFToolbar.GetRTFControl ()->Clear ();
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				pView->m_wndTrackEdit.Clear ();
			}
		}
	}
}

void CMainFrame::OnEditSelectAll() 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		m_wndRTFToolbar.GetRTFControl ()->SetSel (0, -1);
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				pView->m_wndTrackEdit.SetSel (0, -1);
			}
		}
	}
}

void CMainFrame::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		pCmdUI->Enable (m_wndRTFToolbar.GetRTFControl ()->CanUndo ());
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				if (pView->m_wndTrackEdit.IsWindowVisible ())
					pCmdUI->Enable (pView->m_wndTrackEdit.CanUndo ());
				else
					pCmdUI->Enable (pView->GetDocument ()->CanUndo ());
			}
			else
				pCmdUI->Enable (false);
		}
		else
			pCmdUI->Enable (false);
	}
}

void CMainFrame::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		pCmdUI->Enable (true);
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				pCmdUI->Enable (pView->GetDocument ()->CanRedo ());
			}
			else
				pCmdUI->Enable (false);
		}
		else
			pCmdUI->Enable (false);
	}
}

void CMainFrame::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
	{
		long lSelStart, lSelEnd;
		m_wndRTFToolbar.GetRTFControl ()->GetSel (lSelStart, lSelEnd);
		pCmdUI->Enable (lSelStart != lSelEnd);
	}
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				if (pView->m_wndTrackEdit.IsWindowVisible ())
				{
					long lSelStart, lSelEnd;
					pView->m_wndTrackEdit.GetSel (lSelStart, lSelEnd);
					pCmdUI->Enable (lSelStart != lSelEnd);
				}
				else
					pCmdUI->Enable (false);
			}
			else
				pCmdUI->Enable (false);
		}
		else
			pCmdUI->Enable (false);
	}
}

void CMainFrame::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
	{
		long lSelStart, lSelEnd;
		m_wndRTFToolbar.GetRTFControl ()->GetSel (lSelStart, lSelEnd);
		pCmdUI->Enable (lSelStart != lSelEnd);
	}
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				if (pView->m_wndTrackEdit.IsWindowVisible ())
				{
					long lSelStart, lSelEnd;
					pView->m_wndTrackEdit.GetSel (lSelStart, lSelEnd);
					pCmdUI->Enable (lSelStart != lSelEnd);
				}
				else
					pCmdUI->Enable (false);
			}
			else
				pCmdUI->Enable (false);
		}
		else
			pCmdUI->Enable (false);
	}
}

void CMainFrame::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
		pCmdUI->Enable (m_wndRTFToolbar.GetRTFControl ()->CanPaste ());
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				if (pView->m_wndTrackEdit.IsWindowVisible ())
					pCmdUI->Enable (pView->m_wndTrackEdit.CanPaste ());
				else
					pCmdUI->Enable (false);
			}
			else
				pCmdUI->Enable (false);
		}
		else
			pCmdUI->Enable (false);
	}
}

void CMainFrame::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	if (m_wndRTFToolbar.GetRTFControl () != NULL)
	{
		long lSelStart, lSelEnd;
		m_wndRTFToolbar.GetRTFControl ()->GetSel (lSelStart, lSelEnd);
		pCmdUI->Enable (lSelStart != lSelEnd);
	}
	else
	{
		CMDIChildWnd* pChild = MDIGetActive ();
		if (pChild != NULL)
		{
			CView* pAView = pChild->GetActiveView ();
			if (pAView != NULL && pAView->IsKindOf (RUNTIME_CLASS (CCdCoverCreator2View)))
			{
				CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pAView;
				if (pView->m_wndTrackEdit.IsWindowVisible ())
				{
					long lSelStart, lSelEnd;
					pView->m_wndTrackEdit.GetSel (lSelStart, lSelEnd);
					pCmdUI->Enable (lSelStart != lSelEnd);
				}
				else
					pCmdUI->Enable (false);
			}
			else
				pCmdUI->Enable (false);
		}
		else
			pCmdUI->Enable (false);
	}
}

void CMainFrame::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_wndRTFToolbar.GetRTFControl () != NULL);	
}

void CMainFrame::OnRtffBorderStyle (UINT nId)
{
	m_wndRTFToolbar.OnBorderStyle (nId);
}

void CMainFrame::OnRtffBulletsStyle (UINT nId)
{
	m_wndRTFToolbar.OnBulletsStyle (nId);
}

void CMainFrame::OnWindowNextwindow()
{
	MDINext ();
}

void CMainFrame::OnWindowPreviouswindow()
{
	MDIPrev ();
}

void CMainFrame::OnUpdateWindowNextwindow(CCmdUI *pCmdUI)
{
	BOOL b;
	pCmdUI->Enable (MDIGetActive (&b) != NULL);
}

void CMainFrame::OnUpdateWindowPreviouswindow(CCmdUI *pCmdUI)
{
	BOOL b;
	pCmdUI->Enable (MDIGetActive (&b) != NULL);
}
