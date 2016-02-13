// AboutDlg_PF.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "AboutDlg_PF.h"


// CAboutDlg_PF dialog

IMPLEMENT_DYNAMIC(CAboutDlg_PF, CDialog)
CAboutDlg_PF::CAboutDlg_PF(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg_PF::IDD, pParent)
{
}

CAboutDlg_PF::~CAboutDlg_PF()
{
}

void CAboutDlg_PF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_Email);
}


BEGIN_MESSAGE_MAP(CAboutDlg_PF, CDialog)
END_MESSAGE_MAP()


// CAboutDlg_PF message handlers

BOOL CAboutDlg_PF::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strVersion = ((CCdCoverCreator2App*) AfxGetApp ())->GetVersion ();
	SetDlgItemText (IDC_STATIC_VERSION, strVersion);

	return TRUE;
}
