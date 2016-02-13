#if !defined(AFX_ROLLUPWND1_H__E807471F_1935_41A9_92C9_7480A5686E5E__INCLUDED_)
#define AFX_ROLLUPWND1_H__E807471F_1935_41A9_92C9_7480A5686E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RollupWnd1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRollupWnd dialog

class CRollupWnd : public CDialog
{
// Construction
public:
	CRollupWnd(UINT nId, CWnd* pParent = NULL);   // standard constructor
	~CRollupWnd ();

	void CalcRelPosition();
	void Reposition();

// Dialog Data
	//{{AFX_DATA(CRollupWnd)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRollupWnd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool DrawCloseupButton(CPoint pt = CPoint (-1, -1));
	CPoint m_ptRelCoords;
	int m_nArea;
	bool m_bIsExpanded;
	bool m_bIsExpanding;
	CRect m_rectSize;
	UINT m_nIdTimer;
	bool m_bTrackLeave;
	bool m_bCollapseBtnClicked;
	void Collapse();
	void Expand();

	CFont m_fntCloseUpBtn;

	// Generated message map functions
	//{{AFX_MSG(CRollupWnd)
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnNcPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave (WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNcMouseLeave (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CSize GetCaptionSize() const;
	CSize GetFrameSize() const;
	CRect CalculateCloseupButtonRect();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROLLUPWND1_H__E807471F_1935_41A9_92C9_7480A5686E5E__INCLUDED_)
