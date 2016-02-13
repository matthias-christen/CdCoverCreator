// WebCDCovers.cpp: implementation of the CWebCDCovers class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "WebCDCovers.h"

#include "HttpReqSocket.h"
#include "HttpRequest.h"

#include "wcc_data.h"

#include "..\3rdParty\ZipArchive\ZipArchive.h"
#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern DWORD gl_dwNextDownloadId;
extern std::list<DWORD> gl_stkDownloads;

// thread functions

UINT wccGetList(LPVOID pvParams)
{
	WCC_GETLISTPARAMS* pParams = (WCC_GETLISTPARAMS*) pvParams;

	CWebCDCovers wcc;

	wcc.SetDownloadCancelFlag (pParams->pCancelDownload);
	wcc.m_dwDownloadId = ++gl_dwNextDownloadId;

	bool bResult = wcc.GetList (pParams->strValue,
		pParams->hwndList, pParams->htiParent,
		pParams->hwndProgress, pParams->hwndStatus);

	if (pParams->hwndStatus != NULL)
		for (int i = 1; i <= 4; i++)
			::PostMessage (pParams->hwndStatus, SB_SETTEXT, i, (LPARAM) "");

	::SendMessage (pParams->hwndNotify, WCCM_LISTCOVERS, 0, 0);
	delete pParams;
	return bResult ? 0 : 1;
}

UINT wccSearchCovers(LPVOID pvParams)
{
	WCC_GETLISTPARAMS* pParams = (WCC_GETLISTPARAMS*) pvParams;

	CWebCDCovers wcc;
	
	wcc.SetDownloadCancelFlag (pParams->pCancelDownload);
	wcc.m_dwDownloadId = ++gl_dwNextDownloadId;

	bool bResult = wcc.SearchCovers (pParams->strValue,
		pParams->hwndList, pParams->hwndProgress, pParams->hwndStatus);

	if (pParams->hwndStatus != NULL)
	{
		for (int i = 1; i <= 4; i++)
			::PostMessage (pParams->hwndStatus, SB_SETTEXT, i, (LPARAM) "");

		if (!bResult)
		{
			CString str;
			str.LoadString (IDS_QUERYEMPTY);

			::PostMessage (pParams->hwndStatus, SB_SETTEXT, 1, (LPARAM) str.GetBuffer (0));
			::MessageBeep (0);
		}
	}

	::SendMessage (pParams->hwndNotify, WCCM_SEARCHCOVERS, 0, 0);
	delete pParams;
	return bResult ? 0 : 1;
}

UINT wccGetImages(LPVOID pvParams)
{
	WCC_GETIMAGESPARAMS* pParams = (WCC_GETIMAGESPARAMS*) pvParams;

	CWebCDCovers wcc;

	wcc.SetDownloadCancelFlag (pParams->pCancelDownload);
	wcc.m_dwDownloadId = ++gl_dwNextDownloadId;

	wcc.DownloadImage (pParams->strLetter, pParams->strName,
		pParams->pCoverImages->strFile[pParams->rt], pParams->pCoverImages->pHost,
		pParams->hwndProgress, pParams->hwndStatus,
		pParams->pCoverImages->strLocalFile[pParams->rt]);

	::SendMessage (pParams->hwndNotify, WCCM_GETIMAGES, 0, 0);
	delete pParams;
	return 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebCDCovers::CWebCDCovers()
{
	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapCoverHosts;
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

	// do the authorization
}

CWebCDCovers::~CWebCDCovers()
{
	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapCoverHosts;
	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
		FreeLibrary ((*it).second->hInst);
}


void CWebCDCovers::SetDownloadCancelFlag(bool *pbCancelDownload)
{
	m_pbCancelDownload = pbCancelDownload;
}

bool CWebCDCovers::GetList(CString strLetter, HWND hwndList, HTREEITEM htiParent, HWND hwndProgress, HWND hwndStatus)
{
	bool bSuccess = false;

	//if (!CHttpReqSocket::IsInternetConnected ())
	//{
	//	AfxMessageBox (IDS_INTERNETERROR_IMAGES, MB_ICONEXCLAMATION);
	//	m_nError = -1;
	//	return false;
	//}

	CString strMsg;
	strMsg.LoadString (IDS_DOWNLOADING_LIST);

	if (m_dwDownloadId == gl_stkDownloads.back ())
	{
		::SendMessage (hwndProgress, PBM_SETPOS, 0, 0);
		::ShowWindow (hwndProgress, SW_SHOW);
	}
/*
	for (int i = 0; i < pList->GetItemCount (); i++)
		delete (COVERIMAGES*) pList->GetItemData (i);
	pList->DeleteAllItems ();
*/
	// do the downloads
	bSuccess = RetrieveList (WCC_LIST, strLetter, hwndList, htiParent, hwndProgress, hwndStatus, strMsg);

	if (m_dwDownloadId == gl_stkDownloads.back ())
	{
		for (int i = 1; i <= 4; i++)
			::SendMessage (hwndStatus, SB_SETTEXT, i, (LPARAM) "");
		::ShowWindow (hwndProgress, SW_HIDE);
	}

	return bSuccess;
}

bool CWebCDCovers::SearchCovers(CString strKeywords, HWND hwndList, HWND hwndProgress, HWND hwndStatus)
{
	bool bSuccess = false;

	//if (!CHttpReqSocket::IsInternetConnected ())
	//{
	//	AfxMessageBox (IDS_INTERNETERROR_IMAGES, MB_ICONEXCLAMATION);
	//	m_nError = -1;
	//	return false;
	//}

	CString strMsg;
	strMsg.Format (IDS_SEARCHING, (LPSTR) (LPCTSTR) strKeywords);

	if (m_dwDownloadId == gl_stkDownloads.back ())
	{
		::SendMessage (hwndProgress, PBM_SETPOS, 0, 0);
		::ShowWindow (hwndProgress, SW_SHOW);
	}

	LVITEM lvItem;
	for (int i = 0; i < ListView_GetItemCount (hwndList); i++)
	{
		ListView_GetItem (hwndList, &lvItem);
		//delete (COVERIMAGES*) lvItem.lParam;
	}
	ListView_DeleteAllItems (hwndList);

	// do the downloads
	bSuccess = RetrieveList (WCC_SEARCH, CHttpReqSocket::URLEncode (strKeywords),
		hwndList, NULL, hwndProgress, hwndStatus, strMsg);

	if (m_dwDownloadId == gl_stkDownloads.back ())
	{
		for (int i = 1; i <= 4; i++)
			::SendMessage (hwndStatus, SB_SETTEXT, i, (LPARAM) "");
		::ShowWindow (hwndProgress, SW_HIDE);
	}

	return bSuccess;
}

///////////////////////////////////////////////////////////////////
// internal helpers


CString CWebCDCovers::GetCookies(CString strResponse)
{
	CString strCookie;
	strcpy (strCookie.GetBuffer (strResponse.GetLength () + 1), strResponse.GetBuffer (0));
	strCookie.ReleaseBuffer ();

	strCookie.MakeLower ();
	int nPos = strCookie.Find ("set-cookie: ");
	if (nPos >= 0)
	{
		int nPos2 = strCookie.Find (';', nPos);
		strCookie = strCookie.Mid (nPos + 12, nPos2 - nPos - 12);
	}

	return strCookie;
}

bool CWebCDCovers::DownloadImage(CString strLetter, CString strName, CString strFilename, RESOURCEHOST* pHost, HWND hwndProgress, HWND hwndStatus, CString& strLocalFilename)
{
	// get the html page for the image source
	*m_pbCancelDownload = false;

	WCC_GETIMAGEREQUEST* pfn_wccRequest = (WCC_GETIMAGEREQUEST*) GetProcAddress (pHost->hInst, "GetImageRequest");
	WCC_GETIMAGEURL* pfn_wccImgURL = (WCC_GETIMAGEURL*) GetProcAddress (pHost->hInst, "GetImageURL");

	char szUrl[300], szHeaders[300], szData[300];
	int nMethod;
	pfn_wccRequest (strFilename.GetBuffer (0), strLetter.GetBuffer (0), "", szUrl, &nMethod, szHeaders, szData);

	CString strHTML = (*szUrl == 0) ? strFilename.GetBuffer (0) :
		CHttpRequest::DownloadBuffer (szUrl, nMethod, szHeaders, szData,
			m_pbCancelDownload, m_dwDownloadId, m_nError);

	// MCH 29/08/04
	// Send version info to the DLL
	strcpy (szData, ((CCdCoverCreator2App*) AfxGetApp ())->GetVersion ().GetBuffer (-1));

	// get the URL of the image
	CString strURL;
	if (!pfn_wccImgURL (strHTML.GetBuffer (0), strURL.GetBuffer (500), &nMethod, szHeaders, szData))
		return false;
	strURL.ReleaseBuffer ();

	// generate a local temporary file name
	strLocalFilename = ((CCdCoverCreator2App*) AfxGetApp ())->GetImageCacheDir () + strURL.Mid (strURL.ReverseFind ('/') + 1);
	if (strLocalFilename.Find (".php?") >= 0)
	{
		// MCH 15/09/04
		// www.darktown.to sends again JPGs, but via a PHP script. Filename provided as HTTP header which is not available here
		strLocalFilename = ((CCdCoverCreator2App*) AfxGetApp ())->GetImageCacheDir ();
		strLocalFilename.AppendFormat ("dld%d.jpg", GetTickCount ());
	}

	// get the jpeg image
	*m_pbCancelDownload = false;
	CHttpRequest::DownloadFile (strURL, nMethod, szHeaders, szData, strLocalFilename,
		m_pbCancelDownload, m_dwDownloadId, m_nError, 0, hwndProgress, hwndStatus);

	if (m_nError)
		return false;

	// MCH 29/08/04
	// if the download is a zip file, extract the image
	if (strLocalFilename.Right (4).MakeLower () == ".zip")
	{
		CString strPath = strLocalFilename.Left (strLocalFilename.ReverseFind ('\\'));
		CString strZipFile = strLocalFilename;

		// set up and open a zip archive, get the image files inside the zip file
		CZipArchive zip;
		zip.Open (strZipFile);
		CZipWordArray ar;
		zip.FindMatches ("*.jp*g", ar);

		strLocalFilename.Empty ();

		// extract the image files
		for (int i = 0; i < ar.GetSize (); i++)
		{
			// set the local file name
			CZipFileHeader info;
			zip.GetFileInfo (info, i);
			if (!strLocalFilename.IsEmpty ())
				strLocalFilename += ";";
			strLocalFilename += strPath + "\\" + info.GetFileName ();
			
			// extract the file
			zip.ExtractFile (ar.GetAt (i), strPath, false);
		}

		zip.Close ();

		// delete the zip file
		::DeleteFile (strZipFile);
	}

	return true;
}

bool CWebCDCovers::RetrieveList(int nIndex, CString strCriteria, HWND hwndList, HTREEITEM htiParent, HWND hwndProgress, HWND hwndStatus, CString strMsg)
{
	bool bSuccess = false;

	COVERIMAGES* pCoverImages;

	HOSTMAP* pMap = &((CCdCoverCreator2App*) AfxGetApp ())->m_mapCoverHosts;
	RESOURCEHOST* pHost;

	char* lpszData = NULL;
	CString strHTML, strName;
	char szUrl[300], szHeaders[300], szData[300];
	int nMethod, nPages;
	bool bPagesSet, bExpanded = false;

	::PostMessage (hwndProgress, PBM_SETPOS, 0, 0);
	::ShowWindow (hwndProgress, SW_SHOW);

	for (HOSTMAP::iterator it = pMap->begin (); it != pMap->end (); it++)
	{
		pHost = (*it).second;
		if (!pHost->bEnabled)
			continue;

		WCC_GETREQUEST* pfn_wccRequest = (WCC_GETREQUEST*) GetProcAddress (pHost->hInst, nIndex == WCC_LIST ? "GetListCoversRequest" : "GetSearchCoversRequest");
		WCC_GETLIST* pfn_wccList = (WCC_GETLIST*) GetProcAddress (pHost->hInst, nIndex == WCC_LIST ? "ListCovers" : "SearchCovers");

		bSuccess = true;
		bPagesSet = false;

		pfn_wccRequest (strCriteria.GetBuffer (0), "", szUrl, &nMethod, szHeaders, szData);

		while (*szUrl)
		{
			// Send the request
			AfxGetApp ()->BeginWaitCursor ();
			strHTML = CHttpRequest::DownloadBuffer (szUrl, nMethod, szHeaders, szData,
				m_pbCancelDownload, m_dwDownloadId, m_nError, 0, NULL, hwndStatus, strMsg);
			AfxGetApp ()->EndWaitCursor ();
			
			lpszData = new char[strHTML.GetLength () + 1];

			// Parse the list
			if (!pfn_wccList (strHTML.GetBuffer (0), strCriteria.GetBuffer (0), lpszData, szUrl, &nMethod, szHeaders, szData, &nPages))
			{
				delete lpszData;
				lpszData = NULL;
				continue;
			}

			if (nPages >= 1)
			{
				// has multiple pages
				if (!bPagesSet)
				{
					::SendMessage (hwndProgress, PBM_SETRANGE32, 1, nPages);
					::SendMessage (hwndProgress, PBM_SETSTEP, 1, 0);
					::SendMessage (hwndProgress, PBM_SETPOS, 0, 0);
					::ShowWindow (hwndProgress, SW_SHOW);
					bPagesSet = true;
				}
				else
					::SendMessage (hwndProgress, PBM_STEPIT, 0, 0);
			}
			else
			{
				// only one page...
				::ShowWindow (hwndProgress, SW_HIDE);
			}

			// fill the List
			for (int nPos = 0; ; )
			{
				pCoverImages = new COVERIMAGES;
				pCoverImages->pHost = pHost;

				if (!GetData (lpszData, nPos, strName, pCoverImages))
				{
					delete pCoverImages;
					break;
				}

				if (htiParent == NULL)
				{
					// this is a list control

					LVITEM item;
					item.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
					item.iItem = ListView_GetItemCount (hwndList);
					item.iSubItem = 0;
					item.pszText = (LPSTR) (LPCTSTR) strName;
					item.iImage = 1;
					item.lParam = (DWORD) pCoverImages;
					ListView_InsertItem (hwndList, &item);
				}
				else
				{
					bool bChanged = true;

					// look for item already in the tree
					TVITEM item;
					char szText[500];
					for (HTREEITEM hItem = TreeView_GetChild (hwndList, htiParent); hItem != NULL; hItem = TreeView_GetNextSibling (hwndList, hItem))
					{
						item.mask = TVIF_TEXT | TVIF_PARAM;
						item.hItem = hItem;
						item.pszText = szText;
						item.cchTextMax = 500;
						TreeView_GetItem (hwndList, &item);

						// item has been found
						if (strName == szText)
						{
							if (!CoverImagesAreEqual (pCoverImages, (COVERIMAGES*) item.lParam))
							{
								// remove old item and replace it by new one if it has changed
								delete (COVERIMAGES*) item.lParam;
								TreeView_DeleteItem (hwndList, hItem);
							}
							else
								bChanged = false;

							break;
						}
					}

					// insert the item if it has changed (or is new)
					if (bChanged)
					{
						TVINSERTSTRUCT insert;
						insert.hParent = htiParent;
						insert.hInsertAfter = TVI_SORT;
						insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
						insert.item.pszText = (LPSTR) (LPCTSTR) strName;
						insert.item.iImage = 1;
						insert.item.iSelectedImage = 1;
						insert.item.lParam = (DWORD) pCoverImages;

						TreeView_InsertItem (hwndList, &insert);
					}
				}
			}

			// expand the branch
			if (!bExpanded)
			{
				TreeView_Expand (hwndList, htiParent, TVE_EXPAND);
				bExpanded = true;
			}

			// clean up
			if (lpszData != NULL)
			{
				delete lpszData;
				lpszData = NULL;
			}
		}
	}

	::ShowWindow (hwndProgress, SW_HIDE);

	return bSuccess;
}

bool CWebCDCovers::GetData(LPSTR lpszData, int& nPos, CString &strName, COVERIMAGES* pCoverImages)
{
	if (lpszData[nPos] == 0)
		return false;

	CopyData (lpszData, nPos, strName);
	CopyData (lpszData, nPos, pCoverImages->strFile[Cover]);
	CopyData (lpszData, nPos, pCoverImages->strFile[Back]);
	CopyData (lpszData, nPos, pCoverImages->strFile[Inlay]);
	CopyData (lpszData, nPos, pCoverImages->strFile[Label]);

	return true;
}

void CWebCDCovers::CopyData(LPSTR lpszData, int &nPos, CString &strRet)
{
	int nOldPos = nPos;
	for ( ; lpszData[nPos] != 1; nPos++)
		;
	int n = nPos - nOldPos;
	LPSTR lpszBuf = strRet.GetBuffer (n + 1);
	strncpy (lpszBuf, lpszData + nOldPos, n);
	lpszBuf[n] = 0;
	strRet.ReleaseBuffer ();

	strRet.Replace ("&nbsp;", " ");
	strRet.Replace ("&amp;", "&");
	strRet.Replace ("&lt;", "<");
	strRet.Replace ("&gt;", ">");

	nPos++;
}

bool CWebCDCovers::CoverImagesAreEqual(COVERIMAGES *pCi1, COVERIMAGES *pCi2)
{
	if (pCi1->pHost != pCi2->pHost)
		return false;
	for (int i = 0; i < 4; i++)
		if (pCi1->strFile[i] != pCi2->strFile[i])
			return false;
	return true;
}
