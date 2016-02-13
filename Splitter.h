#if !defined(AFX_SPLITTER_H__FA429BE7_C8DB_4EFE_AADD_F82173F92E61__INCLUDED_)
#define AFX_SPLITTER_H__FA429BE7_C8DB_4EFE_AADD_F82173F92E61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Splitter.h : header file
//

#define SPLIT_RESIZE WM_USER+0x0100

/////////////////////////////////////////////////////////////////////////////
// CSplitter window

class CSplitter : public CStatic
{
// Construction
public:
	CSplitter();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitter)
	//}}AFX_VIRTUAL

// Implementation
public:
	CRect m_rectTracker;
	virtual ~CSplitter();

	// Generated message map functions
protected:
	void OnInvertTracker(const CRect& rect);
	//{{AFX_MSG(CSplitter)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITTER_H__FA429BE7_C8DB_4EFE_AADD_F82173F92E61__INCLUDED_)
