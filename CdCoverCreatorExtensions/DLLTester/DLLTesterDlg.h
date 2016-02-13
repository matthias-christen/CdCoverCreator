// DLLTesterDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CDLLTesterDlg dialog
class CDLLTesterDlg : public CDialog
{
// Construction
public:
	CDLLTesterDlg(CWnd* pParent = NULL);	// standard constructor
	~CDLLTesterDlg ();

// Dialog Data
	enum { IDD = IDD_DLLTESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_wndList;
	CEdit m_wndEditInput;
	afx_msg void OnBnClickedButtonList();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnLbnSelchangeListResult();
	afx_msg void OnBnClickedButtonLoaddll();
	CEdit m_wndEditDLL;

	HINSTANCE m_hDLL;
};
