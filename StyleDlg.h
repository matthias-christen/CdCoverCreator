#if !defined(AFX_STYLEDLG_H__CD2C2D2E_C6B1_4F35_B96B_21A9B1C6DC5E__INCLUDED_)
#define AFX_STYLEDLG_H__CD2C2D2E_C6B1_4F35_B96B_21A9B1C6DC5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleDlg.h : header file
//

#include "StyleDlgBase.h"
//#include "RollupWnd1.h"
#include "LinearGradientPicker.h"
#include "DegreesStatic.h"

#include "StyleTemplate.h"
#include "StdAfx.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgGradient dialog

class CStyleDlgGradient : public CStyleDlgBase
{
// Construction
public:
	CStyleDlgGradient(CWnd* pParent = NULL);   // standard constructor
	void SetTemplate (CStyleTemplate* pStyle, RenderType rt);

// Dialog Data
	//{{AFX_DATA(CStyleDlgGradient)
	enum { IDD = IDD_DIALOG_STYLE };
	CEdit	m_editAngle;
	CLinearGradientPicker	m_BkColor;
	CSpinButtonCtrl	m_spinAngle;
	CDegreesStatic	m_dgAngle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleDlgGradient)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	RenderType m_RenderType;
	bool m_bDegChanged;
	CStyleTemplate* m_pStyle;

	// Generated message map functions
	afx_msg void OnGradient ();
	afx_msg void OnDegm();
	afx_msg void OnDegmBtnUp();
	afx_msg void OnChangeEditDeg();
	afx_msg LRESULT OnButtonColor(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDLG_H__CD2C2D2E_C6B1_4F35_B96B_21A9B1C6DC5E__INCLUDED_)
