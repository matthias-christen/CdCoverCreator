// AudioCDTracks.cpp: implementation of the CAudioCDTracks class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AudioCDTracks.h"
#include "PickTitleDlg.h"
#include "PickDriveDlg.h"

#include <mmsystem.h>
#include <afxinet.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL (CAudioCDTracks, CObject, VERSIONABLE_SCHEMA | 1)

CAudioCDTracks::CAudioCDTracks()
{
}

CString CAudioCDTracks::GetInfo ()
{
	CWaitCursor ();

	MCI_OPEN_PARMS mciOpen;
	LONG lOpen = OpenCD (mciOpen);

	int nTotalSum = 0, nSum = 0, nSecs = 0;
	int nMin = 0, nSec = 0, nFrame = 0;
	int nPosMin = 0, nPosSec = 0;
	int nMinPos0 = 0, nSecPos0 = 0;
	DWORD dwCDLength = 0;
	int nNumTracks = HIWORD (lOpen);
	CDWordArray arrFramePos;

	m_bAborted = LOWORD (lOpen) == IDABORT;

	if (!m_bAborted)
	{
		// Set the time format to Minute/Second/Frame (MSF) format
		MCI_SET_PARMS mciSetParms;
		mciSetParms.dwTimeFormat = MCI_FORMAT_MSF;
		mciSendCommand(mciOpen.wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, (DWORD) &mciSetParms);

		char szBuf[20];

		MCI_STATUS_PARMS mciStatus;
		mciStatus.dwCallback = 0;
		mciStatus.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;
		mciStatus.dwReturn = 0;

		for (int i = 1; i <= nNumTracks; i++)
		{
			// Get length of tracks
			mciStatus.dwItem = MCI_STATUS_LENGTH;
			mciStatus.dwTrack = i;
			mciSendCommand (mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM | MCI_TRACK, (DWORD) &mciStatus);

			nMin = (int) MCI_MSF_MINUTE (mciStatus.dwReturn);
			nSec = (int) MCI_MSF_SECOND (mciStatus.dwReturn);
			nFrame = (int) MCI_MSF_FRAME (mciStatus.dwReturn);
			sprintf (szBuf, "%02d:%02d", nMin, nSec);
			m_arrLength.Add (szBuf);
//			m_arrTrack.Add (new CTrack (i, szBuf, "", ""));

			// compute DiscId
			mciStatus.dwItem = MCI_STATUS_POSITION;
			mciStatus.dwTrack = i;
			mciSendCommand (mciOpen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM | MCI_TRACK, (DWORD) &mciStatus);

			nPosMin = (int) MCI_MSF_MINUTE (mciStatus.dwReturn);
			nPosSec = (int) MCI_MSF_SECOND (mciStatus.dwReturn);
			arrFramePos.Add (nPosMin * 60 * 75 + nPosSec * 75 + (int) MCI_MSF_FRAME (mciStatus.dwReturn));
			nSecs = nPosMin * 60 + nPosSec;

			if (i == 1)
				nMinPos0 = nPosMin, nSecPos0 = nPosSec;

			for (nSum = 0; nSecs > 0; nSecs /= 10)
				nSum += nSecs % 10;
			nTotalSum += nSum;
		}

		dwCDLength = (nPosMin * 60 * 75 + nPosSec * 75 + MCI_MSF_FRAME (mciStatus.dwReturn) + 1 +
			nMin * 60 * 75 + nSec * 75 + nFrame) / 75;
	}

	MCI_GENERIC_PARMS mciClose;
	mciClose.dwCallback = 0;
	mciSendCommand (mciOpen.wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD) &mciClose);

	// format strings for cddb

	m_strDiscId.Format ("%08x",
		(nTotalSum % 0xFF) << 24 | (dwCDLength - nMinPos0 * 60 - nSecPos0) << 8 | nNumTracks);

	CString str;
	for (int j = 0; j < arrFramePos.GetSize (); j++)
	{
		str.Format ("%d+", arrFramePos.GetAt (j));
		m_strFramePos += str;
	}

	m_strCDLength.Format ("%d", dwCDLength);

	return GetCDDBInfo ();
}

LONG CAudioCDTracks::OpenCD (MCI_OPEN_PARMS& mciOpen)
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwCallback = 0;
	mciStatus.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;

	// Try to open cdrom drives.
	DWORD dwDrives = GetLogicalDrives ();
	CString strDrive;
	CStringArray arrDrives;
	int nCDDrivesCount = 0;

	MCIERROR mciErr;
	mciOpen.dwCallback = 0;
	
	for (int i = 0; i < 32; i++)
	{
		if (dwDrives & (1 << i))
		{
			strDrive.Format ("%c:\\", 'a' + i);
			if (GetDriveType (strDrive) == DRIVE_CDROM)
			{
				nCDDrivesCount++;

//				char szFS[100];
//				GetVolumeInformation (strDrive, NULL, 0, NULL, NULL, NULL, szFS, 100);

				// open device
				mciOpen.lpstrDeviceType = (LPCTSTR) MAKELONG (MCI_DEVTYPE_CD_AUDIO, 0);
				mciOpen.lpstrElementName = strDrive;
				mciErr = mciSendCommand (NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_SHAREABLE | MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD) &mciOpen);
				if (mciErr)
					continue;

				// try to get number of tracks
				// if we succeed, an audio cd has been found
				mciErr = mciSendCommand (mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD) &mciStatus);
				if (!mciErr)
					arrDrives.Add (strDrive);

				MCI_GENERIC_PARMS mciClose;
				mciClose.dwCallback = 0;
				mciSendCommand (mciOpen.wDeviceID, MCI_CLOSE, MCI_WAIT, (DWORD) &mciClose);
			}
		}
	}

	if (nCDDrivesCount == 0)
	{
		// no cd drive has been found
		AfxMessageBox (IDS_NOCDDRIVE, MB_ICONSTOP);
		return IDABORT;
	}

	switch (arrDrives.GetSize ())
	{
	case 0:
		// no cd drive with an audio cd inserted has been found
		m_strDrive.Empty ();

		switch (AfxMessageBox (IDS_NOAUDIOCD, MB_ICONEXCLAMATION | MB_ABORTRETRYIGNORE | MB_DEFBUTTON2))
		{
		case IDABORT:
			return IDABORT;
		case IDIGNORE:
			return IDIGNORE;
		case IDRETRY:
			return OpenCD (mciOpen);
		}

	case 1:
		// exactly one drive with an audio cd inserted has been found
		// take this drive
		m_strDrive = arrDrives.GetAt (0);
		return MAKELONG (0, GetTracksNum (mciOpen));

	default:
		// more that one cd drives with an audio cd inserted.
		// display a dialog from which the user can pick a drive
		{
			CPickDriveDlg dlg;
			dlg.m_pArrDrives = &arrDrives;

			if (dlg.DoModal () == IDOK)
			{
				m_strDrive = dlg.m_strSelectedDrive;
				return MAKELONG (0, GetTracksNum (mciOpen));
			}

			return IDABORT;
		}
	}

	return IDABORT;
}

WORD CAudioCDTracks::GetTracksNum(MCI_OPEN_PARMS &mciOpen)
{
	MCI_STATUS_PARMS mciStatus;
	mciStatus.dwCallback = 0;
	mciStatus.dwItem = MCI_STATUS_NUMBER_OF_TRACKS;

	// open device
	mciOpen.lpstrDeviceType = (LPCTSTR) MAKELONG (MCI_DEVTYPE_CD_AUDIO, 0);
	mciOpen.lpstrElementName = m_strDrive;
	mciSendCommand (NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_SHAREABLE | MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD) &mciOpen);

	// get number of tracks
	mciSendCommand (mciOpen.wDeviceID, MCI_STATUS, MCI_WAIT | MCI_STATUS_ITEM, (DWORD) &mciStatus);
	return (WORD) mciStatus.dwReturn;
}

CAudioCDTracks::~CAudioCDTracks()
{
//	for (int i = 0; i < m_arrTrack.GetSize (); i++)
//		delete m_arrTrack.GetAt (i);
//	m_arrTrack.RemoveAll ();
}

CAudioCDTracks& CAudioCDTracks::operator= (CAudioCDTracks& t)
{
	// remove items
//	for (int i = 0; i < m_arrTrack.GetSize (); i++)
//		delete m_arrTrack.GetAt (i);
//	m_arrTrack.RemoveAll ();

	// add new items
//	for (i = 0; i < t.GetNumTracks (); i++)
//		m_arrTrack.Add (new CTrack (t.GetTrack (i)));

//	m_arrLength.RemoveAll ();
//	m_arrLength.Append (t.m_arrLength);

	m_strTitle = t.GetTitle ();
//	m_strSubtitle = t.GetSubtitle ();
//	for (i = 0; i < NUM_RENDER_TYPES; i++)
//		m_strRTFTitle[i] = t.GetRTFTitle ((RenderType) i);

	m_strCDLength = t.m_strCDLength;
	m_strDiscId = t.m_strDiscId;
	m_strFramePos = t.m_strFramePos;

	return *this;
}
/*
void CAudioCDTracks::Serialize (CArchive& ar)
{
	if (ar.IsStoring ())
	{
		ar << m_strTitle << m_strDiscId << m_strFramePos << m_strCDLength;

		// VERSION 3
		ar << m_strSubtitle;
		// VERSION 4
		ar << m_strRTFTitle[Cover] << m_strRTFTitle[Back] << m_strRTFTitle[Inlay] << m_strRTFTitle[Label];
	}
	else
	{
		ar >> m_strTitle >> m_strDiscId >> m_strFramePos >> m_strCDLength;

		// #begin VERSION 3
		if (((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema == 3)
		{
			ar >> m_strSubtitle;
		}
		else
		{
			m_strSubtitle.Empty ();
		} // #end VERSION 3

		// #begin VERSION 4
		if (((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 4)
		{
			ar >> m_strRTFTitle[Cover] >> m_strRTFTitle[Back] >> m_strRTFTitle[Inlay] >> m_strRTFTitle[Label];
		}
		else
		{
			// let RTF from text (Title + SubTitle) be created
			for (int i = 0; i < NUM_RENDER_TYPES; i++)
				m_strRTFTitle[i].Empty ();
		}
	}

//	m_arrTrack.Serialize (ar);
	m_arrLength.Serialize (ar);
}*/

CString CAudioCDTracks::GetTrackLength(int nTrack)
{
	ASSERT (nTrack > 0);
	ASSERT (nTrack <= m_arrLength.GetSize ());

	return m_arrLength.GetAt (nTrack - 1);
}
/*
bool CAudioCDTracks::GetCDText ()
{
	CASPI aspi;
	CDinfo info;

	info.Clear ();

	if (aspi.CheckASPI ())
	{
		aspi.GetTOC_ASPI (0, info);
		aspi.GetCDTEXT_ASPI (0, info);
	}
	else
	{
		if (getOsVersion () >= OS_WINNT4)
		{
			CString strDrive = m_strDrive.SpanExcluding ("\\");

			aspi.GetTOC_SPTI (strDrive, info);		
			aspi.GetCDTEXT_SPTI (strDrive, info);
		}
		else
			return false;
	}

	m_strArtist = info.m_AlbumArtist;
	m_strTitle = info.m_AlbumTitle;

	for (int i = 0; i < m_arrTrack.GetSize (); i++)
		((CTrack*) m_arrTrack.GetAt (i))->SetTitle (info.m_Tracks[i + 1].name);

	m_strRTFTitle[0] = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1031{\\fonttbl{\\f0\\fswiss\\fcharset0 Arial;}}{\\colortbl ;\\red0\\green0\\blue0;}\\viewkind4\\uc1\\pard\\qc\\f0\\cf1";
	if (!m_strArtist.IsEmpty ())
		m_strRTFTitle[0] += "\\fs20" + m_strArtist + "\\par";
	m_strRTFTitle[0] += "\\fs32 " +	m_strTitle + "\\par}";

	for (int i = 1; i < NUM_RENDER_TYPES; i++)
		m_strRTFTitle[i] = m_strRTFTitle[0];

	return true;
}*/

CString CAudioCDTracks::GetCDDBInfo()
{
	CString strResult;

	DWORD dwServiceType;
	CString strServer, strObject;
	INTERNET_PORT nPort;

	AfxParseURL ("http://freedb.freedb.org/~cddb/cddb.cgi", dwServiceType, strServer, strObject, nPort);

	CInternetSession session;
	CHttpConnection* pServer = session.GetHttpConnection (strServer);

	// hello string
	char szUser[100];
	DWORD dwSize = 100;
	GetUserName (szUser, &dwSize);
	char szHostName[100];
	dwSize = 100;
	GetComputerName (szHostName, &dwSize);
	CString strHello;
	CString strUser = szUser;
	strUser.Replace (' ', '_');
	strHello.Format ("&hello=%s+%s+CdCoverCreator+2.40&proto=3", strUser, szHostName);

	// query
	CString strNumTracks;
	strNumTracks.Format ("+%d+", m_arrLength.GetSize ());

	CString strQuery = "?cmd=cddb+query+" + m_strDiscId + strNumTracks + m_strFramePos +	// '+' already included
		m_strCDLength + strHello;

	try
	{
		strResult = ">>\r\n" + strObject + strQuery;

		CHttpFile* pFile = pServer->OpenRequest (CHttpConnection::HTTP_VERB_GET,
			strObject + strQuery, NULL, 1, NULL, NULL,
			INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);

		pFile->SendRequest ();

		DWORD dwStatus;
		if (pFile->QueryInfoStatusCode (dwStatus) && (dwStatus >= 400))
			AfxThrowFileException (CFileException::fileNotFound);

		char szBuf[1000];
		int n = pFile->Read (szBuf, 1000);
		szBuf[n] = 0;

		strResult += "\r\n\r\n<<\r\n";
		strResult += szBuf;

		// read
		CString strCategory, strDiscId;
		ParseQuery (szBuf, strCategory, strDiscId);
		if (!strCategory.IsEmpty () && !strDiscId.IsEmpty ())
		{
			CString strRead = "?cmd=cddb+read+" + strCategory + "+" + strDiscId + strHello;
			strResult += "\r\n\r\n>>\r\n" + strObject + strRead;

			pFile = pServer->OpenRequest (CHttpConnection::HTTP_VERB_GET,
				strObject + strRead, NULL, 1, NULL, NULL,
				INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE);
			pFile->SendRequest ();

			if (pFile->QueryInfoStatusCode (dwStatus) && (dwStatus >= 400))
				AfxThrowFileException (CFileException::fileNotFound);

			//ParseResult (pFile);
			// fill titles, remarks
			strResult += "\r\n\r\n<<";
			for (CString s; ; )
			{
				pFile->ReadString (s);

				if (s.IsEmpty ())
					break;

				strResult += s + "\r\n";
			}
		}
	}
	catch (CInternetException *e)
	{
		e->ReportError ();
		e->Delete ();
//		AfxMessageBox (IDS_INTERNETERROR, MB_ICONEXCLAMATION);
	}

	session.Close ();

	return strResult;
}

void CAudioCDTracks::ParseQuery(CString strQueryRes, CString &strCategory, CString &strDiscId)
{
	switch (atoi (strQueryRes.Left (3)))
	{
	case 200:	// found exact match
		{
			int i = 4;
			while (!isspace (strQueryRes[i]))
				i++;
			strCategory = strQueryRes.Mid (3, i - 3);

			int j = i + 1;
			while (!isspace (strQueryRes[j]))
				j++;
			strDiscId = strQueryRes.Mid (i, j - i + 1);

			strCategory.TrimLeft ();
			strCategory.TrimRight ();
			strDiscId.TrimLeft ();
			strDiscId.TrimRight ();
		}
		break;

	case 210:	// found multiple exact matches -> choice
	case 211:	// found inexact matches -> choice
		{
			CPickTitleDlg dlg;
			dlg.m_strTitle = strQueryRes;
			if (dlg.DoModal () == IDOK)
			{
				int nPos = dlg.m_strTitle.Find (';');
				strCategory = dlg.m_strTitle.Left (nPos);
				strDiscId = dlg.m_strTitle.Mid (nPos + 1);
			}
		}
		break;

	case 202:	// no match found
		AfxMessageBox (IDS_CDDB_NOMATCH);
		break;
	case 403:	// database entry is corrupt
		AfxMessageBox (IDS_CDDB_DBENTRYCORRUPT);
		break;
	case 409:	// no handshake
		AfxMessageBox (IDS_CDDB_NOHANDSHAKE);
		break;

	default:	// !?
		AfxMessageBox (IDS_CDDB_UNDEFINEDERROR);
	}
}

/*
void CAudioCDTracks::ParseResult(CStdioFile *pFile)
{
	CTrack* pT;

	// clear titles, remarks
	m_strTitle.Empty ();
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		m_strRTFTitle[i].Empty ();

	for (i = 0; i < m_arrTrack.GetSize (); i++)
	{
		pT = (CTrack*) m_arrTrack.GetAt (i);
		pT->SetText ("", "");
	}

	// fill titles, remarks
	for (CString s; ; )
	{
		pFile->ReadString (s);

		if (s.IsEmpty ())
			break;

		if (s.Left (7) == "DTITLE=")
		{
			m_strTitle += s.Mid (7);
			m_strArtist.Empty ();

			// artist may be separated by a " / "
			int nPos = m_strTitle.Find (" / ");
			if (nPos >= 0)
			{
				m_strArtist = m_strTitle.Left (nPos);
				m_strTitle = m_strTitle.Mid (nPos + 3);
			}

			m_strRTFTitle[0] = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1031{\\fonttbl{\\f0\\fswiss\\fcharset0 Arial;}}{\\colortbl ;\\red0\\green0\\blue0;}\\viewkind4\\uc1\\pard\\qc\\f0\\cf1";
			if (!m_strArtist.IsEmpty ())
				m_strRTFTitle[0] += "\\fs20" + m_strArtist + "\\par";
			m_strRTFTitle[0] += "\\fs32 " +	m_strTitle + "\\par}";

			for (int i = 1; i < NUM_RENDER_TYPES; i++)
				m_strRTFTitle[i] = m_strRTFTitle[0];
		}
		else if (s.Left (6) == "TTITLE")
		{
			pT = (CTrack*) m_arrTrack.GetAt (atoi (s.Mid (6)));
			pT->SetText (pT->GetTitle () + s.Mid (s.Find ('=') + 1), pT->GetRemarks ());
		}
		else if (s.Left (4) == "EXTT")
		{
			pT = (CTrack*) m_arrTrack.GetAt (atoi (s.Mid (4)));
			pT->SetText (pT->GetTitle (), pT->GetRemarks () + s.Mid (s.Find ('=') + 1));
		}
	}
}

void CAudioCDTracks::SetTrackTexts(int nTrackNum, CString strTitle, CString strRemarks)
{
	((CTrack*) m_arrTrack.GetAt (nTrackNum - 1))->SetText (strTitle, strRemarks);
}*/