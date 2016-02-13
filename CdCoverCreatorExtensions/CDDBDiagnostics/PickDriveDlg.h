#if !defined(AFX_PICKDRIVEDLG_H__010EAFBC_8008_47C5_9116_5ED9735B488A__INCLUDED_)
#define AFX_PICKDRIVEDLG_H__010EAFBC_8008_47C5_9116_5ED9735B488A__INCLUDED_

#include "CDDBDiagnostics.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PickDriveDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPickDriveDlg dialog

class CPickDriveDlg : public CDialog
{
// Construction
public:
	CPickDriveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPickDriveDlg)
	enum { IDD = IDD_DIALOG_PICKDRIVE };
	CListCtrl	m_List;
	//}}AFX_DATA

	CStringArray* m_pArrDrives;
	CString m_strSelectedDrive;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPickDriveDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_FileImageList;

	// Generated message map functions
	//{{AFX_MSG(CPickDriveDlg)
	afx_msg void OnDblclkListDrives(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnItemchangedListDrives(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICKDRIVEDLG_H__010EAFBC_8008_47C5_9116_5ED9735B488A__INCLUDED_)
