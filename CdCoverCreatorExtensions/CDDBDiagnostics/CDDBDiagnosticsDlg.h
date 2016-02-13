// CDDBDiagnosticsDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CCDDBDiagnosticsDlg dialog
class CCDDBDiagnosticsDlg : public CDialog
{
// Construction
public:
	CCDDBDiagnosticsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CDDBDIAGNOSTICS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_txtResult;
	afx_msg void OnBnClickedButton1();
};
