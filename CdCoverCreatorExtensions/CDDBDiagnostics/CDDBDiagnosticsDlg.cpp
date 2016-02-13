// CDDBDiagnosticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CDDBDiagnostics.h"
#include "CDDBDiagnosticsDlg.h"
#include ".\cddbdiagnosticsdlg.h"

#include "AudioCDTracks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCDDBDiagnosticsDlg dialog



CCDDBDiagnosticsDlg::CCDDBDiagnosticsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCDDBDiagnosticsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCDDBDiagnosticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtResult);
}

BEGIN_MESSAGE_MAP(CCDDBDiagnosticsDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CCDDBDiagnosticsDlg message handlers

BOOL CCDDBDiagnosticsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCDDBDiagnosticsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCDDBDiagnosticsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCDDBDiagnosticsDlg::OnBnClickedButton1()
{
	CAudioCDTracks act;
	CString s;
	m_txtResult.GetWindowText (s);
	m_txtResult.SetWindowText (s + "\r\n===============================================================\r\n\r\n" + act.GetInfo ());
}
