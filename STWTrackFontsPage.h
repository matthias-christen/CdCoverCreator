// STWTrackFontsPage.h : header file 
//

#ifndef __STWTRACKFONTSPAGE_H__
#define __STWTRACKFONTSPAGE_H__

#include "FontPreviewStatic.h"
#include "MouseOverButton.h"

/////////////////////////////////////////////////////////////////////////////
// CSTWTrackFontsPage dialog

class CSTWTrackFontsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSTWTrackFontsPage)

// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSTWTrackFontsPage();
	~CSTWTrackFontsPage();

// Dialog Data
	//{{AFX_DATA(CSTWTrackFontsPage)
	enum { IDD = IDD_PROPPAGE_STW_CHOOSETRACKFONTS };
	CMouseOverButton	m_btnTrackTitleFont;
	CMouseOverButton	m_btnTrackTitleColor;
	CMouseOverButton	m_btnTrackRemFont;
	CMouseOverButton	m_btnTrackRemColor;
	CMouseOverButton	m_btnTrackNumFont;
	CMouseOverButton	m_btnTrackNumColor;
	CMouseOverButton	m_btnTrackLenFont;
	CMouseOverButton	m_btnTrackLenColor;
	CMouseOverButton	m_btnTrackGrpTitleFont;
	CMouseOverButton	m_btnTrackGrpTitleColor;
	CMouseOverButton	m_btnTrackGrpRemFont;
	CMouseOverButton	m_btnTrackGrpRemColor;
	CFontPreviewStatic	m_TrackTitleFont;
	CFontPreviewStatic	m_TrackRemarksFont;
	CFontPreviewStatic	m_TrackNumFont;
	CFontPreviewStatic	m_TrackLengthFont;
	CFontPreviewStatic	m_TrackGroupingTitleFont;
	CFontPreviewStatic	m_TrackGroupingRemarksFont;
	//}}AFX_DATA								 

	struct tagFont
	{
		LOGFONT logfont;
		COLORREF color;
	} m_fntTrackTitle, m_fntTrackRemarks, m_fntTrackNum, m_fntTrackLength,
		m_fntTrackGroupingTitle, m_fntTrackGroupingRemarks;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSTWTrackFontsPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	HICON m_hIconFont, m_hIconColor;

	// Generated message map functions
	//{{AFX_MSG(CSTWTrackFontsPage)
	afx_msg void OnButtonTrackNumfont();
	afx_msg void OnButtonTrackLengthfont();
	afx_msg void OnButtonTrackTitlefont();
	afx_msg void OnButtonTrackRemarksfont();
	afx_msg void OnButtonTrackGroupingTitlefont();
	afx_msg void OnButtonTrackGroupingRemarksfont();
	afx_msg void OnButtonTrackNumcolor();
	afx_msg void OnButtonTrackLengthcolor();
	afx_msg void OnButtonTrackTitlecolor();
	afx_msg void OnButtonTrackRemarkscolor();
	afx_msg void OnButtonTrackGroupingTitlecolor();
	afx_msg void OnButtonTrackGroupingRemarkscolor();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __STWTRACKFONTSPAGE_H__
