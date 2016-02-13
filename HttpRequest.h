// HttpRequest.h: interface for the CHttpRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPREQUEST_H__B2BC7EA4_AF7B_4081_923C_02029140D5BE__INCLUDED_)
#define AFX_HTTPREQUEST_H__B2BC7EA4_AF7B_4081_923C_02029140D5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HTTPFILE_BUFFLEN	4096
#define BUFFLEN 			255

class CHttpRequest  
{
public:
	CHttpRequest();
	virtual ~CHttpRequest();

	static CString DownloadBuffer(CString strURL, int nMethod, CString strHeaders, CString strData,
		bool* pbCancelDownload, DWORD dwDownloadId, int& nError, int nApproxContentLength = 0,				   
		HWND hwndProgress = NULL, HWND hwndStatus = NULL, CString strMsg = "");
	static void DownloadFile(CString strURL, int nMethod, CString strHeaders, CString strData,
		CString strFilename,
		bool* pbCancelDownload, DWORD dwDownloadId, int& nError, int nApproxContentLength = 0,
		HWND hwndProgress = NULL, HWND hwndStatus = NULL, CString strMsg = "");

private:
	static CString Download (CString strURL, int nMethod, CString strHeaders, CString strData,
		CString strFilename, bool* pbCancelDownload, DWORD dwDownloadId, int& nError, int nApproxContentLength,
		HWND hwndProgress, HWND hwndStatus, CString strMsg);

	static CString Download1 (CString strURL, int nMethod, CString strHeaders, CString strData,
		CString strFilename, bool* pbCancelDownload, DWORD dwDownloadId, int& nError, int nApproxContentLength,
		HWND hwndProgress, HWND hwndStatus, CString strMsg);
};

#endif // !defined(AFX_HTTPREQUEST_H__B2BC7EA4_AF7B_4081_923C_02029140D5BE__INCLUDED_)
