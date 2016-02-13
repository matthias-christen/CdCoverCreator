#include "stdafx.h"
#include "BindStatusCallback2.h"

//////////////////////////////////////////////////////////////////////////////////
// CBindStatusCallback2 Implementation

CBindStatusCallback2::CBindStatusCallback2(/*CStatus* pStatus,*/ BINDVERB dwBindVerb, CString strHeaders, CString strPostData)
{
//	m_pStatus = pStatus;
//	m_pstm = NULL;
    m_cRef = 1;
//	m_cbOld = 0;

	m_dwAction = (BINDVERB_GET == dwBindVerb || BINDVERB_POST == dwBindVerb ? dwBindVerb : BINDVERB_POST);
	m_hDataToPost = NULL;
	m_cbDataToPost = 0;
	m_fRedirect = FALSE;

	m_strHeaders = strHeaders;
	Init (strPostData);
}  // CBindStatusCallback2

HRESULT CBindStatusCallback2::Init (LPCTSTR szData)
{
	if (m_hDataToPost)
	{
		// We're already in use and don't support recycling the same instance
		// Some other client may have a reference on us.
		// If we were to free this data, the client might crash dereferencing the handle
		return E_FAIL; 
	}

	if (szData)
	{
		// MSINTERNAL: See CINetHttp::INetAsyncSendRequest (cnethttp.cxx) that URLMON calls CINetHttp::GetDataToSend() followed by a call to WININET's HttpSendRequest(). GetDataToSend essentially pulls the data out of the BINDINFO that URLMON has cached away when it calls the host's implementation of IBindStatusCallback::GetBindInfo(). 
		// MSINTERNAL: It doesn't attempt to lock down the HGLOBAL at all, so we need to allocated GMEM_FIXED
		m_cbDataToPost = lstrlen(szData);
		m_hDataToPost = GlobalAlloc(GPTR, m_cbDataToPost+1); // GMEM_MOVEABLE won't work because URLMON doesn't attempt GlobalLock before dereferencing
		if (!m_hDataToPost)
		{
			return E_OUTOFMEMORY;
		}

		// the memory was allocate fixed, so no need to lock it down
		lstrcpy((LPTSTR)m_hDataToPost, szData);
	}

	return S_OK;
}

STDMETHODIMP CBindStatusCallback2::QueryInterface(REFIID riid, void** ppv)
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

STDMETHODIMP CBindStatusCallback2::OnStartBinding(DWORD dwReserved, IBinding* pbinding)
{
	return E_NOTIMPL;
}

STDMETHODIMP CBindStatusCallback2::GetPriority(LONG* pnPriority)
{
	return E_NOTIMPL;
}

STDMETHODIMP CBindStatusCallback2::OnLowResource(DWORD dwReserved)
{
    return E_NOTIMPL;
}

#define ODS TRACE

STDMETHODIMP CBindStatusCallback2::OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
{
	TCHAR szStatusMsg[255] = _T("");
    TCHAR szProgress[255];

	switch (ulStatusCode)
	{
	case BINDSTATUS_REDIRECTING: 
		lstrcpy(szStatusMsg, _TEXT("Server redirecting client..."));
		ODS("Server redirecting client\n");
		m_fRedirect = TRUE;
		break;
	case BINDSTATUS_FINDINGRESOURCE:
		lstrcpy(szStatusMsg, _TEXT("Finding resource..."));
		ODS("Finding resource\n");
		break;
	case BINDSTATUS_CONNECTING:
		lstrcpy(szStatusMsg, _TEXT("Connecting..."));
		ODS("Connecting...\n");
		break;
	case BINDSTATUS_BEGINDOWNLOADDATA:
		lstrcpy(szStatusMsg, _TEXT("Beginning to download data..."));
		ODS("Beginning to download data...\n");
		break;
	case BINDSTATUS_DOWNLOADINGDATA:
		lstrcpy(szStatusMsg, _TEXT("Downloading data..."));
		ODS("Downloading data...\n");
		break;
	case BINDSTATUS_ENDDOWNLOADDATA:
		lstrcpy(szStatusMsg, _TEXT("Ending data download..."));
		ODS("Ending data download...\n");
		break;
	case BINDSTATUS_BEGINDOWNLOADCOMPONENTS:
		lstrcpy(szStatusMsg, _TEXT("Beginning to download components..."));
		ODS("Beginning to download components...\n");
		break;
	case BINDSTATUS_INSTALLINGCOMPONENTS:
		lstrcpy(szStatusMsg, _TEXT("Installing components..."));
		ODS("Installing components...\n");
		break;
	case BINDSTATUS_ENDDOWNLOADCOMPONENTS:
		lstrcpy(szStatusMsg, _TEXT("Ending component download..."));
		ODS("Ending component download...\n");
		break;
	case BINDSTATUS_USINGCACHEDCOPY:
		lstrcpy(szStatusMsg, _TEXT("Using cached copy..."));
		ODS("Using cached copy...\n");
		break;
	case BINDSTATUS_SENDINGREQUEST:
		lstrcpy(szStatusMsg, _TEXT("Sending request..."));
		ODS("Sending request...\n");
		break;
	case BINDSTATUS_CLASSIDAVAILABLE:
		lstrcpy(szStatusMsg, _TEXT("CLSID available..."));
		ODS("CLSID available...\n");
		break;
	case BINDSTATUS_MIMETYPEAVAILABLE:
		lstrcpy(szStatusMsg, _TEXT("MIME type available..."));
		ODS("MIME type available...\n");
		break;
	case BINDSTATUS_CACHEFILENAMEAVAILABLE:
		lstrcpy(szStatusMsg, _TEXT("Cache file name available..."));
		ODS("Cache file name available...\n");
		break;
	default:
		ODS("Unknown binding status code!\n");
		break;
	}

	SetStatusText(szStatusMsg);

    wsprintf(szProgress, "%d of %d", ulProgress, (ulProgress>ulProgressMax)?ulProgress:ulProgressMax);

//	SetStatusText(szProgress, SBAR_PART_PROGRESS);
	UpdateProgress(ulProgress, ulProgressMax);

    return S_OK;
}

STDMETHODIMP CBindStatusCallback2::OnStopBinding(HRESULT hrStatus, LPCWSTR pszError)
{
	return E_NOTIMPL;
}

STDMETHODIMP CBindStatusCallback2::GetBindInfo (DWORD* pgrfBINDF, BINDINFO* pbindInfo)
{
	return E_NOTIMPL;

	if (m_fRedirect && BINDVERB_POST == m_dwAction)
	{
		// We're being redirected by the server. Stop posting
		SetStatusText(_T("Switching method to GET"));
		m_dwAction = BINDVERB_GET;
	}

	*pgrfBINDF = BINDF_ASYNCHRONOUS | BINDF_ASYNCSTORAGE | BINDF_PULLDATA;
	*pgrfBINDF |= BINDF_GETNEWESTVERSION | BINDF_NOWRITECACHE;

	// Set up the BINDINFO data structure
	pbindInfo->cbSize = sizeof(BINDINFO);
	pbindInfo->dwBindVerb = m_dwAction; // here's where the action is defined
	pbindInfo->szExtraInfo = NULL;

	// Initialize the STGMEDIUM.
	memset(&pbindInfo->stgmedData, 0, sizeof(STGMEDIUM));
	pbindInfo->grfBindInfoF = 0;
	pbindInfo->szCustomVerb = NULL;

	// set up action-specific members
	if (m_dwAction == BINDVERB_POST)
	{
		if (m_hDataToPost)
		{			
			// Fill the STGMEDIUM with the data to post
			pbindInfo->stgmedData.tymed = TYMED_HGLOBAL; // this is the only medium urlmon supports right now
			pbindInfo->stgmedData.hGlobal = m_hDataToPost;
			pbindInfo->stgmedData.pUnkForRelease = (LPUNKNOWN)(LPBINDSTATUSCALLBACK)this; //  maintain control over the data. 
			AddRef(); // It will be freed on final release
			pbindInfo->cbstgmedData = // this must be exact! 
				m_cbDataToPost;		  // Do not rely on GlobalSize() 
									  // which rounds up to the nearest power of two.
		}
	}

	return S_OK;
}

STDMETHODIMP CBindStatusCallback2::OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC* pfmtetc, STGMEDIUM* pstgmed)
{
	return E_NOTIMPL;
}

STDMETHODIMP CBindStatusCallback2::OnObjectAvailable(REFIID riid, IUnknown* punk)
{
    return E_NOTIMPL;
}

STDMETHODIMP CBindStatusCallback2::BeginningTransaction (LPCWSTR szURL, LPCWSTR szHeaders,
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

STDMETHODIMP CBindStatusCallback2::OnResponse (DWORD dwResponseCode, LPCWSTR szResponseHeaders,
	LPCWSTR szRequestHeaders, LPWSTR __RPC_FAR *pszAdditionalRequestHeaders)
{
	if (!pszAdditionalRequestHeaders)
	{
		return E_POINTER;
	}

	*pszAdditionalRequestHeaders = NULL;

	return S_OK;
}
