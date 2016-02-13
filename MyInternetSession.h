// Copyright (c) 1998, Microsoft Systems Journal
// Author: Aaron Skonnard
//

/////////////////////////////////////////////////////////////////////////////
// CMyInternetSession

class CMyInternetSession : public CInternetSession
{
public:
	CMyInternetSession(LPCTSTR pstrAgent = NULL, DWORD dwContext = 1, DWORD dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG, LPCTSTR pstrProxyName = NULL, LPCTSTR pstrProxyBypass = NULL, DWORD dwFlags = 0 ) 
		: CInternetSession(pstrAgent, dwContext, dwAccessType, pstrProxyName, pstrProxyBypass, dwFlags) { m_pRequest=NULL; };

	//override OnStatusCallback
	virtual void OnStatusCallback(DWORD dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength );

	//pointer to the CHttpRequest object controlling this Internet session
	CHttpRequest* m_pRequest;
};

