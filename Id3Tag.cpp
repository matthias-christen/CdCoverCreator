// Id3Tag.cpp: implementation of the CId3Tag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "Id3Tag.h"

#include "mp3-info\CMP3Info.h"
#include "id3v2lib\lib_id3v23.h"
#include "id3v2lib\genre.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CId3Tag::CId3Tag(CString strMP3Filename)
{
	// initialize
	m_strArtist.Empty ();
	m_strAlbum.Empty ();
	m_strTitle = strMP3Filename.Mid (strMP3Filename.ReverseFind ('\\') + 1);	// default title: filename
	m_strYear.Empty	();
	m_strComment.Empty ();
	m_nTrack = 0;
	m_strGenre.Empty ();
	m_nLengthSec = 0;

	// check whether this is an MP3
	if (strMP3Filename.Right (4).CompareNoCase (".mp3") != 0)
	{
		m_bNoIDTag = true;
		return;
	}

	// get the length of the mp3
	CMP3Info info;
	info.loadInfo (strMP3Filename.GetBuffer (0));
	m_nLengthSec = info.getLengthInSeconds ();

	// retrieve the v1 tags
	GetId3v1Tags (strMP3Filename);

	// retrieve the v2 tags
	id3Tag tag;
	get_id3v2_tag (&tag, strMP3Filename.GetBuffer (0));

	// if there is info, assign it; otherwise keep v1 info
	if (*tag.artist)
		m_strArtist = tag.artist;
	if (*tag.album)
		m_strAlbum = tag.album;
	if (*tag.title)
		m_strTitle = tag.title;
	if (*tag.year)
		m_strYear = tag.year;
	if (*tag.comment)
		m_strComment = tag.comment;
	if (*tag.track)
		m_nTrack = atoi (tag.track);
	if (*tag.genre)
		m_strGenre = tag.genre;

	// if there is still no title, use the file name as title
	if (m_strTitle.IsEmpty ())
		m_strTitle = strMP3Filename;
}

CId3Tag::~CId3Tag()
{
}

void CId3Tag::GetId3v1Tags(CString strMP3Filename)
{
	CFile file (strMP3Filename, CFile::modeRead | CFile::shareDenyWrite);
	file.Seek (-128, CFile::end);
		
	char szTagBuffer[4];
	file.Read (szTagBuffer, 3);
			
	if (strncmp ("TAG", szTagBuffer, 3) == 0)
	{
		CString strTagBuf;
		LPSTR szTagBuf = strTagBuf.GetBuffer (126);

		file.Seek (-125, CFile::end);
		file.Read (szTagBuf, 125);

		m_nTrack = szTagBuf[123];
		m_nGenre = szTagBuf[124];

		szTagBuf[123] = 0;

		// replace 0's with spaces
		for (int i = 0; i <= 122; i++)
			if (szTagBuf[i] == 0)
				szTagBuf[i] = ' ';

		strTagBuf.ReleaseBuffer ();

		m_strTitle = strTagBuf.Left (30);
		m_strArtist = strTagBuf.Mid (30, 30);
		m_strAlbum = strTagBuf.Mid (60, 30);
		m_strYear = strTagBuf.Mid (90, 4);
		m_strComment = strTagBuf.Mid (94, 28);

		if (0 <= m_nGenre && m_nGenre <= 145)
			m_strGenre = genre[m_nGenre];

		m_strTitle.TrimRight ();
		m_strArtist.TrimRight ();
		m_strAlbum.TrimRight ();
		m_strYear.TrimRight ();
		m_strComment.TrimRight ();

		m_bNoIDTag = false;

		file.Close ();
	}
	else
	{
		m_nTrack = 0;
		m_nGenre = 12;
		m_bNoIDTag = true;
	}
}
