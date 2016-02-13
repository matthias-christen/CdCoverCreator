// Track.cpp: implementation of the CTrack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "Track.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL (CTrack, CObject, VERSIONABLE_SCHEMA | 1)

CTrack::CTrack ()
{
	m_nTrackNum = -1;
	m_strLength = "";
	m_strTitle = "";
	m_strRemarks = "";
}

CTrack::CTrack (int n, CString strLen, CString strTitle, CString strRem)
{
	m_nTrackNum = n;
	m_strLength = strLen;
	m_strTitle = strTitle;
	m_strRemarks = strRem;
}

CTrack::CTrack (CTrack& t)
{
	*this = t;
}

CTrack& CTrack::operator= (CTrack& t)
{
	m_nTrackNum = t.GetTrackNum ();
	m_strLength = t.GetLength ();
	m_strTitle = t.GetTitle ();
	m_strRemarks = t.GetRemarks ();

	return *this;
}

bool CTrack::operator== (CTrack& t)
{
	return (m_nTrackNum == t.GetTrackNum ()) && (m_strLength == t.GetLength ()) &&
		(m_strTitle == t.GetTitle ()) && (m_strRemarks == t.GetRemarks ());
}

bool CTrack::operator!= (CTrack& t)
{
	return !(*this == t);
}

void CTrack::Serialize (CArchive& ar)
{
	if (ar.IsStoring ())
		ar << m_nTrackNum << m_strLength << m_strTitle << m_strRemarks;
	else
		ar >> m_nTrackNum >> m_strLength >> m_strTitle >> m_strRemarks;
}

void CTrack::SetText (CString strTitle, CString strRemark)
{
	m_strTitle = strTitle;
	m_strRemarks = strRemark;
}

// returns delta tracklenght in seconds of this track and t
int CTrack::operator -(CTrack &t)
{
	return abs (GetLengthMin () * 60 + GetLengthSec () - t.GetLengthMin () * 60 - t.GetLengthSec ());
}
