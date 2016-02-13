#if !defined(AFX_MAINFRMDLGBAR_H__D2B91733_3F93_48D0_AE4D_E4F686E25BBC__INCLUDED_)
#define AFX_MAINFRMDLGBAR_H__D2B91733_3F93_48D0_AE4D_E4F686E25BBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

// MainFrmDlgBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainFrmDlgBar dialog

class CMainFrmDlgBar : public CDialogBar
{
// Construction
public:
	CString GetCurSel();
	void ListStyles();
	void SetStyle (CString strStyle);
	CMainFrmDlgBar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainFrmDlgBar)
	enum { IDD = IDR_MAINFRAME };
	CComboBox	m_Combo;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrmDlgBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainFrmDlgBar)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnSelchangeComboStyle ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMDLGBAR_H__D2B91733_3F93_48D0_AE4D_E4F686E25BBC__INCLUDED_)
