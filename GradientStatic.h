#if !defined(AFX_GRADIENTSTATIC_H__B4276F02_41A0_4898_B5D9_8BFEE81D307E__INCLUDED_)
#define AFX_GRADIENTSTATIC_H__B4276F02_41A0_4898_B5D9_8BFEE81D307E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GradientStatic.h : header file
//

#include "FontPreviewStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CGradientStatic window

class CGradientStatic : public CFontPreviewStatic
{
// Construction
public:
	CGradientStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetAngle();
	void SetAngle (int nAngle);
	COLORREF GetEndColor ();
	void SetEndColor (COLORREF color);
	COLORREF GetStartColor ();
	void SetStartColor (COLORREF color);
	virtual ~CGradientStatic();

	// Generated message map functions
protected:
	COLORREF m_crStart;
	COLORREF m_crEnd;
	int m_nAngle;

	//{{AFX_MSG(CGradientStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRADIENTSTATIC_H__B4276F02_41A0_4898_B5D9_8BFEE81D307E__INCLUDED_)
