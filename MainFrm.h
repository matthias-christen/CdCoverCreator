// MainFrm.h : interface of the CMainFrame class
//

#include "MainFrmDlgBar.h"
#include "AlphaToolbar.h"
#include "Menubar.h"
//#include "BCMenu.h"
#include "cccTwain.h"	// Added by ClassView
#include "WebCDCovers.h"
#include "RollupWnd1.h"
#include "RollupCtrl.h"
#include "StyleDlg.h"
#include "RollupCtrl.h"	// Added by ClassView
#include "LeftPanelDlgBar.h"	// Added by ClassView
#include "StyleDlgBkImage.h"	// Added by ClassView
#include "StyleDlgAlpha.h"	// Added by ClassView
#include "RTFFormatToolbar.h"	// Added by ClassView
#include "StyleDlgFonts.h"	// Added by ClassView
#include "WebCdCoversBrowser1.h"	// Added by ClassView
#include "StyleDlgMeasures.h"
#include "StyleDlgTrackgroupings.h"
#include "StyleDlgTracks.h"
#include "StyleDlgSpines.h"
#include "DlgBarSelectMedium.h"

#include <Gdiplus.h>

#pragma once
class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	//BCMenu m_menu, m_menuDefault;
	CStatusBar* GetStatusBar () { return &m_wndStatusBar; }
	CProgressCtrl* GetProgressBar () { return &m_wndProgress; }

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CImageList m_ilSystemImageList;
	//Gdiplus::Bitmap* m_pBmpSystemImageList;
	CcccTwain m_twain;

	CWnd* m_pActivateCoverBrowser;

	void ListStyles (CString strStyle) {
		m_wndDlgBar.ListStyles ();
		m_wndDlgBar.SetStyle (strStyle);
	}

	CLeftPanelDlgBar m_wndLeftPanel;
	CRollupWnd* m_pWndRollup;
	CRollupCtrl m_wndRollupCtrl;

	CRTFFormatToolbar m_wndRTFToolbar;

	CStyleDlgGradient m_wndStyleGradient;
	CStyleDlgBkImage m_wndStyleBkImg;
	CStyleDlgFonts m_wndStyleFonts;
	CStyleDlgAlpha m_wndStyleAlpha;
	CStyleDlgMeasures m_wndStyleMeasures;
	CStyleDlgTrackgroupings m_wndStyleTrackgroupings;
	CStyleDlgTracks m_wndStyleTracks;
	CStyleDlgSpines m_wndStyleSpines;
	
	CDlgBarSelectMedium m_wndDlgBarSelectMedium;

	CWebCDCovers* GetWebCDCovers();

	BrowserMode m_bmCoverBrowserMode;
	void UpdateStyleDlgs (CStyleTemplate* pStyle, CBackgroundInfo* pBk, RenderType rt);

protected:  // control bar embedded members
	void PrepareSysImageList ();

	CStatusBar m_wndStatusBar;
	CProgressCtrl m_wndProgress;

//	CToolBar m_wndToolBar;
	CMainFrmDlgBar m_wndDlgBar;
	CReBar m_wndReBar;

	CMenuBar m_wndMenuBar;
	CAlphaToolBar m_wndToolBar, m_wndToolBarWindow;

	CWebCDCovers* m_pWebCdCovers;

	UINT m_nTimerId;
	bool m_bTimerSet;
	DWORD m_dwLastWindowPosChange;
	bool m_bInitPosition;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	afx_msg void OnStyletemplate();
	afx_msg void OnClose();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewStyles();
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewLeftpanel();
	afx_msg void OnUpdateViewLeftpanel(CCmdUI* pCmdUI);
	afx_msg void OnViewTextToolbar ();
	afx_msg void OnUpdateViewTextToolbar (CCmdUI* pCmdUI);
	afx_msg void OnViewLyricsbrowser();
	afx_msg void OnRtffBold();
	afx_msg void OnRtffItalic();
	afx_msg void OnRtffUnderline();
	afx_msg void OnRtffUnderlineDot();
	afx_msg void OnRtffUnderlineDouble();
	afx_msg void OnRtffStrikethru();
	afx_msg void OnRtffAlignleft();
	afx_msg void OnRtffAligncenter();
	afx_msg void OnRtffAlignright();
	afx_msg void OnRtffSup();
	afx_msg void OnRtffSub();
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditSelectAll();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnRtffBorder();
	afx_msg void OnRtffPagebreak();
	afx_msg void OnRtffBullets();
	afx_msg void OnRtffBorderStyle (UINT nId);
	afx_msg void OnRtffBulletsStyle (UINT nId);
public:
	afx_msg void OnViewCoverbrowser();
	afx_msg void OnFilePageSetup();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnWindowNextwindow();
	afx_msg void OnWindowPreviouswindow();
	afx_msg void OnUpdateWindowNextwindow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateWindowPreviouswindow(CCmdUI *pCmdUI);
};


