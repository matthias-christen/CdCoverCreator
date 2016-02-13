#if !defined(AFX_LEFTPANELDLGBAR_H__DB316A8A_F210_463C_88A9_C68297E02F48__INCLUDED_)
#define AFX_LEFTPANELDLGBAR_H__DB316A8A_F210_463C_88A9_C68297E02F48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftPanelDlgBar.h : header file
//

#include "CdCoverCreator2Doc.h"

/////////////////////////////////////////////////////////////////////////////
// CLeftPanelDlgBar dialog

class CLeftPanelDlgBar : public CDialogBar
{
// Construction
public:
	CLeftPanelDlgBar(CWnd* pParent = NULL);   // standard constructor
	~CLeftPanelDlgBar ();

	virtual CSize CalcFixedLayout (BOOL bStretch, BOOL bHorz);

// Dialog Data
	//{{AFX_DATA(CLeftPanelDlgBar)
	enum { IDD = IDD_LEFTPANEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void Update (bool bRepaintBackground = false);

	bool IsLastDocValid();
	CCdCoverCreator2Doc* m_pLastDoc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftPanelDlgBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DrawBooklet (CDC* pDC, int w, int h, CCdCoverCreator2Doc* pDoc, RenderType rt);
	void Mark (CDC *pDC, CRect r, bool bErase = false);

	CRect m_rcCurMark;
	int m_y[NUM_RENDER_TYPES];

	// Generated message map functions
	//{{AFX_MSG(CLeftPanelDlgBar)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	bool m_bRedraw;
	CBitmap m_Bitmap[NUM_RENDER_TYPES];

#ifdef PEROUTKA_FEDORISIN
	CBitmap m_bmpLogo;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTPANELDLGBAR_H__DB316A8A_F210_463C_88A9_C68297E02F48__INCLUDED_)
