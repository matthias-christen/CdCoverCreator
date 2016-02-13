#if !defined(AFX_HYPERLINKSTATIC_H__9B36C1F4_1FE0_4764_A8A2_48F4EE330C71__INCLUDED_)
#define AFX_HYPERLINKSTATIC_H__9B36C1F4_1FE0_4764_A8A2_48F4EE330C71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HyperlinkStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHyperlinkStatic window

class CHyperlinkStatic : public CStatic
{
// Construction
public:
	CHyperlinkStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHyperlinkStatic)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHyperlinkStatic();

	void SetCC (CString str);
	void SetBCC (CString str);
	void SetSubjectText (CString str);
	void SetBodyText (CString str);

	// Generated message map functions
protected:
	CString encode (CString s);
	CString m_strSubject;
	CString m_strCC, m_strBCC;
	CString m_strBody;

	HBRUSH m_hBkBrush;
	HCURSOR m_hLinkCursor;
	static LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	CFont* m_pFontUnderline;
	CFont* m_pFont;
	bool m_bTrackLeave;
	//{{AFX_MSG(CHyperlinkStatic)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClicked();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave (WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	static HINSTANCE GotoURL (LPCTSTR url, int showcmd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINKSTATIC_H__9B36C1F4_1FE0_4764_A8A2_48F4EE330C71__INCLUDED_)
