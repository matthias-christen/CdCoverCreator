// M3UPlaylist.h: interface for the CM3UPlaylist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_M3UPLAYLIST_H__ABF205DA_6A83_40B5_BC95_51BD2F8D3200__INCLUDED_)
#define AFX_M3UPLAYLIST_H__ABF205DA_6A83_40B5_BC95_51BD2F8D3200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "Id3Tag.h"

CString decode_xml_entities (CString s);

class CPlaylistEntry
{
public:
	CPlaylistEntry (CString strTitle, CString strArtist, CString strAlbum, CString strYear, int nLengthSec)
	{
		m_strTitle = strTitle;
		m_strArtist = strArtist;
		m_strAlbum = strAlbum;
		m_strYear = strYear;
		m_nLengthSec = nLengthSec;
	}

	CPlaylistEntry (CString strTitle, int nLengthSec)
	{
		m_strTitle = strTitle;
		m_strArtist.Empty ();
		m_strAlbum.Empty ();
		m_strYear.Empty ();
		m_nLengthSec = nLengthSec;
	}

	CPlaylistEntry (CId3Tag tag)
	{
		m_strTitle = tag.m_strTitle;
		m_strArtist = tag.m_strArtist;
		m_strAlbum = tag.m_strAlbum;
		m_strYear = tag.m_strYear;

		m_nLengthSec = tag.m_nLengthSec;
	}

	CString m_strTitle, m_strArtist, m_strAlbum, m_strYear;
	int m_nLengthSec;
};

class CM3UPlaylist  
{
public:
	CM3UPlaylist(CString strFilename);
	virtual ~CM3UPlaylist();

	std::list<CPlaylistEntry*> m_List;
	CString m_strName;

protected:
	CString m_strFilename;

	void ReadM3U ();
	void ReadExtM3U (CStdioFile& file);
	void ReadPLS ();
	void ReadWPL ();
	void ReadB4S ();

	void SetFiletitleAsName ();
};

#endif // !defined(AFX_M3UPLAYLIST_H__ABF205DA_6A83_40B5_BC95_51BD2F8D3200__INCLUDED_)
