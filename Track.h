// Track.h: interface for the CTrack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACK_H__410D255A_6BD7_4758_9282_BBC3E4B3A4FF__INCLUDED_)
#define AFX_TRACK_H__410D255A_6BD7_4758_9282_BBC3E4B3A4FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTrack: public CObject
{
	DECLARE_SERIAL (CTrack)

public:
	int operator -(CTrack& t);

	CTrack ();
	CTrack (int n, CString strLen, CString strTitle, CString strRem);
	CTrack (CTrack& t);

	CTrack& operator= (CTrack& t);
	bool operator== (CTrack& t);
	bool operator!= (CTrack& t);

	virtual void Serialize (CArchive& ar);
	void SetText (CString strTitle, CString strRemark);

	inline int GetTrackNum () { return m_nTrackNum; }
	inline CString GetLength () { return m_strLength; }
	inline CString GetNiceLength () { return m_strLength.Left (1) == "0" ? m_strLength.Mid (1) : m_strLength; }
	inline int GetLengthMin () { return atoi (m_strLength); }
	inline int GetLengthSec () { return (m_strLength.GetLength () >= 3) ? atoi (m_strLength.Mid (m_strLength.SpanIncluding ("0123456789").GetLength () + 1)) : 0; }
	inline CString GetTitle () { return m_strTitle; }
	inline CString GetRemarks () { return m_strRemarks; }

	inline bool IsTrackGrouping () { return m_nTrackNum == -1; }

	inline void SetTrackNum (CString strTrackNum) { m_nTrackNum = atoi (strTrackNum.GetBuffer (0)); }
	inline void SetLength (CString strLength) { m_strLength = strLength; }
	inline void SetTitle (CString strTitle) { m_strTitle = strTitle; }
	inline void SetRemarks (CString strRemarks) { m_strRemarks = strRemarks; }

private:
	int m_nTrackNum;
	CString m_strLength;
	CString m_strTitle;
	CString m_strRemarks;
};

#endif // !defined(AFX_TRACK_H__410D255A_6BD7_4758_9282_BBC3E4B3A4FF__INCLUDED_)
