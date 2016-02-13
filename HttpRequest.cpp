// HttpRequest.cpp: implementation of the CHttpRequest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "HttpRequest.h"
#include "MyInternetSession.h"
#include "BindStatusCallback.h"

#include <list>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD gl_dwNextDownloadId = 0;
std::list<DWORD> gl_stkDownloads;

CString strStatusMsg, strKB, strPercent, strKBs;

CHttpRequest::CHttpRequest()
{
}

CHttpRequest::~CHttpRequest()
{
}

CString CHttpRequest::DownloadBuffer(CString strURL, int nMethod, CString strHeaders, CString strData,
									 bool* pbCancelDownload, DWORD dwDownloadId, int& nError,
									 int nApproxContentLength,									   
									 HWND hwndProgress, HWND hwndStatus, CString strMsg)
{
	return CHttpRequest::Download1 (strURL, nMethod, strHeaders, strData, "", pbCancelDownload, dwDownloadId, nError, nApproxContentLength, hwndProgress, hwndStatus, strMsg);
}

void CHttpRequest::DownloadFile(CString strURL, int nMethod, CString strHeaders, CString strData,
								CString strFilename,
								bool* pbCancelDownload, DWORD dwDownloadId, int& nError,
								int nApproxContentLength,
								HWND hwndProgress, HWND hwndStatus, CString strMsg)
{
	CHttpRequest::Download1 (strURL, nMethod, strHeaders, strData, strFilename, pbCancelDownload, dwDownloadId, nError, nApproxContentLength, hwndProgress, hwndStatus, strMsg);
}


/////////////////////////////////////////////////////////////////////////////
// download routine

CString CHttpRequest::Download1 (CString strURL, int nMethod, CString strHeaders, CString strData,
	CString strFilename,
	bool* pbCancelDownload, DWORD dwDownloadId, int& nError, int nApproxContentLength,									   
	HWND hwndProgress, HWND hwndStatus, CString strMsg)
{
	gl_stkDownloads.push_back (dwDownloadId);

	CBindStatusCallback callback (nMethod == 1 ? BINDVERB_GET : BINDVERB_POST, strHeaders, strData,
		dwDownloadId, &gl_stkDownloads, GetCurrentTime (), hwndProgress, hwndStatus);

	// show the UI controls
	if (hwndStatus != NULL)
		::PostMessage (hwndStatus, SB_SETTEXT, 1, (LPARAM) strStatusMsg.GetBuffer (0));

	CString strRetData;

	if (!strFilename.IsEmpty ())
	{
		// download to file
		HRESULT hr = URLDownloadToFile (NULL, strURL, strFilename, 0, &callback);
	}
	else
	{
		IStream* pStream;
		char szBuffer[0x100];
		DWORD dwGot;
		HRESULT hr = NOERROR;

		// Open a blocking type stream to the Web site.
		hr = URLOpenBlockingStream (NULL, strURL, &pStream, 0, &callback);
		if (hr != S_OK)
		{
			CString strMsg;
			strMsg.Format (IDS_ERROR_ACCESSING_URL, strURL.GetBuffer (-1));
			AfxMessageBox (strMsg, MB_ICONERROR);
		}
		else
		{
			do
			{
				hr = pStream->Read (szBuffer, sizeof (szBuffer) - 1, &dwGot);
				szBuffer[dwGot] = 0;
				strRetData += szBuffer;
			} while (SUCCEEDED (hr) && hr != S_FALSE);

			pStream->Release();
		}
	}

	// hide UI controls
	if (dwDownloadId == gl_stkDownloads.back ())
	{
		if (hwndProgress != NULL)
			::ShowWindow (hwndProgress, SW_HIDE);
		if (hwndStatus != NULL)
			for (int i = 1; i <= 4; i++)
				::PostMessage (hwndStatus, SB_SETTEXT, i, (LPARAM) "");
	}

	gl_stkDownloads.remove (dwDownloadId);
	return strRetData;
}

CString CHttpRequest::Download (CString strURL, int nMethod, CString strHeaders, CString strData,
	CString strFilename,
	bool* pbCancelDownload, DWORD dwDownloadId, int& nError, int nApproxContentLength,									   
	HWND hwndProgress, HWND hwndStatus, CString strMsg)
{
	gl_stkDownloads.push_back (dwDownloadId);

	bool bUseFile = !strFilename.IsEmpty ();
	strStatusMsg = bUseFile ? strFilename : strMsg;

	// show the UI controls
	if (dwDownloadId == gl_stkDownloads.back ())
	{
		if (hwndProgress != NULL)
		{
			::PostMessage (hwndProgress, PBM_SETPOS, 0, 0);
			::ShowWindow (hwndProgress, SW_SHOW);
		}

		if (hwndStatus != NULL)
			::PostMessage (hwndStatus, SB_SETTEXT, 1, (LPARAM) strStatusMsg.GetBuffer (0));
	}

	/*
	CString strDisplayFilename, strDisplay;
	if (!strFilename.IsEmpty () && (pStatus != NULL))
	{
		strcpy (strDisplayFilename.GetBuffer (strFilename.GetLength () + 1), strFilename.GetBuffer (0));
		strDisplayFilename.ReleaseBuffer ();

		// needs ellipsis?
		CRect r;
		pStatus->GetWindowRect (&r);
		r.right -= 300;
		HDC hdc = ::GetWindowDC (pStatus->GetSafeHwnd ());
		::DrawText (hdc, strDisplayFilename, -1, &r, DT_PATH_ELLIPSIS | DT_MODIFYSTRING);
		::ReleaseDC (pStatus->GetSafeHwnd (), hdc);
	}*/

	CHttpConnection* pHttpConnection = NULL;
	CHttpFile* pHttpFile = NULL;
	LPVOID lpvoid = NULL;

	DWORD dwServiceType;
	CString strServer, strObject;
	INTERNET_PORT nPort;
	AfxParseURL (strURL, dwServiceType, strServer, strObject, nPort);

	// declare CMyInternetSession object
	CInternetSession InternetSession (NULL,
		//"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0; (R1 1.3))",
		1, INTERNET_OPEN_TYPE_PRECONFIG);

	CString strRetData;

	try
	{
		// establish the HTTP connection
		pHttpConnection = InternetSession.GetHttpConnection (strServer, nPort, "", "");

		// open the HTTP request
		pHttpFile = pHttpConnection->OpenRequest (nMethod, strObject, NULL, 1, NULL,
			NULL, INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_TRANSFER_BINARY);

		if (pHttpFile->SendRequest (strHeaders, strHeaders.GetLength (),
			static_cast<void*>(strData.GetBuffer (0)), strData.GetLength ()))
		{
			DWORD dwRet;
			// query the HTTP status code
			pHttpFile->QueryInfoStatusCode (dwRet);
			if (dwRet >= 400)
			{
				nError = dwRet;
				gl_stkDownloads.remove (dwDownloadId);
				return "";
			}
			
			CString strContentLen;
			LPSTR lpszResult = NULL;
			UINT nRead = 0, nTotalRead = 0;

			lpvoid = malloc (HTTPFILE_BUFFLEN);
			// memory error
			if (!lpvoid)
			{
				AfxMessageBox("Error allocating memory");
				if (pHttpConnection)
				{
					pHttpConnection->Close();
					delete pHttpConnection;
				}
				if (pHttpFile)
				{
					pHttpFile->Close();
					delete pHttpFile;
				}
				nError = -1;
				gl_stkDownloads.remove (dwDownloadId);
				return "";
			}

			CFile f1;

			DWORD dwSize;
			if (!pHttpFile->QueryInfo (HTTP_QUERY_CONTENT_LENGTH, dwSize))
				dwSize = nApproxContentLength;
			if ((dwDownloadId == gl_stkDownloads.back ()) && (hwndProgress != NULL))
				::PostMessage (hwndProgress, PBM_SETRANGE32, 0, dwSize);

			if (bUseFile)
				if (!f1.Open (strFilename, CFile::modeCreate | CFile::modeWrite))
				{
					nError = -1;
					gl_stkDownloads.remove (dwDownloadId);
					return "";
				}

			DWORD dwStart = GetTickCount ();

			// read the HTTP response
			while (nRead = pHttpFile->Read ((byte*) lpvoid + (bUseFile ? 0 : nTotalRead), HTTPFILE_BUFFLEN))
			{
				if (pbCancelDownload != NULL)
					if (*pbCancelDownload)
					{
						// download cancelled
						if (pHttpConnection)
						{
							pHttpConnection->Close();
							delete pHttpConnection;
						}
						if (pHttpFile)
						{
							pHttpFile->Close();
							delete pHttpFile;
						}

						// delete the file
						if (bUseFile)
						{
							f1.Close ();
							CFile::Remove (strFilename);
						}

						nError = -1;
						gl_stkDownloads.remove (dwDownloadId);
						return "";
					}

				nTotalRead += nRead;

				// store the data
				if (bUseFile)
					f1.Write (lpvoid, nRead);
				else
				{
					lpvoid = realloc (lpvoid, nTotalRead + HTTPFILE_BUFFLEN);
					if (!lpvoid)
					{
						AfxMessageBox ("Error allocating memory");
						if (pHttpConnection)
						{
							pHttpConnection->Close();
							delete pHttpConnection;
						}
						if (pHttpFile)
						{
							pHttpFile->Close();
							delete pHttpFile;
						}

						nError = -1;
						gl_stkDownloads.remove (dwDownloadId);
						return "";
					}
				}

				// UI Feedback
				if (dwDownloadId == gl_stkDownloads.back ())
				{
					// update progress control
					if (hwndProgress != NULL)
						::PostMessage (hwndProgress, PBM_SETPOS, nTotalRead, 0);

					if (hwndStatus != NULL)
					{
						strKB.Format (IDS_DOWNLOADSTATUS_KB,
							nTotalRead / 1024,			// KBs read
							dwSize / 1024);				// Total size (KBs)
						::PostMessage (hwndStatus, SB_SETTEXT, 2, (LPARAM) strKB.GetBuffer (0));

						// percentage
						if (dwSize != 0)
						{
							strPercent.Format (IDS_DOWNLOADSTATUS_PERCENT, nTotalRead * 100 / dwSize);
							::PostMessage (hwndStatus, SB_SETTEXT, 3, (LPARAM) strPercent.GetBuffer (0));
						}

						// KB/s
						strKBs.Format (IDS_DOWNLOADSTATUS_KBS, (double) nTotalRead / 1024.0 / ((GetTickCount () - dwStart) / 1000.0 + 1));
						::PostMessage (hwndStatus, SB_SETTEXT, 4, (LPARAM) strKBs.GetBuffer (0));
					}
				}
			}
					
			if (bUseFile)
				f1.Close();
			else
			{
				// get response headers
				LPSTR lpszBuf = strRetData.GetBuffer (nTotalRead + HTTPFILE_BUFFLEN);
				DWORD dwBufLen = HTTPFILE_BUFFLEN;
				pHttpFile->QueryInfo (HTTP_QUERY_RAW_HEADERS_CRLF, lpszBuf, &dwBufLen);

				// copy contents
				*((LPSTR) lpvoid + nTotalRead) = 0;
				strcpy (lpszBuf + dwBufLen - 1, (LPSTR) lpvoid);

				strRetData.ReleaseBuffer ();
			}
		}
	}
	catch(CInternetException *e)
	{
		//e->ReportError();
		char szError[1001];
		e->GetErrorMessage (szError, 1000);
		CString strMsg;
		strMsg.Format (IDS_ERROR_HTTPREQUEST, strURL.GetBuffer (-1), szError);
		AfxMessageBox (strMsg, MB_ICONEXCLAMATION);

		e->Delete();
	}

	// cleanup
	if (lpvoid)
		free (lpvoid);

	if (pHttpFile)
	{
		pHttpFile->Close();
		delete pHttpFile;
	}
	if (pHttpConnection)
	{
		pHttpConnection->Close();
		delete pHttpConnection;
	}

	// hide UI controls
	if (dwDownloadId == gl_stkDownloads.back ())
	{
		if (hwndProgress != NULL)
			::ShowWindow (hwndProgress, SW_HIDE);
		if (bUseFile)
			if (hwndStatus != NULL)
				for (int i = 1; i <= 4; i++)
					::PostMessage (hwndStatus, SB_SETTEXT, i, (LPARAM) "");
	}

	gl_stkDownloads.remove (dwDownloadId);
	return strRetData;
}