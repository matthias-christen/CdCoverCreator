#if !defined(AFX_STWBACKGROUNDCOLORSPAGE_H__7F714ECE_D74F_4235_A1A2_F4F0B32BEDC3__INCLUDED_)
#define AFX_STWBACKGROUNDCOLORSPAGE_H__7F714ECE_D74F_4235_A1A2_F4F0B32BEDC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STWBackgroundColorsPage.h : header file
//

#include "GradientStatic.h"
#include "DegreesStatic.h"
#include "MouseOverButton.h"
#include "ColourPicker.h"
#include "LinearGradientPicker.h"

/////////////////////////////////////////////////////////////////////////////
// CSTWBackgroundColorsPage dialog

class CSTWBackgroundColorsPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSTWBackgroundColorsPage)

// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSTWBackgroundColorsPage();
	~CSTWBackgroundColorsPage();

// Dialog Data
	//{{AFX_DATA(CSTWBackgroundColorsPage)
	enum { IDD = IDD_PROPPAGE_STW_CHOOSEBKCOLORS };
	CSpinButtonCtrl	m_SpinDegLabel;
	CSpinButtonCtrl	m_SpinDegInlay;
	CSpinButtonCtrl	m_SpinDegCover;
	CSpinButtonCtrl	m_SpinDegBack;
	CEdit	m_EditDegLabel;
	CEdit	m_EditDegInlay;
	CEdit	m_EditDegCover;
	CEdit	m_EditDegBack;
	CDegreesStatic	m_DegLabel;
	CDegreesStatic	m_DegInlay;
	CDegreesStatic	m_DegCover;
	CDegreesStatic	m_DegBack;
	CLinearGradientPicker m_BkColLabel;
	CLinearGradientPicker m_BkColInlay;
	CLinearGradientPicker	m_BkColCover;
	CLinearGradientPicker	m_BkColBack;
	//}}AFX_DATA

	/*
	CMouseOverButton	m_ButtonColLabel2;
	CMouseOverButton	m_ButtonColInlay2;
	CMouseOverButton	m_ButtonColCover2;
	CMouseOverButton	m_ButtonColBack2;

	CMouseOverButton	m_ButtonColLabel;
	CMouseOverButton	m_ButtonColInlay;
	CMouseOverButton	m_ButtonColCover;
	CMouseOverButton	m_ButtonColBack;
	*/

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSTWBackgroundColorsPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	// Generated message map functions
	//{{AFX_MSG(CSTWBackgroundColorsPage)
	virtual BOOL OnInitDialog();

	afx_msg void OnBkColorLabel();
	afx_msg void OnBkColorInlay();
	afx_msg void OnBkColorCover();
	afx_msg void OnBkColorBack();

	afx_msg void OnDegmBack();
	afx_msg void OnDegmCover();
	afx_msg void OnDegmLabel();
	afx_msg void OnDegmInlay();

	afx_msg void OnDegmBackBtnUp();
	afx_msg void OnDegmCoverBtnUp();
	afx_msg void OnDegmLabelBtnUp();
	afx_msg void OnDegmInlayBtnUp();

	afx_msg void OnChangeEditDegcover();
	afx_msg void OnChangeEditDegback();
	afx_msg void OnChangeEditDeglabel();
	afx_msg void OnChangeEditDeginlay();
	//}}AFX_MSG
	afx_msg LRESULT OnButtonColor(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	bool m_bDegCoverChanged;
	bool m_bDegBackChanged;
	bool m_bDegLabelChanged;
	bool m_bDegInlayChanged;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STWBACKGROUNDCOLORSPAGE_H__7F714ECE_D74F_4235_A1A2_F4F0B32BEDC3__INCLUDED_)
