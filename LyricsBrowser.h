#if !defined(AFX_LYRICSBROWSER_H__3D31FF7A_A771_4644_9960_8B298B52C196__INCLUDED_)
#define AFX_LYRICSBROWSER_H__3D31FF7A_A771_4644_9960_8B298B52C196__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LyricsBrowser.h : header file
//

#include "Splitter.h"
#include "MouseOverButton.h"
#include "AutoRichEditCtrl.h"
#include "WebLyrics.h"
#include "EditTab.h"
#include "AlphaToolbar.h"

typedef enum
{
	LyricsBrowserList,
	LyricsBrowserSearch,
} LyricsBrowserMode;

/////////////////////////////////////////////////////////////////////////////
// CLyricsBrowser frame

class CLyricsBrowser : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CLyricsBrowser)
protected:
	CLyricsBrowser();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLyricsBrowser)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nCurrentSearchItem;
	CString GetRTF (CLyrics* pLyrics);
	CString GetRTFHeader();
	void DeleteSearchItems();
	void DeleteListItems();
	virtual ~CLyricsBrowser();

	void Init();
	void Resize();
	void DisplayControls();

	HICON m_hIconLyrics, m_hIconLyricsOK, m_hIconLyricsNotFound;
	HICON m_hIconWindowList, m_hIconM3U;

	// Generated message map functions
	//{{AFX_MSG(CLyricsBrowser)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLbrowseList();
	afx_msg void OnUpdateLbrowseList(CCmdUI* pCmdUI);
	afx_msg void OnLbrowseSearch();
	afx_msg void OnUpdateLbrowseSearch(CCmdUI* pCmdUI);
	afx_msg void OnItemChangedListBrowser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDragListBrowser(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickListBrowser (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSearch ();
	afx_msg void OnItemChangedListSearch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDragListSearch(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRClickListSearch (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnSearchItem();
	//}}AFX_MSG
	afx_msg LRESULT OnSplitResize (WPARAM wParam, LPARAM lParam);
	afx_msg void OnWindowsListMenu (UINT nId);
	afx_msg void OnM3U();
	afx_msg void OnWindowsList ();
	afx_msg void OnAssignTo (UINT nId);
	afx_msg LRESULT OnGetSearch (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	LyricsBrowserMode m_mode;

	UINT m_nTimerId;
	int m_nCancelDownloadPtr;
	bool m_arrbCancelDownload[128];
	int m_nError;

	int m_nLeftPaneWidth;
	HCURSOR m_hCurSize;

	// List
	CListCtrl m_wndList;
	CMouseOverButton m_wndBtnWindows;
	CMouseOverButton m_wndBtnM3U;

	// Search
	CListCtrl m_wndListSearch;
	CAnimateCtrl m_wndAnimation;
	CButton m_wndBtnSearch;
	CStatic m_wndSearchArtistStatic, m_wndSearchAlbumStatic, m_wndSearchSongStatic;
	CEditTab m_wndSearchArtistEdit, m_wndSearchAlbumEdit, m_wndSearchSongEdit;
	CStatic m_wndStaticSearch;

	CAutoRichEditCtrl m_wndEdit;

	CSplitter m_wndSplitter;

	// Tool bar
	CReBar m_wndReBar;
	CAlphaToolBar m_wndToolBar;

	// Status bar
	CProgressCtrl m_wndProgress;
	CStatusBar m_wndStatusBar;

	CFont m_font;
	CImageList m_imgListSmall;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LYRICSBROWSER_H__3D31FF7A_A771_4644_9960_8B298B52C196__INCLUDED_)
