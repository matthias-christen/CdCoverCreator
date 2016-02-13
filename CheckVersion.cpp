// CheckVersion.cpp: implementation of the CCheckVersion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CheckVersion.h"
#include "UpdateDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCheckVersion::CCheckVersion()
{

}

CCheckVersion::~CCheckVersion()
{

}

bool CCheckVersion::Check (CString strURL, bool bExamineDoCheckVersion)
{
	CCdCoverCreator2App* pApp = (CCdCoverCreator2App*) AfxGetApp ();
	bool bSuccess = false;

	if (bExamineDoCheckVersion)
		if (!pApp->DoCheckVersion () || pApp->m_bVersionChecked)
			return true;

	// Send query to server
	DWORD dwServiceType;
	CString strServer, strObject;
	INTERNET_PORT nPort;

	AfxParseURL (strURL, dwServiceType, strServer, strObject, nPort);

	CInternetSession session;
	CHttpConnection* pServer = session.GetHttpConnection (strServer);

	try
	{
		CHttpFile* pFile = pServer->OpenRequest (CHttpConnection::HTTP_VERB_GET,
			strObject + "?id=1&version=" + CCheckVersion::GetProgramVersion (), NULL, 1, NULL, NULL,
			INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);

#ifdef PEROUTKA_FEDORISIN
		pFile->AddRequestHeaders ("User-Agent: CdCoverCreator " + ((CCdCoverCreator2App*) AfxGetApp ())->GetVersion () + " [PF]\r\n");
#else
		pFile->AddRequestHeaders ("User-Agent: CdCoverCreator " + ((CCdCoverCreator2App*) AfxGetApp ())->GetVersion () + "\r\n");
#endif

		pFile->SendRequest ();

		DWORD dwStatus;
		if (pFile->QueryInfoStatusCode (dwStatus) && (dwStatus >= 400))
			return false;

		char szBuf[1000];
		int n = pFile->Read (szBuf, 1000);
		szBuf[n] = 0;

		bSuccess = true;
		if (n > 6)
		{
			// something was returned. There is a new verison available

			// get version on the server
			CString strServerVersion;
			LPSTR lpstrServerVersion = strServerVersion.GetBuffer (7);
			memcpy (lpstrServerVersion, szBuf, 6);
			lpstrServerVersion[6] = 0;
			strServerVersion.ReleaseBuffer ();

	
			if (strServerVersion != pApp->GetProfileString ("Version", "Ver"))
			{
				// the version on the server is newer
				pApp->WriteProfileString ("Version", "Ver", strServerVersion);
				pApp->WriteProfileString ("Version", "Update", szBuf + 6);
				pApp->WriteProfileString ("Version", "Status", "");
			}
		}

		pApp->m_bVersionChecked = true;
	}
	catch (CInternetException* e)
	{
//		e->ReportError ();
		e->Delete ();
	}

	session.Close ();

	return bSuccess;
}

bool CCheckVersion::Prompt(bool bIgnoreSkip)
{
#ifndef PEROUTKA_FEDORISIN
	CWinApp* pApp = AfxGetApp ();

	// delete update app
	CString strUpdFilename;
	GetModuleFileName (AfxGetInstanceHandle (), strUpdFilename.GetBuffer (MAX_PATH), MAX_PATH);
	strUpdFilename.ReleaseBuffer ();
	strUpdFilename = strUpdFilename.Left (strUpdFilename.ReverseFind ('\\') + 1) + "Update.exe";

	DeleteFile (strUpdFilename);

	CString strVersion = pApp->GetProfileString ("Version", "Ver", "");

	if ((!(strVersion.IsEmpty () || (strVersion == CCheckVersion::GetProgramVersion ()))) &&
		((pApp->GetProfileString ("Version", "Status", "") != "skip") || bIgnoreSkip))
	{
		CUpdateDlg dlg;
		
		dlg.m_strNewFeatures = pApp->GetProfileString ("Version", "Update", "");
		CString strVer = pApp->GetProfileString ("Version", "Ver", "");
		dlg.m_strVersion.Format ("%d.%d", atoi (strVer), atoi (strVer.Mid (2)));

		if (dlg.DoModal () == IDOK)
		{
			// reset version
			pApp->WriteProfileString ("Version", "Ver", "");

			/*
			// write Update app from resource to disk
			HRSRC hRes = FindResource (AfxGetResourceHandle (), MAKEINTRESOURCE (IDR_UPDATE), "EXECUTABLE");
			DWORD dwSize = SizeofResource (AfxGetResourceHandle (), hRes);
			HGLOBAL hglRes = LoadResource (AfxGetResourceHandle (), hRes);
			
			CFile file (strUpdFilename, CFile::modeCreate | CFile::modeWrite);
			file.Write (LockResource (hglRes), dwSize);
			file.Close ();

			UnlockResource (hglRes);

			// start Update app
			WinExec (strUpdFilename, SW_SHOW);

			// exit CdCoveCreator
			PostQuitMessage (0);
			*/

			// Go to web page
			CCheckVersion::GotoURL ("http://www.our-class.net/mmchristen/index.php?file=ccc_update.html", SW_SHOW);
		}

		return true;
	}
#endif

	return false;
}

CString CCheckVersion::GetProgramVersion()
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
	strVersion.Format ("%d.%04d", version.wFileVersionMajor, version.wFileVersionMinor);
	return strVersion;
}

LONG CCheckVersion::GetRegKey (HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy (retdata,data);
		RegCloseKey (hkey);
    }

    return retval;
}

HINSTANCE CCheckVersion::GotoURL (LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];	

    // First try ShellExecute()
    HINSTANCE result = ShellExecute (NULL, _T ("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR)
	{		
        if (CCheckVersion::GetRegKey (HKEY_CLASSES_ROOT, _T (".htm"), key) == ERROR_SUCCESS)
		{
            lstrcat (key, _T ("\\shell\\open\\command"));

			if (CCheckVersion::GetRegKey (HKEY_CLASSES_ROOT, key, key) == ERROR_SUCCESS)
			{
                TCHAR *pos;
                pos = _tcsstr (key, _T ("\"%1\""));
                if (pos == NULL)
				{										// No quotes found
                    pos = strstr(key, _T ("%1"));		// Check for %1, without quotes
                    if (pos == NULL)					// No parameter at all...
                        pos = key + lstrlen (key) - 1;
                    else
                        *pos = '\0';					// Remove the parameter
                }
                else
                    *pos = '\0';						// Remove the parameter

                lstrcat (pos, _T (" "));
                lstrcat (pos, url);
                result = (HINSTANCE) WinExec (key,showcmd);
            }
        }
	}
	  
    return result;
}