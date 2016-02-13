#if !defined(AFX_RTFFORMATTOOLBAR_H__A31A4AD9_AC16_461D_B5FA_001C2231F055__INCLUDED_)
#define AFX_RTFFORMATTOOLBAR_H__A31A4AD9_AC16_461D_B5FA_001C2231F055__INCLUDED_

#include "CJFlatComboBox.h"	// Added by ClassView
#include "ColourPicker.h"	// Added by ClassView
#include "FontSelectComboBox.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RTFFormatToolbar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRTFFormatToolbar window

class CRTFFormatToolbar : public CToolBar
{
// Construction
public:
	CRTFFormatToolbar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRTFFormatToolbar)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRTFControl (CRichEditCtrl* pEdit);
	CColourPicker m_wndColorPicker, m_wndColorPickerBk;
	CCJFlatComboBox m_wndComboFontSize;
	CFontSelectComboBox m_wndComboFont;
	bool CreateBar (CWnd* pParentWnd, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP, UINT nID = AFX_IDW_TOOLBAR);
	virtual ~CRTFFormatToolbar();

	inline CRichEditCtrl* GetRTFControl () { return m_pEdit; }

	// Generated message map functions
protected:
	CFont m_font;
	CRichEditCtrl* m_pEdit;

public:
	void OnUpdateMenuBullets (CCmdUI* pCmdUI);
	void OnBulletsStyle (UINT nId);
	void OnBorderStyle (UINT nId);
	void OnBorderArrow();
	void OnBulletsArrow();
	void OnUnderline();
	void OnUnderlineDot();
//	void OnUnderlineDouble();
	void OnAlignleft();
	void OnAligncenter();
	void OnAlignright();
	void OnStrikethru();
	void OnSuperScript();
	void OnSubScript();
	void OnBold();
	void OnItalic();
	void OnBullets();
	void OnBorder();
	void OnPagebreak();
protected:
	void UpdateVisibility();
	//{{AFX_MSG(CRTFFormatToolbar)
	afx_msg void OnFontChanged();
	afx_msg void OnFontSizeChanged();
	afx_msg void OnFontSizeSelendok ();
	afx_msg void OnFontSizeSelendcancel ();
	afx_msg void OnFontSizeEditChange ();
	afx_msg LRESULT OnColorChanged(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateBold(CCmdUI* pCmdUI);
	afx_msg void OnUpdateItalic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUnderline(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUnderlineDot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStrikethru(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignleft(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAligncenter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlignright(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSub(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFont(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFontsize(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBkColor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBorder(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBullets(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RTFFORMATTOOLBAR_H__A31A4AD9_AC16_461D_B5FA_001C2231F055__INCLUDED_)
