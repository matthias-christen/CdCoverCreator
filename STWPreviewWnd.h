// STWPreviewWnd.h : header file
//
// This file contains the preview window used by the 
// CStyleTemplateWizard property sheet.

/////////////////////////////////////////////////////////////////////////////
// CSTWPreviewWnd window

#ifndef __STWPREVIEWWND_H__
#define __STWPREVIEWWND_H__

#include "TracksInfo.h"	// Added by ClassView
#include "BackgroundInfo.h"	// Added by ClassView
class CSTWPreviewWnd : public CWnd
{
// Construction
public:
	CSTWPreviewWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTWPreviewWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSTWPreviewWnd();

	// Generated message map functions
protected:
	CRect m_rcCurMark;
	void Mark (CDC* pDC, CRect r, bool bErase = false);
	CBackgroundInfo* m_pBkgrnd[NUM_RENDER_TYPES];
	CTracksInfo m_tracks;

	int m_yCover, m_yLabel, m_yInlay;

	//{{AFX_MSG(CSTWPreviewWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif		// __STWPREVIEWWND_H__
