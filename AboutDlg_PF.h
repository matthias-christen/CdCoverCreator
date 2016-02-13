#pragma once
#include "hyperlinkstatic.h"


// CAboutDlg_PF dialog

class CAboutDlg_PF : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg_PF)

public:
	CAboutDlg_PF(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDlg_PF();

// Dialog Data
	enum { IDD = IDD_DIALOG_ABOUT_PF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CHyperlinkStatic m_Email;
	virtual BOOL OnInitDialog();
};
