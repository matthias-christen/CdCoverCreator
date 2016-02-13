#if !defined(ASPI_H_INCLUDED_)
#define ASPI_H_INCLUDED_


#include <windows.h>
#include "ASPI_struc.h"
#include "CDinfo.h"

#define OS_UNKNOWN  -1
#define OS_WIN95    0
#define OS_WIN98    1
#define OS_WINNT35  2
#define OS_WINNT4   3
#define OS_WIN2K    4

int getOsVersion ();

//-------------------------------------------------------------------------
// Typedefs
//-------------------------------------------------------------------------

typedef struct
{
  BYTE      rsvd;
  BYTE      ADR;
  BYTE      trackNumber;
  BYTE      rsvd2;
  BYTE      addr[4];
} TOCTRACK;

typedef struct
{
  WORD      tocLen;
  BYTE      firstTrack;
  BYTE      lastTrack;
  TOCTRACK tracks[100];
} TOC, *PTOC, FAR *LPTOC;


class CdText_data
{
public:
  BYTE  type;
  BYTE  track;
  BYTE  seq;
  BYTE  characterPosition:4;  // character position
  BYTE  block:3;		          // block number 0..7
  BYTE  bDBC:1;		            // double byte character
  char  text[12];
  BYTE  crc[2];
};



class CASPI
{
public:
  BOOL  CheckASPI();
  int   GetTOC_ASPI(int disknr, CDinfo& CD);
  BOOL  GetTOC_SPTI(CString drivename, CDinfo& CD);
  BOOL  GetCDTEXT_ASPI(int disknr, CDinfo& CD);
  BOOL  GetCDTEXT_SPTI(CString drivename, CDinfo& CD);

protected:
  BOOL  ASPI_DriveInfo();
  void  GetTracksFromTOC( CDinfo& CD, TOC* ptoc );
  void  ReadCDTEXT( CDinfo& CD );
  BOOL  ASPI_Command( int disknr, BYTE* indata, BYTE* outdata, int outdatalen );
  BOOL  SPTI_Command( CString drivename, BYTE* indata, BYTE* outdata, int outdatalen );

protected:
  DWORD (*GetASPI32SupportInfo)(void);
  DWORD (*SendASPI32Command)(LPSRB);
  DRIVELIST driveList;
  BYTE      bigbuffer[5000];

};


  DWORD CMsf2DWORD( CMsf org );
  DWORD CTmsf2DWORD( CTmsf org );
  CMsf  DWORD2CMsf( DWORD org );
  DWORD TOC2DWORD( BYTE* ptr );


#endif

