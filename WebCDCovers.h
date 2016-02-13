// WebCDCovers.h: interface for the CWebCDCovers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBCDCOVERS_H__2E2E1BA8_D112_4BA7_9D0E_C2E41497B5A8__INCLUDED_)
#define AFX_WEBCDCOVERS_H__2E2E1BA8_D112_4BA7_9D0E_C2E41497B5A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
//#include "Picture.h"
#include "CdCoverCreator2.h"

// thread functions
UINT wccGetList (LPVOID);
UINT wccSearchCovers (LPVOID);
UINT wccGetImages (LPVOID);

// messages
#define WCCM_LISTCOVERS WM_USER+1001
#define WCCM_SEARCHCOVERS WM_USER+1002
#define WCCM_GETIMAGES WM_USER+1003

// structures

typedef struct tagCoverImages
{
	// host
	RESOURCEHOST* pHost;

	// filename of the images
	CString strFile[NUM_RENDER_TYPES-1];	// without booklet

	// local file names
	CString strLocalFile[NUM_RENDER_TYPES-1];
} COVERIMAGES;

typedef struct tagWccGetListParams
{
	CString strValue;
	HWND hwndList;
	HTREEITEM htiParent;
	HWND hwndProgress;
	HWND hwndStatus;
	HWND hwndNotify;
	bool* pCancelDownload;
	DWORD dwDownloadId;
} WCC_GETLISTPARAMS;

typedef struct tagWccGetImagesParams
{
	CString strLetter, strName;
	COVERIMAGES* pCoverImages;
	RenderType rt;
	HWND hwndProgress;
	HWND hwndStatus;
	HWND hwndNotify;
	bool* pCancelDownload;
	DWORD dwDownloadId;
} WCC_GETIMAGESPARAMS;

#define WCC_LIST 0
#define WCC_SEARCH 1


class CWebCDCovers
{
public:
	CWebCDCovers();
	virtual ~CWebCDCovers();

	bool GetList (CString strLetter, HWND hwndList, HTREEITEM htiParent, HWND hwndProgress, HWND hwndStatus);
	bool SearchCovers (CString strKeywords, HWND hwndList, HWND hwndProgress, HWND hwndStatus);
	bool DownloadImage (CString strLetter, CString strName, CString strFilename, RESOURCEHOST* pHost, HWND hwndProgress, HWND hwndStatus, CString& strLocalFilename);

	void SetDownloadCancelFlag (bool* pbCancelDownload);

	int m_nError;
	DWORD m_dwDownloadId;

private:
	bool RetrieveList (int nIndex, CString strCriteria, HWND hwndList, HTREEITEM htiParent, HWND hwndProgress, HWND hwndStatus, CString strMsg);
	CString GetCookies (CString strResponse);
	bool GetData (LPSTR lpszData, int& nPos, CString& strName, COVERIMAGES* pCoverImages);
	void CopyData (LPSTR lpszData, int& nPos, CString& strRet);

	bool* m_pbCancelDownload;
protected:
	bool CoverImagesAreEqual (COVERIMAGES* pCi1, COVERIMAGES* pCi2);
};

#endif // !defined(AFX_WEBCDCOVERS_H__2E2E1BA8_D112_4BA7_9D0E_C2E41497B5A8__INCLUDED_)
