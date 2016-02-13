// DLLTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DLLTester.h"
#include "DLLTesterDlg.h"

#include "WebCDCovers.h"
#include "HttpRequest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDLLTesterDlg dialog



CDLLTesterDlg::CDLLTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDLLTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hDLL = NULL;
}

CDLLTesterDlg::~CDLLTesterDlg ()
{
	if (m_hDLL != NULL)
		::FreeLibrary (m_hDLL);
}

void CDLLTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_wndList);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_wndEditInput);
	DDX_Control(pDX, IDC_EDIT_DLL, m_wndEditDLL);
}

BEGIN_MESSAGE_MAP(CDLLTesterDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LIST, OnBnClickedButtonList)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_LBN_DBLCLK(IDC_LIST_RESULT, OnLbnSelchangeListResult)
	ON_BN_CLICKED(IDC_BUTTON_LOADDLL, OnBnClickedButtonLoaddll)
END_MESSAGE_MAP()


// CDLLTesterDlg message handlers

BOOL CDLLTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDLLTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDLLTesterDlg::OnPaint() 
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
HCURSOR CDLLTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDLLTesterDlg::OnBnClickedButtonList()
{
	CWebCDCovers wcc (m_hDLL);

	bool bCancelDownload = false;
	wcc.SetDownloadCancelFlag (&bCancelDownload);

	m_wndList.ResetContent ();

	CString s;
	m_wndEditInput.GetWindowText (s);
	bool bResult = wcc.GetList (s, m_wndList.GetSafeHwnd ());
}

void CDLLTesterDlg::OnBnClickedButtonSearch()
{
	CWebCDCovers wcc (m_hDLL);
	
	bool bCancelDownload = false;
	wcc.SetDownloadCancelFlag (&bCancelDownload);

	m_wndList.ResetContent ();

	CString s;
	m_wndEditInput.GetWindowText (s);
	bool bResult = wcc.SearchCovers (s, m_wndList.GetSafeHwnd ());
}

void CDLLTesterDlg::OnLbnSelchangeListResult()
{
	CWebCDCovers wcc (m_hDLL);

	bool bCancelDownload = false;
	wcc.SetDownloadCancelFlag (&bCancelDownload);

	CString strLetter;
	m_wndEditInput.GetWindowText (strLetter);

	CString strName;
	int nIdx = m_wndList.GetCurSel ();
	m_wndList.GetText (nIdx, strName);

	COVERIMAGES* pCoverImages = (COVERIMAGES*) m_wndList.GetItemData (nIdx);

	for (int i = 0; i < NUM_RENDER_TYPES - 1; i++)
	{
		if (!pCoverImages->strFile[i].IsEmpty ())
			wcc.DownloadImage (strLetter, strName, pCoverImages->strFile[i], pCoverImages->strLocalFile[i]);
	}

	AfxMessageBox ("Download image:\nCover: " + pCoverImages->strLocalFile[Cover] + "\nBack: " + pCoverImages->strLocalFile[Back] + "\nInlay: " + pCoverImages->strLocalFile[Inlay] + "\nLabel: " + pCoverImages->strLocalFile[Label]);
}

void CDLLTesterDlg::OnBnClickedButtonLoaddll()
{
	if (m_hDLL != NULL)
		::FreeLibrary (m_hDLL);

	CString strPath;
	m_wndEditDLL.GetWindowText (strPath);
	m_hDLL = ::LoadLibrary (strPath);

	if (m_hDLL == NULL)
	{
		CString s;
		s.Format ("Error loading DLL: %d", GetLastError ());
		AfxMessageBox (s);
	}
	else
		AfxMessageBox ("DLL successfully loaded");
}
