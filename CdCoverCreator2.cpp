// CdCoverCreator2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "MainFrm.h"
#include "AboutDlg_PF.h"
#include "DlgAbout.h"

#include "DocMgrEx.h"

#include "ChildFrm.h"
#include "CdCoverCreator2Doc.h"
#include "CdCoverCreator2View.h"

#include "StyleTemplate.h"
#include "StyleTemplates.h"

#include "HyperlinkStatic.h"
#include "StarWarsCtrl.h"

#include "GradientRenderer.h"
#include "Picture.h"
#include "BackgroundAlphaTexture.h"

#include "ProgramSettingsSheet.h"

#include "CheckVersion.h"
#include "HttpReqSocket.h"
#include <ras.h>
#include <shlwapi.h>

#include "wcc_data.h"

#include "splasher.h"
#include <Gdiplus.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int _AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL, HKEY hkeyBaseKey = HKEY_CLASSES_ROOT)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(hkeyBaseKey, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE1("Warning: registration database update failed for key '%s'.\n",
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if(::RegCreateKey(hkeyBaseKey, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE1("Warning: registration database update failed for key '%s'.\n", lpszKey);
		return FALSE;
	}
}

// Version checking
// This thread waits until a connection to the internet is
// established. Then it calls the version check routines
UINT WaitForRasNotificationsThread (LPVOID lpvoid)
{
	HANDLE hEvent = CreateEvent (NULL, FALSE, FALSE, "RasNotification");
	if (hEvent)
		RasConnectionNotification ((HRASCONN) INVALID_HANDLE_VALUE, hEvent, RASCN_Connection);

	if (WAIT_OBJECT_0 == WaitForSingleObject (hEvent, INFINITE))
	{
		if (CHttpReqSocket::IsInternetConnected ())
		{
			if (!CCheckVersion::Check ("http://www.our-class.net/mmchristen/version.php"))
				CCheckVersion::Check ("http://our-class.net/mmchristen/version.php");
		}
		CloseHandle(hEvent);
	}
	return 0;
}

UINT CheckVersion (LPVOID lpvoid)
{
	if (!CCheckVersion::Check ("http://www.our-class.net/mmchristen/version.php"))
		CCheckVersion::Check ("http://our-class.net/mmchristen/version.php");

	return 0;
}

// CCdCoverCreator2App

BEGIN_MESSAGE_MAP(CCdCoverCreator2App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)

//	ON_UPDATE_COMMAND_UI(ID_WINDOW_NEW, OnUpdateWindowNew)
	ON_COMMAND(ID_FILE_PROGRAMOPTIONS, OnFileProgramoptions)
	ON_COMMAND(ID_CHECKVERSION, OnCheckversion)
//	ON_UPDATE_COMMAND_UI(ID_FILE_MRU_FILE1, OnUpdateRecentFileMenu)
	ON_COMMAND(ID_FILE_NEW_EMPTYPROJECT, OnFileNewEmptyproject)
	ON_COMMAND(ID_FILE_NEW_WIZARD, OnFileNewWizard)
	ON_COMMAND(ID_NEW_DATACD, OnFileNewDataCD)
	ON_COMMAND(ID_FILE_NEW_LISTWEB, OnFileNewListweb)
	ON_COMMAND(ID_FILE_NEW_IMPORTM3U, OnFileNewImportM3U)
	
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_HELP_CDCOVERCREATORONTHEWEB, OnHelpCdcovercreatorontheweb)
END_MESSAGE_MAP()


// CCdCoverCreator2App construction

CCdCoverCreator2App::CCdCoverCreator2App()
{
	EnableHtmlHelp();

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_NewDocMode = AudioCD;
}


// The one and only CCdCoverCreator2App object

CCdCoverCreator2App theApp;

// CCdCoverCreator2App initialization

BOOL CCdCoverCreator2App::InitInstance()
{
	// set lang to Polish if Peroutka release
#ifdef PEROUTKA_FEDORISIN
	SetThreadLocale (MAKELCID (MAKELANGID (LANG_POLISH, SUBLANG_NEUTRAL), SORT_DEFAULT));
#endif

	CString strCmdLine (GetCommandLine ());
	int nPos = strCmdLine.Find ("-lang=");
	if (nPos > -1)
	{
		int nPosEnd = strCmdLine.Find (" ", nPos);
		if (nPosEnd == -1)
			nPosEnd = strCmdLine.GetLength ();
		int nLang = atoi (strCmdLine.GetBuffer () + nPos + 6);
		int nPosComma = strCmdLine.Find (',', nPos + 6);
		int nSubLang = (nPosComma == -1 || nPosComma > nPosEnd) ? SUBLANG_DEFAULT : atoi (strCmdLine.GetBuffer () + nPosComma + 1);

		SetThreadLocale (MAKELCID (MAKELANGID (nLang, nSubLang), SORT_DEFAULT));
	}

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	AfxInitRichEdit2 ();

	CoInitializeEx (NULL, COINIT_APARTMENTTHREADED);

	// Initialize GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup (&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// Splash screen
	// (Bring up the splash screen in a secondary UI thread)
	CSplashThread* pSplashThread = (CSplashThread*) AfxBeginThread (RUNTIME_CLASS (CSplashThread), THREAD_PRIORITY_NORMAL, 0);

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey (_T (""));

	// export old templates and delete old registry key
	m_pTemplates = new CStyleTemplates ();
	m_pTemplates->ExportTemplates ();
	SHDeleteKey (HKEY_CURRENT_USER, "Software\\CdCoverCreator");

	free ((void*) m_pszRegistryKey);
	m_pszRegistryKey = NULL;
	SetRegistryKey (_T ("thyanté"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

/*
	// Version checking
	m_bDoCheckVersion = GetProfileInt ("Settings", "DoCheckVersion", 1) ? true : false;
	m_bVersionChecked = false;

	if (m_bDoCheckVersion)
	{
//		if (CHttpReqSocket::IsInternetConnected ())
			AfxBeginThread (CheckVersion, NULL);
//		else
//			// no connection. Wait until one is established...
//			AfxBeginThread (WaitForRasNotificationsThread, NULL);
	}
*/
	
	CString s = m_pszHelpFilePath;
	s = s.Left (s.GetLength () - 3) + "chm";
	free ((void*) m_pszHelpFilePath);
	m_pszHelpFilePath = _tcsdup (s);


	// use extended CDocManager for new file open dialog
//	m_pDocManager = new CDocManagerEx;

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_CdCoverCreatorTYPE,
		RUNTIME_CLASS(CCdCoverCreator2Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CCdCoverCreator2View));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

/*
	// bc menus
	BCMenu::SetMenuDrawMode (BCMENU_DRAWMODE_XP);

	BCMenu* pMenu = &((CMainFrame*) m_pMainWnd)->m_menuDefault;
	pMenu->LoadMenu (IDR_LABELSTYPE);
	pMenu->LoadToolbar (IDR_MAINFRAME);
	pMenu->LoadToolbar (IDR_TOOLBAR_NEW);
	pDocTemplate->m_hMenuShared = pMenu->GetSafeHmenu ();

	pMenu = &((CMainFrame*) m_pMainWnd)->m_menu;
	pMenu->LoadMenu (IDR_MAINFRAME);
	pMenu->LoadToolbar (IDR_MAINFRAME);
	pMenu->LoadToolbar (IDR_TOOLBAR_NEW);
	((CMainFrame*) m_pMainWnd)->m_hMenuDefault = pMenu->GetSafeHmenu ();
	
	pMainFrame->OnUpdateFrameMenu (pMainFrame->m_menuDefault.GetSafeHmenu ());
*/

	::DestroyMenu (pDocTemplate->m_hMenuShared);
	pDocTemplate->m_hMenuShared = NULL;

	m_bFirstDoc = true;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	/*
	// update the .lab file type to be used with CdCoverCreator
	CString strFileTypeId;
	pDocTemplate->GetDocString (strFileTypeId, CDocTemplate::regFileTypeId);
	_AfxSetRegKey (".lab", strFileTypeId);
	char szFilename[MAX_PATH];
	GetModuleFileName (NULL, szFilename, MAX_PATH);
	_AfxSetRegKey ("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\.lab",
		strrchr (szFilename, '\\') + 1, "Application", HKEY_CURRENT_USER);
	*/

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// load settings
	GetHosts (&m_mapCoverHosts, "Covers", "*.wcclib");
	GetHosts (&m_mapLyricsHosts, "Lyrics", "*.lyrlib");

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	// Update/Version Checking
//	CCheckVersion::Prompt (false);

	// hide the splash screen
	pSplashThread->HideSplash ();

	return TRUE;
}

CString CCdCoverCreator2App::GetVersion ()
{
	// Get program version
	struct
	{
		WORD wLength;
		WORD wValueLength;
		WORD wType;
		WCHAR szKey[17];
		DWORD dwSignature;
		DWORD dwStrucVersion;
		WORD wFileVersionMinor;
		WORD wFileVersionMajor;
	} version;

	HRSRC hRes = FindResource (AfxGetResourceHandle (), MAKEINTRESOURCE (VS_VERSION_INFO), RT_VERSION);
	DWORD dwSize = SizeofResource (AfxGetResourceHandle (), hRes);
	HGLOBAL hglRes = LoadResource (AfxGetResourceHandle (), hRes);
	memcpy (&version, LockResource (hglRes), sizeof (version));
	UnlockResource (hglRes);

	CString strVersion;
	strVersion.Format ("%d.%d", version.wFileVersionMajor, version.wFileVersionMinor);

	return strVersion;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStarWarsCtrl	m_stcCredits;
	CHyperlinkStatic	m_Email;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_CREDITS, m_stcCredits);
	DDX_Control(pDX, IDC_STATIC_EMAIL, m_Email);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strVersion = ((CCdCoverCreator2App*) AfxGetApp ())->GetVersion ();
	SetDlgItemText (IDC_STATIC_VERSION, strVersion);

	CString strText;
	for (UINT i = IDS_CREDITS; ; i++)
	{
		if (!strText.LoadString (i))
			break;

		strText.Replace ("%VERSION%", strVersion);
		m_stcCredits.AddTextLine (strText);
	}

	m_stcCredits.SetStarSpeed (50);
	m_stcCredits.SetScrollSpeed (2);

	return TRUE;
}

// App command to run the dialog
void CCdCoverCreator2App::OnAppAbout()
{
#ifdef PEROUTKA_FEDORISIN
	CAboutDlg_PF aboutDlg;
	aboutDlg.m_Email.SetSubjectText ("CdCoverCreator");
#else
	//CAboutDlg aboutDlg;
	CDlgAbout aboutDlg;
#endif

	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCdCoverCreator2App message handlers

void CCdCoverCreator2App::HtmlHelp (DWORD_PTR dwData, UINT nCmd)
{
	switch (nCmd)
	{
	case HELP_CONTEXT:
		/*
		HtmlHelp (AfxGetMainWnd ()->GetSafeHwnd (), m_pszHelpFilePath,
			HH_HELP_CONTEXT, dwData);
		*/
		HH_POPUP hhPopup;
		hhPopup.cbStruct = sizeof (HH_POPUP);
		hhPopup.hinst = AfxGetInstanceHandle ();
		hhPopup.idString = (dwData > 0x00011000)  ? dwData - 0x00011000 : dwData - 0x00010000;
		GetCursorPos (&hhPopup.pt);
		hhPopup.clrForeground = -1;
		hhPopup.clrBackground = -1;
		hhPopup.rcMargins.left = -1;
		hhPopup.rcMargins.top = -1;
		hhPopup.rcMargins.right = -1;
		hhPopup.rcMargins.bottom = -1;
		hhPopup.pszFont = "MS Sans Serif, 8";
		::HtmlHelp (AfxGetMainWnd ()->GetSafeHwnd (), m_pszHelpFilePath,
			HH_DISPLAY_TEXT_POPUP, (DWORD) &hhPopup);
		break;

	default:
		::HtmlHelp (AfxGetMainWnd ()->GetSafeHwnd (), m_pszHelpFilePath,
			HH_DISPLAY_TOPIC, NULL);
		break;
	}
}


/*
void CCdCoverCreator2App::OnUpdateRecentFileMenu(CCmdUI *pCmdUI)
{
	// copied from CRecentFileList::UpdateMenu (which is called
	// in the original CWinApp::OnUpdateRecentFileMenu,
	// but replaced CMenu by BCMenu in order to have
	// XP-like menu style

	BCMenu* pMenu = (BCMenu*) pCmdUI->m_pMenu;
	if (m_pRecentFileList->m_strOriginal.IsEmpty() && pMenu != NULL)
		pMenu->GetMenuString(pCmdUI->m_nID, m_pRecentFileList->m_strOriginal, MF_BYCOMMAND);

	if (m_pRecentFileList->m_arrNames[0].IsEmpty())
	{
		// no MRU files
		if (!m_pRecentFileList->m_strOriginal.IsEmpty())
			pCmdUI->SetText(m_pRecentFileList->m_strOriginal);
		pCmdUI->Enable(FALSE);
		return;
	}

	if (pCmdUI->m_pMenu == NULL)
		return;

	for (int iMRU = 0; iMRU < m_pRecentFileList->m_nSize; iMRU++)
		((BCMenu*) pCmdUI->m_pMenu)->DeleteMenu(pCmdUI->m_nID + iMRU, MF_BYCOMMAND);

	FillRecentFileMenu ((BCMenu*) pCmdUI->m_pMenu, pCmdUI->m_nIndex, pCmdUI->m_nID);

	// update end menu count
	pCmdUI->m_nIndex--; // point to last menu added
	pCmdUI->m_nIndexMax = pCmdUI->m_pMenu->GetMenuItemCount();

	pCmdUI->m_bEnableChanged = TRUE;    // all the added items are enabled
}*/

void CCdCoverCreator2App::FillRecentFileMenu (CMenu* pMenu, UINT& nIdx1, UINT& nId1)
{
	TCHAR szCurDir[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, szCurDir);
	int nCurDir = lstrlen(szCurDir);
	ASSERT(nCurDir >= 0);
	szCurDir[nCurDir] = '\\';
	szCurDir[++nCurDir] = '\0';

	CString strName;
	CString strTemp;
	for (int iMRU = 0; iMRU < m_pRecentFileList->m_nSize; iMRU++)
	{
		if (!m_pRecentFileList->GetDisplayName(strName, iMRU, szCurDir, nCurDir))
			break;

		// double up any '&' characters so they are not underlined
		LPCTSTR lpszSrc = strName;
		LPTSTR lpszDest = strTemp.GetBuffer(strName.GetLength()*2);
		while (*lpszSrc != 0)
		{
			if (*lpszSrc == '&')
				*lpszDest++ = '&';
			if (_istlead(*lpszSrc))
				*lpszDest++ = *lpszSrc++;
			*lpszDest++ = *lpszSrc++;
		}
		*lpszDest = 0;
		strTemp.ReleaseBuffer();

		// insert mnemonic + the file name
		TCHAR buf[10];
		wsprintf(buf, _T("&%d "), (iMRU+1+m_pRecentFileList->m_nStart) % 10);
		pMenu->InsertMenu (nIdx1++, MF_STRING | MF_BYPOSITION, nId1++, CString(buf) + strTemp);
	}
}

/*
void CCdCoverCreator2App::OnUpdateWindowNew(CCmdUI* pCmdUI) 
{
	// simulate XP menus by inserting the menu items
	// through BCMenu

	// remove the current window menu items, that are added
	// automatically by Windows and replace them by
	// BCMenu items

	// if no menu is passed there's nothing to do...
	if (pCmdUI->m_pMenu == NULL)
		return;
	if (!::IsMenu (pCmdUI->m_pMenu->GetSafeHmenu ()))
		return;

	CStringArray arrWnds;
	CDWordArray arrFlgs, arrIds;
	CString s;

	// find position of first MDI child window in the menu
	int nFirstPos = 0;
	for (UINT i = 0; i < pCmdUI->m_pMenu->GetMenuItemCount (); i++)
		if (pCmdUI->m_pMenu->GetMenuItemID (i) == AFX_IDM_FIRST_MDICHILD)
		{
			nFirstPos = i;
			break;
		}

	for (i = nFirstPos; i < pCmdUI->m_pMenu->GetMenuItemCount (); i++)
	{
		pCmdUI->m_pMenu->GetMenuString (i, s, MF_BYPOSITION);

		// if the menu has not been changed (through Windows)
		// the menu string s is empty (does BCMenu not return the
		// menu string?) - so skip replacing procedure
		// (since nothing has changed, there is no problem skipping this...)
		if (s.IsEmpty ())
			return;

		// remember the menu item's properties (string, state, and id)
		arrWnds.Add (s);
		arrFlgs.Add (pCmdUI->m_pMenu->GetMenuState (i, MF_BYPOSITION));
		arrIds.Add (pCmdUI->m_pMenu->GetMenuItemID (i));
	}

	// delete the previous menu item, append a new BCMenu item
	for (UINT j = nFirstPos; j < i; j++)
	{
		pCmdUI->m_pMenu->DeleteMenu (nFirstPos, MF_BYPOSITION);

		((BCMenu*) pCmdUI->m_pMenu)->AppendMenu (arrFlgs.GetAt (j - nFirstPos),
			arrIds.GetAt (j - nFirstPos), arrWnds.GetAt (j - nFirstPos));
	}

	// replace separator
	pCmdUI->m_pMenu->DeleteMenu (nFirstPos - 1, MF_BYPOSITION);
	((BCMenu*) pCmdUI->m_pMenu)->InsertMenu (nFirstPos - 1, MF_BYPOSITION | MF_SEPARATOR);
}*/

void CCdCoverCreator2App::SetPaperOrientation(int nOrientation)
{
	PRINTDLG pd;
	pd.lStructSize = (DWORD) sizeof (PRINTDLG);
	BOOL bRet = GetPrinterDeviceDefaults (&pd);
	if (bRet)
	{
		// protect memory handle with ::GlobalLock and ::GlobalUnlock
		DEVMODE FAR *pDevMode = (DEVMODE FAR *) ::GlobalLock (m_hDevMode);
		// set orientation to landscape
		pDevMode->dmOrientation = nOrientation == 0 ? DMORIENT_PORTRAIT : DMORIENT_LANDSCAPE;
		::GlobalUnlock (m_hDevMode);
	}
}

int CCdCoverCreator2App::ExitInstance() 
{
	CoUninitialize ();

	CDirTree::EmptyImageStore ();
	Gdiplus::GdiplusShutdown (m_gdiplusToken);

	// clean up the host maps
	for (HOSTMAP::iterator it = m_mapCoverHosts.begin (); it != m_mapCoverHosts.end (); it++)
		delete (*it).second;
	for (HOSTMAP::iterator it = m_mapLyricsHosts.begin (); it != m_mapLyricsHosts.end (); it++)
		delete (*it).second;
	m_mapCoverHosts.clear ();
	m_mapLyricsHosts.clear ();

	delete m_pTemplates;

	return CWinApp::ExitInstance();
}

double CCdCoverCreator2App::GetPaperLength(int nOrientation)
{
	PRINTDLG pd;
	pd.lStructSize = (DWORD) sizeof (PRINTDLG);
	BOOL bRet = GetPrinterDeviceDefaults (&pd);

	double dPaperLength = 0;

	if (bRet)
	{
		// protect memory handle with ::GlobalLock and ::GlobalUnlock
		DEVMODE FAR *pDevMode = (DEVMODE FAR *) ::GlobalLock (m_hDevMode);
		switch (pDevMode->dmPaperSize)
		{
		case DMPAPER_LETTERSMALL:	// Letter Small 8 1/2 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : 21.59; break;
		case DMPAPER_TABLOID:		// Tabloid 11 x 17 in
			dPaperLength = nOrientation == 0 ? 43.18 : 27.94; break;
		case DMPAPER_LEDGER:		// Ledger 17 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : 43.18; break;
		case DMPAPER_LEGAL:			// Legal 8 1/2 x 14 in
			dPaperLength = nOrientation == 0 ? 35.56 : 21.59; break;
		case DMPAPER_STATEMENT:		// Statement 5 1/2 x 8 1/2 in
			dPaperLength = nOrientation == 0 ? 21.59 : 13.97; break;
		case DMPAPER_EXECUTIVE:		// Executive 7 1/4 x 10 1/2 in
			dPaperLength = nOrientation == 0 ? 26.67 : 18.41; break;
		case DMPAPER_A3:			// A3 297 x 420 mm
			dPaperLength = nOrientation == 0 ? 42 : 29.7; break;
		case DMPAPER_A4:			// A4 210 x 297 mm
		case DMPAPER_A4SMALL:		// A4 Small 210 x 297 mm
			dPaperLength = nOrientation == 0 ? 29.7 : 21; break;
		case DMPAPER_A5:			// A5 148 x 210 mm
			dPaperLength = nOrientation == 0 ? 21 : 14.8; break;
		case DMPAPER_B4:			// B4 (JIS) 250 x 354
			dPaperLength = nOrientation == 0 ? 35.4 : 25; break;
		case DMPAPER_B5:			// B5 (JIS) 182 x 257 mm
			dPaperLength = nOrientation == 0 ? 25.7 : 18.2; break;
		case DMPAPER_FOLIO:			// Folio 8 1/2 x 13 in
			dPaperLength = nOrientation == 0 ? 33.02 : 21.59; break;
		case DMPAPER_QUARTO:		// Quarto 215 x 275 mm
			dPaperLength = nOrientation == 0 ? 27.5 : 21.5; break;
		case DMPAPER_10X14:			// 10x14 in
			dPaperLength = nOrientation == 0 ? 35.56 : 25.4; break;
		case DMPAPER_11X17:			// 11x17 in
			dPaperLength = nOrientation == 0 ? 43.18 : 27.94; break;
		case DMPAPER_NOTE:			// Note 8 1/2 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : 21.59; break;
		case DMPAPER_ENV_9:			// Envelope #9 3 7/8 x 8 7/8
			dPaperLength = nOrientation == 0 ? 22.5425 : 9.84; break;
		case DMPAPER_ENV_10:		// Envelope #10 4 1/8 x 9 1/2
			dPaperLength = nOrientation == 0 ? 24.13 : 10.48; break;
		case DMPAPER_ENV_11:		// Envelope #11 4 1/2 x 10 3/8
			dPaperLength = nOrientation == 0 ? 26.3525 : 11.43; break;
		case DMPAPER_ENV_12:		// Envelope #12 4 3/4 x 11
			dPaperLength = nOrientation == 0 ? 27.94 : 12.07; break;
		case DMPAPER_ENV_14:		// Envelope #14 5 x 11 1/2
			dPaperLength = nOrientation == 0 ? 29.21 : 12.7; break;
		case DMPAPER_CSHEET:		// C size sheet, 17 x 22 in
			dPaperLength = nOrientation == 0 ? 55.88 : 43.18; break;
		case DMPAPER_DSHEET:		// D size sheet, 22 x 34 in
			dPaperLength = nOrientation == 0 ? 86.36 : 55.88; break;
		case DMPAPER_ESHEET:		// E size sheet, 34 x 44 in
			dPaperLength = nOrientation == 0 ? 111.76 : 86.36; break;
		case DMPAPER_ENV_DL:		// Envelope DL 110 x 220mm
			dPaperLength = nOrientation == 0 ? 22 : 11; break;
		case DMPAPER_ENV_C5:		// Envelope C5 162 x 229 mm
			dPaperLength = nOrientation == 0 ? 22.9 : 16.2; break;
		case DMPAPER_ENV_C3:		// Envelope C3  324 x 458 mm
			dPaperLength = nOrientation == 0 ? 45.8 : 32.4; break;
		case DMPAPER_ENV_C4:		// Envelope C4  229 x 324 mm
			dPaperLength = nOrientation == 0 ? 32.4 : 22.9; break;
		case DMPAPER_ENV_C6:		// Envelope C6  114 x 162 mm
			dPaperLength = nOrientation == 0 ? 16.2 : 11.4; break;
		case DMPAPER_ENV_C65:		// Envelope C65 114 x 229 mm
			dPaperLength = nOrientation == 0 ? 22.9 : 11.4; break;
		case DMPAPER_ENV_B4:		// Envelope B4  250 x 353 mm
			dPaperLength = nOrientation == 0 ? 35.3 : 25; break;
		case DMPAPER_ENV_B5:		// Envelope B5  176 x 250 mm
			dPaperLength = nOrientation == 0 ? 25 : 17.6; break;
		case DMPAPER_ENV_B6:		// Envelope B6  176 x 125 mm
			dPaperLength = nOrientation == 0 ? 12.5 : 17.6; break;
		case DMPAPER_ENV_ITALY:		// Envelope 110 x 230 mm
			dPaperLength = nOrientation == 0 ? 23 : 11; break;
		case DMPAPER_ENV_MONARCH:	// Envelope Monarch 3.875 x 7.5 in
			dPaperLength = nOrientation == 0 ? 19.05 : 9.84; break;
		case DMPAPER_ENV_PERSONAL:  // 6 3/4 Envelope 3 5/8 x 6 1/2 in
			dPaperLength = nOrientation == 0 ? 16.51 : 9.2; break;
		case DMPAPER_FANFOLD_US:	// US Std Fanfold 14 7/8 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : 37.78; break;
		case DMPAPER_FANFOLD_STD_GERMAN:  // German Std Fanfold 8 1/2 x 12 in
			dPaperLength = nOrientation == 0 ? 30.48 : 21.59; break;
		case DMPAPER_FANFOLD_LGL_GERMAN:  // German Legal Fanfold 8 1/2 x 13 in
			dPaperLength = nOrientation == 0 ? 33.02 : 21.59; break;
		case DMPAPER_ISO_B4:		// B4 (ISO) 250 x 353 mm
			dPaperLength = nOrientation == 0 ? 35.3 : 25; break;
		case DMPAPER_JAPANESE_POSTCARD:  // Japanese Postcard 100 x 148 mm
			dPaperLength = nOrientation == 0 ? 14.8 : 14.8; break;
		case DMPAPER_9X11:			// 9 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : 22.86; break;
		case DMPAPER_10X11:			// 10 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : 25.4; break;
		case DMPAPER_15X11:			// 15 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : 38.1; break;
		case DMPAPER_ENV_INVITE:	// Envelope Invite 220 x 220 mm
			dPaperLength = nOrientation == 0 ? 22 : 22; break;
		case DMPAPER_RESERVED_48:	// RESERVED--DO NOT USE
		case DMPAPER_RESERVED_49:	// RESERVED--DO NOT USE
			dPaperLength = nOrientation == 0 ? 0 : 0; break;
		case DMPAPER_LETTER_EXTRA:  // Letter Extra 9 \275 x 12 in
			dPaperLength = nOrientation == 0 ? 30.48 : 23.56; break;
		case DMPAPER_LEGAL_EXTRA:	// Legal Extra 9 \275 x 15 in
			dPaperLength = nOrientation == 0 ? 38.1 : 23.56; break;
		case DMPAPER_TABLOID_EXTRA: // Tabloid Extra 11.69 x 18 in
			dPaperLength = nOrientation == 0 ? 45.72 : 29.69; break;
		case DMPAPER_A4_EXTRA:		// A4 Extra 9.27 x 12.69 in
			dPaperLength = nOrientation == 0 ? 32.2326 : 23.55; break;
		case DMPAPER_LETTER_TRANSVERSE:  // Letter Transverse 8.275 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : 21.02; break;
		case DMPAPER_A4_TRANSVERSE: // A4 Transverse 210 x 297 mm
			dPaperLength = nOrientation == 0 ? 29.7 : 21; break;
		case DMPAPER_LETTER_EXTRA_TRANSVERSE: // Letter Extra Transverse 9\275 x 12 in
			dPaperLength = nOrientation == 0 ? 30.48 : 23.56; break;
		case DMPAPER_A_PLUS:		// SuperA/SuperA/A4 227 x 356 mm
			dPaperLength = nOrientation == 0 ? 35.6 : 22.7; break;
		case DMPAPER_B_PLUS:		// SuperB/SuperB/A3 305 x 487 mm
			dPaperLength = nOrientation == 0 ? 48.7 : 30.5; break;
		case DMPAPER_LETTER_PLUS:	// Letter Plus 8.5 x 12.69 in
			dPaperLength = nOrientation == 0 ? 32.2326 : 21.59; break;
		case DMPAPER_A4_PLUS:		// A4 Plus 210 x 330 mm
			dPaperLength = nOrientation == 0 ? 33 : 21; break;
		case DMPAPER_A5_TRANSVERSE: // A5 Transverse 148 x 210 mm
			dPaperLength = nOrientation == 0 ? 21 : 14.8; break;
		case DMPAPER_B5_TRANSVERSE: // B5 (JIS) Transverse 182 x 257 mm
			dPaperLength = nOrientation == 0 ? 25.7 : 18.2; break;
		case DMPAPER_A3_EXTRA:		// A3 Extra 322 x 445 mm
			dPaperLength = nOrientation == 0 ? 44.5 : 32.2; break;
		case DMPAPER_A5_EXTRA:		// A5 Extra 174 x 235 mm
			dPaperLength = nOrientation == 0 ? 23.5 : 17.4; break;
		case DMPAPER_B5_EXTRA:		// B5 (ISO) Extra 201 x 276 mm
			dPaperLength = nOrientation == 0 ? 27.6 : 20.1; break;
		case DMPAPER_A2:			// A2 420 x 594 mm
			dPaperLength = nOrientation == 0 ? 59.4 : 42; break;
		case DMPAPER_A3_TRANSVERSE: // A3 Transverse 297 x 420 mm
			dPaperLength = nOrientation == 0 ? 42 : 29.7; break;
		case DMPAPER_A3_EXTRA_TRANSVERSE:	// A3 Extra Transverse 322 x 445 mm
			dPaperLength = nOrientation == 0 ? 44.5 : 32.2; break;
/*
		case DMPAPER_DBL_JAPANESE_POSTCARD: // Japanese Double Postcard 200 x 148 mm
		case DMPAPER_A6:			// A6 105 x 148 mm
			dPaperLength = nOrientation == 0 ? 14.8 : ; break;
		case DMPAPER_JENV_KAKU2:	// Japanese Envelope Kaku #2
		case DMPAPER_JENV_KAKU3:	// Japanese Envelope Kaku #3
		case DMPAPER_JENV_CHOU3:	// Japanese Envelope Chou #3
		case DMPAPER_JENV_CHOU4:	// Japanese Envelope Chou #4
			dPaperLength = nOrientation == 0 ? 0 : ; break; // ???????
		case DMPAPER_LETTER_ROTATED:// Letter Rotated 11 x 8 1/2 11 in
			dPaperLength = nOrientation == 0 ? 21.59 : ; break;
		case DMPAPER_A3_ROTATED:	// A3 Rotated 420 x 297 mm
			dPaperLength = nOrientation == 0 ? 29.7 : ; break;
		case DMPAPER_A4_ROTATED:	// A4 Rotated 297 x 210 mm
			dPaperLength = nOrientation == 0 ? 21 : ; break;
		case DMPAPER_A5_ROTATED:	// A5 Rotated 210 x 148 mm
			dPaperLength = nOrientation == 0 ? 14.8 : ; break;
		case DMPAPER_B4_JIS_ROTATED:// B4 (JIS) Rotated 364 x 257 mm
			dPaperLength = nOrientation == 0 ? 25.7 : ; break;
		case DMPAPER_B5_JIS_ROTATED:// B5 (JIS) Rotated 257 x 182 mm
			dPaperLength = nOrientation == 0 ? 18.2 : ; break;
		case DMPAPER_JAPANESE_POSTCARD_ROTATED:		// Japanese Postcard Rotated 148 x 100 mm
			dPaperLength = nOrientation == 0 ? 10 : ; break;
		case DMPAPER_DBL_JAPANESE_POSTCARD_ROTATED: // Double Japanese Postcard Rotated 148 x 200 mm
			dPaperLength = nOrientation == 0 ? 20 : ; break;
		case DMPAPER_A6_ROTATED:					// A6 Rotated 148 x 105 mm
			dPaperLength = nOrientation == 0 ? 10.5 : ; break;
		case DMPAPER_JENV_KAKU2_ROTATED:	// Japanese Envelope Kaku #2 Rotated
		case DMPAPER_JENV_KAKU3_ROTATED:	// Japanese Envelope Kaku #3 Rotated
		case DMPAPER_JENV_CHOU3_ROTATED:	// Japanese Envelope Chou #3 Rotated
		case DMPAPER_JENV_CHOU4_ROTATED:	// Japanese Envelope Chou #4 Rotated
			dPaperLength = nOrientation == 0 ? 0 : ; break; // ??????????
		case DMPAPER_B6_JIS:				// B6 (JIS) 128 x 182 mm
			dPaperLength = nOrientation == 0 ? 18.2 : ; break;
		case DMPAPER_B6_JIS_ROTATED:		// B6 (JIS) Rotated 182 x 128 mm
			dPaperLength = nOrientation == 0 ? 12.8 : ; break;
		case DMPAPER_12X11:					// 12 x 11 in
			dPaperLength = nOrientation == 0 ? 27.94 : ; break;
		case DMPAPER_JENV_YOU4:				// Japanese Envelope You #4
		case DMPAPER_JENV_YOU4_ROTATED:		// Japanese Envelope You #4 Rotated
			dPaperLength = nOrientation == 0 ? 0 : ; break; 
		case DMPAPER_P16K:					// PRC 16K 146 x 215 mm
			dPaperLength = nOrientation == 0 ? 21.5 : ; break;
		case DMPAPER_P32K:					// PRC 32K 97 x 151 mm
		case DMPAPER_P32KBIG:				// PRC 32K(Big) 97 x 151 mm
			dPaperLength = nOrientation == 0 ? 15.1 : ; break;
		case DMPAPER_PENV_1:				// PRC Envelope #1 102 x 165 mm
			dPaperLength = nOrientation == 0 ? 16.5 : ; break;
		case DMPAPER_PENV_2:				// PRC Envelope #2 102 x 176 mm
		case DMPAPER_PENV_3:				// PRC Envelope #3 125 x 176 mm
			dPaperLength = nOrientation == 0 ? 17.6 : ; break;
		case DMPAPER_PENV_4:				// PRC Envelope #4 110 x 208 mm
			dPaperLength = nOrientation == 0 ? 20.8 : ; break;
		case DMPAPER_PENV_5:				// PRC Envelope #5 110 x 220 mm
			dPaperLength = nOrientation == 0 ? 22 : ; break;
		case DMPAPER_PENV_6:				// PRC Envelope #6 120 x 230 mm
		case DMPAPER_PENV_7:				// PRC Envelope #7 160 x 230 mm
			dPaperLength = nOrientation == 0 ? 23 : ; break;
		case DMPAPER_PENV_8:				// PRC Envelope #8 120 x 309 mm
			dPaperLength = nOrientation == 0 ? 30.9 : ; break;
		case DMPAPER_PENV_9:				// PRC Envelope #9 229 x 324 mm
			dPaperLength = nOrientation == 0 ? 32.4 : ; break;
		case DMPAPER_PENV_10:				// PRC Envelope #10 324 x 458 mm
			dPaperLength = nOrientation == 0 ? 45.8 : ; break;
		case DMPAPER_P16K_ROTATED:			// PRC 16K Rotated
		case DMPAPER_P32K_ROTATED:			// PRC 32K Rotated
		case DMPAPER_P32KBIG_ROTATED:		// PRC 32K(Big) Rotated
			dPaperLength = nOrientation == 0 ? 0 : ; break; // ??????
		case DMPAPER_PENV_1_ROTATED:		// PRC Envelope #1 Rotated 165 x 102 mm
		case DMPAPER_PENV_2_ROTATED:		// PRC Envelope #2 Rotated 176 x 102 mm
			dPaperLength = nOrientation == 0 ? 10.2 : ; break;
		case DMPAPER_PENV_3_ROTATED:		// PRC Envelope #3 Rotated 176 x 125 mm
			dPaperLength = nOrientation == 0 ? 12.5 : ; break;
		case DMPAPER_PENV_4_ROTATED:		// PRC Envelope #4 Rotated 208 x 110 mm
		case DMPAPER_PENV_5_ROTATED:		// PRC Envelope #5 Rotated 220 x 110 mm
			dPaperLength = nOrientation == 0 ? 11 : ; break;
		case DMPAPER_PENV_6_ROTATED:		// PRC Envelope #6 Rotated 230 x 120 mm
			dPaperLength = nOrientation == 0 ? 12 : ; break;
		case DMPAPER_PENV_7_ROTATED:		// PRC Envelope #7 Rotated 230 x 160 mm
			dPaperLength = nOrientation == 0 ? 16 : ; break;
		case DMPAPER_PENV_8_ROTATED:		// PRC Envelope #8 Rotated 309 x 120 mm
			dPaperLength = nOrientation == 0 ? 12 : ; break;
		case DMPAPER_PENV_9_ROTATED:		// PRC Envelope #9 Rotated 324 x 229 mm
			dPaperLength = nOrientation == 0 ? 22.9 : ; break;
		case DMPAPER_PENV_10_ROTATED:		// PRC Envelope #10 Rotated 458 x 324 mm
			dPaperLength = nOrientation == 0 ? 32.4 : ; break;
*/
		}

		::GlobalUnlock (m_hDevMode);
	}

	return dPaperLength;
}

void CCdCoverCreator2App::OnFileProgramoptions() 
{
	CProgramSettingsSheet propSheet;
	propSheet.DoModal();
}

void CCdCoverCreator2App::SetCheckVersion (BOOL bCheck)
{
	m_bDoCheckVersion = bCheck ? true : false;
	WriteProfileInt ("Settings", "DoCheckVersion", bCheck);
}

void CCdCoverCreator2App::OnCheckversion() 
{
	bool bVersionCheckedPreviously = m_bVersionChecked;
	m_bVersionChecked = false;

	AfxGetApp ()->DoWaitCursor (1);
	bool bResult = CCheckVersion::Check ("http://www.our-class.net/mmchristen/version.php", false);
	AfxGetApp ()->DoWaitCursor (-1);

	if (bResult)
	{
		if (!CCheckVersion::Prompt (true))
			AfxMessageBox (IDS_NONEWVERSION, MB_ICONINFORMATION);
	}
	else
	{
		AfxGetApp ()->DoWaitCursor (1);
		bResult = CCheckVersion::Check ("http://our-class.net/mmchristen/version.php", false);
		AfxGetApp ()->DoWaitCursor (-1);

		if (bResult)
		{
			if (!CCheckVersion::Prompt (true))
				AfxMessageBox (IDS_NONEWVERSION, MB_ICONINFORMATION);
		}
		else
		{
			AfxMessageBox (IDS_INTERNETERROR_UPDATE, MB_ICONEXCLAMATION);
			m_bVersionChecked = bVersionCheckedPreviously;
		}
	}
}

void CCdCoverCreator2App::GetHosts(HOSTMAP *pMap, CString strKey, CString strSuffix)
{
	char szFile[MAX_PATH + 1];
	GetModuleFileName (AfxGetInstanceHandle (), szFile, MAX_PATH);
	CString strPath = szFile;
	strPath = strPath.Left (strPath.ReverseFind ('\\') + 1);
	CString str;

	RESOURCEHOST* pHost;

	CFileFind finder;
	BOOL bWorking = finder.FindFile (strPath + strSuffix);
	while (bWorking)
	{
		bWorking = finder.FindNextFile ();

		HINSTANCE hLib = LoadLibrary (finder.GetFilePath ());
		if (hLib != NULL)
		{
			WCC_GETHOST* pfn_GetHost = (WCC_GETHOST*) GetProcAddress (hLib, "GetHost");
			if (pfn_GetHost == NULL)
			{
				FreeLibrary (hLib);
				continue;
			}

			WCC_GETLIBVERSION* pfn_GetVersion = (WCC_GETLIBVERSION*) GetProcAddress (hLib, "GetLibVersion");
			if (pfn_GetVersion == NULL)
			{
				FreeLibrary (hLib);
				continue;
			}

			pHost = new RESOURCEHOST;
			pHost->bEnabled = GetProfileInt ("Hosts\\" + strKey, pfn_GetHost (), 1) ? true : false;
			pHost->strLibFilename = finder.GetFilePath ();
			pHost->nVersion = pfn_GetVersion ();
			pHost->hInst = NULL;

			pMap->insert (HOSTMAP::value_type (CString (pfn_GetHost ()), pHost));
			FreeLibrary (hLib);
		}
	}
}

void CCdCoverCreator2App::OnFileNewEmptyproject() 
{
	m_NewDocMode = Empty;
	OnFileNew ();	
}

void CCdCoverCreator2App::OnFileNewWizard() 
{
	m_NewDocMode = AudioCD;
	OnFileNew ();
}

void CCdCoverCreator2App::OnFileNewDataCD ()
{
	m_NewDocMode = DataCD;
	OnFileNew ();
}

void CCdCoverCreator2App::OnFileNewListweb() 
{
	m_NewDocMode = Web;
	OnFileNew ();
}

void CCdCoverCreator2App::OnFileNewImportM3U() 
{
	m_NewDocMode = ImportM3U;
	OnFileNew ();
}

CString CCdCoverCreator2App::GetApplicationDirectory ()
{
	char szFile[MAX_PATH + 1];
	GetModuleFileName (AfxGetInstanceHandle (), szFile, MAX_PATH);
	CString strPath = szFile;
	strPath = strPath.Left (strPath.ReverseFind ('\\') + 1);
	return strPath;
}

CString CCdCoverCreator2App::GetAppDir ()
{
	/*
	char szFile[MAX_PATH + 1];
	GetModuleFileName (AfxGetInstanceHandle (), szFile, MAX_PATH);
	CString strPath = szFile;
	strPath = strPath.Left (strPath.ReverseFind ('\\') + 1);
	return strPath;
	*/

	TCHAR szPath[MAX_PATH];
	SHGetFolderPath (NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, szPath);
	CString strPath = szPath;
	strPath += "\\CdCoverCreator\\";
	return strPath;
}

CString CCdCoverCreator2App::CheckDirExists (CString strDir)
{
	if (!PathFileExists (strDir))
	{
		// path does not exist
		int nLen = strDir.GetLength ();
		CString strDir1 = strDir.GetAt (nLen - 1) == '\\' ? strDir.Left (nLen - 1) : strDir;
		if (!CreateDirectory (strDir1, NULL))
		{
			// directory could not be created
			// create a directory one level lower
			CheckDirExists (strDir1.Left (strDir1.ReverseFind ('\\')));
			CreateDirectory (strDir1, NULL);
		}
	}

	return strDir;
}

CString CCdCoverCreator2App::GetListCacheDir ()
{
	return CheckDirExists (GetAppDir () + "Cache\\List\\");
}

CString CCdCoverCreator2App::GetImageCacheDir ()
{
	return CheckDirExists (GetAppDir () + "Cache\\Images\\");
}

CString CCdCoverCreator2App::GetAlphaTextureDir ()
{
	return CheckDirExists (GetAppDir () + "Textures\\");
}

CString CCdCoverCreator2App::GetTextureDir ()
{
	char szFile[MAX_PATH + 1];
	GetModuleFileName (AfxGetInstanceHandle (), szFile, MAX_PATH);
	CString strPath = szFile;
	strPath = strPath.Left (strPath.ReverseFind ('\\') + 1);

	return CheckDirExists (strPath + "Backgrounds\\");
}

CString CCdCoverCreator2App::GetTemplateDir ()
{
	TCHAR szPath[MAX_PATH];
	SHGetFolderPath (NULL, CSIDL_COMMON_TEMPLATES, NULL, SHGFP_TYPE_CURRENT, szPath);
	CString strPath = szPath;
	strPath += "\\CdCoverCreator\\";

	return CheckDirExists (strPath);
}

void CCdCoverCreator2App::OnHelpCdcovercreatorontheweb()
{
	CHyperlinkStatic::GotoURL ("http://www.thyante.com/index.php?cdcovercreator", SW_SHOW);
}
