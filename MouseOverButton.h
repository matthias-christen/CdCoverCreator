#if !defined(AFX_MOUSEOVERBUTTON_H__5FE9CA5D_06A1_462C_950A_AB18BFD6A84E__INCLUDED_)
#define AFX_MOUSEOVERBUTTON_H__5FE9CA5D_06A1_462C_950A_AB18BFD6A84E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MouseOverButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMouseOverButton window

class CMouseOverButton : public CButton
{
// Construction
public:
	CMouseOverButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMouseOverButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetIcon (HICON hIcon);
	virtual ~CMouseOverButton();

	// Generated message map functions
protected:
	HICON m_hIcon;
	bool m_bTrackLeave;
	bool m_bButtonDown;

	//{{AFX_MSG(CMouseOverButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave (WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOUSEOVERBUTTON_H__5FE9CA5D_06A1_462C_950A_AB18BFD6A84E__INCLUDED_)
