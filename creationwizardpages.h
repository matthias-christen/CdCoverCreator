// CreationWizardPages.h : header file
//

#ifndef __CREATIONWIZARDPAGES_H__
#define __CREATIONWIZARDPAGES_H__

#include "EditableListCtrl.h"
#include "MouseOverButton.h"
#include "CJFlatComboBox.h"
#include "GradientStatic.h"
#include "AutoRichEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CCWArrangePage dialog

#include "Picture.h"	// Added by ClassView
#include "TracksInfo.h"	// Added by ClassView
#include "BackgroundInfo.h"
#include "StyleTemplate.h"
#include "StdAfx.h"	// Added by ClassView

class CCWArrangePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCWArrangePage)

// Construction
public:
	void SetTracksInfo (CTracksInfo& tracks);
	CTracksInfo& GetTracksInfo();
	CCWArrangePage();
	~CCWArrangePage();

// Dialog Data
	//{{AFX_DATA(CCWArrangePage)
	enum { IDD = IDD_PROPPAGE_CW_ARRANGETITLES };
	CAutoRichEditCtrl	m_EditTitle;
	CEditableListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCWArrangePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetTracks();
	CTracksInfo* m_pTracks;

	void DeleteGrouping();
	void AddGrouping();

	CImageList m_ImageList;
	HICON m_hIconSep, m_hIconTrack;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnRemoveRemarks ();
	afx_msg void OnButtonAddgrp();
	afx_msg void OnRclickListTracks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddgrouping();
	afx_msg void OnDeletegrouping();
	afx_msg void OnButtonRemovegrp();
	afx_msg void OnItemchangedListTracks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonCddb();
	afx_msg void OnButtonCdText ();
	afx_msg void OnButtonSpecialFunctions ();
	afx_msg void OnMatchMerge ();
	afx_msg void OnCopyToClipboard ();
	afx_msg void OnExportHTML ();
	afx_msg void OnBegindragListTracks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeletetrack();
	afx_msg LRESULT OnEnddragListTracks(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CCWBkImgPage dialog

class CCWBkImgPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCWBkImgPage)

// Construction
public:
	RenderType m_nBkImageType;
	void SetBackgroundInfo (CBackgroundInfo& bkgrnd);
	CBackgroundInfo& GetBackgroundInfo();
	CCWBkImgPage();
	~CCWBkImgPage();

// Dialog Data
	//{{AFX_DATA(CCWBkImgPage)
	enum { IDD = IDD_PROPPAGE_CW_BACKGROUND };
	CMouseOverButton	m_btnScan;
	CMouseOverButton	m_btnRemoveTransparent;
	CMouseOverButton	m_btnSetTransparent;
	CGradientStatic	m_StaticTransparentCol;
	CMouseOverButton	m_BrowseBkImg;
	CString	m_strImgFilename;
	int		m_enumMode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCWBkImgPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	COLORREF m_crTransparent;
	bool m_bSettingTransparentColor;
	CBackgroundInfo* m_pBackgroundInfo;
	CPicture* m_pPic;

	HICON m_hIconBrowse, m_hIconScan;
	HICON m_hIconSetTransparent, m_hIconRemoveTransparent;
	HICON m_hIconTL, m_hIconTC, m_hIconTR, m_hIconCL, m_hIconCC, m_hIconCR, m_hIconBL, m_hIconBC, m_hIconBR;



	HCURSOR m_hPickCursor;

	// Generated message map functions
	//{{AFX_MSG(CCWBkImgPage)
	afx_msg void OnBrowseBkImg();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTransparentcolor();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonRemovetransparent();
	afx_msg void OnScan();
	//}}AFX_MSG
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnButtonPreview();
	DECLARE_MESSAGE_MAP()

};

class CCWFormatPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCWFormatPage)

// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetStyleTemplate (CStyleTemplate& style);
	CStyleTemplate& GetStyleTemplate();
	CCWFormatPage();
	~CCWFormatPage();

// Dialog Data
	//{{AFX_DATA(CCWFormatPage)
	enum { IDD = IDD_PROPPAGE_CW_SELECTSTYLE };
	CTabCtrl	m_Tab;
	CCJFlatComboBox	m_Combo;
	CMouseOverButton	m_ButtonStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCWFormatPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	RenderType m_RenderType;

	HICON m_hIconStyle;

	// Generated message map functions
	//{{AFX_MSG(CCWFormatPage)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboStyle();
	afx_msg void OnButtonStyle();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CStyleTemplate* m_pStyle;
};


#endif // __CREATIONWIZARDPAGES_H__
