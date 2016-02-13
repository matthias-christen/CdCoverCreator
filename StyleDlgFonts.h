#if !defined(AFX_STYLEDLGFONTS_H__3A15FF43_6EA3_4D06_B4A2_7B6564C3FFDD__INCLUDED_)
#define AFX_STYLEDLGFONTS_H__3A15FF43_6EA3_4D06_B4A2_7B6564C3FFDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleDlgFonts.h : header file
//

#include "StyleDlgBase.h"
#include "FontPreviewStatic.h"
#include "MouseOverButton.h"
#include "ColourPicker.h"
#include "StyleTemplate.h"

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgFonts dialog

class CStyleDlgFonts : public CStyleDlgBase
{
// Construction
public:
	void SetTemplate (CStyleTemplate* pStyle, RenderType rt);
	CStyleDlgFonts(CWnd* pParent = NULL);   // standard constructor
	~CStyleDlgFonts ();

// Dialog Data
	//{{AFX_DATA(CStyleDlgFonts)
	enum { IDD = IDD_DIALOG_STYLEFONTS };
	CMouseOverButton	m_btnFntTrkTitle;
	CColourPicker		m_btnColTrkTitle;
	CMouseOverButton	m_btnFntTrkRemarks;
	CColourPicker		m_btnColTrkRemarks;
	CMouseOverButton	m_btnFntTrkNum;
	CColourPicker		m_btnColTrkNum;
	CMouseOverButton	m_btnFntTrkLength;
	CColourPicker		m_btnColTrkLength;
	CMouseOverButton	m_btnFntTrkGrpTitle;
	CColourPicker		m_btnColTrkGrpTitle;
	CMouseOverButton	m_btnFntTrkGrpRemarks;
	CColourPicker		m_btnColTrkGrpRemarks;
	CMouseOverButton	m_btnFont;
	CColourPicker		m_btnColor;
	CFontPreviewStatic	m_stcFont;
	CFontPreviewStatic	m_stcTrkTitle;
	CFontPreviewStatic	m_stcTrkRemarks;
	CFontPreviewStatic	m_stcTrkNum;
	CFontPreviewStatic	m_stcTrkLength;
	CFontPreviewStatic	m_stcTrkGrpTitle;
	CFontPreviewStatic	m_stcTrkgpRemarks;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleDlgFonts)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	RenderType m_RenderType;
	CStyleTemplate* m_pStyle;

	HICON m_hIconFont;
	LOGFONT m_lfDisabled;

	// Generated message map functions
	//{{AFX_MSG(CStyleDlgFonts)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFont();
	afx_msg void OnButtonTracknumFont();
	afx_msg void OnButtonTracktitleFont();
	afx_msg void OnButtonTrackremarksFont();
	afx_msg void OnButtonTracklengthFont();
	afx_msg void OnButtonTrackgroupingtitleFont();
	afx_msg void OnButtonTrackgroupingremarksFont();
	//}}AFX_MSG
	afx_msg LRESULT OnButtonColor (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDLGFONTS_H__3A15FF43_6EA3_4D06_B4A2_7B6564C3FFDD__INCLUDED_)
