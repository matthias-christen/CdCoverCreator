#if !defined(AFX_STYLEDLGMEASURES_H__DBF6B626_2DDA_4991_9588_86E51434D8B7__INCLUDED_)
#define AFX_STYLEDLGMEASURES_H__DBF6B626_2DDA_4991_9588_86E51434D8B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleDlgMeasures.h : header file
//

#include "StyleDlgBase.h"
#include "StyleTemplate.h"

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgMeasures dialog

class CStyleDlgMeasures : public CStyleDlgBase
{
// Construction
public:
	CStyleDlgMeasures(CWnd* pParent = NULL);   // standard constructor

	void SetTemplate (CStyleTemplate* pStyle, RenderType rt);
	
	void EnableTitleAlignmentEdit();
	void EnableLabelTitleEdit();

// Dialog Data
	//{{AFX_DATA(CStyleDlgMeasures)
	enum { IDD = IDD_DIALOG_STYLEMEASURES };
	CButton	m_grpVert;
	CButton	m_grpHorz;
	CStatic	m_stcTop;
	CStatic	m_stcHeight;
	CStatic	m_stcCm2;
	CStatic	m_stcCm1;
	CButton	m_btnLabelTrackTitleCenter;
	BOOL	m_bShowTitle;
	double	m_dHAlignLeft;
	double	m_dHAlignRight;
	double	m_dVAlignHeight;
	double	m_dVAlignTop;
	int		m_nLabelTitleMode;
	int		m_nTitleMargin;
	//}}AFX_DATA

	double m_dTitleVAlign;

	CStyleTemplate* m_pStyle;
	RenderType m_RenderType;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleDlgMeasures)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStyleDlgMeasures)
	afx_msg void OnRadioTitlemargincenter();
	afx_msg void OnRadioTitlemarginleft();
	afx_msg void OnRadioTitlemarginright();
	afx_msg void OnChangeEditTitlehalignleft();
	afx_msg void OnChangeEditTitlehalignright();
	afx_msg void OnChangeEditTitlevalignheight();
	afx_msg void OnChangeEditTitlevaligntop();
	afx_msg void OnCheckTitleShow();
	afx_msg void OnButtonLabeltracktitlecenter();
	afx_msg void OnRadioLabelTitleround();
	afx_msg void OnRadioLabelTitlestraight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDLGMEASURES_H__DBF6B626_2DDA_4991_9588_86E51434D8B7__INCLUDED_)
