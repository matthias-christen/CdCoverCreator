// Id3Tag.h: interface for the CId3Tag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ID3TAG_H__7C01EB2F_6072_452E_BC4D_BD0B1DE49A8B__INCLUDED_)
#define AFX_ID3TAG_H__7C01EB2F_6072_452E_BC4D_BD0B1DE49A8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CId3Tag  
{
public:
	CId3Tag(CString strMP3Filename);
	virtual ~CId3Tag();

	// ID3v11, ID3v2*
	CString	m_strTitle;
	CString	m_strArtist;
	CString	m_strAlbum;
	CString	m_strYear;
	CString	m_strComment;
	CString m_strGenre;
	int	m_nTrack;
	int	m_nGenre;
	bool m_bNoIDTag;
	int m_nLengthSec;

protected:
	void GetId3v1Tags(CString strMP3Filename);
};

#endif // !defined(AFX_ID3TAG_H__7C01EB2F_6072_452E_BC4D_BD0B1DE49A8B__INCLUDED_)
