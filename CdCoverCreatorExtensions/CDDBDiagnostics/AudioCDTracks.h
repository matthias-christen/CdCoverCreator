// AudioCDTracks.h: interface for the CAudioCDTracks class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIOCDTRACKS_H__C19DF98F_D6E7_41B4_9717_896D25D6D147__INCLUDED_)
#define AFX_AUDIOCDTRACKS_H__C19DF98F_D6E7_41B4_9717_896D25D6D147__INCLUDED_

//#include "Track.h"
//#include "RTFTitles.h"
#include <MMSystem.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum RetrieveMode
{
	DontRetrieve,
	RetrieveCDDB,
	RetrieveCDText
};

class CAudioCDTracks : public CObject//, public CRTFTitles
{
	DECLARE_SERIAL (CAudioCDTracks)

public:
	CAudioCDTracks ();
	//CAudioCDTracks (RetrieveMode mode);
	CString GetInfo ();
	LONG OpenCD(MCI_OPEN_PARMS& mciOpen);
	virtual ~CAudioCDTracks();

	CAudioCDTracks& operator= (CAudioCDTracks& t);

//	virtual void Serialize (CArchive& ar);

	inline bool Aborted () { return m_bAborted; }

	CString GetTrackLength (int nTrack);

//	bool GetCDText ();
	CString GetCDDBInfo ();

	inline int GetNumTracks () { return m_arrTrack.GetSize (); }
//	inline CTrack& GetTrack (int n) { return (CTrack&) *(m_arrTrack.GetAt (n)); }

	inline CString GetTitle () { return m_strTitle; }
	inline CString GetArtist () { return m_strArtist; }
	inline CString GetSubtitle () { return m_strSubtitle; }
	void SetTitle (CString strTitle) { m_strTitle = strTitle; }

	void SetSubtitle (CString strSubtitle) { m_strSubtitle = strSubtitle; }
//	void SetTrackTexts (int nTrackNum, CString strTitle, CString strRemarks);

protected:
	WORD GetTracksNum (MCI_OPEN_PARMS& mciOpen);
//	void ParseResult (CStdioFile* pFile);
	void ParseQuery (CString strQueryRes, CString& strCategory, CString& strDiscId);

	bool m_bAborted;

	CStringArray m_arrLength;
	CString m_strFramePos;
	CString m_strCDLength;
	CString m_strDiscId;

	CString m_strTitle, m_strSubtitle;
	CString m_strArtist;

	CObArray m_arrTrack;

	CString m_strDrive;
};

#endif // !defined(AFX_AUDIOCDTRACKS_H__C19DF98F_D6E7_41B4_9717_896D25D6D147__INCLUDED_)
