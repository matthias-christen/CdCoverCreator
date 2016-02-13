// M3UPlaylist.cpp: implementation of the CM3UPlaylist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "M3UPlaylist.h"

#define PUGAPI_VARIANT 0x58475550
#define PUGAPI_VERSION_MAJOR 1
#define PUGAPI_VERSION_MINOR 2

#include "pugxml.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString decode_xml_entities (CString s)
{
	/*
	s.Replace ("&amp;", "&");
	s.Replace ("&apos;", "'");
	s.Replace ("&gt;", ">");
	s.Replace ("&lt;", "<");
	s.Replace ("&quot;", "\"");*/

	CString strDecoded;
	LPSTR lpszDec = strDecoded.GetBuffer (s.GetLength () + 1);

	int j = 0;
	char chNum;

	for (int i = 0; i < s.GetLength (); i++)
	{
		char c = s.GetAt (i);

		if (c == '&')
		{
			c = s.GetAt (++i);
			switch (c)
			{
			case 'a':
				if (s.GetAt (++i) == 'm')	// &amp;
					lpszDec[j++] = '&';
				else						// &apos;
					lpszDec[j++] = '\'';
				break;
			case 'g':	// &gt;
				lpszDec[j++] = '>';
				break;
			case 'l':	// &lt;
				lpszDec[j++] = '<';
				break;
			case 'q':	// &quot;
				lpszDec[j++] = '"';
				break;
			case '#':
				chNum = 0;
				if (tolower (s.GetAt (++i)) == 'x')
				{
					i++;
					while (s.GetAt (i) != ';')
					{
						c = tolower (s.GetAt (i));
						chNum = 16 * chNum + (isdigit (c) ? (c - '0') : (c - 'a' + 10));
						i++;
					}
				}
				else
				{
					while (s.GetAt (i) != ';')
					{
						chNum = 10 * chNum + (s.GetAt (i) - '0');
						i++;
					}
				}
				lpszDec[j++] = chNum;
				break;
			}
			while (s.GetAt (i) != ';')
				i++;
		}
		else
			lpszDec[j++] = c;
	}

	lpszDec[j] = 0;

	strDecoded.ReleaseBuffer ();
	return strDecoded;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CM3UPlaylist::CM3UPlaylist (CString strFilename)
{
	m_strFilename = strFilename;
	CString strExtension = strFilename.Mid (strFilename.ReverseFind ('.') + 1).MakeLower ();

	m_strName.Empty ();

	if ((strExtension == "m3u") || (strExtension == "m3u8"))
		ReadM3U ();
	else if (strExtension == "pls")
		ReadPLS ();
	else if (strExtension == "wpl")
		ReadWPL ();
	else if (strExtension == "b4s")
		ReadB4S ();
}

CM3UPlaylist::~CM3UPlaylist ()
{
	for (std::list<CPlaylistEntry*>::iterator it = m_List.begin (); it != m_List.end (); it++)
		delete *it;
	m_List.clear ();
}

// M3U
void CM3UPlaylist::ReadM3U ()
{
	CStdioFile file (m_strFilename, CFile::modeRead);

	CString str;
	file.ReadString (str);

	if (str.Find ("#EXTM3U") >= 0)
		ReadExtM3U (file);
	else
		for ( ; ; )
		{
			m_List.push_back (new CPlaylistEntry (CId3Tag (str)));
			if (!file.ReadString (str))
				break;
		}

	file.Close ();

	SetFiletitleAsName ();
}

// #EXTM3U
void CM3UPlaylist::ReadExtM3U (CStdioFile& file)
{
	CString str;
	while (file.ReadString (str))
	{
		if (str.Left (8) == "#EXTINF:")
		{
			int nPosLen = str.Find (',');
			int nPosArtist = str.Find (" - ");
			if (nPosArtist == -1)
				nPosArtist = str.Find (" / ");

			if (nPosArtist >= 0)
				m_List.push_back (new CPlaylistEntry (
					str.Mid (nPosArtist + 3),
					str.Mid (nPosLen + 1, nPosArtist - nPosLen - 1),
					"", "",
					atoi (str.Mid (8, nPosLen - 8))));
			else
				m_List.push_back (new CPlaylistEntry (
					str.Mid (nPosLen + 1), "", "", "", atoi (str.Mid (8, nPosLen - 8))));
		}
	}
}

// Playlist in INI format
void CM3UPlaylist::ReadPLS ()
{
	// get the name
	::GetPrivateProfileString ("playlist", "PlaylistName", "", m_strName.GetBuffer (1000), 1000, m_strFilename);
	m_strName.ReleaseBuffer ();
	if (m_strName.IsEmpty ())
		SetFiletitleAsName ();

	// get the entries
	int nNumEntries = ::GetPrivateProfileInt ("playlist", "NumberOfEntries", 0, m_strFilename);
	CString s, strTitle, strArtist;
	for (int i = 0; i < nNumEntries; i++)
	{
		s.Format ("%d", i + 1);
		::GetPrivateProfileString ("playlist", "Title" + s, "", strTitle.GetBuffer (1000), 1000, m_strFilename);
		strTitle.ReleaseBuffer ();
		strArtist.Empty ();

		int nPosArtist = strTitle.Find (" - ");
		if (nPosArtist == -1)
			nPosArtist = strTitle.Find (" / ");

		if (nPosArtist >= 0)
		{
			strArtist = strTitle.Left (nPosArtist);
			strTitle = strTitle.Mid (nPosArtist + 3);
		}

		m_List.push_back (new CPlaylistEntry (strTitle, strArtist, "", "",
			::GetPrivateProfileInt ("playlist", "Length" + s, 0, m_strFilename)));
	}
}

// Windows Media Player
void CM3UPlaylist::ReadWPL ()
{
	// create an XML parser
	pug::xml_parser parser;

	parser.parse_file (m_strFilename.GetBuffer (-1),
		//parse_dtd_only |			//Parse only '<!DOCTYPE [*]>'
		pug::parse_doctype |		//Parse '<!DOCTYPE ...>' section, with '[...]' as data member.
		pug::parse_dtd |			//Parse whatever is in DOCTYPE data member ('[...]').
		pug::parse_pi |				//Parse '<?...?>'
		pug::parse_cdata |			//Parse '<![CDATA[...]]>', and '<![INCLUDE[...]]>'
		pug::parse_comments |		//Parse <!--...-->'
		pug::parse_wnorm |			//Normalize all entities that are flagged to be trimmed.
		pug::parse_trim_entity |	//Trim DTD entities.
		pug::parse_trim_attribute |	//Trim 'foo="..."'.
		pug::parse_trim_pcdata |	//Trim '>...<'
		pug::parse_trim_cdata |		//Trim '<![CDATA[...]]>'
		pug::parse_trim_comment		//Trim <!--...-->'
	);

	pug::xml_node document = parser.document ();
	pug::xml_node nodeRoot = document.first_node (pug::node_element);
	if (nodeRoot.empty ())
		return;

	// find the head node
	pug::xml_node nodeHead = nodeRoot.first_element_by_name ("head");
	if (!nodeHead.empty ())
	{
		pug::xml_node nodeTitle = nodeHead.first_element_by_name ("title");
		if (!nodeTitle.empty ())
		{
			pug::xml_node nodeData = nodeTitle.first_node (pug::node_pcdata);
			if (!nodeTitle.empty ())
				m_strName = nodeData.value ();
		}
	}

	// find the body node
	pug::xml_node nodeBody = nodeRoot.first_element_by_name ("body");
	if (nodeBody.empty ())
		return;

	// find the seq node
	pug::xml_node nodeSeq = nodeBody.first_element_by_name ("seq");
	if (nodeSeq.empty ())
		return;

	// list media nodes
	for (pug::xml_node::child_iterator it = nodeSeq.children_begin (); it != nodeSeq.children_end (); ++it)
		if (strcmp (it->name (), "media") == 0)
		{
			pug::xml_attribute attrSrc = it->attribute ("src");
			if (!attrSrc.empty ())
				m_List.push_back (new CPlaylistEntry (CId3Tag (decode_xml_entities (attrSrc.value ()))));
		}
}

// WinAmp XML playlist
void CM3UPlaylist::ReadB4S ()
{
	// create an XML parser
	pug::xml_parser parser;

	parser.parse_file (m_strFilename.GetBuffer (-1),
		//parse_dtd_only |			//Parse only '<!DOCTYPE [*]>'
		pug::parse_doctype |		//Parse '<!DOCTYPE ...>' section, with '[...]' as data member.
		pug::parse_dtd |			//Parse whatever is in DOCTYPE data member ('[...]').
		pug::parse_pi |				//Parse '<?...?>'
		pug::parse_cdata |			//Parse '<![CDATA[...]]>', and '<![INCLUDE[...]]>'
		pug::parse_comments |		//Parse <!--...-->'
		pug::parse_wnorm |			//Normalize all entities that are flagged to be trimmed.
		pug::parse_trim_entity |	//Trim DTD entities.
		pug::parse_trim_attribute |	//Trim 'foo="..."'.
		pug::parse_trim_pcdata |	//Trim '>...<'
		pug::parse_trim_cdata |		//Trim '<![CDATA[...]]>'
		pug::parse_trim_comment		//Trim <!--...-->'
	);

	pug::xml_node document = parser.document ();
	pug::xml_node nodeRoot = document.first_node (pug::node_element);
	if (nodeRoot.empty ())
		return;

	// find the playlist node
	pug::xml_node nodePlaylist = nodeRoot.first_element_by_name ("playlist");
	if (!nodePlaylist.empty ())
	{
		pug::xml_attribute attrLabel = nodePlaylist.attribute ("label");
		if (!attrLabel.empty ())
			m_strName = attrLabel.value ();
	}

	// list media nodes
	for (pug::xml_node::child_iterator it = nodePlaylist.children_begin (); it != nodePlaylist.children_end (); ++it)
		if (strcmp (it->name (), "entry") == 0)
		{
			pug::xml_node nodeName = it->first_element_by_name ("Name");
			if (nodeName.empty ())
				continue;
			pug::xml_node nodeData = nodeName.first_node (pug::node_pcdata);
			if (nodeData.empty ())
				continue;

			CString strTitle = nodeData.value ();
			CString strArtist;

			int nPosArtist = strTitle.Find (" - ");
			if (nPosArtist == -1)
				nPosArtist = strTitle.Find (" / ");

			if (nPosArtist >= 0)
			{
				strArtist = strTitle.Left (nPosArtist);
				strTitle = strTitle.Mid (nPosArtist + 3);
			}

			int nLength = 0;
			pug::xml_node nodeLength = it->first_element_by_name ("Length");
			if (!nodeLength.empty ())
			{
				nodeData = nodeLength.first_node (pug::node_pcdata);
				if (!nodeData.empty ())
					nLength = atoi (nodeData.value ());
			}

			m_List.push_back (new CPlaylistEntry (
				decode_xml_entities (strTitle), decode_xml_entities (strArtist), "", "", nLength / 1000));
		}
}

void CM3UPlaylist::SetFiletitleAsName ()
{
	m_strName = m_strFilename.Mid (m_strFilename.ReverseFind ('\\') + 1);

	int nPosDot = m_strName.ReverseFind ('.');
	if (nPosDot >= 0)
		m_strName = m_strName.Left (nPosDot);
}