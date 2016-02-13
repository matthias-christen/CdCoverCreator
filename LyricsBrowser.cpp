// LyricsBrowser.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "LyricsBrowser.h"
#include "CdCoverCreator2Doc.h"

#include "M3UPlaylist.h"

#include <afxole.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLyricsBrowser

IMPLEMENT_DYNCREATE(CLyricsBrowser, CMDIChildWnd)

CLyricsBrowser::CLyricsBrowser()
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
	m_nCancelDownloadPtr = 0;

	m_mode = LyricsBrowserList;
	m_nLeftPaneWidth = 250;

	m_nCurrentSearchItem = -1;

	m_hCurSize = AfxGetApp ()->LoadStandardCursor (IDC_SIZEWE);
}

CLyricsBrowser::~CLyricsBrowser()
{
	::DestroyIcon (m_hIconLyrics);
	::DestroyIcon (m_hIconLyricsOK);
	::DestroyIcon (m_hIconLyricsNotFound);

	::DestroyIcon (m_hIconWindowList);
	::DestroyIcon (m_hIconM3U);

	m_imgListSmall.DeleteImageList ();
	m_font.DeleteObject ();
}


BEGIN_MESSAGE_MAP(CLyricsBrowser, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLyricsBrowser)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_LBROWSE_LIST, OnLbrowseList)
	ON_UPDATE_COMMAND_UI(ID_LBROWSE_LIST, OnUpdateLbrowseList)
	ON_COMMAND(ID_LBROWSE_SEARCH, OnLbrowseSearch)
	ON_UPDATE_COMMAND_UI(ID_LBROWSE_SEARCH, OnUpdateLbrowseSearch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_BROWSER, OnItemChangedListBrowser)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_BROWSER, OnBeginDragListBrowser)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BROWSER, OnRClickListBrowser)
	ON_COMMAND(IDC_BUTTON_SEARCH, OnSearch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SEARCH, OnItemChangedListSearch)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_SEARCH, OnBeginDragListSearch)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SEARCH, OnRClickListSearch)
	ON_COMMAND(IDC_BUTTON_WINDOWSLIST, OnWindowsList)
	ON_COMMAND(IDC_BUTTON_M3U, OnM3U)
	ON_WM_DESTROY()
	ON_COMMAND(ID_SEARCH, OnSearchItem)
	//}}AFX_MSG_MAP
	ON_MESSAGE(SPLIT_RESIZE, OnSplitResize)
	ON_COMMAND_RANGE(1000, 1100, OnWindowsListMenu)
	ON_MESSAGE(WSLM_GETLYRICS, OnGetSearch)
	ON_COMMAND_RANGE(ID_ASSIGNTO, ID_ASSIGNTO+100, OnAssignTo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLyricsBrowser message handlers

static UINT indicators[] =
{
	IDC_PROGRESS_BROWSER,
	IDC_PROGRESS_BROWSER,
	IDS_DOWNLOADSTATUS_KB,
	IDS_DOWNLOADSTATUS_PERCENT,
	IDS_DOWNLOADSTATUS_KBS
};

int CLyricsBrowser::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create (this, AFX_IDW_TOOLBAR) ||
		!m_wndToolBar.LoadToolBar (IDR_TOOLBAR_LYRICSBROWSER, AILS_NEW))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strLabel;
	for (int i = 0; i < 2; i++)
	{
		strLabel.LoadString (IDS_BROWSER_LIST + i);
		m_wndToolBar.SetButtonText (i, strLabel);
	}
	m_wndToolBar.SetSizes (CSize (42, 22), CSize (16, 16));
	m_wndToolBar.ModifyStyle (0, TBSTYLE_LIST);

	if (!m_wndReBar.Create (this) ||
		!m_wndReBar.AddBar (&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\\n");
		return -1;      // fail to create
	}
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\\n");
		return -1;      // fail to create
	}

	CRect r (10, 10, 10, 10);

	// Left pane
	m_wndSplitter.Create ("", WS_CHILD | WS_VISIBLE | SS_NOTIFY, r, this, IDC_SPLITTER);

	// List
	m_wndList.Create (
		WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS,
		r, this, IDC_LIST_BROWSER);
	m_wndBtnWindows.Create ("", WS_CHILD | WS_TABSTOP, r, this, IDC_BUTTON_WINDOWSLIST);
	m_wndBtnM3U.Create ("", WS_CHILD | WS_TABSTOP, r, this, IDC_BUTTON_M3U);

	// Search
	m_wndAnimation.Create (WS_CHILD | ACS_TRANSPARENT, r, this, IDC_ANIMATION_BROWSER);
	m_wndListSearch.Create (WS_CHILD | LVS_LIST | LVS_SORTASCENDING | LVS_SHOWSELALWAYS,
		r, this, IDC_LIST_SEARCH);
	m_wndSearchArtistEdit.Create (WS_CHILD | WS_TABSTOP, r, this, IDC_EDIT_SEARCHARTIST);
	m_wndSearchAlbumEdit.Create (WS_CHILD | WS_TABSTOP, r, this, IDC_EDIT_SEARCHALBUM);
	m_wndSearchSongEdit.Create (WS_CHILD | WS_TABSTOP, r, this, IDC_EDIT_SEARCHSONG);

	CString strCaption;
	strCaption.LoadString (IDS_BUTTON_SEARCH);
	m_wndBtnSearch.Create (strCaption, WS_CHILD | WS_TABSTOP, r, this, IDC_BUTTON_SEARCH);

	strCaption.LoadString (IDS_STATIC_ARTISTSEARCH);
	m_wndSearchArtistStatic.Create (strCaption, WS_CHILD, r, this);
	strCaption.LoadString (IDS_STATIC_ALBUMSEARCH);
	m_wndSearchAlbumStatic.Create (strCaption, WS_CHILD, r, this);
	strCaption.LoadString (IDS_STATIC_SONGSEARCH);
	m_wndSearchSongStatic.Create (strCaption, WS_CHILD, r, this);

	strCaption.LoadString (IDS_SEARCHWEBCDCOVERS);
	m_wndStaticSearch.Create (strCaption, WS_CHILD, r, this);

	// Right pane: RichEdit Control
	m_wndEdit.Create (WS_CHILD | WS_VSCROLL | WS_VISIBLE |
		ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_WANTRETURN,
		r, this, 100);

	// Progress control
	m_wndStatusBar.GetItemRect (0, &r);
	m_wndProgress.Create (WS_VISIBLE | WS_CHILD | PBS_SMOOTH, r, &m_wndStatusBar, 1);

	Init ();

	return 0;
}

BOOL CLyricsBrowser::PreCreateWindow(CREATESTRUCT& cs) 
{
	CMDIChildWnd::PreCreateWindow(cs);
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;
}

void CLyricsBrowser::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	Resize ();
}

void CLyricsBrowser::Resize ()
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
	m_wndList.SetWindowPos (NULL, 0, nTop + 23, m_nLeftPaneWidth, nHeight - 23, SWP_NOZORDER);
	m_wndBtnWindows.SetWindowPos (NULL, 1, nTop + 1, 30, 20, SWP_NOZORDER);
	m_wndBtnM3U.SetWindowPos (NULL, 32, nTop + 1, 20, 20, SWP_NOZORDER);

	// Search
	m_wndListSearch.SetWindowPos (NULL, 0, nTop + 140, m_nLeftPaneWidth, nHeight - 140, SWP_NOZORDER);
	m_wndSearchArtistStatic.SetWindowPos (NULL, 10, nTop + 34, 42, 16, SWP_NOZORDER);
	m_wndSearchArtistEdit.SetWindowPos (NULL, 55, nTop + 32, m_nLeftPaneWidth - 65, 20, SWP_NOZORDER);
	m_wndSearchAlbumStatic.SetWindowPos (NULL, 10, nTop + 59, 42, 16, SWP_NOZORDER);
	m_wndSearchAlbumEdit.SetWindowPos (NULL, 55, nTop + 57, m_nLeftPaneWidth - 65, 20, SWP_NOZORDER);
	m_wndSearchSongStatic.SetWindowPos (NULL, 10, nTop + 84, 42, 16, SWP_NOZORDER);
	m_wndSearchSongEdit.SetWindowPos (NULL, 55, nTop + 82, m_nLeftPaneWidth - 65, 20, SWP_NOZORDER);
	m_wndAnimation.SetWindowPos (NULL, 10, nTop + 10, 16, 16, SWP_NOZORDER);
	m_wndBtnSearch.SetWindowPos (NULL, 10, nTop + 110, 100, 24, SWP_NOZORDER);
	m_wndStaticSearch.SetWindowPos (NULL, 32, nTop + 10, m_nLeftPaneWidth - 40, 16, SWP_NOZORDER);

	// Right pane, status bar
	m_wndEdit.SetWindowPos (NULL, m_nLeftPaneWidth + 1, nTop, cx - m_nLeftPaneWidth - 1, nHeight, SWP_NOZORDER);

	m_wndStatusBar.GetItemRect (0, &r);
	m_wndProgress.SetWindowPos (&wndTop, r.left, r.top, r.Width (), r.Height (), 0);
}

LRESULT CLyricsBrowser::OnSplitResize (WPARAM wParam, LPARAM lParam)
{
	m_nLeftPaneWidth += (int) wParam;
	Resize ();
	return S_OK;
}

BOOL CLyricsBrowser::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (pWnd->GetDlgCtrlID () == IDC_SPLITTER)
	{
		SetCursor (m_hCurSize);
		return true;
	}
	
	return CMDIChildWnd::OnSetCursor(pWnd, nHitTest, message);
}

HBRUSH CLyricsBrowser::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	switch (pWnd->GetDlgCtrlID ())
	{
	case 0xFFFF:
		return (HBRUSH) ::GetStockObject (WHITE_BRUSH);
	default:
		return CMDIChildWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	}
}

void CLyricsBrowser::Init()
{
	// set styles / fonts
	CWinApp* pApp = AfxGetApp ();

	m_imgListSmall.Create (16, 16, ILC_COLOR8 | ILC_MASK, 3, 1);
	m_imgListSmall.Add (m_hIconLyrics = pApp->LoadIcon (IDI_ICON_LYRICS));
	m_imgListSmall.Add (m_hIconLyricsOK = pApp->LoadIcon (IDI_ICON_LYRICS_OK));
	m_imgListSmall.Add (m_hIconLyricsNotFound = pApp->LoadIcon (IDI_ICON_LYRICS_NOTFOUND));

	// List
	SetWindowLong (m_wndList.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndList.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);

	m_wndList.SetImageList (&m_imgListSmall, LVSIL_SMALL);

	CString str;
	str.LoadString (IDS_LYRICSBROWSER_TITLE);
	m_wndList.InsertColumn (0, str, LVCFMT_LEFT, 250);

	m_wndBtnWindows.SetIcon (m_hIconWindowList = pApp->LoadIcon (IDI_ICON_WINDOWSLIST));
	m_wndBtnM3U.SetIcon (m_hIconM3U = pApp->LoadIcon (IDI_ICON_M3U));

	// Search
	SetWindowLong (m_wndListSearch.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndListSearch.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	SetWindowLong (m_wndSearchArtistEdit.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndSearchArtistEdit.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	SetWindowLong (m_wndSearchAlbumEdit.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndSearchAlbumEdit.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	SetWindowLong (m_wndSearchSongEdit.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndSearchSongEdit.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);
	m_wndSearchArtistStatic.SetFont (&m_font);
	m_wndSearchArtistEdit.SetFont (&m_font);
	m_wndSearchAlbumStatic.SetFont (&m_font);
	m_wndSearchAlbumEdit.SetFont (&m_font);
	m_wndSearchSongStatic.SetFont (&m_font);
	m_wndSearchSongEdit.SetFont (&m_font);
	m_wndBtnSearch.SetFont (&m_font);
	m_wndStaticSearch.SetFont (&m_font);
	m_wndAnimation.Open (IDR_AVIFIND);
	m_wndListSearch.SetImageList (&m_imgListSmall, LVSIL_SMALL);

	// Rich edit
	SetWindowLong (m_wndEdit.GetSafeHwnd (), GWL_EXSTYLE,
		GetWindowLong (m_wndEdit.GetSafeHwnd (), GWL_EXSTYLE) | WS_EX_CLIENTEDGE);

	for (int i = 1; i <= 4; i++)
		m_wndStatusBar.SetPaneText (i, "");

	DisplayControls ();
}

void CLyricsBrowser::DisplayControls()
{
	// List
	m_wndList.ShowWindow (m_mode == LyricsBrowserList ? SW_SHOW : SW_HIDE);
	m_wndBtnWindows.ShowWindow (m_mode == LyricsBrowserList ? SW_SHOW : SW_HIDE);
	m_wndBtnM3U.ShowWindow (m_mode == LyricsBrowserList ? SW_SHOW : SW_HIDE);

	// Search
	m_wndAnimation.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndSearchArtistStatic.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndSearchArtistEdit.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndSearchAlbumStatic.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndSearchAlbumEdit.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndSearchSongStatic.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndSearchSongEdit.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndListSearch.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndBtnSearch.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
	m_wndStaticSearch.ShowWindow (m_mode == LyricsBrowserSearch ? SW_SHOW : SW_HIDE);
}

void CLyricsBrowser::OnLbrowseList() 
{
	m_mode = LyricsBrowserList;
	m_nCurrentSearchItem = -1;
	DisplayControls ();	
}

void CLyricsBrowser::OnUpdateLbrowseList(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (m_mode == LyricsBrowserList);	
}

void CLyricsBrowser::OnLbrowseSearch() 
{
	m_mode = LyricsBrowserSearch;
	m_nCurrentSearchItem = -1;
	DisplayControls ();
	m_wndSearchArtistEdit.SetFocus ();
}

void CLyricsBrowser::OnUpdateLbrowseSearch(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio (m_mode == LyricsBrowserSearch);
}

void CLyricsBrowser::OnWindowsList()
{
	// add all open documents to the menu
	POSITION pos = AfxGetApp ()->GetFirstDocTemplatePosition ();
	CDocTemplate* pDocTmpl = AfxGetApp ()->GetNextDocTemplate (pos);
	pos = pDocTmpl->GetFirstDocPosition ();
	if (pos == NULL)
		return;

	CMenu menu;
	menu.CreatePopupMenu ();

	int i = 1000;
	//if (((CCdCoverCreator2Doc*) pDocTmpl->GetNextDoc (pos))->m_pTracks != NULL)
		while (pos)
			menu.AppendMenu (MF_STRING, i++,
				((CCdCoverCreator2Doc*) pDocTmpl->GetNextDoc (pos))->m_pTracks->GetTitle ());

	if (i > 1000)
	{
		CRect r;
		m_wndBtnWindows.GetWindowRect (&r);
		menu.TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, r.left, r.bottom, this);
	}
	else
		menu.DestroyMenu ();
}

void CLyricsBrowser::OnM3U()
{
	CString strFilename;
	strFilename.LoadString (IDS_OPENM3U_FILENAME);
	CString strFilter;
	strFilter.LoadString (IDS_OPENM3U_FILTER);

	WSL_GETLYRICSPARAMS* pParams;

	CFileDialog dlg (TRUE, NULL, strFilename, 0, strFilter, this);
	if (dlg.DoModal () == IDOK)
	{
		DeleteListItems ();
		pParams = new WSL_GETLYRICSPARAMS;

		AfxGetApp ()->BeginWaitCursor ();

		CM3UPlaylist pl (dlg.GetPathName ());
		int n;
		for (std::list<CPlaylistEntry*>::iterator it = pl.m_List.begin (); it != pl.m_List.end (); it++)
		{
			n = m_wndList.InsertItem (m_wndList.GetItemCount (), (*it)->m_strTitle, 0);
			m_wndList.SetItemData (n, 0);

			pParams->list.push_back (new CWslItem ((*it)->m_strArtist, (*it)->m_strAlbum, (*it)->m_strTitle));
		}

		AfxGetApp ()->EndWaitCursor ();

		// start download thread
		pParams->hwndList = m_wndList.GetSafeHwnd ();
		pParams->hwndProgress = m_wndProgress.GetSafeHwnd ();
		pParams->hwndStatus = m_wndStatusBar.GetSafeHwnd ();
		pParams->hwndNotify = GetSafeHwnd ();
		pParams->pCancelDownload = NULL;//&m_bCancelDownload;
		AfxBeginThread (wslGetLyrics, pParams, THREAD_PRIORITY_NORMAL);
	}
}

void CLyricsBrowser::DeleteListItems()
{
	for (int i = 0; i < m_wndList.GetItemCount (); i++)
		if (m_wndList.GetItemData (i) != 0)
			delete (LPSTR) m_wndList.GetItemData (i);
	m_wndList.DeleteAllItems ();
}

void CLyricsBrowser::DeleteSearchItems()
{
	for (int i = 0; i < m_wndListSearch.GetItemCount (); i++)
		if (m_wndListSearch.GetItemData (i) != 0)
			delete (LPSTR) m_wndListSearch.GetItemData (i);
	m_wndListSearch.DeleteAllItems ();
}

void CLyricsBrowser::OnWindowsListMenu(UINT nId)
{
	// add all open documents to the menu
	POSITION pos = AfxGetApp ()->GetFirstDocTemplatePosition ();
	CDocTemplate* pDocTmpl = AfxGetApp ()->GetNextDocTemplate (pos);
	pos = pDocTmpl->GetFirstDocPosition ();

	CCdCoverCreator2Doc* pDoc;
	CTrack* pTrack;

	for (UINT i = 1000; i <= nId; i++)
		pDoc = (CCdCoverCreator2Doc*) pDocTmpl->GetNextDoc (pos);

	DeleteListItems ();
	WSL_GETLYRICSPARAMS* pParams = new WSL_GETLYRICSPARAMS;

	int n;
	if (pDoc->m_pTracks != NULL)
		for (UINT i = 0; i < pDoc->m_pTracks->GetNumTracks (); i++)
			if (!pDoc->m_pTracks->GetTrack (i).IsTrackGrouping ())
			{
				pTrack = &(pDoc->m_pTracks->GetTrack (i));
				n = m_wndList.InsertItem (m_wndList.GetItemCount (), pTrack->GetTitle (), 0);
				m_wndList.SetItemData (n, 0);

				pParams->list.push_back (new CWslItem (pTrack->GetRemarks (), "", pTrack->GetTitle ()));
			}

	// start download thread
	pParams->hwndList = m_wndList.GetSafeHwnd ();
	pParams->hwndProgress = m_wndProgress.GetSafeHwnd ();
	pParams->hwndStatus = m_wndStatusBar.GetSafeHwnd ();
	pParams->hwndNotify = GetSafeHwnd ();
	pParams->pCancelDownload = NULL;//&m_bCancelDownload;
	AfxBeginThread (wslGetLyrics, pParams, THREAD_PRIORITY_NORMAL);
}

void CLyricsBrowser::OnItemChangedListBrowser (NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	POSITION pos = m_wndList.GetFirstSelectedItemPosition ();
	int nSel = m_wndList.GetNextSelectedItem (pos);

	if (nSel != (DWORD) -1)
		m_wndEdit.SetRTF (GetRTFHeader () + GetRTF ((CLyrics*) m_wndList.GetItemData (nSel)) + "\\par}");
}

void CLyricsBrowser::OnBeginDragListBrowser (NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	CString strText = GetRTFHeader ();
	int nSel;

	for (POSITION pos = m_wndList.GetFirstSelectedItemPosition (); pos != NULL; )
	{
		nSel = m_wndList.GetNextSelectedItem (pos);
		strText += GetRTF ((CLyrics*) m_wndList.GetItemData (nSel));
		strText += "\\par\\par\\par ";
	}
	strText += "\\par}";

	// drag source
	HGLOBAL hGl = GlobalAlloc (GHND, strText.GetLength () + 1);
	strcpy ((LPSTR) GlobalLock (hGl), (LPSTR) (LPCTSTR) strText);
	GlobalUnlock (hGl);

	COleDataSource ds;
	ds.CacheGlobalData (::RegisterClipboardFormat (CF_RTF), hGl);
	ds.DoDragDrop ();
}

void CLyricsBrowser::OnRClickListBrowser (NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	if (m_wndList.GetSelectionMark () == -1)
		return;

	CMenu menu;
	menu.CreatePopupMenu ();

	CString str;
	str.Format (IDS_SEARCH, m_wndList.GetItemText (m_wndList.GetSelectionMark (), 0));
	menu.AppendMenu (MF_STRING | MF_BYCOMMAND, ID_SEARCH, str);

	CPoint pt;
	GetCursorPos (&pt);
	menu.TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

//	menu.DestroyMenu ();
}

void CLyricsBrowser::OnSearch ()
{
	m_wndAnimation.Play (0, -1, -1);
	DeleteSearchItems ();

	// start download thread
	WSL_SEARCHLYRICSPARAMS* pParams = new WSL_SEARCHLYRICSPARAMS;

	m_wndSearchArtistEdit.GetWindowText (pParams->strArtist);
	m_wndSearchAlbumEdit.GetWindowText (pParams->strAlbum);
	m_wndSearchSongEdit.GetWindowText (pParams->strTitle);

	pParams->hwndList = m_wndListSearch.GetSafeHwnd ();
	pParams->hwndProgress = m_wndProgress.GetSafeHwnd ();
	pParams->hwndStatus = m_wndStatusBar.GetSafeHwnd ();
	pParams->hwndNotify = GetSafeHwnd ();
	pParams->pCancelDownload = NULL;//&m_bCancelDownload;
	AfxBeginThread (wslSearchLyrics, pParams, THREAD_PRIORITY_NORMAL);
}

LRESULT CLyricsBrowser::OnGetSearch (WPARAM wParam, LPARAM lParam)
{
	m_wndAnimation.Stop ();
	return S_OK;
}

void CLyricsBrowser::OnItemChangedListSearch (NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	POSITION pos = m_wndListSearch.GetFirstSelectedItemPosition ();
	int nSel = m_wndListSearch.GetNextSelectedItem (pos);

	if (nSel != (DWORD) -1)
		m_wndEdit.SetRTF (GetRTFHeader () + GetRTF ((CLyrics*) m_wndListSearch.GetItemData (nSel)) + "\\par}");
}

void CLyricsBrowser::OnBeginDragListSearch (NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	CString strText = GetRTFHeader ();
	int nSel;

	for (POSITION pos = m_wndListSearch.GetFirstSelectedItemPosition (); pos != NULL; )
	{
		nSel = m_wndListSearch.GetNextSelectedItem (pos);
		strText += GetRTF ((CLyrics*) m_wndListSearch.GetItemData (nSel));
		strText += "\\par\\par\\par ";
	}
	strText += "\\par}";

	// drag source
	HGLOBAL hGl = GlobalAlloc (GHND, strText.GetLength () + 1);
	strcpy ((LPSTR) GlobalLock (hGl), (LPSTR) (LPCTSTR) strText);
	GlobalUnlock (hGl);

	COleDataSource ds;
	ds.CacheGlobalData (::RegisterClipboardFormat (CF_RTF), hGl);
	ds.DoDragDrop ();
}

void CLyricsBrowser::OnRClickListSearch (NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;

	if (m_wndListSearch.GetSelectionMark () == -1)
		return;

	// build the popup menu
	CMenu menuPopup;
	menuPopup.CreateMenu ();
	for (int i = 0; i < m_wndList.GetItemCount (); i++)
		menuPopup.AppendMenu (MF_STRING | MF_BYCOMMAND, ID_ASSIGNTO + i, m_wndList.GetItemText (i, 0));

	if (m_nCurrentSearchItem != -1)
		menuPopup.SetDefaultItem (ID_ASSIGNTO + m_nCurrentSearchItem);

	// build the menu
	CMenu menu;
	menu.CreatePopupMenu ();

	CString str;
	str.LoadString (IDS_ASSIGNTO);
	menu.AppendMenu (MF_POPUP, (UINT_PTR) menuPopup.GetSafeHmenu (), str); //AppendODPopupMenuA (str);

	menu.AppendMenu (MF_STRING | MF_BYCOMMAND | (m_nCurrentSearchItem == -1 ? MF_GRAYED | MF_DISABLED: MF_ENABLED),
		m_nCurrentSearchItem == -1 ? (UINT) -1 : ID_ASSIGNTO + m_nCurrentSearchItem,
		str + " '" + m_wndList.GetItemText (m_nCurrentSearchItem, 0) + "'");

	CPoint pt;
	GetCursorPos (&pt);
	menu.TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);

//	menu.DestroyMenu ();
}

CString CLyricsBrowser::GetRTFHeader()
{
	return CString ("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1031{\\fonttbl{\\f0\\fswiss\\fprq2\\fcharset0 Tahoma;}}");
}

CString CLyricsBrowser::GetRTF(CLyrics *pLyrics)
{
	if (pLyrics == NULL)
		return "";

	return CString (
		"\\viewkind4\\uc1\\pard\\b\\f0\\fs20 " + pLyrics->m_strArtist + "\\par "
		"\\b0\\fs16 " + pLyrics->m_strAlbum + "\\par\\par "
		"\\b\\fs22 " + pLyrics->m_strSong + "\\par "
		"\\b0\\fs16\\par " + pLyrics->m_strText);
}

void CLyricsBrowser::OnDestroy() 
{
	DeleteListItems ();
	DeleteSearchItems ();

	CMDIChildWnd::OnDestroy();	
}

void CLyricsBrowser::OnSearchItem()
{
	CLyrics* pLyrics = (CLyrics*) m_wndList.GetItemData (m_nCurrentSearchItem = m_wndList.GetSelectionMark ());
	if (pLyrics == NULL)
	{
		MessageBeep (0);
		return;
	}

	m_wndSearchArtistEdit.SetWindowText (pLyrics->m_strArtist);
	m_wndSearchAlbumEdit.SetWindowText (pLyrics->m_strAlbum);
	m_wndSearchSongEdit.SetWindowText (pLyrics->m_strSong);

	m_mode = LyricsBrowserSearch;
	DisplayControls ();
}

void CLyricsBrowser::OnAssignTo (UINT nId)
{
	CLyrics* pFrom = (CLyrics*) m_wndListSearch.GetItemData (m_wndListSearch.GetSelectionMark ());
	if (pFrom == NULL)
	{
		MessageBeep (0);
		return;
	}

	CLyrics* pTo = (CLyrics*) m_wndList.GetItemData (nId - ID_ASSIGNTO);
	if (pTo != NULL)
		delete pTo;

	m_wndList.SetItem (nId - ID_ASSIGNTO, 0, LVIF_PARAM | LVIF_IMAGE | LVIF_STATE, NULL, 1, LVIS_SELECTED, LVIS_SELECTED,
		(DWORD) (new CLyrics (pFrom->m_strArtist, pFrom->m_strAlbum, pFrom->m_strSong, pFrom->m_strText)));
	m_mode = LyricsBrowserList;
	DisplayControls ();

	m_nCurrentSearchItem = -1;
}
