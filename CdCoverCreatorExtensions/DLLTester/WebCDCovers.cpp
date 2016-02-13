// WebCDCovers.cpp: implementation of the CWebCDCovers class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WebCDCovers.h"

#include "HttpReqSocket.h"
#include "HttpRequest.h"

#include "wcc_data.h"
#include <windowsx.h>

#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWebCDCovers::CWebCDCovers(HINSTANCE hInst)
{
	m_hInst = hInst;
}

CWebCDCovers::~CWebCDCovers()
{
}


void CWebCDCovers::SetDownloadCancelFlag(bool *pbCancelDownload)
{
	m_pbCancelDownload = pbCancelDownload;
}

bool CWebCDCovers::GetList(CString strLetter, HWND hwndList)
{
	bool bSuccess = false;

/*	if (!CHttpReqSocket::IsInternetConnected ())
	{
		AfxMessageBox ("No internet connection", MB_ICONEXCLAMATION);
		m_nError = -1;
		return false;
	}*/

	// do the downloads
	CString strMsg = "";
	bSuccess = RetrieveList (WCC_LIST, strLetter, hwndList, strMsg);

	return bSuccess;
}

bool CWebCDCovers::SearchCovers(CString strKeywords, HWND hwndList)
{
	bool bSuccess = false;

/*	if (!CHttpReqSocket::IsInternetConnected ())
	{
		AfxMessageBox ("No internet connection", MB_ICONEXCLAMATION);
		m_nError = -1;
		return false;
	}*/

	ListBox_ResetContent (hwndList);

	// do the downloads
	CString strMsg;
	bSuccess = RetrieveList (WCC_SEARCH, CHttpRequest::URLEncode (strKeywords), hwndList, strMsg);

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

bool CWebCDCovers::DownloadImage(CString strLetter, CString strName, CString strFilename, CString& strLocalFilename)
{
	// get the html page for the image source
	*m_pbCancelDownload = false;

	WCC_GETIMAGEREQUEST* pfn_wccRequest = (WCC_GETIMAGEREQUEST*) GetProcAddress (m_hInst, "GetImageRequest");
	WCC_GETIMAGEURL* pfn_wccImgURL = (WCC_GETIMAGEURL*) GetProcAddress (m_hInst, "GetImageURL");

	char szUrl[300], szHeaders[300], szData[300];
	int nMethod;
	pfn_wccRequest (strFilename.GetBuffer (0), strLetter.GetBuffer (0), "", szUrl, &nMethod, szHeaders, szData);

	CString strHTML = (*szUrl == 0) ? strFilename.GetBuffer (0) :
		CHttpRequest::DownloadBuffer (szUrl, nMethod, szHeaders, szData,
			m_pbCancelDownload, 0, m_nError);

	// get the URL of the image
	CString strURL;
	strcpy (szData, "2.4");	// CdCoverCreator version
	if (!pfn_wccImgURL (strHTML.GetBuffer (0), strURL.GetBuffer (500), &nMethod, szHeaders, szData))
		return false;
	strURL.ReleaseBuffer ();

	// generate a local temporary file name
	//strLocalFilename = strURL.Mid (strURL.ReverseFind ('/') + 1);
	strLocalFilename.Format ("dlg%d.jpg", GetTickCount ());

	// get the jpeg image
	*m_pbCancelDownload = false;
	CHttpRequest::DownloadFile (strURL, nMethod, szHeaders, szData, strLocalFilename,
		m_pbCancelDownload, m_dwDownloadId, m_nError);

	if (m_nError)
		return false;

	return true;
}

bool CWebCDCovers::RetrieveList(int nIndex, CString strCriteria, HWND hwndList, CString strMsg)
{
	bool bSuccess = false;

	COVERIMAGES* pCoverImages;

	char* lpszData = NULL;
	CString strHTML, strName;
	char szUrl[300], szHeaders[300], szData[300];
	int nMethod, nPages;
	bool bPagesSet, bExpanded = false;

	WCC_GETREQUEST* pfn_wccRequest = (WCC_GETREQUEST*) GetProcAddress (m_hInst, nIndex == WCC_LIST ? "GetListCoversRequest" : "GetSearchCoversRequest");
	WCC_GETLIST* pfn_wccList = (WCC_GETLIST*) GetProcAddress (m_hInst, nIndex == WCC_LIST ? "ListCovers" : "SearchCovers");

	bSuccess = true;
	bPagesSet = false;

	pfn_wccRequest (strCriteria.GetBuffer (0), "", szUrl, &nMethod, szHeaders, szData);

	while (*szUrl)
	{
		// Send the request
		AfxGetApp ()->BeginWaitCursor ();
		strHTML = CHttpRequest::DownloadBuffer (szUrl, nMethod, szHeaders, szData,
			m_pbCancelDownload, m_dwDownloadId, m_nError, 0, NULL, NULL, strMsg);
		AfxGetApp ()->EndWaitCursor ();
		
		lpszData = new char[strHTML.GetLength () + 1];

		// Parse the list
		if (!pfn_wccList (strHTML.GetBuffer (0), strCriteria.GetBuffer (0), lpszData, szUrl, &nMethod, szHeaders, szData, &nPages))
		{
			delete lpszData;
			lpszData = NULL;
			continue;
		}

		// fill the List
		for (int nPos = 0; ; )
		{
			pCoverImages = new COVERIMAGES;
			//pCoverImages->pHost = pHost;

			if (!GetData (lpszData, nPos, strName, pCoverImages))
			{
				delete pCoverImages;
				break;
			}

			int nIdx = ListBox_AddString (hwndList, (LPSTR) (LPCTSTR) strName);
			ListBox_SetItemData (hwndList, nIdx, pCoverImages);
		}

		// clean up
		if (lpszData != NULL)
		{
			delete lpszData;
			lpszData = NULL;
		}
	}

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
	for (int nOldPos = nPos; lpszData[nPos] != 1; nPos++);
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
//	if (pCi1->pHost != pCi2->pHost)
//		return false;
	for (int i = 0; i < 4; i++)
		if (pCi1->strFile[i] != pCi2->strFile[i])
			return false;
	return true;
}
