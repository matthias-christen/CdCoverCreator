// ProgramSettingsPages.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "resource.h"
#include "ProgramSettingsPages.h"

#include "BitmapPrinter.h"
#include "HttpRequest.h"
#include "wcc_data.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern DWORD gl_dwNextDownloadId;
extern std::list<DWORD> gl_stkDownloads;

IMPLEMENT_DYNCREATE(CPSHosts, CPropertyPage)
IMPLEMENT_DYNCREATE(CPSCheckVersion, CPropertyPage)


/////////////////////////////////////////////////////////////////////////////
// CPSHosts property page

CPSHosts::CPSHosts() : CPropertyPage(CPSHosts::IDD)
{
	//{{AFX_DATA_INIT(CPSHosts)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPSHosts::~CPSHosts()
{
}

void CPSHosts::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPSHosts)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_LIST2, m_ListLyricsHosts);
	DDX_Control(pDX, IDC_LIST1, m_ListCoverHosts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPSHosts, CPropertyPage)
	//{{AFX_MSG_MAP(CPSHosts)
	ON_BN_CLICKED(IDC_BUTTON_CHECKNEWLIBS, OnButtonChecknewlibs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPSHosts::OnSetActive() 
{
	CCdCoverCreator2App* pApp = (CCdCoverCreator2App*) AfxGetApp ();
	AddLibs (&m_ListCoverHosts, "*.wcclib", &pApp->m_mapCoverHosts);
	AddLibs (&m_ListLyricsHosts, "*.lyrlib", &pApp->m_mapLyricsHosts);

	return CPropertyPage::OnSetActive();
}

void AddLibs(CCheckListBox* pList, CString strSuffix, HOSTMAP* pMap)
{
	CString strKey, strVal;
	int nIdx;

	pList->ResetContent ();

	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
	{
		nIdx = pList->AddString ((*it).first);
		pList->SetCheck (nIdx, (*it).second->bEnabled ? 1 : 0);
	}
}

BOOL CPSHosts::OnKillActive() 
{
	CCdCoverCreator2App* pApp = (CCdCoverCreator2App*) AfxGetApp ();
	SetHosts ("Covers", &m_ListCoverHosts, &pApp->m_mapCoverHosts);
	SetHosts ("Lyrics", &m_ListLyricsHosts, &pApp->m_mapLyricsHosts);

	return CPropertyPage::OnKillActive();
}

void CPSHosts::SetHosts(CString strKey, CCheckListBox *pList, HOSTMAP *pMap)
{
	CString str;
	int nValue;

	for (int i = 0; i < pList->GetCount (); i++)
	{
		pList->GetText (i, str);
		nValue = pList->GetCheck (i);
		(*(pMap->find (str))).second->bEnabled = nValue == 1;

		AfxGetApp ()->WriteProfileInt ("Hosts\\" + strKey, str, nValue);
	}
}

UINT CheckNewLibs (LPVOID lpParam)
{
	CHECKLIBSPARAM* pParam = (CHECKLIBSPARAM*) lpParam;

	// Send query to server
	DWORD dwServiceType;
	CString strServer, strObject;
	INTERNET_PORT nPort;

//	AfxParseURL ("http://www.our-class.net/mmchristen/ver_wcc.txt", dwServiceType, strServer, strObject, nPort);
	AfxParseURL ("http://www.thyante.com/cdcovercreator/ver_wcc.txt", dwServiceType, strServer, strObject, nPort);

	CInternetSession session;
	CHttpConnection* pServer = session.GetHttpConnection (strServer);

	try
	{
		CHttpFile* pFile = pServer->OpenRequest (CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL,
			INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);

		pFile->SendRequest ();

		DWORD dwStatus;
		if (pFile->QueryInfoStatusCode (dwStatus) && (dwStatus >= 400))
			AfxThrowFileException (CFileException::fileNotFound);

		char szBuf[1000];
		int n = pFile->Read (szBuf, 1000);
		szBuf[n] = 0;

		if (n > 0 && n < 1000)	// 1000 is suspicious
		{
			CCdCoverCreator2App* pApp = (CCdCoverCreator2App*) AfxGetApp ();

			// parse the returned string
			LPSTR lpszTmp = ParseHosts (szBuf, &pApp->m_mapCoverHosts, "Covers", ".wcclib", pParam);
			ParseHosts (lpszTmp, &pApp->m_mapLyricsHosts, "Lyrics", ".lyrlib", pParam);

			AddLibs (pParam->pCoversList, "*.wcclib", &pApp->m_mapCoverHosts);
			AddLibs (pParam->pLyricsList, "*.lyrlib", &pApp->m_mapLyricsHosts);
		}
	}
	catch (CInternetException* e)
	{
		e->ReportError ();
		e->Delete ();
	}

	session.Close ();

	delete pParam;
	return 0;
}

LPSTR ParseHosts(LPSTR lpszBuf, HOSTMAP *pMap, CString strKey, CString strExtension, CHECKLIBSPARAM* pParam)
{
	LPSTR lpszTemp, lpszCpy;
	CString strHost, strVersion, strFile;
	HOSTMAP::iterator it;
	RESOURCEHOST* pHost;
	bool bIsNewer;
	int n;

	int nHosts = atoi (lpszBuf);
	lpszBuf = strchr (lpszBuf, '\n');
	while (isspace (*lpszBuf))
		lpszBuf++;

	for (int i = 0; i < nHosts; i++)
	{
		// Get Host
		lpszTemp = lpszBuf;
		lpszBuf = strchr (lpszBuf, ' ');
		n = lpszBuf - lpszTemp;
		strncpy (lpszCpy = strHost.GetBuffer (n + 1), lpszTemp, n);
		lpszCpy[n] = 0;
		strHost.ReleaseBuffer ();
		lpszBuf++;

		// Get Version
		lpszTemp = lpszBuf;
		lpszBuf = strchr (lpszBuf, ' ');
		n = lpszBuf - lpszTemp;
		strncpy (lpszCpy = strVersion.GetBuffer (n + 1), lpszTemp, n);
		lpszCpy[n] = 0;
		strVersion.ReleaseBuffer ();
		lpszBuf++;

		// Get Filename
		lpszTemp = lpszBuf;
		lpszBuf = strchr (lpszBuf, '\n');
		n = lpszBuf - lpszTemp;
		strncpy (lpszCpy = strFile.GetBuffer (n + 1), lpszTemp, n);
		lpszCpy[n] = 0;
		strFile.ReleaseBuffer ();
		lpszBuf++;

		it = pMap->find (strHost);
		bIsNewer = it == pMap->end ();
		if (it != pMap->end ())
			bIsNewer |= ((*it).second->nVersion < atoi (strVersion));

		if (bIsNewer)
		{
			*(pParam->pbCancelDownload) = false;

			// create the local filename
			CString strLocalFilename;
			GetModuleFileName (AfxGetInstanceHandle (), strLocalFilename.GetBuffer (MAX_PATH), MAX_PATH);
			strLocalFilename.ReleaseBuffer ();
			strLocalFilename = strLocalFilename.Left (strLocalFilename.ReverseFind ('\\') + 1);
			strLocalFilename += strHost;
			strLocalFilename.Replace ('.', '_');
			strLocalFilename += strExtension;
			
			// download the file
			::ShowWindow (pParam->hwndProgress, SW_SHOW);
			CHttpRequest::DownloadFile (strFile, 1, "", "", strLocalFilename,
				pParam->pbCancelDownload, ++gl_dwNextDownloadId, *(pParam->pnError),
				0, pParam->hwndProgress);
			::ShowWindow (pParam->hwndProgress, SW_HIDE);

			// insert into map
			HINSTANCE hLib = LoadLibrary (strLocalFilename);
			if (hLib != NULL)
			{
				WCC_GETHOST* pfn_GetHost = (WCC_GETHOST*) GetProcAddress (hLib, "GetHost");
				WCC_GETLIBVERSION* pfn_GetVersion = (WCC_GETLIBVERSION*) GetProcAddress (hLib, "GetLibVersion");

				if ((pfn_GetHost != NULL) && (pfn_GetVersion != NULL))
				{
					pHost = new RESOURCEHOST;
					pHost->bEnabled = GetProfileInt ("Hosts\\" + strKey, pfn_GetHost (), 1) ? true : false;
					pHost->strLibFilename = strLocalFilename;
					pHost->nVersion = pfn_GetVersion ();
					pHost->hInst = NULL;

					if (it == pMap->end ())
						pMap->insert (HOSTMAP::value_type (CString (pfn_GetHost ()), pHost));
					else
					{
						delete (*it).second;
						(*it).second = pHost;
					}
				}

				FreeLibrary (hLib);
			}
		}
	}

	return lpszBuf;
}

void CPSHosts::OnButtonChecknewlibs() 
{
	CHECKLIBSPARAM* pParam = new CHECKLIBSPARAM;
	pParam->pCoversList = &m_ListCoverHosts;
	pParam->pLyricsList = &m_ListLyricsHosts;
	pParam->pbCancelDownload = &m_bCancelDownload;
	pParam->pnError = &m_nError;
	pParam->hwndProgress = m_Progress.GetSafeHwnd ();

	AfxBeginThread (CheckNewLibs, pParam, THREAD_PRIORITY_NORMAL);
}

/////////////////////////////////////////////////////////////////////////////
// CPSCheckVersion property page

CPSCheckVersion::CPSCheckVersion() : CPropertyPage(CPSCheckVersion::IDD)
{
	//{{AFX_DATA_INIT(CPSCheckVersion)
	m_bDoCheckVersion = FALSE;
	//}}AFX_DATA_INIT
}

CPSCheckVersion::~CPSCheckVersion()
{
}

void CPSCheckVersion::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPSCheckVersion)
	DDX_Check(pDX, IDC_CHECK_PROGVERSION, m_bDoCheckVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPSCheckVersion, CPropertyPage)
	//{{AFX_MSG_MAP(CPSCheckVersion)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPSCheckVersion::OnSetActive() 
{
	m_bDoCheckVersion = ((CCdCoverCreator2App*)	AfxGetApp ())->DoCheckVersion ();
	UpdateData (false);

	return CPropertyPage::OnSetActive();
}

BOOL CPSCheckVersion::OnKillActive() 
{
	UpdateData ();
	((CCdCoverCreator2App*) AfxGetApp ())->SetCheckVersion (m_bDoCheckVersion);

	return CPropertyPage::OnKillActive();
}