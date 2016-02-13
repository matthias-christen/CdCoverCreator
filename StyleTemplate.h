// StyleTemplate.h: interface for the CStyleTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STYLETEMPLATE_H__A2886251_9E27_4E19_8C1B_4257EBEB6EE8__INCLUDED_)
#define AFX_STYLETEMPLATE_H__A2886251_9E27_4E19_8C1B_4257EBEB6EE8__INCLUDED_

#include "RenderDC.h"
#include "Gradient.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef PI
	#define PI 3.1415926535897932384626433832795
#endif

enum MediumType
{
	MediumJewelCase,
	MediumPaperSleeve,
	MediumSlimCase,
	MediumDVDBox
};

class CStyleTemplate : public CObject  
{
	DECLARE_SERIAL (CStyleTemplate)

	friend class CStyleTemplates;

	friend class CSTWSelectTmplPage;
	friend class CSTWTitleFontsPage;
	friend class CSTWTrackFontsPage;
	friend class CSTWBackgroundColorsPage;
	friend class CSTWCoverTitleStylePage;
	friend class CSTWLabelTitleStylePage;
	friend class CSTWInlayTitleStylePage;
	friend class CSTWTracksStylePage;
	friend class CSTWBookletStylePage;

	friend class CStyleDlgGradient;
	friend class CStyleDlgFonts;
	friend class CStyleDlgTracks;
	friend class CStyleDlgMeasures;
	friend class CStyleDlgTrackgroupings;
	friend class CStyleDlgSpines;

public:
	void SetTrackRect (CRect r, RenderType rt, CRenderDC& rdc);
	void SetTitleRect (CRect r, RenderType rt, CRenderDC& rdc);
	CRect* GetTotalRect (CRenderDC& rdc, RenderType rt);
	CRect* GetTotalRect (RenderType rt);
	CRect* GetLabelTrackRectStraight(CRenderDC& rdc);
	CRect* GetLabelTrackRectRound(CRenderDC& rdc);
	UINT GetDrawRectVAlignment (int nAlign);
	virtual void Serialize (CArchive& ar);

	CStyleTemplate ();
	CStyleTemplate (CString strName);
	CStyleTemplate (CStyleTemplate& style);
/*	CStyleTemplate (CString strName,
		LOGFONT* pLfTrackNum, LOGFONT* pLfTrackLength, LOGFONT* pLfTrackTitle, LOGFONT* pLfTrackRemarks,
		LOGFONT* pLfTrackGroupingTitle,	LOGFONT* pLfTrackGroupingRemarks,
		LOGFONT* pLfTitle, LOGFONT* pLfSide,
		COLORREF crTrackNum, COLORREF crTrackLength, COLORREF crTrackTitle, COLORREF crTrackRemarks,
		COLORREF crTrackGroupingTitle, COLORREF crTrackGroupingRemarks,
		COLORREF crTitle, COLORREF crSide,
		int nHTitleAlign, double dHTitleMargin,	double dTitleTop,	double dTitleHeight,
		int nVSideAlign, double dVSideMargin,
		bool bTrackFloating, bool bTrackTotalTime,
		int nTrackNumPos, bool bTrackNumDisplay, int nTrackNumAlign, CString strTrackNumFormatting,
		int nTrackLenPos, bool bTrackLenDisplay, int nTrackLenAlign, CString strTrackLenFormatting,
		int nTrackTitlePos, bool bTrackTitleDisplay, int nTrackTitleAlign,
		int nTrackRemPos, bool bTrackRemDisplay, int nTrackRemAlign,
		double dTrackMarginLeft, double dTrackMarginRight, double dTrackInterItemSpacing,
		double dTrackGrpSpacingAbove, double dTrackGrpSpacingBelow, bool bTrackGrpNewLine);
*/
	virtual ~CStyleTemplate();

	CStyleTemplate& operator= (CStyleTemplate& s);
	bool operator== (CStyleTemplate& s);
	bool operator!= (CStyleTemplate& s);

	CFont* SelectTrackNumFont (CRenderDC& rdc, double dFactor = 1.0);
	CFont* SelectTrackLengthFont (CRenderDC& rdc, double dFactor = 1.0);
	CFont* SelectTrackTitleFont (CRenderDC& rdc, double dFactor = 1.0);
	CFont* SelectTrackRemarksFont (CRenderDC& rdc, double dFactor = 1.0);
	CFont* SelectTrackGroupingTitleFont (CRenderDC& rdc, double dFactor = 1.0);
	CFont* SelectTrackGroupingRemarksFont (CRenderDC& rdc, double dFactor = 1.0);
	CFont* SelectTitleFont (CRenderDC& rdc, RenderType rt, double dFactor = 1.0);
	CFont* SelectInlaySideFont (CRenderDC& rdc, bool bIsLeftSide, double dFactor = 1.0);

	CString GetTrackNumFontName () { return m_lfTrackNum.lfFaceName; }
	CString GetTrackLengthFontName () { return m_lfTrackLength.lfFaceName; }
	CString GetTrackTitleFontName () { return m_lfTrackTitle.lfFaceName; }
	CString GetTrackRemarksFontName () { return m_lfTrackRemarks.lfFaceName; }
	CString GetTrackGroupingTitleFontName () { return m_lfTrackGroupingTitle.lfFaceName; }
	CString GetTrackGroupingRemarksFontName () { return m_lfTrackGroupingRemarks.lfFaceName; }

	CString GetTitleFontName (RenderType rt) { return m_lfTitle[rt].lfFaceName; }
	CString GetInlaySideFontName () { return m_lfInlaySide.lfFaceName; }

	inline CString GetName () { return m_strTemplateName; }

	inline LOGFONT* GetTrackNumFont () { return &m_lfTrackNum; }
	inline LOGFONT* GetTrackLengthFont () { return &m_lfTrackLength; }
	inline LOGFONT* GetTrackTitleFont () { return &m_lfTrackTitle; }
	inline LOGFONT* GetTrackRemarksFont () { return &m_lfTrackRemarks; }
	inline LOGFONT* GetTrackGroupingTitleFont () { return &m_lfTrackGroupingTitle; }
	inline LOGFONT* GetTrackGroupingRemarksFont () { return &m_lfTrackGroupingRemarks; }
	inline LOGFONT* GetTitleFont (RenderType rt) { return &m_lfTitle[rt]; }
	inline LOGFONT* GetInlaySideFont () { return &m_lfInlaySide; }
	inline LOGFONT* GetBookletPageNumFont () { return &m_lfBookletPageNumFont; }

	inline COLORREF GetTrackNumColor () { return m_crTrackNum; }
	inline COLORREF GetTrackLengthColor () { return m_crTrackLength; }
	inline COLORREF GetTrackTitleColor () { return m_crTrackTitle; }
	inline COLORREF GetTrackRemarksColor () { return m_crTrackRemarks; }
	inline COLORREF GetTrackGroupingTitleColor () { return m_crTrackGroupingTitle; }
	inline COLORREF GetTrackGroupingRemarksColor () { return m_crTrackGroupingRemarks; }

	inline COLORREF GetTitleColor (RenderType rt) { return m_crTitle[rt]; }
	//inline COLORREF GetBkColor (RenderType rt) { return m_crBkStart[rt]; }
	//inline COLORREF GetBkColor2 (RenderType rt) { return m_crBkEnd[rt]; }

	inline COLORREF GetInlaySideColor () { return m_crInlaySide; }

	inline COLORREF GetBookletPageNumColor () { return m_crBookletPageNumColor; }

	inline int GetBkAngle (RenderType rt) { return rad2deg (m_lgdBackground[rt].dAngle); }
	inline LINEARGRADIENTDATA* GetGradientData (RenderType rt) { return &m_lgdBackground[rt]; }

	// cover measures
	CRect* GetTotalCoverRect (CRenderDC& rdc);
	CRect* GetCoverTrackRect(CRenderDC &rdc);
	CRect* GetCoverTitleRect(CRenderDC& rdc, RenderType rt = Cover);

	// back measures
	CRect* GetBackTrackRect (CRenderDC& rdc);

	// label measures
	CRect* GetTotalLabelRect (CRenderDC& rdc);
	CRect* GetInnerLabelRect (CRenderDC& rdc);
	CRect* GetInner2LabelRect (CRenderDC& rdc);
	CRect* GetLabelTrackRect(CRenderDC &rdc);
	CRect* GetLabelTitleRect(CRenderDC& rdc);

	// inlay measures
	CRect* GetTotalInlayRect (CRenderDC& rdc);
	int GetInlaySideVAlignTop (CRenderDC& rdc, int nTxtExtent);
	CRect* GetInlayRightSideRect (CRenderDC &rdc);
	CRect* GetInlayLeftSideRect (CRenderDC& rdc);
	CRect* GetInlayTitleRect(CRenderDC& rdc);
	CRect* GetInlayTrackRect(CRenderDC &rdc);

	// track measures
	int GetTrackInterItemSpacing (CRenderDC& rdc, RenderType rt);
	int GetTrackPositionById (int nId);
	UINT GetDrawRectAlignment (int nAlign);

	void SetTitleSize (CSize size, RenderType rt) { m_sizeTitleRect[rt] = size; }
	inline CSize GetTitleSize (RenderType rt) { return m_sizeTitleRect[rt]; }

	// general
	inline double GetTitleHeight (RenderType rt) {
		if ((rt == Label) && !m_bLabelTitleRound)
			return 3;	// if straight label text return constant (rect remains the same)
		return m_dTitleHeight[rt];
	}
	inline double GetTitleTop (RenderType rt) { return m_dTitleTop[rt]; }
	inline bool GetDisplayTitles (RenderType rt) { return m_bDisplayTitles[rt]; }
	inline bool GetDisplayTrackGroups (RenderType rt) { return m_bDisplayTrackgroups[rt]; }

	// horizontal margin for cover, back and inlay, vertical margin for label
	inline double GetTitleMargin (RenderType rt) { return m_dTitleMargin[rt]; }
	inline int GetTitleAlign (RenderType rt) { return m_nTitleAlign[rt]; }

	inline int GetTrackgroupHAlign (RenderType rt) { return m_nTrackgroupHAlign[rt]; }
	inline int GetTrackgroupVAlign (RenderType rt) { return m_nTrackgroupVAlign[rt]; }
	inline double GetTrackgroupHMargin (RenderType rt) { return m_dTrackgroupHMargin[rt]; }
	inline double GetTrackgroupVMargin (RenderType rt) { return m_dTrackgroupVMargin[rt]; }

	inline void SetTrackgroupAlign (RenderType rt, int nHAlign, int nVAlign) {
		m_nTrackgroupHAlign[rt] = nHAlign, m_nTrackgroupVAlign[rt] = nVAlign; }

	// cover
	//////////////////////////////
	
	// label
	//////////////////////////////
	// title
	inline int GetLabelTitlePosition () { return m_nLabelTitlePosition; }
	inline bool IsLabelTitleRound () { return m_bLabelTitleRound; }
	// tracks
	inline bool IsLabelTrackDisplayed () { return m_bLabelDisplayTracks; }
	//inline bool IsLabelTrackgroupDisplayed () { return m_bLabelDisplayTrackgroups; }
	inline int GetLabelTrackVAlign () { return m_nLabelTrackVAlign; }
	inline double GetLabelTrackVMargin () { return m_dLabelTrackVMargin; }
	inline int GetLabelTrackPosition () { return m_nLabelTrackPosition; }
	inline bool IsLabelTrackRound () { return m_bLabelTrackRound; }

	// inlay
	//////////////////////////////
	// title
	inline bool GetInlayDisplaySidebarTitles () { return m_bInlayDisplaySidebarTitles; }
	// spines
	inline int GetInlaySideVAlign () { return m_nInlaySideVAlign; }
	inline double GetInlaySideVMargin () { return m_dInlaySideVMargin; }
	inline double GetInlaySideWidth () { return (double) m_rcInlaySideLeft.Width () / 1000.0; }
	inline int GetInlaySideTextDirection () { return m_bInlaySideTextDirection; }
	inline bool IsInlaySideAntiparallel () { return m_bInlaySideAntiparallel; }

	// booklet
	//////////////////////////////
	inline double GetBookletMarginLeft () { return m_dBookletMarginLeft; }
	inline double GetBookletMarginTop () { return m_dBookletMarginTop; }
	inline double GetBookletMarginRight () { return m_dBookletMarginRight; }
	inline double GetBookletMarginBottom () { return m_dBookletMarginBottom; }
	inline bool ShowBookletPageNums () { return m_bBookletShowPageNums; }
	inline int GetBookletPageNumAlign () { return m_nBookletPageNumAlign; }

	// tracks
	inline bool GetTrackFloating () { return m_bTrackFloating; }
	inline bool GetTrackTotalTime () { return m_bTrackTotalTime; }

	inline int GetTrackNumPosition () { return m_htaTrack[TrackNum].nPosition; }
	inline bool GetTrackNumDisplay () { return m_htaTrack[TrackNum].bDisplay; }
	inline int GetTrackNumHAlign () { return m_htaTrack[TrackNum].nHAlign; }
	inline CString GetTrackNumFormatting () { return m_htaTrack[TrackNum].strFormatting; }
	inline int GetTrackLenPosition () { return m_htaTrack[TrackLen].nPosition; }
	inline bool GetTrackLenDisplay () { return m_htaTrack[TrackLen].bDisplay; }
	inline int GetTrackLenHAlign () { return m_htaTrack[TrackLen].nHAlign; }
	inline CString GetTrackLenFormatting () { return m_htaTrack[TrackLen].strFormatting; }
	inline int GetTrackTitlePosition () { return m_htaTrack[TrackTitle].nPosition; }
	inline bool GetTrackTitleDisplay () { return m_htaTrack[TrackTitle].bDisplay; }
	inline int GetTrackTitleHAlign () { return m_htaTrack[TrackTitle].nHAlign; }
	inline int GetTrackRemarkPosition () { return m_htaTrack[TrackRemark].nPosition; }
	inline bool GetTrackRemarkDisplay () { return m_htaTrack[TrackRemark].bDisplay; }
	inline int GetTrackRemarkHAlign () { return m_htaTrack[TrackRemark].nHAlign; }

	inline double GetTrackMarginLeft () { return m_dTrackMarginLeft; }
	inline double GetTrackMarginRight () { return m_dTrackMarginRight; }
	inline double GetTrackInterItemSpacing () { return m_dTrackInterItemSpacing; }

	inline double GetTrackGrpSpacingAbove () { return m_dTrackGrpSpacingAbove; }
	inline double GetTrackGrpSpacingBelow () { return m_dTrackGrpSpacingBelow; }
	inline bool GetTrackGrpNewLine () { return m_bTrackGrpNewLine; }

	inline bool IsTemplateNameChanged () { return m_bTemplateNameChanged; }

	int MapY (double y, CRenderDC& rdc, RenderType rt);
	int MapX (double x, CRenderDC& rdc, RenderType rt);
	double MapYr (double y, CRenderDC &rdc, RenderType rt);
	double MapXr (double x, CRenderDC& rdc, RenderType rt);

	CRect* GetTotalInlayRect () { return &m_rcInlayTotal; }
	CRect* GetTotalLabelRect () { return &m_rcLabelTotal; }
	CRect* GetInnerLabelRect () { return &m_rcLabelInner; }
	CRect* GetInner2LabelRect () { return &m_rcLabelInner2; }
	CRect* GetTotalCoverRect () { return &m_rcCoverTotal; }
	CRect* GetTotalBookletRect () { return &m_rcBookletTotal; }

	void SetMeasures (MediumType type);
	bool m_bHasRenderType[NUM_RENDER_TYPES];
	MediumType m_medium;
	inline MediumType GetMediumType () { return m_medium; }

protected:
	void CheckBoundaries (CRect rcRef, CRenderDC& rdc, RenderType rt);
	bool m_bTemplateNameChanged;
	void Init();
	CString m_strTemplateName;

	CFont* CreateFont (LOGFONT* pLf, COLORREF col, CRenderDC& rdc, double dFactor);

	LOGFONT m_lfTitle[NUM_RENDER_TYPES];
	COLORREF m_crTitle[NUM_RENDER_TYPES];
//	COLORREF m_crBkStart[NUM_RENDER_TYPES], m_crBkEnd[NUM_RENDER_TYPES];
	LINEARGRADIENTDATA m_lgdBackground[NUM_RENDER_TYPES];			// ver 5

	LOGFONT m_lfInlaySide;
	COLORREF m_crInlaySide;

	LOGFONT m_lfTrackNum;
	LOGFONT m_lfTrackLength;
	LOGFONT m_lfTrackTitle;
	LOGFONT m_lfTrackRemarks;
	LOGFONT m_lfTrackGroupingTitle;
	LOGFONT m_lfTrackGroupingRemarks;
	COLORREF m_crTrackNum;
	COLORREF m_crTrackLength;
	COLORREF m_crTrackTitle;
	COLORREF m_crTrackRemarks;
	COLORREF m_crTrackGroupingTitle;
	COLORREF m_crTrackGroupingRemarks;
//	int m_nBkAngle[NUM_RENDER_TYPES];

	CRect m_rcInlayTotal;
	CRect m_rcLabelTotal, m_rcLabelInner, m_rcLabelInner2;
	CRect m_rcCoverTotal;
	CRect m_rcBookletTotal;

	// margins		// ver 4
	// horizontal margin for cover, back and inlay, vertical margin for label
	int m_nTitleAlign[NUM_RENDER_TYPES];
	double m_dTitleMargin[NUM_RENDER_TYPES];

	// general
	CSize m_sizeTitleRect[NUM_RENDER_TYPES];
	double m_dTitleHeight[NUM_RENDER_TYPES];
	double m_dTitleTop[NUM_RENDER_TYPES];
	bool m_bDisplayTitles[NUM_RENDER_TYPES];	// ver 3

	// track groupings
	bool m_bDisplayTrackgroups[NUM_RENDER_TYPES];	// ver 4
	int m_nTrackgroupHAlign[NUM_RENDER_TYPES];
	int m_nTrackgroupVAlign[NUM_RENDER_TYPES];
	double m_dTrackgroupHMargin[NUM_RENDER_TYPES];
	double m_dTrackgroupVMargin[NUM_RENDER_TYPES];

	// cover
	//////////////////////////////

	// back										// ver 4
	//////////////////////////////
	
	// label
	//////////////////////////////
	// title
	int m_nLabelTitlePosition;
	bool m_bLabelTitleRound;
	// tracks
	bool m_bLabelDisplayTracks;
	//bool m_bLabelDisplayTrackgroups;
	int m_nLabelTrackVAlign;
	double m_dLabelTrackVMargin;
	int m_nLabelTrackPosition;
	bool m_bLabelTrackRound;

	// inlay
	//////////////////////////////
	// title
	bool m_bInlayDisplaySidebarTitles;	// ver 3
	// sides
	CRect m_rcInlaySideLeft;
	CRect m_rcInlaySideRight;
	int m_nInlaySideVAlign;
	double m_dInlaySideVMargin;
	bool m_bInlaySideTextDirection;			// ver 6
	bool m_bInlaySideAntiparallel;			// ver 9

	// tracks
	CRect m_rcTracks;
	bool m_bTrackFloating;
	bool m_bTrackTotalTime;
	struct tagTrackHAlign
	{
		int nPosition;
		bool bDisplay;
		int nHAlign;
		CString strFormatting;
	} m_htaTrack[NUM_TRACK_TYPES];	// Num (0), Len (1), Title (2), Remark (3)
	double m_dTrackMarginLeft;
	double m_dTrackMarginRight;
	double m_dTrackInterItemSpacing;

	double m_dTrackGrpSpacingAbove;
	double m_dTrackGrpSpacingBelow;
	bool m_bTrackGrpNewLine;

	// booklet
	double m_dBookletMarginLeft, m_dBookletMarginTop,
		m_dBookletMarginRight, m_dBookletMarginBottom;
	bool m_bBookletShowPageNums;
	int m_nBookletPageNumAlign;
	LOGFONT m_lfBookletPageNumFont;
	COLORREF m_crBookletPageNumColor;

private:
	CRect m_rect;
};

#endif // !defined(AFX_STYLETEMPLATE_H__A2886251_9E27_4E19_8C1B_4257EBEB6EE8__INCLUDED_)
