// CDinfo.cpp : implementation file
//

#include "stdafx.h"
#include "CDinfo.h"



//*************************************************************************
// TrackRecord
//*************************************************************************

//-------------------------------------------------------------------------
TrackRecord::TrackRecord()
{
  nr        = 0;     // uniek
  name      = "";
  length    = 0;
  startpos  = 0;
  IsAudio   = FALSE;
}


//-------------------------------------------------------------------------
TrackRecord::TrackRecord(const TrackRecord& track)
{
  *this = track;
}


//-------------------------------------------------------------------------
TrackRecord& TrackRecord::operator=(const TrackRecord& track)
{
  nr        = track.nr;
  name      = track.name;
  length    = track.length;
  startpos  = track.startpos;
  IsAudio   = track.IsAudio;

  return *this;
}



//*************************************************************************
// CDinfo
//*************************************************************************


//-------------------------------------------------------------------------
CDinfo& CDinfo::operator=(const CDinfo& CD)
{
  m_Tracks.Copy( CD.m_Tracks );
  m_EndPos       = CD.m_EndPos;
  m_NrOfTracks   = CD.m_NrOfTracks;

  m_AlbumArtist  = CD.m_AlbumArtist;
  m_AlbumTitle   = CD.m_AlbumTitle;
  m_ExtendedData = CD.m_ExtendedData;

  return *this;
}


//-------------------------------------------------------------------------
void CDinfo::Clear()
{
  m_Tracks.RemoveAll();
  m_EndPos               = 0;
  m_NrOfTracks           = 0;

  m_AlbumArtist          = "?";            
  m_AlbumTitle           = "?";
  m_ExtendedData         = "";
}

