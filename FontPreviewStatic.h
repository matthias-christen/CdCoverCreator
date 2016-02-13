#if !defined(AFX_FONTPREVIEWSTATIC_H__FD611232_B215_4F12_9FD7_37C688EF9E28__INCLUDED_)
#define AFX_FONTPREVIEWSTATIC_H__FD611232_B215_4F12_9FD7_37C688EF9E28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FontPreviewStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFontPreviewStatic window

class CFontPreviewStatic : public CStatic
{
// Construction
public:
	CFontPreviewStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontPreviewStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor (COLORREF color);
	void SetPreviewFont (LOGFONT* pLf);
	LOGFONT* GetLogfont();
	void SetPreviewFont (CHOOSEFONT* pCf);
	virtual ~CFontPreviewStatic();

	inline COLORREF GetColor () { return m_color; }

	// Generated message map functions
protected:
	LOGFONT m_lf;
	HBRUSH m_hBkBrush;
	CFont* m_pFont;
	COLORREF m_color;

	//{{AFX_MSG(CFontPreviewStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FONTPREVIEWSTATIC_H__FD611232_B215_4F12_9FD7_37C688EF9E28__INCLUDED_)
