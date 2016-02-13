#if !defined(AFX_STYLEDLGTRACKGROUPINGS_H__ACF8C5FD_C0F3_448D_9629_2B9E45F62665__INCLUDED_)
#define AFX_STYLEDLGTRACKGROUPINGS_H__ACF8C5FD_C0F3_448D_9629_2B9E45F62665__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleDlgTrackgroupings.h : header file
//

#include "StyleDlgBase.h"
#include "StyleTemplate.h"
#include "StdAfx.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgTrackgroupings dialog

class CStyleDlgTrackgroupings : public CStyleDlgBase
{
// Construction
public:
	void EnableVAlignmentEdit();
	void EnableHAlignmentEdit();
	void SetTemplate (CStyleTemplate* pStyle, RenderType rt);
	CStyleDlgTrackgroupings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStyleDlgTrackgroupings)
	enum { IDD = IDD_DIALOG_STYLETRACKGROUPINGS };
	CButton	m_btnHAlignCenter;
	BOOL	m_bShowTrackGroupings;
	BOOL	m_bShowTracks;
	double	m_dHAlignLeft;
	double	m_dHAlignRight;
	double	m_dVAlignBottom;
	double	m_dVAlignTop;
	int		m_nHAlign;
	int		m_nTextMode;
	int		m_nVAlign;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleDlgTrackgroupings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	RenderType m_RenderType;
	CStyleTemplate* m_pStyle;

	// Generated message map functions
	//{{AFX_MSG(CStyleDlgTrackgroupings)
	afx_msg void OnButtonHalignCenter();
	afx_msg void OnCheckShowgroupings();
	afx_msg void OnCheckShowtracks();
	afx_msg void OnChangeEditHalignLeft();
	afx_msg void OnChangeEditHalignright();
	afx_msg void OnChangeEditValignBottom();
	afx_msg void OnChangeEditValignTop();
	afx_msg void OnRadioHalignCenter();
	afx_msg void OnRadioHalignLeft();
	afx_msg void OnRadioHalignRight();
	afx_msg void OnRadioRoundtext();
	afx_msg void OnRadioStraighttext();
	afx_msg void OnRadioValignBottom();
	afx_msg void OnRadioValignCenter();
	afx_msg void OnRadioValignTop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDLGTRACKGROUPINGS_H__ACF8C5FD_C0F3_448D_9629_2B9E45F62665__INCLUDED_)
