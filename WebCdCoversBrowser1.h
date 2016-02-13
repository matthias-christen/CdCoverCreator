#if !defined(AFX_WEBCDCOVERSBROWSER1_H__556CCF6E_B9D7_4392_B317_56F1837D675D__INCLUDED_)
#define AFX_WEBCDCOVERSBROWSER1_H__556CCF6E_B9D7_4392_B317_56F1837D675D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// WebCdCoversBrowser1.h : header file
//

#include <afxtempl.h>
#include "CdCoverCreator2.h"
#include "WebCDCovers.h"
#include "Splitter.h"
#include "MouseOverButton.h"
#include "ShellTreeView.h"
#include "AlphaToolbar.h"
#include "EditTab.h"

typedef enum
{
	BrowserList,
	BrowserSearch,
	BrowserImages,
	BrowserBkTextures,
	BrowserBkAlphas,
	BrowserWeb
} BrowserMode;

/////////////////////////////////////////////////////////////////////////////
// CWebCdCoversBrowser frame

class CWebCdCoversBrowser : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CWebCdCoversBrowser)

public:
	CWebCdCoversBrowser ();
	~CWebCdCoversBrowser ();

protected:

// Attributes
public:

// Operations
public:
	void SetMode (BrowserMode mode);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebCdCoversBrowser)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation

protected:
	CCdCoverCreator2Doc* m_pCurrentDoc;
	void FillAlphaList();
	void FillTexturesList ();
	void Navigate2Empty();
	void SaveWebListEntries();
	void LoadWebListEntries();
	void Init();
	void Resize();

	void DownloadImages (COVERIMAGES* pCoverImages, CString strName, CString strLetter);
	
	CString GenerateHTML (CString strTitle, COVERIMAGES* pCoverImages);
	CString GenerateHTMLImageSrc(CString strTitle, CString strFilename, CString strSizeCString = "width=160 height= 160 ");

	void DisplayControls();

	void SaveToCache();
	void LoadFromCache (CString strLetter);

	BrowserMode m_mode;

	UINT m_nTimerId;
	int m_nCancelDownloadPtr;
	bool m_arrbCancelDownload[128];
	int m_nError;

	int m_nLeftPaneWidth;
	HCURSOR m_hCurSize;

	// List
	CTreeCtrl m_wndTreeList;

	// Search
	CListCtrl m_wndListSearch;
	CAnimateCtrl m_wndAnimation;
	CButton m_wndBtnSearch;
	CEditTab m_wndEdit;
	CStatic m_wndStaticSearch;

	// Images
	CShellTreeView m_wndTreeImages;	// dir list

	// Background Textures
	CListCtrl m_wndListBkTextures;

	// Alpha Textures
	CListCtrl m_wndListBkAlpha;

	// Web
	CListCtrl m_wndListWeb;
	CMouseOverButton m_wndBtnNewLink;
	CMouseOverButton m_wndBtnDelLink;

	// Right pane: Browser
	IWebBrowser2* m_pBrowserApp;
	CWnd m_wndBrowser;
	CSplitter m_wndSplitter;

	// Tool bar
	CReBar m_wndReBar;
	CAlphaToolBar m_wndToolBar;

	// Status bar
	CProgressCtrl m_wndProgress;
	CStatusBar m_wndStatusBar;

	DWORD m_dwCurId, m_dwPrevId, m_dwDownloadItemId;
	CFont m_font;
	CImageList m_imgListSmall, m_imgListNormal;
	HTREEITEM m_htiExpand;

	HICON m_hIconFolder16, m_hIconFolder32, m_hIconWebCover;
	HICON m_hIconNewLink, m_hIconDelLink;

	// Generated message map functions
	//{{AFX_MSG(CWebCdCoversBrowser)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBrowseList();
	afx_msg void OnUpdateBrowseList(CCmdUI* pCmdUI);
	afx_msg void OnBrowseApply();
	afx_msg void OnBrowseSearch();
	afx_msg void OnUpdateBrowseSearch(CCmdUI* pCmdUI);
	afx_msg void OnItemExpandingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSearch();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBrowseImage();
	afx_msg void OnUpdateBrowseImage(CCmdUI* pCmdUI);
	afx_msg void OnBrowseBktexture();
	afx_msg void OnUpdateBrowseBktexture(CCmdUI* pCmdUI);
	afx_msg void OnBrowseBkalpha();
	afx_msg void OnUpdateBrowseBkalpha(CCmdUI* pCmdUI);
	afx_msg void OnBrowseWeb();
	afx_msg void OnUpdateBrowseWeb(CCmdUI* pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEndLabelEditListWeb(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelChangedListWeb(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNewLink();
	afx_msg void OnDelLink();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnUpdateBrowseApply(CCmdUI* pCmdUI);
	afx_msg void OnItemexpandingTreeImages(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeImages(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickTreeImages(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedBkAlphas(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemChangedBkTextures(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	afx_msg LRESULT OnGetList (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetSearch (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetImages (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSplitResize (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBCDCOVERSBROWSER1_H__556CCF6E_B9D7_4392_B317_56F1837D675D__INCLUDED_)
