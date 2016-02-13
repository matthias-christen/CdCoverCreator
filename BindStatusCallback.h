//////////////////////////////////////////////////////////////////////
//
// Written by Michael Dunn (mdunn at inreach dot com).  Copyright and all
// that stuff.  Use however you like but give me credit where it's due.  I'll
// know if you don't. bwa ha ha ha ha!
// 
// Release history:
//   December 24, 1999: Version 1.0.  First release.
//
//////////////////////////////////////////////////////////////////////

// BindStatusCallback.h: interface for the CBindStatusCallback class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINDSTATUSCALLBACK_H__6B469ECE_B785_11D3_8D3B_D5CFB868D41C__INCLUDED_)
#define AFX_BINDSTATUSCALLBACK_H__6B469ECE_B785_11D3_8D3B_D5CFB868D41C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>

#pragma warning(disable:4100)   // disable warnings about unreferenced params

class CBindStatusCallback : public IBindStatusCallback, IHttpNegotiate
{
public:
	CBindStatusCallback (BINDVERB dwBindVerb, CString strHeaders, CString strPostData,
		DWORD dwDownloadId, std::list<DWORD>* pStkDownloads,
		DWORD dwStartTime, HWND hwndProgress, HWND hwndStatus);

	~CBindStatusCallback ();

    // IBindStatusCallback methods.  Note that the only method called by IE
    // is OnProgress(), so the others just return E_NOTIMPL.

    STDMETHOD(OnStartBinding)(
        /* [in] */ DWORD dwReserved,
        /* [in] */ IBinding __RPC_FAR *pib)
        { return E_NOTIMPL; }

    STDMETHOD(GetPriority)(
        /* [out] */ LONG __RPC_FAR *pnPriority)
        { return E_NOTIMPL; }

    STDMETHOD(OnLowResource)(
        /* [in] */ DWORD reserved)
        { return E_NOTIMPL; }

    STDMETHOD(OnProgress)(
        /* [in] */ ULONG ulProgress,
        /* [in] */ ULONG ulProgressMax,
        /* [in] */ ULONG ulStatusCode,
        /* [in] */ LPCWSTR wszStatusText);

    STDMETHOD(OnStopBinding)(
        /* [in] */ HRESULT hresult,
        /* [unique][in] */ LPCWSTR szError)
        { return E_NOTIMPL; }

    STDMETHOD(GetBindInfo)(
        /* [out] */ DWORD __RPC_FAR *grfBINDF,
        /* [unique][out][in] */ BINDINFO __RPC_FAR *pbindinfo)
        { return E_NOTIMPL; }

    STDMETHOD(OnDataAvailable)(
        /* [in] */ DWORD grfBSCF,
        /* [in] */ DWORD dwSize,
        /* [in] */ FORMATETC __RPC_FAR *pformatetc,
        /* [in] */ STGMEDIUM __RPC_FAR *pstgmed)
        { return E_NOTIMPL; }

    STDMETHOD(OnObjectAvailable)(
        /* [in] */ REFIID riid,
        /* [iid_is][in] */ IUnknown __RPC_FAR *punk)
        { return E_NOTIMPL; }

	// IHttpNegotiate methods
	STDMETHODIMP BeginningTransaction(/* [in] */ LPCWSTR szURL,
					/* [unique][in] */ LPCWSTR szHeaders,
					/* [in] */ DWORD dwReserved,
					/* [out] */ LPWSTR __RPC_FAR *pszAdditionalHeaders);
        
    STDMETHODIMP OnResponse(/* [in] */ DWORD dwResponseCode,
        /* [unique][in] */ LPCWSTR szResponseHeaders,
        /* [unique][in] */ LPCWSTR szRequestHeaders,
        /* [out] */ LPWSTR __RPC_FAR *pszAdditionalRequestHeaders);

    // IUnknown methods.  Note that IE never calls any of these methods, since
    // the caller owns the IBindStatusCallback interface, so the methods all
    // return zero/E_NOTIMPL.

    STDMETHODIMP_(ULONG)    AddRef()    { return m_cRef++; }
    STDMETHODIMP_(ULONG)    Release()   { if (--m_cRef == 0) { delete this; return 0; } return m_cRef; }

    STDMETHOD(QueryInterface)(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);

protected:
	void Init (LPCTSTR szData);

	DWORD m_dwDownloadId;
	std::list<DWORD>* m_pStkDownloads;

	DWORD m_dwStartTime;

	HWND m_hwndProgress;
	HWND m_hwndStatus;

	CString m_strKB;
	CString m_strKBs;
	CString m_strPercent;

    DWORD m_cRef;
	HGLOBAL m_hDataToPost;	// data that we're going to post
	DWORD m_cbDataToPost;
	BOOL m_fRedirect;		// need to be informed when we're being redirected by the server
	BINDVERB m_dwAction;

	CString m_strHeaders;
};

#pragma warning(default:4100)

#endif // !defined(AFX_BINDSTATUSCALLBACK_H__6B469ECE_B785_11D3_8D3B_D5CFB868D41C__INCLUDED_)
