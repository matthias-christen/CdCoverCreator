//*************************************************************************
//CFontSelectComboBox.h

#if !defined(AFX_FONTSELECTCOMBOBOX_H__F8528B4F_396E_11D1_9384_00A0248F6145__INCLUDED_)
#define AFX_FontSelectCOMBOBOX_H__F8528B4F_396E_11D1_9384_00A0248F6145__INCLUDED_
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CJFlatComboBox.h"

// FontSelectComboBox.h : header file
//
///////////////////////////////////////////////////////////////////////////
//
// CFontSelectComboBox window
typedef enum
{
	FONTS
} STYLE;	// Why have I enumerated, Cos, Maybe I might want something other than Fonts here

class CFontSelectComboBox : public CCJFlatComboBox
{
// Construction
public:
	CFontSelectComboBox();
	CFontSelectComboBox (STYLE);
// Attributes
public:
	void SetHilightColors (COLORREF hilight,COLORREF hilightText)
	{
		m_clrHilight = hilight;
		m_clrHilightText = hilightText;
	};
	void SetNormalColors (COLORREF clrBkgnd,COLORREF clrText)
	{
		m_clrNormalText = clrText;
		m_clrBkgnd = clrBkgnd;
	};
	static BOOL CALLBACK EnumFontProc (LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData);
	void FillFonts ();
	int GetSelFont (LOGFONT&);
	
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontSelectComboBox)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	// Implementation
public:
	virtual ~CFontSelectComboBox();
	// Generated message map functions
protected:
	STYLE m_enStyle;
	COLORREF m_clrHilight;
	COLORREF m_clrNormalText;
	COLORREF m_clrHilightText;
	COLORREF m_clrBkgnd;
	BOOL m_bInitOver;
	void DrawDefault (LPDRAWITEMSTRUCT);
	void DrawFont(LPDRAWITEMSTRUCT);
	void InitFonts ();
	//{{AFX_MSG(CFontSelectComboBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnInitFonts (WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};
///////////////////////////////////////////////////////////////////////////
//
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.
#endif //!defined(AFX_FONTSELECTCOMBOBOX_H__F8528B4F_396E_11D1_9384_00A0248F6145__INCLUDED_)
