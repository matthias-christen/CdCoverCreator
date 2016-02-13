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

// BindStatusCallback.cpp: implementation of the CBindStatusCallback class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "BindStatusCallback.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBindStatusCallback::CBindStatusCallback (BINDVERB dwBindVerb, CString strHeaders, CString strPostData,
										  DWORD dwDownloadId, std::list<DWORD>* pStkDownloads,
										  DWORD dwStartTime,
										  HWND hwndProgress, HWND hwndStatus)
{
    m_cRef = 1;

	m_dwAction = (BINDVERB_GET == dwBindVerb || BINDVERB_POST == dwBindVerb ? dwBindVerb : BINDVERB_POST);
	m_hDataToPost = NULL;
	m_cbDataToPost = 0;
	m_fRedirect = FALSE;

	m_strHeaders = strHeaders;
	Init (strPostData);

	m_dwDownloadId = dwDownloadId;
	m_pStkDownloads = pStkDownloads;

	m_dwStartTime = dwStartTime;

	m_hwndProgress = hwndProgress;
	m_hwndStatus = hwndStatus;
}

CBindStatusCallback::~CBindStatusCallback()
{
	if (m_hDataToPost) 
	{
		GlobalFree (m_hDataToPost);
	}
}

void CBindStatusCallback::Init (LPCTSTR szData)
{
	if (m_hDataToPost)
	{
		// We're already in use and don't support recycling the same instance
		// Some other client may have a reference on us.
		// If we were to free this data, the client might crash dereferencing the handle
		return; 
	}

	if (szData)
	{
		// MSINTERNAL: See CINetHttp::INetAsyncSendRequest (cnethttp.cxx) that URLMON calls CINetHttp::GetDataToSend() followed by a call to WININET's HttpSendRequest(). GetDataToSend essentially pulls the data out of the BINDINFO that URLMON has cached away when it calls the host's implementation of IBindStatusCallback::GetBindInfo(). 
		// MSINTERNAL: It doesn't attempt to lock down the HGLOBAL at all, so we need to allocated GMEM_FIXED
		m_cbDataToPost = lstrlen(szData);
		m_hDataToPost = GlobalAlloc(GPTR, m_cbDataToPost+1); // GMEM_MOVEABLE won't work because URLMON doesn't attempt GlobalLock before dereferencing
		if (!m_hDataToPost)
			return;	// out of memory

		// the memory was allocate fixed, so no need to lock it down
		lstrcpy((LPTSTR)m_hDataToPost, szData);
	}
}

STDMETHODIMP CBindStatusCallback::QueryInterface(REFIID riid, void** ppv)
{
    *ppv = NULL;

    if (riid == IID_IUnknown || riid == IID_IBindStatusCallback)	
    {
	    *ppv = (IBindStatusCallback*) this;
		AddRef ();
		return S_OK;
	}
	else if (riid == IID_IHttpNegotiate)
	{
	    *ppv = (IHttpNegotiate*) this;
		AddRef ();
		return S_OK;
	}

	return E_NOINTERFACE;
}

HRESULT CBindStatusCallback::OnProgress (ULONG ulProgress, ULONG ulProgressMax,
	ULONG ulStatusCode, LPCWSTR wszStatusText)
{
	int nProgress = ulProgressMax == 0 ? 0 : int (100.0 * ulProgress / ulProgressMax);

	// show the UI controls
	if (m_dwDownloadId == m_pStkDownloads->back ())
	{
		if (m_hwndProgress != NULL)
		{
			::PostMessage (m_hwndProgress, PBM_SETPOS, nProgress, 0);

			if (!IsWindowVisible (m_hwndProgress))
				::ShowWindow (m_hwndProgress, SW_SHOW);
		}

		if (m_hwndStatus != NULL)
		{
			m_strKB.Format (IDS_DOWNLOADSTATUS_KB,
				ulProgress / 1024,			// KBs read
				ulProgressMax / 1024);		// Total size (KBs)
			::PostMessage (m_hwndStatus, SB_SETTEXT, 2, (LPARAM) m_strKB.GetBuffer (0));

			// percentage
			if (ulProgressMax != 0)
			{
				m_strPercent.Format (IDS_DOWNLOADSTATUS_PERCENT, nProgress);
				::PostMessage (m_hwndStatus, SB_SETTEXT, 3, (LPARAM) m_strPercent.GetBuffer (0));
			}

			// KB/s
			m_strKBs.Format (IDS_DOWNLOADSTATUS_KBS, (double) ulProgress / 1024.0 / ((GetTickCount () - m_dwStartTime) / 1000.0 + 1));
			::PostMessage (m_hwndStatus, SB_SETTEXT, 4, (LPARAM) m_strKBs.GetBuffer (0));
		}
	}

    return S_OK;
}

STDMETHODIMP CBindStatusCallback::BeginningTransaction (LPCWSTR szURL, LPCWSTR szHeaders,
	DWORD dwReserved, LPWSTR __RPC_FAR *pszAdditionalHeaders)
{
	// Here's our opportunity to add headers
	if (!pszAdditionalHeaders)
		return E_POINTER;

	*pszAdditionalHeaders = NULL;

	// This header is required when performing a POST operation
	if (BINDVERB_POST == m_dwAction && m_hDataToPost)
		if (m_strHeaders.Find ("Content-Type: application/x-www-form-urlencoded") == -1)
			m_strHeaders += "Content-Type: application/x-www-form-urlencoded\r\n";

	if (!m_strHeaders.IsEmpty ())
	{
		*pszAdditionalHeaders = reinterpret_cast<LPWSTR> (CoTaskMemAlloc ((m_strHeaders.GetLength () + 1) * sizeof (WCHAR)));
		BSTR bstr = m_strHeaders.AllocSysString ();
		wcscpy (*pszAdditionalHeaders, bstr);
		::SysFreeString (bstr);
	}

	return S_OK;
}        

STDMETHODIMP CBindStatusCallback::OnResponse (DWORD dwResponseCode, LPCWSTR szResponseHeaders,
	LPCWSTR szRequestHeaders, LPWSTR __RPC_FAR *pszAdditionalRequestHeaders)
{
	if (!pszAdditionalRequestHeaders)
	{
		return E_POINTER;
	}

	*pszAdditionalRequestHeaders = NULL;

	return S_OK;
}
