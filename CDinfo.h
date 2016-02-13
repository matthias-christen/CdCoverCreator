#if !defined(CD_INFO_H)
#define CD_INFO_H

#include <afxtempl.h>
#include <mmsystem.h>


//*************************************************************************
// CMsf
//*************************************************************************
class CMsf 
{
public:
	CMsf() 
  {
		m_dwMsf = 0;
	}

	CMsf(DWORD dwMsf) 
  {		
		m_dwMsf = dwMsf;
	}

	CMsf(BYTE minute, BYTE second, BYTE frame) 
  {		
		m_dwMsf = MCI_MAKE_MSF(minute, second, frame);
	}
	
	operator DWORD() const {return m_dwMsf;}
	
	BYTE GetMinute() const { return MCI_MSF_MINUTE(m_dwMsf); }
	BYTE GetSecond() const { return MCI_MSF_SECOND(m_dwMsf); }
	BYTE GetFrame() const { return MCI_MSF_FRAME(m_dwMsf); }

protected:
	DWORD m_dwMsf;
};



//*************************************************************************
// CTmsf
//*************************************************************************
class CTmsf 
{
public:
	CTmsf() 
  {
		m_dwTmsf = 0;
	}

	CTmsf(DWORD dwTmsf) 
  {		
		m_dwTmsf = dwTmsf;
	}

	CTmsf(BYTE track, BYTE minute, BYTE second, BYTE frame) 
  {		
		m_dwTmsf = MCI_MAKE_TMSF(track, minute, second, frame);
	}
	
	operator DWORD() const {return m_dwTmsf;}

	BYTE GetTrack() const { return MCI_TMSF_TRACK(m_dwTmsf); }
	BYTE GetMinute() const { return MCI_TMSF_MINUTE(m_dwTmsf); }
	BYTE GetSecond() const { return MCI_TMSF_SECOND(m_dwTmsf); }
	BYTE GetFrame() const { return MCI_TMSF_FRAME(m_dwTmsf); }

protected:
	DWORD m_dwTmsf;
};


//*************************************************************************
// TrackRecord
//*************************************************************************
class TrackRecord
{
public:
//Constructors / Destructors
  TrackRecord();
  TrackRecord(const TrackRecord& track);

//Methods
  TrackRecord& operator=(const TrackRecord& track);

  int         nr;     // unique
  CString     name;
  CMsf        length;
  CMsf        startpos;
  BOOL        IsAudio;
};



//*************************************************************************
// CDinfo
//*************************************************************************

class CDinfo
{
public:
  void              Clear();

  CDinfo& operator=(const CDinfo& CD);

public:
  CArray<TrackRecord, TrackRecord&> m_Tracks;
  DWORD             m_EndPos;
  int               m_NrOfTracks;

  CString           m_AlbumArtist;
  CString           m_AlbumTitle;
  CString           m_ExtendedData;             //The extended data for this CD
};



#endif

