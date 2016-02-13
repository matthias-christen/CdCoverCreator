#if !defined(AFX_PICKTITLEDLG_H__01082977_5905_4F98_9B48_D4AF72F59B81__INCLUDED_)
#define AFX_PICKTITLEDLG_H__01082977_5905_4F98_9B48_D4AF72F59B81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PickTitleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPickTitleDlg dialog

class CPickTitleDlg : public CDialog
{
// Construction
public:
	CString m_strTitle;
	CPickTitleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPickTitleDlg)
	enum { IDD = IDD_DIALOG_PICKTITLE };
	CListBox	m_ListTitles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPickTitleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPickTitleDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListCdtitles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICKTITLEDLG_H__01082977_5905_4F98_9B48_D4AF72F59B81__INCLUDED_)
