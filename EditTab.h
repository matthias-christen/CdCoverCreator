#if !defined(AFX_EDITTAB_H__3939E3FF_97DF_4875_9633_B4E304C9494C__INCLUDED_)
#define AFX_EDITTAB_H__3939E3FF_97DF_4875_9633_B4E304C9494C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditTab.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditTab window

class CEditTab : public CEdit
{
// Construction
public:
	CEditTab();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditTab)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditTab();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditTab)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITTAB_H__3939E3FF_97DF_4875_9633_B4E304C9494C__INCLUDED_)
