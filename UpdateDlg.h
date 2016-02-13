#if !defined(AFX_UPDATEDLG_H__D0AE1D5C_6257_4261_A306_27AA21AB4C20__INCLUDED_)
#define AFX_UPDATEDLG_H__D0AE1D5C_6257_4261_A306_27AA21AB4C20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpdateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpdateDlg dialog

class CUpdateDlg : public CDialog
{
// Construction
public:
	CUpdateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUpdateDlg)
	enum { IDD = IDD_DIALOG_UPDATE };
	CString	m_strVersion;
	CString	m_strNewFeatures;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpdateDlg)
	afx_msg void OnButtonSkip();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATEDLG_H__D0AE1D5C_6257_4261_A306_27AA21AB4C20__INCLUDED_)
