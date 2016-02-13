// WebLyrics.cpp: implementation of the CWebLyrics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "WebLyrics.h"

#include "HttpReqSocket.h"
#include "HttpRequest.h"

#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern DWORD gl_dwNextDownloadId;
extern std::list<DWORD> gl_stkDownloads;

// thread functions

UINT wslGetLyrics(LPVOID pvParams)
{
	WSL_GETLYRICSPARAMS* pParams = (WSL_GETLYRICSPARAMS*) pvParams;

	CWebLyrics wsl;

	wsl.SetDownloadCancelFlag (pParams->pCancelDownload);
	wsl.m_dwDownloadId = ++gl_dwNextDownloadId;

	bool bResult = false;
	::SendMessage (pParams->hwndProgress, PBM_SETRANGE32, 1, pParams->list.size ());
	::SendMessage (pParams->hwndProgress, PBM_SETPOS, 0, 0);
	::SendMessage (pParams->hwndProgress, PBM_SETSTEP, 1, 0);
	::ShowWindow (pParams->hwndProgress, SW_SHOW);

	int i = 0;
	for (std::list<CWslItem*>::iterator it = pParams->list.begin (); it != pParams->list.end (); it++)
	{
		bResult |= wsl.GetLyrics ((*it)->m_strArtist, (*it)->m_strAlbum, (*it)->m_strSong,
			pParams->hwndList, i, pParams->hwndProgress, pParams->hwndStatus);
		i++;
	}

	if (pParams->hwndStatus != NULL)
		for (int i = 1; i <= 4; i++)
			::PostMessage (pParams->hwndStatus, SB_SETTEXT, i, (LPARAM) "");

	::SendMessage (pParams->hwndNotify, WSLM_GETLYRICS, 0, 0);
	::ShowWindow (pParams->hwndProgress, SW_HIDE);
	
	// clean up
	for (std::list<CWslItem*>::iterator it = pParams->list.begin (); it != pParams->list.end (); it++)
		delete *it;
	delete pParams;

	return bResult ? 0 : 1;
}

UINT wslSearchLyrics (LPVOID pvParams)
{
	WSL_SEARCHLYRICSPARAMS* pParams = (WSL_SEARCHLYRICSPARAMS*) pvParams;

	CWebLyrics wsl;

	wsl.SetDownloadCancelFlag (pParams->pCancelDownload);
	wsl.m_dwDownloadId = ++gl_dwNextDownloadId;

	bool bResult = wsl.SearchLyrics (pParams->strArtist, pParams->strAlbum, pParams->strTitle,
		pParams->hwndList, pParams->hwndProgress, pParams->hwndStatus);

	if (pParams->hwndStatus != NULL)
		for (int i = 1; i <= 4; i++)
			::PostMessage (pParams->hwndStatus, SB_SETTEXT, i, (LPARAM) "");

	::SendMessage (pParams->hwndNotify, WSLM_GETLYRICS, 0, 0);
	::ShowWindow (pParams->hwndProgress, SW_HIDE);
	
	// clean up
	delete pParams;

	return bResult ? 0 : 1;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebLyrics::CWebLyrics()
{
	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapLyricsHosts;
	RESOURCEHOST* pHost;
	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
	{
		pHost = (*it).second;
		pHost->hInst = LoadLibrary (pHost->strLibFilename);
	}

	m_nError = 0;

	//if (!CHttpReqSocket::IsInternetConnected ())
	//{
	//	AfxMessageBox (IDS_INTERNETERROR_IMAGES, MB_ICONEXCLAMATION);
	//	m_nError = -1;
	//	return;
	//}
}

CWebLyrics::~CWebLyrics()
{
	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapCoverHosts;
	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
		FreeLibrary ((*it).second->hInst);
}

void CWebLyrics::SetDownloadCancelFlag(bool *pbCancelDownload)
{
	m_pbCancelDownload = pbCancelDownload;
}

bool CWebLyrics::GetLyrics (CString strArtist, CString strAlbum, CString strSong, HWND hwndList, int nListItem, HWND hwndProgress, HWND hwndStatus)
{
	bool bSuccess = false;

	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapLyricsHosts;
	RESOURCEHOST* pHost;

	CString strArtist1, strAlbum1, strSong1;

	char* lpszData = NULL;
	CString strHTML, strName, strMsg;
	char szUrl[1000], szHeaders[300], szData[300];
	int nMethod;
	bool bFoundLyrics;

	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
	{
		pHost = (*it).second;
		if (!pHost->bEnabled)
			continue;

		WSL_GETREQUEST* pfn_wslRequest = (WSL_GETREQUEST*) GetProcAddress (pHost->hInst, "GetLyricsRequest");
		WSL_GETLYRICS* pfn_wslGetLyrics = (WSL_GETLYRICS*) GetProcAddress (pHost->hInst, "GetLyrics");

		bSuccess = true;

		strArtist1 = CHttpReqSocket::URLEncode (strArtist);
		strAlbum1 = CHttpReqSocket::URLEncode (strAlbum);
		strSong1 = CHttpReqSocket::URLEncode (strSong);
		pfn_wslRequest (strSong1.GetBuffer (0), strArtist1.GetBuffer (0), szUrl, &nMethod, szHeaders, szData);
	
		while (*szUrl)
		{
			// Send the request
			AfxGetApp ()->BeginWaitCursor ();
			strHTML = CHttpRequest::DownloadBuffer (szUrl, nMethod, szHeaders, szData,
				m_pbCancelDownload, m_dwDownloadId, m_nError, 0, NULL, hwndStatus, strMsg);
			AfxGetApp ()->EndWaitCursor ();

//			TRACE (strHTML + "\n");
			
			lpszData = new char[strHTML.GetLength () + 1];

			// Parse the list
			if ((bFoundLyrics = pfn_wslGetLyrics (strHTML.GetBuffer (0),
				strArtist1.GetBuffer (200), strAlbum1.GetBuffer (200), strSong1.GetBuffer (200),
				lpszData, szUrl, false, &nMethod, szHeaders, szData)))
			{
				strArtist1.ReleaseBuffer ();
				strAlbum1.ReleaseBuffer ();
				strSong1.ReleaseBuffer ();
				break;
			}
			strArtist1.ReleaseBuffer ();
			strAlbum1.ReleaseBuffer ();
			strSong1.ReleaseBuffer ();

			delete lpszData;
			lpszData = NULL;
		}

		// set list item data
		LVITEM item;
		item.mask = LVIF_PARAM | LVIF_IMAGE;
		item.iItem = nListItem;
		item.iSubItem = 0;

		// delete the CLyrics object, if already one has been assigned to this item
		ListView_GetItem (hwndList, &item);
		if (item.lParam != 0)
			delete (CLyrics*) item.lParam;

		item.iImage = bFoundLyrics ? 1 : 2;
		item.lParam = bFoundLyrics ?
			(DWORD) (new CLyrics (strArtist1, strAlbum1, strSong1, lpszData)) :
			(DWORD) (new CLyrics (strArtist, strAlbum, strSong, ""));		// original data, if not found

		ListView_SetItem (hwndList, &item);

		// clean up
		if (lpszData != NULL)
		{
			delete lpszData;
			lpszData = NULL;
		}

		// if the lyrics have been found, do not search further
		if (bFoundLyrics)
			break;
	}

	::PostMessage (hwndProgress, PBM_STEPIT, 0, 0);

	return bSuccess;
}

bool CWebLyrics::SearchLyrics (CString strArtist, CString strAlbum, CString strSong, HWND hwndList, HWND hwndProgress, HWND hwndStatus)
{
	bool bSuccess = false;

	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapLyricsHosts;
	RESOURCEHOST* pHost;

	CString strArtist1, strAlbum1, strSong1;

	char* lpszData = NULL;
	CString strHTML, strName, strMsg;
	char szUrl[1000], szHeaders[300], szData[300];
	int nMethod;

	::SendMessage (hwndProgress, PBM_SETRANGE32, 1, pMap->size ());
	::SendMessage (hwndProgress, PBM_SETPOS, 0, 0);
	::SendMessage (hwndProgress, PBM_SETSTEP, 1, 0);
	::ShowWindow (hwndProgress, SW_SHOW);

	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
	{
		pHost = (*it).second;
		if (!pHost->bEnabled)
			continue;

		WSL_GETREQUEST* pfn_wslRequest = (WSL_GETREQUEST*) GetProcAddress (pHost->hInst, "GetLyricsRequest");
		WSL_GETLYRICS* pfn_wslSearchLyrics = (WSL_GETLYRICS*) GetProcAddress (pHost->hInst, "GetLyrics");

		bSuccess = true;

		strArtist1 = CHttpReqSocket::URLEncode (strArtist);
		strAlbum1 = CHttpReqSocket::URLEncode (strAlbum);
		strSong1 = CHttpReqSocket::URLEncode (strSong);
		pfn_wslRequest (strSong1.GetBuffer (0), strArtist1.GetBuffer (0), szUrl, &nMethod, szHeaders, szData);

		ReadLyrics (szUrl, nMethod, szHeaders, szData, pfn_wslSearchLyrics, hwndList, hwndProgress, hwndStatus);
	}
	
	if (ListView_GetItemCount (hwndList) == 0)
		MessageBeep (0);

	return bSuccess;
}

CString CWebLyrics::ExtractUrl(LPSTR lpszUrl, int &nStartPos)
{
	if (!lpszUrl[nStartPos])	// '0' found: end
		return "";

	int nOldPos = nStartPos;
	for ( ; lpszUrl[nStartPos] > 1; nStartPos++)
		;

	// copy substring
	int n = nStartPos - nOldPos;
	CString strRet;
	strncpy (strRet.GetBuffer (n + 1), lpszUrl + nOldPos, n);
	strRet.ReleaseBuffer (n);

	if (lpszUrl[nStartPos])
		nStartPos++;

	return strRet;
}

void CWebLyrics::ReadLyrics(CString strUrl, int& nMethod, LPSTR szHeaders, LPSTR szData, WSL_GETLYRICS* pfn_wslSearchLyrics, HWND hwndList, HWND hwndProgress, HWND hwndStatus)
{
	bool bFoundLyrics;
	CString strNewUrl;
	CString strArtist1, strAlbum1, strSong1;

	char* lpszData = NULL;
	char* lpszUrl = NULL;

	CString strHTML, strName, strMsg;

	// the URL might contain several '1' separated URLs
	for (int nStartPos = 0; ; )
	{
		strNewUrl = ExtractUrl (strUrl.GetBuffer (0), nStartPos);
		if (strNewUrl.IsEmpty ())
			break;

		// Send the request
		AfxGetApp ()->BeginWaitCursor ();
		strHTML = CHttpRequest::DownloadBuffer (strNewUrl, nMethod, szHeaders, szData,
			m_pbCancelDownload, m_dwDownloadId, m_nError, 0, NULL, hwndStatus, strMsg);
		AfxGetApp ()->EndWaitCursor ();
		if (strHTML.IsEmpty ())
			continue;

		lpszData = new char[strHTML.GetLength () + 1];
		lpszUrl = new char[strHTML.GetLength () + 1];

		// Parse the list
		bFoundLyrics = pfn_wslSearchLyrics (strHTML.GetBuffer (0),
			strArtist1.GetBuffer (200), strAlbum1.GetBuffer (200), strSong1.GetBuffer (200),
			lpszData, lpszUrl, true, &nMethod, szHeaders, szData);

		strArtist1.ReleaseBuffer ();
		strAlbum1.ReleaseBuffer ();
		strSong1.ReleaseBuffer ();

		if (bFoundLyrics && (lpszData != NULL))
		{
			// set list item data
			LVITEM item;
			item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			item.pszText = new char[strSong1.GetLength () + 1];
			strcpy (item.pszText, strSong1.GetBuffer (0));
			item.iItem = ListView_GetItemCount (hwndList);
			item.iSubItem = 0;
			item.iImage = 0;
			item.lParam = (DWORD) (new CLyrics (strArtist1, strAlbum1, strSong1, lpszData));

			ListView_InsertItem (hwndList, &item);
		}

		if (*lpszUrl)
			ReadLyrics (lpszUrl, nMethod, szHeaders, szData, pfn_wslSearchLyrics, hwndList, hwndProgress, hwndStatus);

		delete lpszData;
		lpszData = NULL;

		delete lpszUrl;
		lpszUrl = NULL;
	}

	::PostMessage (hwndProgress, PBM_STEPIT, 0, 0);
}
