// WebCdCoversBrowser1.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "MainFrm.h"
#include "CdCoverCreator2Doc.h"
#include "CdCoverCreator2View.h"

#include "WebCdCoversBrowser1.h"
#include "wcc_data.h"

#include "HttpReqSocket.h"
#include "HttpRequest.h"
#include "mshtml.h"
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWebCdCoversBrowser

IMPLEMENT_DYNCREATE(CWebCdCoversBrowser, CMDIChildWnd)

CWebCdCoversBrowser::CWebCdCoversBrowser()
{
	// select font
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof (NONCLIENTMETRICS);
	SystemParametersInfo (SPI_GETNONCLIENTMETRICS, sizeof (NONCLIENTMETRICS), &ncm, 0);

	m_font.CreateFontIndirect (&ncm.lfMessageFont);

	// Initialize hosts
	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapCoverHosts;
	RESOURCEHOST* pHost;
	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
	{
		pHost = (*it).second;
		pHost->hInst = LoadLibrary (pHost->strLibFilename);
	}

	m_nError = 0;
	m_htiExpand = NULL;
	m_nCancelDownloadPtr = 0;

	m_dwCurId = (DWORD) -1;
	m_dwPrevId = (DWORD) -1;
	m_dwDownloadItemId = (DWORD) -1;

	m_mode = BrowserSearch;
	m_nLeftPaneWidth = 250;

	m_pCurrentDoc = NULL;

	m_hCurSize = AfxGetApp ()->LoadStandardCursor (IDC_SIZEWE);
}

CWebCdCoversBrowser::~CWebCdCoversBrowser()
{
	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapCoverHosts;
	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
		FreeLibrary ((*it).second->hInst);

	::DestroyIcon (m_hIconFolder16);
	::DestroyIcon (m_hIconFolder32);
	::DestroyIcon (m_hIconWebCover);
	::DestroyIcon (m_hIconNewLink);
	::DestroyIcon (m_hIconDelLink);

	m_imgListSmall.DeleteImageList ();
	m_font.DeleteObject ();
}


BEGIN_MESSAGE_MAP(CWebCdCoversBrowser, CMDIChildWnd)
	//{{AFX_MSG_MAP(CWebCdCoversBrowser)
	ON_WM_CREATE()
	ON_COMMAND(ID_BROWSE_LIST, OnBrowseList)
	ON_UPDATE_COMMAND_UI(ID_BROWSE_LIST, OnUpdateBrowseList)
	ON_COMMAND(ID_BROWSE_APPLY, OnBrowseApply)
	ON_COMMAND(ID_BROWSE_SEARCH, OnBrowseSearch)
	ON_UPDATE_COMMAND_UI(ID_BROWSE_SEARCH, OnUpdateBrowseSearch)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_BROWSER, OnItemExpandingTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_BROWSER, OnSelChangedTree)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BROWSER, OnItemChangedList)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_BROWSER, OnDblClkTree)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_BROWSER, OnDblClkList)
	ON_WM_SIZE()
	ON_COMMAND(IDC_BUTTON_SEARCH, OnSearch)
	ON_WM_TIMER()
	ON_COMMAND(ID_BROWSE_IMAGE, OnBrowseImage)
	ON_UPDATE_COMMAND_UI(ID_BROWSE_IMAGE, OnUpdateBrowseImage)
	ON_COMMAND(ID_BROWSE_BKTEXTURE, OnBrowseBktexture)
	ON_UPDATE_COMMAND_UI(ID_BROWSE_BKTEXTURE, OnUpdateBrowseBktexture)
	ON_COMMAND(ID_BROWSE_BKALPHA, OnBrowseBkalpha)
	ON_UPDATE_COMMAND_UI(ID_BROWSE_BKALPHA, OnUpdateBrowseBkalpha)
	ON_COMMAND(ID_BROWSE_WEB, OnBrowseWeb)
	ON_UPDATE_COMMAND_UI(ID_BROWSE_WEB, OnUpdateBrowseWeb)
	ON_WM_SETCURSOR()
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_WEB, OnEndLabelEditListWeb)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WEB, OnSelChangedListWeb)
	ON_COMMAND(IDC_BUTTON_NEWLINK, OnNewLink)
	ON_COMMAND(IDC_BUTTON_DELLINK, OnDelLink)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(ID_BROWSE_APPLY, OnUpdateBrowseApply)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_IMAGES, OnItemexpandingTreeImages)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_IMAGES, OnSelchangedTreeImages)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_IMAGES, OnRclickTreeImages)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BKALPHAS, OnItemChangedBkAlphas)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BKTEXTURES, OnItemChangedBkTextures)
	ON_WM_ACTIVATE()
	ON_MESSAGE(SPLIT_RESIZE, OnSplitResize)
	ON_MESSAGE(WCCM_LISTCOVERS, OnGetList)
	ON_MESSAGE(WCCM_SEARCHCOVERS, OnGetSearch)
	ON_MESSAGE(WCCM_GETIMAGES, OnGetImages)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebCdCoversBrowser message handlers


///////////////////////////////////////////////////////////////////
// General Window Message handlers
//

static UINT indicators[] =
{
	IDC_PROGRESS_BROWSER,
	IDC_PROGRESS_BROWSER,
	IDS_DOWNLOADSTATUS_KB,
	IDS_DOWNLOADSTATUS_PERCENT,
	IDS_DOWNLOADSTATUS_KBS
};

int CWebCdCoversBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create (this, AFX_IDW_TOOLBAR) ||
		!m_wndToolBar.LoadToolBar (IDR_TOOLBAR_BROWSE, AILS_NEW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strLabel;
	for (int i = 0; i < /*8*/ /*7*/ 6; i++)
	{
		strLabel.LoadString (/*IDS_BROWSER_LIST*/ IDS_BROWSER_SEARCH + i);
		m_wndToolBar.SetButtonText (i, strLabel);
	}
	m_wndToolBar.SetSizes (CSize (42, 22), CSize (16, 16));
	m_wndToolBar.ModifyStyle (0, TBSTYLE_LIST);

	if (!m_wndReBar.Create (this) ||
		!m_wndReBar.AddBar (&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CRect r (10, 10, 10, 10);

	// Left pane
	m_wndSplitter.Create ("", WS_CHILD | WS_VISIBLE | SS_NOTIFY, r, this, IDC_SPLITTER);

/*
	// List
	m_wndTreeList.Create (
		WS_VISIBLE | WS_TABSTOP | WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP | TVS_SHOWSELALWAYS,
		r, this, IDC_TREE_BROWSER);
*/

	// Search
	m_wndAnimation.Create (WS_CHILD | ACS_TRANSPARENT, r, this, IDC_ANIMATION_BROWSER);
	m_wndListSearch.Create (WS_CHILD | LVS_LIST | LVS_SORTASCENDING | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
		r, this, IDC_LIST_BROWSER);
	m_wndEdit.Create (WS_CHILD | WS_TABSTOP, r, this, IDC_EDIT_BROWSER);

	CString strCaption;
	strCaption.LoadString (IDS_BUTTON_SEARCH);
	m_wndBtnSearch.Create (strCaption, WS_CHILD | WS_TABSTOP, r, this, IDC_BUTTON_SEARCH);

	strCaption.LoadString (IDS_SEARCHWEBCDCOVERS);
	m_wndStaticSearch.Create (strCaption, WS_CHILD, r, this);

	// Images
	m_wndTreeImages.Create (
		WS_TABSTOP | WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP | TVS_SHOWSELALWAYS,
		r, this, IDC_TREE_IMAGES);

	// Background Textures
	m_wndListBkTextures.Create (WS_CHILD | LVS_ICON | LVS_SORTASCENDING | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
		r, this, IDC_LIST_BKTEXTURES);

	// Alpha Textures
//	m_wndListBkAlpha.Create (WS_CHILD | LVS_ICON | LVS_SORTASCENDING | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
//		r, this, IDC_LIST_BKALPHAS);

	// Web
	m_wndListWeb.Create (WS_CHILD | LVS_REPORT | LVS_SORTASCENDING | LVS_SHOWSELALWAYS | LVS_SINGLESEL | LVS_EDITLABELS,
		r, this, IDC_LIST_WEB);
	m_wndBtnNewLink.Create ("", WS_CHILD | WS_TABSTOP, r, this, IDC_BUTTON_NEWLINK);
	m_wndBtnDelLink.Create ("", WS_CHILD | WS_TABSTOP, r, this, IDC_BUTTON_DELLINK);

	// Right pane: WebBrowser Control
	if (!m_wndBrowser.CreateControl (CLSID_WebBrowser, "Browser", WS_CHILD | WS_VISIBLE, r, this, AFX_IDW_PANE_FIRST))
	{
		DestroyWindow();
		return FALSE;
	}

	LPUNKNOWN lpUnk = m_wndBrowser.GetControlUnknown ();
	HRESULT hr = lpUnk->QueryInterface (IID_IWebBrowser2, (void**) &m_pBrowserApp);
	if (!SUCCEEDED (hr))
	{
		m_pBrowserApp = NULL;
		m_wndBrowser.DestroyWindow ();
		DestroyWindow ();
		return -1;
	}

	// Progress control
	m_wndStatusBar.GetItemRect (0, &r);
	m_wndProgress.Create (WS_CHILD | PBS_SMOOTH, r, &m_wndStatusBar, 1);

	Init ();

	return 0;
}

void CWebCdCoversBrowser::OnDestroy() 
{
	KillTimer (m_nTimerId);
	SaveToCache ();
	SaveWebListEntries ();
	CMDIChildWnd::OnDestroy();
}

BOOL CWebCdCoversBrowser::PreCreateWindow(CREATESTRUCT& cs) 
{
	CMDIChildWnd::PreCreateWindow(cs);
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

void CWebCdCoversBrowser::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	Resize ();
}

void CWebCdCoversBrowser::Resize ()
{
	int nTop, nHeight;
	CRect r;

	GetClientRect (&r);
	int cx = r.Width ();
	int cy = r.Height ();

	m_wndToolBar.GetWindowRect (&r);
	nTop = r.Height () + 2;
	m_wndStatusBar.GetWindowRect (&r);
	nHeight = cy - nTop - r.Height ();

	m_wndSplitter.SetWindowPos (NULL, m_nLeftPaneWidth - 1, nTop, 3, nHeight, SWP_NOZORDER);

	// List
//	m_wndTreeList.SetWindowPos (NULL, 0, nTop, m_nLeftPaneWidth, nHeight, SWP_NOZORDER);

	// Search
	m_wndListSearch.SetWindowPos (NULL, 0, nTop + 94, m_nLeftPaneWidth, nHeight - 94, SWP_NOZORDER);
	m_wndEdit.SetWindowPos (NULL, 10, nTop + 32, m_nLeftPaneWidth - 20, 20, SWP_NOZORDER);
	m_wndAnimation.SetWindowPos (NULL, 10, nTop + 10, 16, 16, SWP_NOZORDER);
	m_wndBtnSearch.SetWindowPos (NULL, 10, nTop + 60, 100, 24, SWP_NOZORDER);
	m_wndStaticSearch.SetWindowPos (NULL, 32, nTop + 10, m_nLeftPaneWidth - 40, 16, SWP_NOZORDER);

	// Images
	m_wndTreeImages.SetWindowPos (NULL, 0, nTop, m_nLeftPaneWidth, nHeight, SWP_NOZORDER);

	// Background Textures
	m_wndListBkTextures.SetWindowPos (NULL, 0, nTop, m_nLeftPaneWidth, nHeight, SWP_NOZORDER);

	// Alpha Textures
//	m_wndListBkAlpha.SetWindowPos (NULL, 0, nTop, m_nLeftPaneWidth, nHeight, SWP_NOZORDER);

	// Web
	m_wndListWeb.SetWindowPos (NULL, 0, nTop + 23, m_nLeftPaneWidth, nHeight - 23, SWP_NOZORDER);
	m_wndBtnNewLink.SetWindowPos (NULL, 1, nTop + 1, 20, 20, SWP_NOZORDER);
	m_wndBtnDelLink.SetWindowPos (NULL, 22, nTop + 1, 20, 20, SWP_NOZORDER);

	// Right pane, status bar
	m_wndBrowser.SetWindowPos (NULL, m_nLeftPaneWidth + 1, nTop, cx - m_nLeftPaneWidth - 1, nHeight, SWP_NOZORDER);

	m_wndStatusBar.GetItemRect (0, &r);
	m_wndProgress.SetWindowPos (&wndTop, r.left, r.top, r.Width (), r.Height (), 0);
}

LRESULT CWebCdCoversBrowser::OnSplitResize (WPARAM wParam, LPARAM lParam)
{
	m_nLeftPaneWidth += (int) wParam;
	Resize ();
	return S_OK;
}

BOOL CWebCdCoversBrowser::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (pWnd->GetDlgCtrlID () == IDC_SPLITTER)
	{
		SetCursor (m_hCurSize);
		return true;
	}
	
	return CMDIChildWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CWebCdCoversBrowser::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CMDIChildWnd::OnActivate(nState, pWndOther, bMinimized);
	((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (NULL, NULL, (RenderType) -1);	
}

void CWebCdCoversBrowser::Init()
{
	// set styles / fonts
	CWinApp* pApp = AfxGetApp ();

	m_imgListSmall.Create (16, 16, ILC_COLOR8 | ILC_MASK, 2, 1);
	m_imgListSmall.Add (m_hIconFolder16 = pApp->LoadIcon (IDI_ICON_FOLDER));
	m_imgListSmall.Add (m_hIconWebCover = pApp->LoadIcon (IDI_ICON_WEBCOVER));

	m_imgListNormal.Create (32, 32, ILC_COLOR8 | ILC_MASK, 1, 1);
	m_imgListNormal.Add (m_hIconFolder32 = pApp->LoadIcon (IDI_ICON_FOLDER));

/*
	// List
	SetWindowLong (m_wndTreeList.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndTreeList.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);

	TVINSERTSTRUCT insert;

	insert.hParent = TVI_ROOT;
	insert.hInsertAfter = TVI_LAST;
	insert.item.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	insert.item.cChildren = 1;
	insert.item.iImage = 0;
	insert.item.iSelectedImage = 0;

	for (char szLetter[2] = "A"; szLetter[0] <= 'Z'; szLetter[0]++)
	{
		insert.item.pszText = szLetter;
		m_wndTreeList.InsertItem (&insert);
	}
	m_wndTreeList.SetImageList (&m_imgListSmall, TVSIL_NORMAL);
*/

	// Search
	SetWindowLong (m_wndListSearch.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndListSearch.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	SetWindowLong (m_wndEdit.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndEdit.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	m_wndEdit.SetFont (&m_font);
	m_wndBtnSearch.SetFont (&m_font);
	m_wndStaticSearch.SetFont (&m_font);
	m_wndAnimation.Open (IDR_AVIFIND);
	m_wndListSearch.SetImageList (&m_imgListSmall, LVSIL_SMALL);

	// Images
	SetWindowLong (m_wndTreeImages.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndTreeImages.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
//	m_wndTreeImages.SetImageList (&((CMainFrame*) AfxGetMainWnd ())->m_ilSystemImageList, TVSIL_NORMAL);
	m_wndTreeImages.EnableImages ();
	m_wndTreeImages.PopulateTree ();

	// Background Textures
	SetWindowLong (m_wndListBkTextures.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndListBkTextures.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	m_wndListBkTextures.SetImageList (&m_imgListNormal, LVSIL_NORMAL);
	FillTexturesList ();

	// Background Alphas
/*	SetWindowLong (m_wndListBkAlpha.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndListBkAlpha.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	m_wndListBkAlpha.SetImageList (&m_imgListNormal, LVSIL_NORMAL);
	FillAlphaList ();
*/
	// Web
	CString s;
	s.LoadString (IDS_WEBADDRESS);
	m_wndListWeb.InsertColumn (0, s, LVCFMT_LEFT, m_nLeftPaneWidth);
	SetWindowLong (m_wndListWeb.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndListWeb.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	ListView_SetExtendedListViewStyleEx (m_wndListWeb.GetSafeHwnd (), LVS_EX_TRACKSELECT, LVS_EX_TRACKSELECT);
	m_wndBtnNewLink.SetIcon (m_hIconNewLink = pApp->LoadIcon (IDI_ICON_NEWLINK));
	m_wndBtnDelLink.SetIcon (m_hIconDelLink = pApp->LoadIcon (IDI_ICON_DELLINK));
	LoadWebListEntries ();

	// Browser
	//Navigate2Empty ();

	for (int i = 1; i <= 4; i++)
		m_wndStatusBar.SetPaneText (i, "");

	m_nTimerId = SetTimer (1, 500, NULL);

	DisplayControls ();
}

void CWebCdCoversBrowser::OnTimer(UINT nIDEvent) 
{
	// start downloads only after a certain amount of time

	if ((m_dwPrevId == m_dwCurId) && (m_dwDownloadItemId != m_dwCurId))
	{
		// time has elapsed. start download
		m_dwDownloadItemId = m_dwCurId;

		// stop previous downloads
		for (int i = 0; i < 128; i++)
			m_arrbCancelDownload[i] = true;

		// start download threads
		/*if (m_wndTreeList.IsWindowVisible ())
		{
			// List Mode
			COVERIMAGES* pCoverImages = (COVERIMAGES*) m_wndTreeList.GetItemData ((HTREEITEM) m_dwCurId);
			if (pCoverImages != NULL)
				DownloadImages (pCoverImages, m_wndTreeList.GetItemText ((HTREEITEM) m_dwCurId),
					m_wndTreeList.GetItemText (m_wndTreeList.GetParentItem ((HTREEITEM) m_dwCurId)));
		}
		else */
		if (m_wndListSearch.IsWindowVisible ())
		{
			// Search Mode
			POSITION pos = m_wndListSearch.GetFirstSelectedItemPosition ();
			int nSel = m_wndListSearch.GetNextSelectedItem (pos);
			if (nSel > -1)
			{
				COVERIMAGES* pCoverImages = (COVERIMAGES*) m_wndListSearch.GetItemData (nSel);
				CString strName = m_wndListSearch.GetItemText (nSel, 0);
				DownloadImages (pCoverImages, strName, strName.Left (1));
			}
		}
	}

	m_dwPrevId = m_dwCurId;
	
	CMDIChildWnd::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////
// Toolbar commands
//

void CWebCdCoversBrowser::OnBrowseList()
{
	m_mode = BrowserList;
	DisplayControls ();
	Navigate2Empty ();
}

void CWebCdCoversBrowser::OnUpdateBrowseList(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (m_mode == BrowserList);	
}

void CWebCdCoversBrowser::OnBrowseSearch() 
{
	m_mode = BrowserSearch;
	DisplayControls ();
	Navigate2Empty ();
	m_wndEdit.SetFocus ();
}

void CWebCdCoversBrowser::OnUpdateBrowseSearch(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (m_mode == BrowserSearch);
}


void CWebCdCoversBrowser::OnBrowseImage() 
{
	m_mode = BrowserImages;
	DisplayControls ();
	Navigate2Empty ();
}

void CWebCdCoversBrowser::OnUpdateBrowseImage(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (m_mode == BrowserImages);
}

void CWebCdCoversBrowser::OnBrowseBktexture() 
{
	m_mode = BrowserBkTextures;
	DisplayControls ();
	Navigate2Empty ();
}

void CWebCdCoversBrowser::OnUpdateBrowseBktexture(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (m_mode == BrowserBkTextures);
}

void CWebCdCoversBrowser::OnBrowseBkalpha() 
{
	m_mode = BrowserBkAlphas;
	DisplayControls ();
	Navigate2Empty ();
}

void CWebCdCoversBrowser::OnUpdateBrowseBkalpha(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (m_mode == BrowserBkAlphas);
}

void CWebCdCoversBrowser::OnBrowseWeb() 
{
	m_mode = BrowserWeb;
	DisplayControls ();
	Navigate2Empty ();
}

void CWebCdCoversBrowser::OnUpdateBrowseWeb(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (m_mode == BrowserWeb);
}

void CWebCdCoversBrowser::OnBrowseApply() 
{
	if (((CMainFrame*) AfxGetMainWnd ())->m_wndLeftPanel.IsLastDocValid ())
	{
		CCdCoverCreator2Doc* pDoc = ((CMainFrame*) AfxGetMainWnd ())->m_wndLeftPanel.m_pLastDoc;
		COVERIMAGES* pCoverImages;
		CString strTitle;

		switch (m_mode)
		{
		case BrowserList:
			{
				HTREEITEM htiSel = m_wndTreeList.GetSelectedItem ();
				if (htiSel == NULL)
					return;

				pCoverImages = (COVERIMAGES*) m_wndTreeList.GetItemData (htiSel);
				if (pCoverImages == NULL)
					return;
				strTitle = m_wndTreeList.GetItemText (htiSel);
			}
			break;

		case BrowserSearch:
			{
				POSITION pos = m_wndListSearch.GetFirstSelectedItemPosition ();
				if (pos == NULL)
					return;
				int nSel = m_wndListSearch.GetNextSelectedItem (pos);
				if (nSel == -1)
					return;

				pCoverImages = (COVERIMAGES*) m_wndListSearch.GetItemData (nSel);
				if (pCoverImages == NULL)
					return;
				strTitle = m_wndListSearch.GetItemText (nSel, 0);
			}
			break;
		}

		// search first non-empty file name
		CString strFilename;
		for (int i = Cover; i <= Label; i++)
			if (!pCoverImages->strLocalFile[i].IsEmpty ())
			{
				strFilename = pCoverImages->strLocalFile[i];
				break;
			}

		// set file names and set empty entries to the above retrieved file name
		for (int i = Cover; i <= Label; i++)
		{
			pDoc->m_pBackground[i]->SetAlignmentMode (BIAM_STRETCH);
			pDoc->m_pBackground[i]->SetImage (
				pCoverImages->strLocalFile[i].IsEmpty () ? strFilename : pCoverImages->strLocalFile[i]);
		}

		// set document title
		strTitle.Replace ("\"", "-");
		pDoc->SetTitle (strTitle.SpanExcluding (":"));

		pDoc->UpdateAllViews (NULL);
		((CMainFrame*) AfxGetMainWnd ())->m_wndLeftPanel.Update (true);
		pDoc->SetModifiedFlag ();
	}
}

void CWebCdCoversBrowser::OnUpdateBrowseApply(CCmdUI* pCmdUI) 
{
	if (!((CMainFrame*) AfxGetMainWnd ())->m_wndLeftPanel.IsLastDocValid ())
	{
		pCmdUI->Enable (false);
		return;
	}

	switch (m_mode)
	{
	case BrowserList:
		if (m_wndTreeList.GetSelectedItem () == NULL)
			pCmdUI->Enable (false);
		else
			pCmdUI->Enable (m_wndTreeList.GetItemData (m_wndTreeList.GetSelectedItem ()) != NULL);

		break;

	case BrowserSearch:
		pCmdUI->Enable (m_wndListSearch.GetFirstSelectedItemPosition () != NULL);
		break;

	default:
		pCmdUI->Enable (false);
	/*
	case BrowserImages:
	case BrowserBkTextures:
	case BrowserBkAlphas:
	case BrowserWeb:
	*/
 	}
}

///////////////////////////////////////////////////////////////////
// List related functions
//
void CWebCdCoversBrowser::OnItemExpandingTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	if (m_htiExpand != NULL)
	{
		// this method has been called previously
		m_htiExpand = NULL;
		return;
	}

	NMTREEVIEW* pTv = (NMTREEVIEW*) pNMHDR;
	if (pTv->action == TVE_COLLAPSE)
		return;

	m_htiExpand = pTv->itemNew.hItem;
	if (m_wndTreeList.GetChildItem (m_htiExpand) != NULL)
		return;		// list has already been downloaded

	// no list yet: download!

	CString strChar = m_wndTreeList.GetItemText (m_htiExpand);

	LoadFromCache (strChar);

	// start download thread
	WCC_GETLISTPARAMS* pParams = new WCC_GETLISTPARAMS;
	pParams->strValue = strChar;
	pParams->hwndList = m_wndTreeList.GetSafeHwnd ();
	pParams->htiParent = m_htiExpand;
	pParams->hwndProgress = m_wndProgress.GetSafeHwnd ();
	pParams->hwndStatus = m_wndStatusBar.GetSafeHwnd ();
	pParams->hwndNotify = GetSafeHwnd ();
	pParams->pCancelDownload = NULL;//&m_bCancelDownload;
	AfxBeginThread (wccGetList, pParams, THREAD_PRIORITY_NORMAL);
}

void CWebCdCoversBrowser::OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// get the browser's document
	IHTMLDocument2* pDoc;
	if (m_pBrowserApp->get_Document ((struct IDispatch**) &pDoc) != S_OK)
		return;

	// get the body
	IHTMLElement* pBody;
	if (pDoc->get_body (&pBody) != S_OK)
	{
		pDoc->Release ();
		return;
	}

	// put new data
	HTREEITEM htiSel = m_wndTreeList.GetSelectedItem ();
	COVERIMAGES* pCoverImages = (COVERIMAGES*) m_wndTreeList.GetItemData (htiSel);
	if (pCoverImages != NULL)
	{
		BSTR bstrHTML = GenerateHTML (m_wndTreeList.GetItemText (htiSel), pCoverImages).AllocSysString ();
		pBody->put_innerHTML (bstrHTML);
		::SysFreeString (bstrHTML);
	}
	else
		pBody->put_innerHTML (L"");

	m_dwCurId = (DWORD) htiSel;

	// clean up
	pBody->Release ();
	pDoc->Release ();
}

void CWebCdCoversBrowser::OnDblClkTree (NMHDR* pNMHDR, LRESULT* pResult)
{
	OnSelChangedTree (pNMHDR, pResult);
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////
// Search related functions
//

void CWebCdCoversBrowser::OnSearch ()
{
	m_wndAnimation.Play (0, -1, -1);

	// start download thread
	WCC_GETLISTPARAMS* pParams = new WCC_GETLISTPARAMS;
	m_wndEdit.GetWindowText (pParams->strValue);
	pParams->hwndList = m_wndListSearch.GetSafeHwnd ();
	pParams->hwndProgress = m_wndProgress.GetSafeHwnd ();
	pParams->hwndStatus = m_wndStatusBar.GetSafeHwnd ();
	pParams->hwndNotify = GetSafeHwnd ();
	pParams->pCancelDownload = NULL;
	AfxBeginThread (wccSearchCovers, pParams, THREAD_PRIORITY_NORMAL);
}

void CWebCdCoversBrowser::OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// get the browser's document
	IHTMLDocument2* pDoc;
	if (m_pBrowserApp->get_Document ((struct IDispatch**) &pDoc) != S_OK)
		return;

	// get the body
	IHTMLElement* pBody;
	if (pDoc->get_body (&pBody) != S_OK)
	{
		pDoc->Release ();
		return;
	}

	// put new data
//	m_dwCurId = m_wndListSearch.GetSelectionMark ();
	POSITION pos = m_wndListSearch.GetFirstSelectedItemPosition ();
	m_dwCurId = m_wndListSearch.GetNextSelectedItem (pos);

	if (m_dwCurId != (DWORD) -1)
	{
		COVERIMAGES* pCoverImages = (COVERIMAGES*) m_wndListSearch.GetItemData (m_dwCurId);
		if (pCoverImages != NULL)
		{
			BSTR bstrHTML = GenerateHTML (m_wndListSearch.GetItemText (m_dwCurId, 0),
				pCoverImages).AllocSysString ();
			pBody->put_innerHTML (bstrHTML);
			::SysFreeString (bstrHTML);
		}
		else
			pBody->put_innerHTML (L"");
	}

	// clean up
	pBody->Release ();
	pDoc->Release ();
}

void CWebCdCoversBrowser::OnDblClkList (NMHDR* pNMHDR, LRESULT* pResult)
{
	OnItemChangedList (pNMHDR, pResult);
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////
// Images related functions
//

void CWebCdCoversBrowser::OnItemexpandingTreeImages(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;

	AfxGetApp ()->BeginWaitCursor ();
	m_wndTreeImages.OnFolderExpanding (pNMHDR, pResult);
	AfxGetApp ()->EndWaitCursor ();

	*pResult = 0;
}

void CWebCdCoversBrowser::OnSelchangedTreeImages(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

//	HTREEITEM hItem; // currently selected TreeItem
	CString	strPath;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;
	m_wndTreeImages.OnFolderSelected (pNMHDR, pResult, strPath);
/*
	// get the browser's document
	IHTMLDocument2* pDoc;
	if (m_pBrowserApp->get_Document ((struct IDispatch**) &pDoc) != S_OK)
		return;
	if (pDoc == NULL)
		return;

	// get the body
	IHTMLElement* pBody;
	if (pDoc->get_body (&pBody) != S_OK)
	{
		pDoc->Release ();
		return;
	}

	// display the images
	WIN32_FIND_DATA wfd;
	CString strTitle;
	int i = 0;

	CString strHTML = "<font face=Tahoma size=4><center>" + strPath +
		"<br/>&nbsp;</center></font><table align=center><tr>";

	strPath += "\\";
	CString strSearchPath = strPath + "*.*";

	AfxGetApp ()->BeginWaitCursor ();
	HANDLE hFFile = ::FindFirstFile (strSearchPath.GetBuffer (0), &wfd);
	if (hFFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			strTitle = wfd.cFileName;
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				if ((strTitle.Right (3).CompareNoCase ("bmp") == 0) ||
					(strTitle.Right (3).CompareNoCase ("gif") == 0) ||
					(strTitle.Right (3).CompareNoCase ("jpg") == 0) ||
					(strTitle.Right (4).CompareNoCase ("jpeg") == 0))
				{
					strHTML += GenerateHTMLImageSrc (strTitle, strPath + wfd.cFileName);
					if (++i % 4 == 0)
						strHTML += "</tr><tr>";
				}
		} while (::FindNextFile (hFFile, &wfd));
		::FindClose (hFFile);
	}
	AfxGetApp ()->EndWaitCursor ();

	strHTML += "</tr></table>";

	BSTR bstrHTML = strHTML.AllocSysString ();
	pBody->put_innerHTML (bstrHTML);
	::SysFreeString (bstrHTML);

	// clean up
	pBody->Release ();
	pDoc->Release ();
*/

	_variant_t varEmpty;
	BSTR bstrURL = strPath.AllocSysString ();
	m_pBrowserApp->Navigate (bstrURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty);
	::SysFreeString (bstrURL);
}

void CWebCdCoversBrowser::OnRclickTreeImages(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_wndTreeImages.GetContextMenu (pNMHDR, pResult);
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////
// Alpha texture related functions
//
void CWebCdCoversBrowser::FillTexturesList()
{
	// get the images
	WIN32_FIND_DATA wfd;
	CString strSearchPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetTextureDir () + "*.*";

	AfxGetApp ()->BeginWaitCursor ();
	HANDLE hFFile = ::FindFirstFile (strSearchPath.GetBuffer (0), &wfd);
	if (hFFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			// add directories
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				if (wfd.cFileName[0] != '.')
					m_wndListBkTextures.InsertItem (0, wfd.cFileName);
		} while (::FindNextFile (hFFile, &wfd));
		::FindClose (hFFile);
	}
	AfxGetApp ()->EndWaitCursor ();
}

void CWebCdCoversBrowser::OnItemChangedBkTextures (NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// get the browser's document
	IHTMLDocument2* pDoc;
	if (m_pBrowserApp->get_Document ((struct IDispatch**) &pDoc) != S_OK)
		return;
	if (pDoc == NULL)
		return;

	// get the body
	IHTMLElement* pBody;
	if (pDoc->get_body (&pBody) != S_OK)
	{
		pDoc->Release ();
		return;
	}

	POSITION pos = m_wndListBkTextures.GetFirstSelectedItemPosition ();
	int nSel = m_wndListBkTextures.GetNextSelectedItem (pos);

	if (nSel != (DWORD) -1)
	{
		// display the images
		WIN32_FIND_DATA wfd;
		int i = 0;

		CString strPath = m_wndListBkTextures.GetItemText (nSel, 0);
		CString strHTML = "<font face=Tahoma size=4><center>" + strPath +
			"<br/>&nbsp;</center></font><table align=center><tr>";

		strPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetTextureDir () + strPath + "\\";
		CString strSearchPath = strPath + "*.*";
		CString strTitle;

		AfxGetApp ()->BeginWaitCursor ();
		HANDLE hFFile = ::FindFirstFile (strSearchPath.GetBuffer (0), &wfd);
		if (hFFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				strTitle = wfd.cFileName;
				if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					if ((strTitle.Right (3).CompareNoCase ("bmp") == 0) ||
						(strTitle.Right (3).CompareNoCase ("gif") == 0) ||
						(strTitle.Right (3).CompareNoCase ("jpg") == 0) ||
						(strTitle.Right (4).CompareNoCase ("jpeg") == 0))
					{
						strHTML += GenerateHTMLImageSrc (wfd.cFileName, strPath + wfd.cFileName, "");
						if (++i % 3 == 0)
							strHTML += "</tr><tr>";
					}
			} while (::FindNextFile (hFFile, &wfd));
			::FindClose (hFFile);
		}
		AfxGetApp ()->EndWaitCursor ();

		strHTML += "</tr></table>";

		BSTR bstrHTML = strHTML.AllocSysString ();
		pBody->put_innerHTML (bstrHTML);
		::SysFreeString (bstrHTML);
	}
	else
		pBody->put_innerHTML (L"");

	// clean up
	pBody->Release ();
	pDoc->Release ();
}

///////////////////////////////////////////////////////////////////
// Alpha texture related functions
//
void CWebCdCoversBrowser::FillAlphaList()
{
	// get the images
	WIN32_FIND_DATA wfd;
	CString strSearchPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetAlphaTextureDir () + "*.*";

	AfxGetApp ()->BeginWaitCursor ();
	HANDLE hFFile = ::FindFirstFile (strSearchPath.GetBuffer (0), &wfd);
	if (hFFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			// add directories
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				if (wfd.cFileName[0] != '.')
					m_wndListBkAlpha.InsertItem (0, wfd.cFileName);
		} while (::FindNextFile (hFFile, &wfd));
		::FindClose (hFFile);
	}
	AfxGetApp ()->EndWaitCursor ();
}

void CWebCdCoversBrowser::OnItemChangedBkAlphas (NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	// get the browser's document
	IHTMLDocument2* pDoc;
	if (m_pBrowserApp->get_Document ((struct IDispatch**) &pDoc) != S_OK)
		return;
	if (pDoc == NULL)
		return;

	// get the body
	IHTMLElement* pBody;
	if (pDoc->get_body (&pBody) != S_OK)
	{
		pDoc->Release ();
		return;
	}

	POSITION pos = m_wndListBkAlpha.GetFirstSelectedItemPosition ();
	int nSel = m_wndListBkAlpha.GetNextSelectedItem (pos);

	if (nSel != (DWORD) -1)
	{
		// display the images
		WIN32_FIND_DATA wfd;
		int i = 0;

		CString strPath = m_wndListBkAlpha.GetItemText (nSel, 0);
		CString strHTML = "<font face=Tahoma size=4><center>" + strPath +
			"<br/>&nbsp;</center></font><table align=center><tr>";

		strPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetAlphaTextureDir () + strPath + "\\";
		CString strSearchPath = strPath + "*.bmp";

		AfxGetApp ()->BeginWaitCursor ();
		HANDLE hFFile = ::FindFirstFile (strSearchPath.GetBuffer (0), &wfd);
		if (hFFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				strHTML += GenerateHTMLImageSrc (wfd.cFileName, strPath + wfd.cFileName, "");
				if (++i % 3 == 0)
					strHTML += "</tr><tr>";
			} while (::FindNextFile (hFFile, &wfd));
			::FindClose (hFFile);
		}
		AfxGetApp ()->EndWaitCursor ();

		strHTML += "</tr></table>";

		BSTR bstrHTML = strHTML.AllocSysString ();
		pBody->put_innerHTML (bstrHTML);
		::SysFreeString (bstrHTML);
	}
	else
		pBody->put_innerHTML (L"");

	// clean up
	pBody->Release ();
	pDoc->Release ();

/*
	// get the browser's document
	IHTMLDocument2* pDoc;
	if (m_pBrowserApp->get_Document ((struct IDispatch**) &pDoc) != S_OK)
		return;

	// get the body
	IHTMLElement* pBody;
	if (pDoc->get_body (&pBody) != S_OK)
	{
		pDoc->Release ();
		return;
	}

	// put new data
	POSITION pos = m_wndListBkAlpha.GetFirstSelectedItemPosition ();
	int nSel = m_wndListBkAlpha.GetNextSelectedItem (pos);

	if (nSel != (DWORD) -1)
	{
		CString strTitle = m_wndListBkAlpha.GetItemText (nSel, 0);
		CString strHTML = "<font face=Tahoma size=4><center>" + strTitle +			
			"<br/>&nbsp;</center></font><table align=center><tr>" +
			GenerateHTMLImageSrc ("", ((CCdCoverCreator2App*) AfxGetApp ())->GetAlphaTextureDir () + strTitle + ".bmp", "");

		BSTR bstrHTML = strHTML.AllocSysString ();
		pBody->put_innerHTML (bstrHTML);
		::SysFreeString (bstrHTML);
	}
	else
		pBody->put_innerHTML (L"");

	// clean up
	pBody->Release ();
	pDoc->Release ();
*/
}

///////////////////////////////////////////////////////////////////
// Web related functions
//
void CWebCdCoversBrowser::LoadWebListEntries()
{
	HKEY hKey;
	DWORD dwDisposition;

	CString strKey = "Software\\";
	strKey += AfxGetApp ()->m_pszRegistryKey;
	strKey += "\\CdCoverCreator\\Links\\";

	RegCreateKeyEx (HKEY_CURRENT_USER, strKey.GetBuffer (0), 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ,
		NULL, &hKey, &dwDisposition);

	TCHAR szName[200];
	DWORD dwBufLen;
	for (DWORD dwIndex = 0; ; dwIndex++)
	{
		dwBufLen = 200;
		if (RegEnumValue (hKey, dwIndex, szName, &dwBufLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
			break;

		m_wndListWeb.InsertItem (0, szName);
	}

	RegCloseKey (hKey);
}

void CWebCdCoversBrowser::SaveWebListEntries()
{
	// delete values
	HKEY hKey;
	DWORD dwDisposition;

	CString strKey = "Software\\";
	strKey += AfxGetApp ()->m_pszRegistryKey;
	strKey += "\\CdCoverCreator\\Links\\";

	RegCreateKeyEx (HKEY_CURRENT_USER, strKey.GetBuffer (0), 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ | KEY_SET_VALUE,
		NULL, &hKey, &dwDisposition);

	char szName[200];
	DWORD dwBufLen;
	for (DWORD dwIndex = 0; ; dwIndex++)
	{
		dwBufLen = 200;
		LONG l;
		if (l = RegEnumValue (hKey, dwIndex, szName, &dwBufLen, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
			break;
		RegDeleteValue (hKey, szName);
	}

	RegCloseKey (hKey);

	// add values
	CWinApp* pApp = AfxGetApp ();
	for (int i = 0; i < m_wndListWeb.GetItemCount (); i++)
		pApp->WriteProfileString ("Links", m_wndListWeb.GetItemText (i, 0), "");
}

void CWebCdCoversBrowser::OnNewLink ()
{
	m_wndListWeb.SetFocus ();
	CEdit* pEdit = m_wndListWeb.EditLabel (m_wndListWeb.InsertItem (0, "http://"));
	if (pEdit != NULL)
		pEdit->SetSel (8, 8);
}

void CWebCdCoversBrowser::OnDelLink ()
{
	POSITION pos = m_wndListWeb.GetFirstSelectedItemPosition ();
	int nSel = m_wndListWeb.GetNextSelectedItem (pos);
	if (nSel > -1)
		m_wndListWeb.DeleteItem (nSel);
}

void CWebCdCoversBrowser::OnEndLabelEditListWeb (NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVDISPINFO* pDispInfo = (NMLVDISPINFO*) pNMHDR;
	if (pDispInfo->item.pszText)
		m_wndListWeb.SetItemText (pDispInfo->item.iItem, pDispInfo->item.iSubItem, pDispInfo->item.pszText);
}

void CWebCdCoversBrowser::OnSelChangedListWeb (NMHDR* pNMHDR, LRESULT* pResult)
{
	POSITION pos = m_wndListWeb.GetFirstSelectedItemPosition ();
	int nSel = m_wndListWeb.GetNextSelectedItem (pos);
	if (nSel > -1)
	{
		_variant_t varEmpty;
		CString strURL = m_wndListWeb.GetItemText (nSel, 0);
		BSTR bstrURL = strURL.AllocSysString ();
		m_pBrowserApp->Navigate (bstrURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty);
		::SysFreeString (bstrURL);
	}
}

///////////////////////////////////////////////////////////////////
// Notification functions called on download thread termination
//

LRESULT CWebCdCoversBrowser::OnGetList (WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

LRESULT CWebCdCoversBrowser::OnGetSearch (WPARAM wParam, LPARAM lParam)
{
	m_wndAnimation.Stop ();
	return S_OK;
}

LRESULT CWebCdCoversBrowser::OnGetImages (WPARAM wParam, LPARAM lParam)
{
	// set the HTML

	// get the browser's document
	IHTMLDocument2* pDoc;
	if (m_pBrowserApp->get_Document ((struct IDispatch**) &pDoc) != S_OK)
		return S_OK;

	// get the body
	IHTMLElement* pBody;
	if (pDoc->get_body (&pBody) != S_OK)
	{
		pDoc->Release ();
		return S_OK;
	}

	// put new data
	/*if (m_wndTreeList.IsWindowVisible ())
	{
		HTREEITEM htiSel = m_wndTreeList.GetSelectedItem ();
		COVERIMAGES* pCoverImages = (COVERIMAGES*) m_wndTreeList.GetItemData (htiSel);
		if (pCoverImages != NULL)
		{
			BSTR bstrHTML = GenerateHTML (m_wndTreeList.GetItemText (htiSel), pCoverImages).AllocSysString ();
			pBody->put_innerHTML (bstrHTML);
			::SysFreeString (bstrHTML);
		}
		else
			pBody->put_innerHTML (L"");
	}
	else*/
	if (m_wndListSearch.IsWindowVisible ())
	{
		POSITION pos = m_wndListSearch.GetFirstSelectedItemPosition ();
		int nSel = m_wndListSearch.GetNextSelectedItem (pos);

		if (nSel > -1)
		{
			COVERIMAGES* pCoverImages = (COVERIMAGES*) m_wndListSearch.GetItemData (nSel);
			if (pCoverImages != NULL)
			{
				CString strName = m_wndListSearch.GetItemText (nSel, 0);
				BSTR bstrHTML = GenerateHTML (strName, pCoverImages).AllocSysString ();
				pBody->put_innerHTML (bstrHTML);
				::SysFreeString (bstrHTML);
			}
		}
	}

	// clean up
	pBody->Release ();
	pDoc->Release ();

	return S_OK;
}

///////////////////////////////////////////////////////////////////
// Helper functions
//

CString CWebCdCoversBrowser::GenerateHTML(CString strTitle, COVERIMAGES *pCoverImages)
{
	CString strHost = "";
	WCC_GETHOST* pfn_GetHost = (WCC_GETHOST*) GetProcAddress (pCoverImages->pHost->hInst, "GetHost");
	if (pfn_GetHost != NULL)
		strHost = pfn_GetHost ();

	CString strHTML = "<center><font face=Tahoma size=4>" + strTitle +
		"<br/>&nbsp;</font><font face=Tahoma size=2><a href=http://" + strHost +
		">" + strHost + "</a></font></center><table align=center><tr>";

	int nCount = 0;
	if (!pCoverImages->strFile[Cover].IsEmpty ())
	{
		nCount++;
		strHTML += GenerateHTMLImageSrc ("Cover", pCoverImages->strLocalFile[Cover]);
	}

	if (!pCoverImages->strFile[Back].IsEmpty ())
	{
		nCount++;
		strHTML += GenerateHTMLImageSrc ("Back", pCoverImages->strLocalFile[Back]);
	}

	if (nCount == 2)
		strHTML += "</tr><tr>";
	if (!pCoverImages->strFile[Inlay].IsEmpty ())
	{
		nCount++;
		strHTML += GenerateHTMLImageSrc ("Inlay", pCoverImages->strLocalFile[Inlay]);
	}

	if (nCount == 2)
		strHTML += "</tr><tr>";
	if (!pCoverImages->strFile[Label].IsEmpty ())
	{
		nCount++;
		strHTML += GenerateHTMLImageSrc ("Label", pCoverImages->strLocalFile[Label]);
	}

	bool bNewRow = false;
	if ((nCount == 2) || (nCount == 4))
		strHTML += "</tr><tr>", bNewRow = true;

	CString strWarning;
	strWarning.LoadString (IDS_DOWNLOADCOVERNOTE);

	CString strUencTitle = CHttpReqSocket::URLEncode (strTitle);

	strHTML += "<td";
	if (bNewRow)
		strHTML += " colspan=2";
	else
		strHTML += " style=\"{width:160px;}\"";
	strHTML += "><font face=Tahoma size=1><br/>" + strWarning + "<br/>";

	CString strURL, strAssoc;
	strURL.LoadString (IDS_AMAZON);
	strAssoc.LoadString (IDS_AMAZON_ASSOC);

	char szModFn[MAX_PATH + 1];
	GetModuleFileName (NULL, szModFn, MAX_PATH);

	strHTML += " <a href=" + strURL + "&keyword=" + strUencTitle + "&index=music>";
	strHTML.AppendFormat ("<img src=\"res://%s/#2/#%d\" border=0 width=60 height=21 alt=\"", szModFn, IDB_BITMAP_AMAZON);
	strHTML += strAssoc + "\"></a>" +
		
/*
	strHTML +=
		//" <a href=http://www.jpc.de/jpcdb/partner/index.html?iampartner=wmp&ref=156987&jumppage=/ >"
		" <a href=http://www.jpc.de/jpcdb/frames/jpcset.html?fastsearch=" + 
			strUencTitle + "&iampartner=wmp&ref=156987&maktion=fastsearch&language=de>"
		"<img src=res://CdCoverCreator.exe/#2/#270 border=0 width=60 height=21 alt=\"jpc - einer der führenden Anbieter für CDs, DVDs, Bücher und Spiele\"></a>";
		
	strHTML +=
		//" <a href=http://partners.webmasterplan.com/click.asp?ref=156987&site=1683&type=b26&bnb=26>"
		" <a href=http://partners.webmasterplan.com/click.asp?ref=156987&site=1683&type=text&tnb=8&titel=" +
			strUencTitle + ">"
		"<img src=res://CdCoverCreator.exe/#2/#269 border=0 width=60 height=21 alt=\"PrimusMedia.de - Ihr Musik & Movie Shop\"></a>"
*/
		"</font></td></tr></table>";

	return strHTML;
}

CString CWebCdCoversBrowser::GenerateHTMLImageSrc(CString strTitle, CString strFilename, CString strSize)
{
	return
		"<td>"
			"<table>"
				"<tr><td>"
					"<table width=160 height=160 border=1 bordercolorlight=#CCCCCC bordercolordark=#808080 cellpadding=0 cellspacing=0>"
					"<tr><td align=center><img " + (strFilename.IsEmpty () ? CString ("") : strSize) +
							"src='" + strFilename + "'/></td></tr>"
					"</table>"
				"</td></tr>"
				"<tr><td align=center><font face=Tahoma size=2>" + strTitle + "</font></td></tr>"
			"</table></td>";
}

void CWebCdCoversBrowser::DownloadImages(COVERIMAGES *pCoverImages, CString strName, CString strLetter)
{
	m_arrbCancelDownload[m_nCancelDownloadPtr % 128] = false;

	for (int i = 0; i < NUM_RENDER_TYPES - 1; i++)
	{
		if (!pCoverImages->strFile[i].IsEmpty ())
		{
			// download only if file is not in cache
			bool bDownload = pCoverImages->strLocalFile[i].IsEmpty () ? true : false;
			if (!bDownload)
			{
				TRY
				{
					CFile file (pCoverImages->strLocalFile[i], CFile::modeRead);
					file.Close ();
					bDownload = false;
				}
				CATCH(CFileException, pEx)
				{
					bDownload = true;
				}
				END_CATCH
			}

			if (bDownload)
			{
				WCC_GETIMAGESPARAMS* pParams = new WCC_GETIMAGESPARAMS;
				pParams->strLetter = strLetter;
				pParams->strName = strName;
				pParams->pCoverImages = pCoverImages;
				pParams->rt = (RenderType) i;
				pParams->hwndProgress = m_wndProgress.GetSafeHwnd ();
				pParams->hwndStatus = m_wndStatusBar.GetSafeHwnd ();
				pParams->hwndNotify = GetSafeHwnd ();
				pParams->pCancelDownload = &(m_arrbCancelDownload[m_nCancelDownloadPtr % 128]);
				AfxBeginThread (wccGetImages, pParams, THREAD_PRIORITY_NORMAL);
			}
		}
	}

	m_nCancelDownloadPtr++;
}

void CWebCdCoversBrowser::LoadFromCache(CString strLetter)
{
	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapCoverHosts;

	TRY
	{
		CFile file (((CCdCoverCreator2App*) AfxGetApp ())->GetListCacheDir () + strLetter, CFile::modeRead);

		CArchive ar (&file, CArchive::load);

		COVERIMAGES* pCi;
		CString strName, strHost;
		TVINSERTSTRUCT insert;

		for ( ; ; )
		{
			ar >> strName;
			if (strName == "____$END$____")
				break;

			pCi = new COVERIMAGES;

			ar >> strHost;
			pCi->pHost = (*(pMap->find (strHost))).second;
			ar >> pCi->strFile[Cover] >> pCi->strFile[Back] >> pCi->strFile[Inlay] >> pCi->strFile[Label];
			ar >> pCi->strLocalFile[Cover] >> pCi->strLocalFile[Back] >> pCi->strLocalFile[Inlay] >> pCi->strLocalFile[Label];

			insert.hParent = m_htiExpand;
			insert.hInsertAfter = TVI_LAST;
			insert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
			insert.item.iImage = 1;
			insert.item.iSelectedImage = 1;
			insert.item.pszText = strName.GetBuffer (0);
			insert.item.lParam = (LPARAM) pCi;

			m_wndTreeList.InsertItem (&insert);
		}

		ar.Close ();
		file.Close ();
	}
	CATCH(CFileException, pEx)
	{
		// we can't load the archive...
	}
	END_CATCH
}

void CWebCdCoversBrowser::SaveToCache()
{
	/*
	COVERIMAGES* pCi;
	CString strHost;

	for (HTREEITEM hItem = m_wndTreeList.GetRootItem (); hItem != NULL; hItem = m_wndTreeList.GetNextSiblingItem (hItem))
	{
		if (m_wndTreeList.GetChildItem (hItem) == NULL)
			continue;

		CFile file (((CCdCoverCreator2App*) AfxGetApp ())->GetListCacheDir () + m_wndTreeList.GetItemText (hItem), CFile::modeWrite | CFile::modeCreate);
		CArchive ar (&file, CArchive::store);

		for (HTREEITEM h = m_wndTreeList.GetChildItem (hItem); h != NULL; h = m_wndTreeList.GetNextSiblingItem (h))
		{
			pCi = (COVERIMAGES*) m_wndTreeList.GetItemData (h);
			WCC_GETHOST* pfn_GetHost = (WCC_GETHOST*) GetProcAddress (pCi->pHost->hInst, "GetHost");

			if (pfn_GetHost != NULL)
			{
				strHost = pfn_GetHost ();

				ar << m_wndTreeList.GetItemText (h);
				ar << strHost;
				ar << pCi->strFile[Cover] << pCi->strFile[Back] << pCi->strFile[Inlay] << pCi->strFile[Label];
				ar << pCi->strLocalFile[Cover] << pCi->strLocalFile[Back] << pCi->strLocalFile[Inlay] << pCi->strLocalFile[Label];
			}
		}

		ar << CString ("____$END$____");

		ar.Close ();
		file.Close ();
	}
	*/
}

void CWebCdCoversBrowser::DisplayControls()
{
	// List
//	m_wndTreeList.ShowWindow (m_mode == BrowserList ? SW_SHOW : SW_HIDE);

	// Search
	m_wndAnimation.ShowWindow (m_mode == BrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndEdit.ShowWindow (m_mode == BrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndListSearch.ShowWindow (m_mode == BrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndBtnSearch.ShowWindow (m_mode == BrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndStaticSearch.ShowWindow (m_mode == BrowserSearch ? SW_SHOW : SW_HIDE);

	// Images
	m_wndTreeImages.ShowWindow (m_mode == BrowserImages ? SW_SHOW : SW_HIDE);

	// Background Textures
	m_wndListBkTextures.ShowWindow (m_mode == BrowserBkTextures ? SW_SHOW : SW_HIDE);

	// Alpha Textures
//	m_wndListBkAlpha.ShowWindow (m_mode == BrowserBkAlphas ? SW_SHOW : SW_HIDE);

	// Web
	m_wndListWeb.ShowWindow (m_mode == BrowserWeb ? SW_SHOW : SW_HIDE);
	m_wndBtnNewLink.ShowWindow (m_mode == BrowserWeb ? SW_SHOW : SW_HIDE);
	m_wndBtnDelLink.ShowWindow (m_mode == BrowserWeb ? SW_SHOW : SW_HIDE);
}

HBRUSH CWebCdCoversBrowser::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	switch (pWnd->GetDlgCtrlID ())
	{
	case 0xFFFF:
		return (HBRUSH) ::GetStockObject (WHITE_BRUSH);
	default:
		return CMDIChildWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CWebCdCoversBrowser::Navigate2Empty()
{
	// navigate to empty page
	char szModFn[MAX_PATH + 1];
	GetModuleFileName (NULL, szModFn, MAX_PATH);

	CString strURL;
	strURL.Format ("res://%s/%d", szModFn, IDR_HTML_EMPTY);

	BSTR bstrURL = strURL.AllocSysString ();
	_variant_t varEmpty;
	m_pBrowserApp->Navigate (bstrURL, &varEmpty, &varEmpty, &varEmpty, &varEmpty);
	::SysFreeString (bstrURL);
}

void CWebCdCoversBrowser::SetMode(BrowserMode mode)
{
	m_mode = mode;
	switch (mode)
	{
	case BrowserList:
		OnBrowseList ();
		break;
	case BrowserSearch:
		OnBrowseSearch ();
		break;
	case BrowserImages:
		OnBrowseImage ();
		break;
	case BrowserBkTextures:
		OnBrowseBktexture ();
		break;
	case BrowserBkAlphas:
		OnBrowseBkalpha ();
		break;
	case BrowserWeb:
		OnBrowseWeb ();
		break;
	}
}

void CWebCdCoversBrowser::OnClose()
{
	((CMainFrame*) AfxGetMainWnd ())->m_pActivateCoverBrowser = NULL;
	CMDIChildWnd::OnClose();
}
