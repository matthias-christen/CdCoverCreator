#if !defined(AFX_STWTITLEFONTSPAGE_H__5F4CCEB5_8A59_4770_A286_7B2F618E9F86__INCLUDED_)
#define AFX_STWTITLEFONTSPAGE_H__5F4CCEB5_8A59_4770_A286_7B2F618E9F86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STWTitleFontsPage.h : header file
//

#include "MouseOverButton.h"
#include "FontPreviewStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CSTWTitleFontsPage dialog

class CSTWTitleFontsPage : public CPropertyPage
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSTWTitleFontsPage ();   // standard constructor
	~CSTWTitleFontsPage ();

// Dialog Data
	//{{AFX_DATA(CSTWTitleFontsPage)
	enum { IDD = IDD_PROPPAGE_STW_CHOOSETITLEFONTS };
	CFontPreviewStatic	m_LabelFont;
	CFontPreviewStatic	m_InlayTitleFont;
	CFontPreviewStatic	m_InlaySideFont;
	CFontPreviewStatic	m_CoverFont;
	CMouseOverButton	m_btnLabelFont;
	CMouseOverButton	m_btnLabelColor;
	CMouseOverButton	m_btnInlayTitleFont;
	CMouseOverButton	m_btnInlayTitleColor;
	CMouseOverButton	m_btnInlaySideFont;
	CMouseOverButton	m_btnInlaySideColor;
	CMouseOverButton	m_btnCoverFont;
	CMouseOverButton	m_btnCoverColor;
	//}}AFX_DATA

	struct tagFont
	{
		LOGFONT logfont;
		COLORREF color;
	} m_fntCover, m_fntLabel, m_fntInlayTitle, m_fntInlaySide;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTWTitleFontsPage)
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
	//{{AFX_MSG(CSTWTitleFontsPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTitlefontCoverfont();
	afx_msg void OnButtonTitlefontLabelfont();
	afx_msg void OnButtonTitlefontInlaysidefont();
	afx_msg void OnButtonTitlefontInlaytitlefont();
	afx_msg void OnButtonTitlefontCovercolor();
	afx_msg void OnButtonTitlefontLabelcolor();
	afx_msg void OnButtonTitlefontInlaysidecolor();
	afx_msg void OnButtonTitlefontInlaytitlecolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STWTITLEFONTSPAGE_H__5F4CCEB5_8A59_4770_A286_7B2F618E9F86__INCLUDED_)
