// TracksInfo.h: interface for the CTracksInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACKSINFO_H__C9EC6C86_7894_4DDF_B76B_B6C3B4969DB5__INCLUDED_)
#define AFX_TRACKSINFO_H__C9EC6C86_7894_4DDF_B76B_B6C3B4969DB5__INCLUDED_

#include "AudioCDTracks.h"
#include "Track.h"
#include "StyleTemplate.h"
#include "RenderDC.h"
#include "StdAfx.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTracksInfo : public CAudioCDTracks
{
public:
	CTracksInfo();
	CTracksInfo (CTracksInfo& tracks);
	CTracksInfo (RetrieveMode mode);
	virtual ~CTracksInfo();

	virtual void Serialize (CArchive& ar);

	bool operator== (CTracksInfo& ti);
	bool operator!= (CTracksInfo& ti);

	void Add (CTrack* pTrack);
	void Insert (CTrack* pTrack, int nPos);
	void Remove (int nPos);

	void Render (CRenderDC& rdc, RenderType rt, CStyleTemplate& style);

	void Match (CTracksInfo* pMergedTracks, bool bOverwrite);

	inline bool IsManuallyEditable () { return m_bManualEdit; }
	inline void SetManuallyEditable (bool bMan) { m_bManualEdit = bMan; }

	void CalcTitleRects2 (CRenderDC& rdc, RenderType rt, CStyleTemplate& style);
	void DrawTitle2 (CRenderDC &rdc, RenderType rt, CStyleTemplate& style);

	CRect m_rcDisplayTitleRect[NUM_RENDER_TYPES];

	bool GetTrackRectFromPoint (CPoint pt, CRenderDC& rdc, RenderType rt, CStyleTemplate& style, CRect &r, CString &strText, int &nNum, int &nIdx, LOGFONT *pLogfont, COLORREF& col, DWORD& dwAlign);
	bool GetTrackRectFromIdx (int nNum, int nIdx, CRenderDC& rdc, RenderType rt, CStyleTemplate& style, CRect& r, CString& strText, LOGFONT* pLogfont, COLORREF& color, DWORD& dwAlign);

	bool SetTrackText (CString strText, int nNum, int nIdx);

private:
	CRect m_rect;
	CSize m_szMax;

protected:
	bool m_bSizeChanged, m_bRtChanged;
	int m_nOldWidth;
	RenderType m_rtOldRt;
	CStyleTemplate m_styleCurrent;

	int m_nOffsetY;
	CPoint m_ptTracksOffset;
	double m_dSideFontFactor;
	double m_dTitleFontFactor;
	double m_dTrackFontFactor;
	int m_nPosX[6];
	int m_nDeltaY;
	
	bool m_bManualEdit;

	void CalcTextRects(CRenderDC& rdc, RenderType rt, CStyleTemplate& style);
	void CalcTitleRects (CRenderDC &rdc, RenderType rt, CStyleTemplate& style);
	CString CalcTotalTime (CStyleTemplate& style);

	CRect* GetInlayTrackRect (int nRow, int nIdx, CRenderDC& rdc, CStyleTemplate& style);
	UINT GetTrackRectAlignment (CStyleTemplate& style, RenderType rt, TrackType tt);

	void DrawLabelTracks (CRenderDC& rdc, CStyleTemplate& style);
	void DrawTotalTime (int nIndex, CRenderDC& rdc, RenderType rt, CStyleTemplate& style);
	void DrawInlayTrack (CTrack* pTrack, int nIndex, CRenderDC& rdc, CStyleTemplate& style);
	void DrawSideBars (CRenderDC& rdc, RenderType rt, CStyleTemplate& style);
	void DrawTitle (CRenderDC& rdc, RenderType rt, CStyleTemplate& style);

	CSize& Max (CSize s1, CSize s2);
};

#endif // !defined(AFX_TRACKSINFO_H__C9EC6C86_7894_4DDF_B76B_B6C3B4969DB5__INCLUDED_)
