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
#include "BindStatusCallback.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
CBindStatusCallback::CBindStatusCallback (DWORD dwDownloadId, std::list<DWORD>* pStkDownloads,
										  DWORD dwStartTime,
										  HWND hwndProgress, HWND hwndStatus)
{
	m_nRef = 0;

	m_dwDownloadId = dwDownloadId;
	m_pStkDownloads = pStkDownloads;

	m_dwStartTime = dwStartTime;

	m_hwndProgress = hwndProgress;
	m_hwndStatus = hwndStatus;
}*/

CBindStatusCallback::CBindStatusCallback ()
{
	m_nRef = 0;
}

CBindStatusCallback::~CBindStatusCallback()
{
}

STDMETHODIMP CBindStatusCallback::QueryInterface (REFIID riid, void** ppv)
{
    *ppv = NULL;

    if (riid == IID_IUnknown)
        *ppv = (LPUNKNOWN)(IBindStatusCallback*) this;
/*	else if (riid == IID_IAuthenticate)
		*ppv = (IAuthenticate*) this;
	else if (riid == IID_IHttpNegotiate)
		*ppv = (IHttpNegotiate*) this;*/
	else if (riid == IID_IBindStatusCallback)
        *ppv = (IBindStatusCallback*) this;

	if (NULL != *ppv)
	{
        AddRef ();
        return S_OK;
    }

	return E_NOINTERFACE;
}

HRESULT CBindStatusCallback::OnProgress (ULONG ulProgress, ULONG ulProgressMax,
	ULONG ulStatusCode, LPCWSTR wszStatusText)
{
/*	int nProgress = ulProgressMax == 0 ? 0 : int (100.0 * ulProgress / ulProgressMax);

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
	}*/

    return S_OK;
}

HRESULT CBindStatusCallback::BeginningTransaction (LPCWSTR szURL, DWORD dwReserved, LPCWSTR szHeaders, LPWSTR* pszAdditionalHeaders)
{
	return S_OK;
}

HRESULT CBindStatusCallback::OnResponse (DWORD dwResponseCode, LPCWSTR szResponseHeaders, LPCWSTR szRequestHeaders, LPWSTR* pszAdditionalRequestHeaders)
{
	return S_OK;
}
