#ifndef __BSC_H__
#define __BSC_H__

class CBindStatusCallback2 : public IBindStatusCallback, IHttpNegotiate {
  public:
    // IUnknown methods
    STDMETHODIMP    QueryInterface(REFIID riid,void ** ppv);
    STDMETHODIMP_(ULONG)    AddRef()    { return m_cRef++; }
    STDMETHODIMP_(ULONG)    Release()   { if (--m_cRef == 0) { delete this; return 0; } return m_cRef; }

    // IBindStatusCallback methods
    STDMETHODIMP    OnStartBinding(DWORD dwReserved, IBinding* pbinding);
    STDMETHODIMP    GetPriority(LONG* pnPriority);
    STDMETHODIMP    OnLowResource(DWORD dwReserved);
    STDMETHODIMP    OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode,
                        LPCWSTR pwzStatusText);
    STDMETHODIMP    OnStopBinding(HRESULT hrResult, LPCWSTR szError);
    STDMETHODIMP    GetBindInfo(DWORD* pgrfBINDF, BINDINFO* pbindinfo);
    STDMETHODIMP    OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pfmtetc,
                        STGMEDIUM* pstgmed);
    STDMETHODIMP    OnObjectAvailable(REFIID riid, IUnknown* punk);

	// IHttpNegotiate methods
	STDMETHODIMP BeginningTransaction(/* [in] */ LPCWSTR szURL,
					/* [unique][in] */ LPCWSTR szHeaders,
					/* [in] */ DWORD dwReserved,
					/* [out] */ LPWSTR __RPC_FAR *pszAdditionalHeaders);
        
    STDMETHODIMP OnResponse(/* [in] */ DWORD dwResponseCode,
        /* [unique][in] */ LPCWSTR szResponseHeaders,
        /* [unique][in] */ LPCWSTR szRequestHeaders,
        /* [out] */ LPWSTR __RPC_FAR *pszAdditionalRequestHeaders);

    // constructors/destructors
	CBindStatusCallback2(/*CStatus* pStatus,*/ BINDVERB dwBindVerb, CString strHeaders, CString strPostData);

    ~CBindStatusCallback2() 
	{ 
		if (m_hDataToPost) 
		{
			GlobalFree(m_hDataToPost);
		}
	}

	HRESULT Init(LPCTSTR szData);

    inline void SetStatusText(LPCTSTR szStatus, int iPart = 0)     
	{ 
//		if (m_pStatus) m_pStatus->SetStatusText(szStatus, iPart); 
	}

	inline void UpdateProgress(ULONG cProgress, ULONG maxProgress) 
	{
//		if (m_pStatus) m_pStatus->UpdateProgress(cProgress, maxProgress);
	}

    // data members
    DWORD           m_cRef;
//	LPBINDING       m_pbinding;
//	CStatus*		m_pStatus;
	HGLOBAL			m_hDataToPost; // data that we're going to post
	DWORD			m_cbDataToPost;
	BOOL			m_fRedirect; // need to be informed when we're being redirected by the server
	BINDVERB		m_dwAction;

	CString m_strHeaders;
};
#endif // __BSC_H__