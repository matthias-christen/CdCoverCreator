#if !defined(AFX_TRACKEDIT_H__FF5C7F8C_DA35_4B93_B52B_580CB2EADBD5__INCLUDED_)
#define AFX_TRACKEDIT_H__FF5C7F8C_DA35_4B93_B52B_580CB2EADBD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackEdit.h : header file
//

#include "AutoRichEditCtrl.h"

#define EN_ESCAPE	0x0701
#define EN_UP		0x0702
#define EN_DOWN		0x0703
#define EN_PREV		0x0704
#define EN_NEXT		0x0705

/////////////////////////////////////////////////////////////////////////////
// CTrackEdit window

class CTrackEdit : public CAutoRichEditCtrl
{
// Construction
public:
	CTrackEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrackEdit();

	void SetText (CString strText);
	CString GetText();
	void SetAppearance (COLORREF col, LOGFONT* pLogfont, DWORD dwAlign);

	// Generated message map functions
protected:
	COLORREF m_col;
	CFont m_font;
	//{{AFX_MSG(CTrackEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKEDIT_H__FF5C7F8C_DA35_4B93_B52B_580CB2EADBD5__INCLUDED_)
