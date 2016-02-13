// WebLyrics.h: interface for the CWebLyrics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEBLYRICS_H__B188C309_9CC3_48D8_93D3_85992601A594__INCLUDED_)
#define AFX_WEBLYRICS_H__B188C309_9CC3_48D8_93D3_85992601A594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "wcc_data.h"

// thread functions
UINT wslGetLyrics (LPVOID);
UINT wslSearchLyrics (LPVOID);

// messages
#define WSLM_GETLYRICS WM_USER+1011

class CWslItem
{
public:
	CWslItem (CString strArtist, CString strAlbum, CString strSong)
	{
		m_strArtist = strArtist;
		m_strAlbum = strAlbum;
		m_strSong = strSong;
	}

	CString m_strArtist;
	CString m_strAlbum;
	CString m_strSong;
};

class CLyrics
{
public:
	CLyrics (CString strArtist, CString strAlbum, CString strSong, CString strText)
	{
		m_strArtist = strArtist;
		m_strAlbum = strAlbum;
		m_strSong = strSong;
		m_strText = strText;

		m_strText.Replace ("&nbsp;", " ");
		m_strText.Replace ("&amp;", "&");
		m_strText.Replace ("&lt;", "<");
		m_strText.Replace ("&gt;", ">");
		m_strText.Replace ("&auml;", "ä");
		m_strText.Replace ("&Auml;", "Ä");
		m_strText.Replace ("&ouml;", "ö");
		m_strText.Replace ("&Ouml;", "Ö");
		m_strText.Replace ("&uuml;", "ü");
		m_strText.Replace ("&Uuml;", "Ü");
		m_strText.Replace ("&szlig;", "ß");
		/*
		m_strText.Replace ();
		m_strText.Replace ();
		m_strText.Replace ();
		m_strText.Replace ();
		m_strText.Replace ();
		m_strText.Replace ();
		m_strText.Replace ();
		m_strText.Replace ();
		m_strText.Replace ();
		*/
	}

	CString m_strArtist;
	CString m_strAlbum;
	CString m_strSong;
	CString m_strText;
};

typedef struct tagWslGetLyricsParams
{
	std::list<CWslItem*> list;
	HWND hwndList;
	HWND hwndProgress;
	HWND hwndStatus;
	HWND hwndNotify;
	bool* pCancelDownload;
	DWORD dwDownloadId;
} WSL_GETLYRICSPARAMS;

typedef struct tagWslSearchLyricsParams
{
	CString strArtist;
	CString strAlbum;
	CString strTitle;
	HWND hwndList;
	HWND hwndProgress;
	HWND hwndStatus;
	HWND hwndNotify;
	bool* pCancelDownload;
	DWORD dwDownloadId;
} WSL_SEARCHLYRICSPARAMS;

class CWebLyrics  
{
public:
	CWebLyrics();
	virtual ~CWebLyrics();

	bool GetLyrics (CString strArtist, CString strAlbum, CString strSong, HWND hwndList, int nListItem, HWND hwndProgress, HWND hwndStatus);
	bool SearchLyrics (CString strArtist, CString strAlbum, CString strSong, HWND hwndList, HWND hwndProgress, HWND hwndStatus);

	void SetDownloadCancelFlag (bool* pbCancelDownload);

	int m_nError;
	DWORD m_dwDownloadId;

private:
	bool* m_pbCancelDownload;
protected:
	void ReadLyrics (CString strUrl, int& nMethod, LPSTR szHeaders, LPSTR szData, WSL_GETLYRICS* pfn_wslSearchLyrics, HWND hwndList, HWND hwndProgress, HWND hwndStatus);
	CString ExtractUrl (LPSTR lpszUrl, int& nStartPos);
};

#endif // !defined(AFX_WEBLYRICS_H__B188C309_9CC3_48D8_93D3_85992601A594__INCLUDED_)
