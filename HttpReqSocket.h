#if !defined(AFX_HTTPREQSOCKET_H__F29136D4_F3FA_476C_B3A9_86EDC6D5A8CC__INCLUDED_)
#define AFX_HTTPREQSOCKET_H__F29136D4_F3FA_476C_B3A9_86EDC6D5A8CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HttpReqSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CHttpReqSocket command target

class CHttpReqSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CHttpReqSocket();
	virtual ~CHttpReqSocket();

	static CString GetHTTPContent (CString strHost, CString strRequest, bool* pbCancelDownload,
		int& nError, int nApproxContentLength = 0,
		CProgressCtrl* pProgress = NULL, CWnd* pStatus = NULL,
		CString strFilename = "");

	static bool IsInternetConnected();

	static CString URLEncode (CString str);

	// Overrides
public:

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpReqSocket)
	public:
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CHttpReqSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPREQSOCKET_H__F29136D4_F3FA_476C_B3A9_86EDC6D5A8CC__INCLUDED_)
