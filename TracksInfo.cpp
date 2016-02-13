// TracksInfo.cpp: implementation of the CTracksInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"

#include "TracksInfo.h"

#include "Formatter.h"
#include "TextRenderer.h"
#include "FormattedTextDraw.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int sum (int n, int* pSummands)
{
	int s = 0;
	for (int i = 0; i < n; i++)
		s += pSummands[i];
	return s;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTracksInfo::CTracksInfo()
{
	m_rtOldRt = (RenderType) -1;
	m_nOldWidth = -1;
	m_bRtChanged = false;
	m_bSizeChanged = false;
}

CTracksInfo::CTracksInfo(CTracksInfo& tracks)
{
	m_rtOldRt = (RenderType) -1;
	m_nOldWidth = -1;
	m_bRtChanged = false;
	m_bSizeChanged = false;

	*this = tracks;
}

CTracksInfo::CTracksInfo (RetrieveMode mode)
	: CAudioCDTracks (mode)
{
	m_rtOldRt = (RenderType) -1;
	m_nOldWidth = -1;
	m_bRtChanged = false;
	m_bSizeChanged = false;
}

CTracksInfo::~CTracksInfo()
{
}

void CTracksInfo::Add(CTrack *pTrack)
{
	m_arrTrack.Add (pTrack);
}

void CTracksInfo::Insert(CTrack *pTrack, int nPos)
{
	m_arrTrack.InsertAt (nPos, pTrack);
}

void CTracksInfo::Remove(int nPos)
{
	delete m_arrTrack.GetAt (nPos);
	m_arrTrack.RemoveAt (nPos);
}

void CTracksInfo::DrawTitle(CRenderDC &rdc, RenderType rt, CStyleTemplate &style)
{
	CFont* pOldFont;

	// Draw title
	switch (rt)
	{
	case Cover:
	case Back:
		if (style.GetDisplayTitles (Cover))
		{
			/*
			pOldFont = style.SelectCoverTitleFont (rdc, m_dTitleFontFactor);
			rdc.m_pDC->DrawText (m_strTitle, style.GetCoverTitleRect (rdc),
				style.GetDrawRectAlignment (style.GetCoverTitleHAlign ()));
			rdc.m_pDC->SelectObject (pOldFont)->DeleteObject ();
			*/

			CRect r1 = *style.GetCoverTitleRect (rdc);
			CRect r2 = r1;
			r1.bottom = r2.top = (LONG) (r1.top + 0.6 * r1.Height ());

//			pOldFont = style.SelectCoverTitleFont (rdc, m_dTitleFontFactor * (m_strSubtitle.IsEmpty () ? 1 : 0.6));
			pOldFont = style.SelectTitleFont (rdc, Cover, 0.6);

			// draw the text
			rdc.m_pDC->DrawText (m_strTitle, &r1,
				style.GetDrawRectAlignment (style.GetTitleAlign (Cover)));

			// calculate the rectangle
			rdc.m_pDC->DrawText (m_strTitle, &r1,
				style.GetDrawRectAlignment (style.GetTitleAlign (Cover)) | DT_CALCRECT);

			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();

			if (!m_strSubtitle.IsEmpty ())
			{
				pOldFont = style.SelectTitleFont (rdc, Cover, m_dTitleFontFactor * 0.4);

				// draw the text
				rdc.m_pDC->DrawText (m_strSubtitle, &r2,
					style.GetDrawRectAlignment (style.GetTitleAlign (Cover)));

				// calculate the rectangle
				rdc.m_pDC->DrawText (m_strSubtitle, &r2,
					style.GetDrawRectAlignment (style.GetTitleAlign (Cover)) | DT_CALCRECT);

				pOldFont = rdc.m_pDC->SelectObject (pOldFont);
				if (pOldFont != NULL)
					pOldFont->DeleteObject ();
			}

			m_rcDisplayTitleRect[rt].left = m_rcDisplayTitleRect[rt].top = 0;
			if (m_strSubtitle.IsEmpty ())
			{
				m_rcDisplayTitleRect[rt].right = r1.Width ();
				m_rcDisplayTitleRect[rt].bottom = r1.Height ();
			}
			else
			{
				m_rcDisplayTitleRect[rt].right = max (r1.Width (), r2.Width ());
				m_rcDisplayTitleRect[rt].bottom = r1.Height () + r2.Height ();
			}

			m_ptTracksOffset.x = 0;
			m_ptTracksOffset.y = r2.bottom;
		}
		break;           

	case Inlay:
		if (style.GetDisplayTitles (Inlay))
		{
			/*
			pOldFont = style.SelectInlayTitleFont (rdc, m_dTitleFontFactor);
			rdc.m_pDC->DrawText (m_strTitle, style.GetInlayTitleRect (rdc),
				style.GetDrawRectAlignment (style.GetInlayTitleHAlign ()));
			*/

			CRect r1 = *style.GetInlayTitleRect (rdc);
			CRect r2 = r1;
			r1.bottom = r2.top = (LONG) (r1.top + 0.6 * r1.Height ());

			pOldFont = style.SelectTitleFont (rdc, Inlay, m_dTitleFontFactor * 0.6);
			rdc.m_pDC->DrawText (m_strTitle, &r1,
				style.GetDrawRectAlignment (style.GetTitleAlign (Inlay)));
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();

			pOldFont = style.SelectTitleFont (rdc, Inlay, m_dTitleFontFactor * 0.4);
			rdc.m_pDC->DrawText (m_strSubtitle, &r2,
				style.GetDrawRectAlignment (style.GetTitleAlign (Inlay)));
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();


			m_ptTracksOffset.x = 0;
			m_ptTracksOffset.y = style.GetInlayTitleRect (rdc)->bottom;
		}
		break;

	case Label:
		if (style.GetDisplayTitles (Label))
		{
			if (style.IsLabelTitleRound ())
			{
				bool bFlip = style.GetTitleAlign (Label) == 0 ||
					(style.GetTitleAlign (Label) == 2 && style.GetLabelTitlePosition () == 0);

				CRect r = *style.GetLabelTitleRect (rdc);
				pOldFont = style.SelectTitleFont (rdc, Label, m_dTitleFontFactor);
				int nHeight = rdc.m_pDC->GetTextExtent ("X").cy;

				// split lines
				int nPos;
				for (int nOldPos = 0; ; )
				{
					nPos = m_strTitle.Find ('\n', nOldPos);
					if (nPos != -1)
					{
						CTextRenderer::DrawRoundText (rdc.m_pDC, rdc.m_hndPrinter, r, !bFlip,
							m_strTitle.Mid (nOldPos, nPos - nOldPos - 1),
							style.GetDisplayTrackGroups (Label) || style.IsLabelTrackDisplayed () ? PI : 2 * PI - 0.2);
						r.DeflateRect (nHeight, nHeight);
					}
					else
					{
						CTextRenderer::DrawRoundText (rdc.m_pDC, rdc.m_hndPrinter, r, !bFlip,
							m_strTitle.Mid (nOldPos),
							style.GetDisplayTrackGroups (Label) || style.IsLabelTrackDisplayed () ? PI : 2 * PI - 0.2);
						break;
					}

					nOldPos = nPos + 1;
				}

				pOldFont = rdc.m_pDC->SelectObject (pOldFont);
				if (pOldFont != NULL)
					pOldFont->DeleteObject ();
			}
			else
			{
				CRect r1 = *style.GetLabelTitleRect (rdc);
				CRect r2 = r1;
				r1.bottom = r2.top = (LONG) (r1.top + 0.6 * r1.Height ());
				UINT nFormat = style.GetDrawRectAlignment (2) | style.GetDrawRectVAlignment (style.GetTitleAlign (Label));

				pOldFont = style.SelectTitleFont (rdc, Label, m_dTitleFontFactor * 0.6);
				rdc.m_pDC->DrawText (m_strTitle, &r1, nFormat);
				pOldFont = rdc.m_pDC->SelectObject (pOldFont);
				if (pOldFont != NULL)
					pOldFont->DeleteObject ();

				pOldFont = style.SelectTitleFont (rdc, Label, m_dTitleFontFactor * 0.4);
				rdc.m_pDC->DrawText (m_strSubtitle, &r2, nFormat);
				pOldFont = rdc.m_pDC->SelectObject (pOldFont);
				if (pOldFont != NULL)
					pOldFont->DeleteObject ();
			}

			m_ptTracksOffset.x = 0;
			m_ptTracksOffset.y = 0;
		}
		break;
	}
}

void CTracksInfo::DrawTitle2(CRenderDC &rdc, RenderType rt, CStyleTemplate &style)
{
	if (m_strRTFTitle[rt].IsEmpty () && !m_strTitle.IsEmpty ())
	{
		// init RTF according to style format
		CString strColors;
		strColors.Format (";\\red%d\\green%d\\blue%d;}",
			GetRValue (style.GetTitleColor (rt)),
			GetGValue (style.GetTitleColor (rt)),
			GetBValue (style.GetTitleColor (rt)));

		CString strAlignment;
		switch (style.GetTitleAlign (Cover))
		{
		case 0:	// left
			strAlignment = "\\ql"; break;
		case 1:	// right
			strAlignment = "\\qr"; break;
		case 2:	// center
			strAlignment = "\\qc"; break;
		}

		CString strFontSize, strFontSizeSub;
		strFontSize.Format ("\\fs%d ", style.GetTitleFont (rt)->lfHeight * 2);
		strFontSizeSub.Format ("\\fs%d ", (int) (style.GetTitleFont (rt)->lfHeight * 1.2));

		m_strRTFTitle[rt] = 
			"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1031{\\fonttbl{\\f0\\fscript\\fprq2\\fcharset0 " +
			style.GetTitleFontName (rt) + ";}}"
			"{\\colortbl " + strColors +
			"\\viewkind4\\uc1\\pard" + strAlignment + "\\cf1"
			"\\f0" + strFontSize + m_strTitle + "\\par" +
			(m_strSubtitle.IsEmpty () ? CString ("") : CString ("\\f0" + strFontSizeSub + m_strSubtitle + "\\par")) +
			"}";
		m_strRTFTitle[rt].Replace (" · ", " \\par ");
	}

	if (style.GetMediumType () != MediumSlimCase || rt != Inlay)
		CRTFTitles::DrawTitle2 (rdc, rt, style, m_ptTracksOffset);

	if (rt == Label && style.IsLabelTitleRound ())
		DrawTitle (rdc, rt, style);
}

void CTracksInfo::DrawSideBars(CRenderDC &rdc, RenderType rt, CStyleTemplate &style)
{
	if ((rt == Inlay) && (style.GetInlayDisplaySidebarTitles ()))
	{
		// draw left side
		CFont* pOldFont = style.SelectInlaySideFont (rdc, true, m_dSideFontFactor);

		if (style.GetInlaySideTextDirection ())
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strTitle).cx);

			CRect* pRect = style.GetInlayLeftSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->left, pRect->bottom - nTop, ETO_CLIPPED, NULL, m_strTitle, NULL);
		}
		else
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strTitle).cx);

			CRect* pRect = style.GetInlayLeftSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->right, nTop, ETO_CLIPPED, NULL, m_strTitle, NULL);
		}

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();

		// draw right side
		pOldFont = style.SelectInlaySideFont (rdc, false, m_dSideFontFactor);

		if ((style.GetInlaySideTextDirection () && !style.IsInlaySideAntiparallel ()) || (!style.GetInlaySideTextDirection () && style.IsInlaySideAntiparallel ()))
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strTitle).cx);

			CRect* pRect = style.GetInlayRightSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->left, pRect->bottom - nTop, ETO_CLIPPED, NULL, m_strTitle, NULL);
		}
		else
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strTitle).cx);

			CRect* pRect = style.GetInlayRightSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->right, nTop, ETO_CLIPPED, NULL, m_strTitle, NULL);
		}

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
	}
/*
	if ((rt == Inlay) && (style.GetInlayDisplaySidebarTitles ()))
	{
		// draw sides
		CFont* pOldFont = style.SelectInlaySideFont (rdc, m_dSideFontFactor);

		if (style.GetInlaySideTextDirection ())
		{
			CRect* pRect = style.GetInlayLeftSideRect (rdc);
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strTitle).cx);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->left, pRect->bottom - nTop, ETO_CLIPPED, NULL, m_strTitle, NULL);

			pRect = style.GetInlayRightSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->left, pRect->bottom - nTop, ETO_CLIPPED, NULL, m_strTitle, NULL);
		}
		else
		{
			CRect* pRect = style.GetInlayLeftSideRect (rdc);
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strTitle).cx);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->right, nTop, ETO_CLIPPED, NULL, m_strTitle, NULL);

			pRect = style.GetInlayRightSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->right, nTop, ETO_CLIPPED, NULL, m_strTitle, NULL);
		}

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
	}
*/
}

// renders tracks for inlay and back
void CTracksInfo::DrawInlayTrack(CTrack* pTrack, int nIndex, CRenderDC &rdc, CStyleTemplate &style)
{
	CFont* pOldFont;

	if (pTrack->IsTrackGrouping ())
	{
		// track grouping
		if (style.GetTrackTitleDisplay ())
		{
			pOldFont = style.SelectTrackGroupingTitleFont (rdc, m_dTrackFontFactor);
			rdc.m_pDC->DrawText (pTrack->GetTitle (), GetInlayTrackRect (nIndex, 2, rdc, style),
				GetTrackRectAlignment (style, Inlay, TrackTitle));
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();
		}

		if (style.GetTrackRemarkDisplay ())
		{
			pOldFont = style.SelectTrackGroupingRemarksFont (rdc, m_dTrackFontFactor);
			rdc.m_pDC->DrawText (pTrack->GetRemarks (), GetInlayTrackRect (nIndex, 3, rdc, style),
				GetTrackRectAlignment (style, Inlay, TrackRemark));
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();
		}
	}
	else
	{
		// track
		if (style.GetTrackNumDisplay ())
		{
			pOldFont = style.SelectTrackNumFont (rdc, m_dTrackFontFactor);
			rdc.m_pDC->DrawText (CFormatter::Format (style.GetTrackNumFormatting (),
				pTrack->GetTrackNum ()),
				GetInlayTrackRect (nIndex, 0, rdc, style),
				GetTrackRectAlignment (style, Inlay, TrackNum));
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();
		}

		if (style.GetTrackLenDisplay ())
		{
			pOldFont = style.SelectTrackLengthFont (rdc, m_dTrackFontFactor);
			int nSecs = pTrack->GetLengthSec ();
			rdc.m_pDC->DrawText (CFormatter::Format (style.GetTrackLenFormatting (),
				pTrack->GetLengthMin () + nSecs / 60, nSecs % 60),
				GetInlayTrackRect (nIndex, 1, rdc, style),
				GetTrackRectAlignment (style, Inlay, TrackLen));
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();
		}

		if (style.GetTrackTitleDisplay ())
		{
			pOldFont = style.SelectTrackTitleFont (rdc, m_dTrackFontFactor);
			rdc.m_pDC->DrawText (pTrack->GetTitle (), GetInlayTrackRect (nIndex, 2, rdc, style),
				GetTrackRectAlignment (style, Inlay, TrackTitle));
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();		
		}

		if (style.GetTrackRemarkDisplay ())
		{
			pOldFont = style.SelectTrackRemarksFont (rdc, m_dTrackFontFactor);
			rdc.m_pDC->DrawText (pTrack->GetRemarks (), GetInlayTrackRect (nIndex, 3, rdc, style),
				GetTrackRectAlignment (style, Inlay, TrackRemark));
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();		
		}
	}
}

void CTracksInfo::DrawLabelTracks(CRenderDC &rdc, CStyleTemplate &style)
{
	CTrack* pT;
	enum
	{
		none,
		TrackFont,
		TrackGrpFont
	} eCurFont = none;

	CRect r = *style.GetLabelTrackRect (rdc);
	if (style.GetLabelTrackVAlign () == 2)	// centered
	{
		// compute height of tracks
		int nDispTracksCnt = 0;
		for (int i = 0; i < m_arrTrack.GetSize (); i++)
		{
			pT = (CTrack*) m_arrTrack.GetAt (i);
			if (pT->IsTrackGrouping ())
			{
				if (style.GetDisplayTrackGroups (Label))
					nDispTracksCnt++;
			}
			else
			{
				if (style.IsLabelTrackDisplayed ())
					nDispTracksCnt++;
			}
		}

		if (style.IsLabelTrackRound ())
		{
			int nOffs = (style.GetLabelTrackRectStraight (rdc)->Height () - nDispTracksCnt * m_nDeltaY) / 2;
			r.DeflateRect (nOffs, nOffs);
		}
		else
		{
			if (style.GetLabelTrackPosition () == 0)
				// above hole - move down
				r.OffsetRect (0, (r.Height () - nDispTracksCnt * m_nDeltaY) / 2);
			else
				// below hole - move up
				r.OffsetRect (0, -(r.Height () - nDispTracksCnt * m_nDeltaY) / 2);
		}
	}

	CString s;

	// get end angle for round track texts
	bool bFlip = style.GetLabelTrackVAlign () == 0 ||
		(style.GetLabelTrackVAlign () == 2 && style.GetLabelTrackPosition () == 0);

	INT_PTR nNumTracks = m_arrTrack.GetSize ();

	// if the tracks are bottom aligned,
	// draw them from bottom to top (for not having to
	// calculate the top margin)
	bool bAlignBottom = (style.GetLabelTrackVAlign () == 1) ||
		((style.GetLabelTrackVAlign () == 2) && (style.GetLabelTrackPosition () == 1));

	CFont* pOldFont = NULL;

	for (int i = 0; i < nNumTracks; i++)
	{
		pT = (CTrack*) m_arrTrack.GetAt (bAlignBottom ? nNumTracks - i - 1 : i);
		if (pT->IsTrackGrouping ())
		{
			if (style.GetDisplayTrackGroups (Label))
			{
				// select track grouping font, if not yet selected
				if (eCurFont != TrackGrpFont)
				{
					if (pOldFont != NULL)
					{
						pOldFont = rdc.m_pDC->SelectObject (pOldFont);
						if (pOldFont != NULL)
							pOldFont->DeleteObject ();
					}

					pOldFont = style.SelectTrackGroupingTitleFont (rdc, m_dTrackFontFactor);
					eCurFont = TrackGrpFont;
				}

				if (style.IsLabelTrackRound ())
				{
					CTextRenderer::DrawRoundText (rdc.m_pDC, rdc.m_hndPrinter, r, !bFlip, pT->GetTitle (), PI);
					r.DeflateRect (m_nDeltaY, m_nDeltaY, m_nDeltaY, m_nDeltaY);
				}
				else
				{
					rdc.m_pDC->DrawText (pT->GetTitle (), &r,
						style.GetDrawRectAlignment (2) | (bAlignBottom ? DT_BOTTOM : DT_TOP));
					r.OffsetRect (0, bAlignBottom ? -m_nDeltaY : m_nDeltaY);
				}
			}
		}
		else
		{
			if (style.IsLabelTrackDisplayed ())
			{
				// select track font, if not yet selected
				if (eCurFont != TrackFont)
				{
					if (pOldFont != NULL)
					{
						pOldFont = rdc.m_pDC->SelectObject (pOldFont);
						if (pOldFont != NULL)
							pOldFont->DeleteObject ();
					}

					pOldFont = style.SelectTrackTitleFont (rdc, m_dTrackFontFactor);
					eCurFont = TrackFont;
				}

				s = (style.GetTrackNumDisplay () ?
					CFormatter::Format (style.GetTrackNumFormatting (),	pT->GetTrackNum ()) + " " : "") +
					pT->GetTitle ();

				// draw text
				if (style.IsLabelTrackRound ())
				{
					CTextRenderer::DrawRoundText (rdc.m_pDC, rdc.m_hndPrinter, r, !bFlip, s, PI);
					r.DeflateRect (m_nDeltaY, m_nDeltaY, m_nDeltaY, m_nDeltaY);
				}
				else
				{
					rdc.m_pDC->DrawText (s, &r,
						style.GetDrawRectAlignment (2) | (bAlignBottom ? DT_BOTTOM : DT_TOP));
					r.OffsetRect (0, bAlignBottom ? -m_nDeltaY : m_nDeltaY);
				}
			}
		}
	}

	if (pOldFont != NULL)
	{
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
	}
}

void CTracksInfo::DrawTotalTime(int nIndex, CRenderDC &rdc, RenderType rt, CStyleTemplate &style)
{
	if (m_arrTrack.GetSize () == 0)
		return;

	if ((rt == Back || rt == Inlay) && style.GetTrackTotalTime ())
	{
		CFont* pOldFont = style.SelectTrackLengthFont (rdc, m_dTrackFontFactor);
		rdc.m_pDC->DrawText (CalcTotalTime (style), GetInlayTrackRect (nIndex, 1, rdc, style),
			style.GetDrawRectAlignment (style.GetTrackLenHAlign ()));
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
	}
}

void CTracksInfo::Render(CRenderDC& rdc, RenderType rt, CStyleTemplate &style)
{
	// changed flags
	m_bSizeChanged = m_nOldWidth != rdc.GetWidth (rt);
	m_bRtChanged = rt != m_rtOldRt;
	m_nOldWidth = rdc.GetWidth (rt);
	m_rtOldRt = rt;

	CalcTextRects (rdc, rt, style);
	CalcTitleRects2 (rdc, rt, style);

	CFont* pOldFont = NULL;

	// draw title and side bars
	DrawTitle2 (rdc, rt, style);
	DrawSideBars (rdc, rt, style);

	// draw tracks
	CTrack* pT;
	int i;
	CString s;

	switch (rt)
	{
	case Inlay:
	case Back:
		// draw tracks

		// don't draw tracks on Inlay of slim case
		if (style.GetMediumType () != MediumSlimCase || rt != Inlay)
		{
			for (i = 0; i < m_arrTrack.GetSize (); i++)
				DrawInlayTrack ((CTrack*) m_arrTrack.GetAt (i), i, rdc, style);

			// draw total time
			DrawTotalTime (i + 1, rdc, rt, style);
		}
		break;

	case Cover:
		// build display string
		for (i = 0; i < m_arrTrack.GetSize (); i++)
		{
			pT = (CTrack*) m_arrTrack.GetAt (i);
			if (style.GetDisplayTrackGroups (Cover) && pT->IsTrackGrouping ())
				s += pT->GetTitle () + "\n";
		}
		// draw the text
		pOldFont = style.SelectTrackGroupingTitleFont (rdc, m_dTrackFontFactor);
		{
			CRect r = *style.GetCoverTrackRect (rdc);
			CRect r1 = r;
			rdc.m_pDC->DrawText (s, &r1,
				(style.GetDrawRectAlignment (style.GetTrackgroupHAlign (Cover)) | DT_CALCRECT) & ~DT_SINGLELINE);
			switch (style.GetTrackgroupVAlign (Cover))
			{
			case 0:	// top
				break;
			case 1:	// bottom
				r.top += r.Height () - r1.Height ();
				break;
			case 2:	// centered
				r.top += (r.Height () - r1.Height ()) / 2;
				break;
			}
			rdc.m_pDC->DrawText (s, &r,
				style.GetDrawRectAlignment (style.GetTrackgroupHAlign (Cover)) & ~DT_SINGLELINE);
		}
		break;

	case Label:
		DrawLabelTracks (rdc, style);
		break;
	}

	if (pOldFont != NULL)
	{
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
	}
}

CSize& CTracksInfo::Max (CSize s1, CSize s2)
{
	m_szMax = CSize (max (s1.cx, s2.cx), max (s1.cy, s2.cy));
	return m_szMax;
}

void CTracksInfo::CalcTextRects(CRenderDC &rdc, RenderType rt, CStyleTemplate& style)
{
	// check whether anything has changed.
	// if not, do not re-calculate
	if ((m_styleCurrent == style) && !m_bRtChanged && !m_bSizeChanged)
		return;

	m_styleCurrent = style;

	// calculate track text rectangles

	CSize szMaxNum = 0, szMaxLen = 0, szMaxTitle = 0, szMaxRem = 0;
	CTrack* pT;

	int nInterItemSpacingCount = -1;

	// calc max of nums
	CFont* pOldFont = style.SelectTrackNumFont (rdc);
	if (style.GetTrackNumDisplay ())
	{
		nInterItemSpacingCount++;

		for (int i = 0; i < m_arrTrack.GetSize (); i++)
		{
			pT = (CTrack*) m_arrTrack.GetAt (i);
			if (!pT->IsTrackGrouping ())
				szMaxNum = Max (szMaxNum, rdc.m_pDC->GetTextExtent (
					CFormatter::Format (style.GetTrackNumFormatting (), pT->GetTrackNum ())));
		}
	}
	pOldFont = rdc.m_pDC->SelectObject (pOldFont);
	if (pOldFont != NULL)
		pOldFont->DeleteObject ();

	// calc max of length
	if (style.GetTrackLenDisplay ())
	{
		nInterItemSpacingCount++;

		pOldFont = style.SelectTrackLengthFont (rdc);
		int nSecs;
		for (int i = 0; i < m_arrTrack.GetSize (); i++)
		{
			pT = (CTrack*) m_arrTrack.GetAt (i);
			if (!pT->IsTrackGrouping ())
			{
				nSecs = pT->GetLengthSec ();
				szMaxLen = Max (szMaxLen, rdc.m_pDC->GetTextExtent (
					CFormatter::Format (style.GetTrackLenFormatting (),
					pT->GetLengthMin () + nSecs / 60, nSecs % 60)));
			}
		}
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
	}

	// calc max of titles
	if (style.GetTrackTitleDisplay ())
	{
		nInterItemSpacingCount++;

		pOldFont = style.SelectTrackTitleFont (rdc);
		for (int i = 0; i < m_arrTrack.GetSize (); i++)
		{
			pT = (CTrack*) m_arrTrack.GetAt (i);
			if (!pT->IsTrackGrouping ())
				szMaxTitle = Max (szMaxTitle, rdc.m_pDC->GetTextExtent (pT->GetTitle ()));
		}
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();

		pOldFont = style.SelectTrackGroupingTitleFont (rdc);
		for (int i = 0; i < m_arrTrack.GetSize (); i++)
		{
			pT = (CTrack*) m_arrTrack.GetAt (i);
			if (pT->IsTrackGrouping ())
				szMaxTitle = Max (szMaxTitle, rdc.m_pDC->GetTextExtent (pT->GetTitle ()));
		}
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
	}

	// calc max of remarks
	if (style.GetTrackRemarkDisplay ())
	{
		nInterItemSpacingCount++;

		pOldFont = style.SelectTrackRemarksFont (rdc);
		for (int i = 0; i < m_arrTrack.GetSize (); i++)
		{
			pT = (CTrack*) m_arrTrack.GetAt (i);
			if (!pT->IsTrackGrouping ())
				szMaxRem = Max (szMaxRem, rdc.m_pDC->GetTextExtent (pT->GetRemarks ()));
		}
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();

		pOldFont = style.SelectTrackGroupingRemarksFont (rdc);
		for (int i = 0; i < m_arrTrack.GetSize (); i++)
		{
			pT = (CTrack*) m_arrTrack.GetAt (i);
			if (pT->IsTrackGrouping ())
				szMaxRem = Max (szMaxRem, rdc.m_pDC->GetTextExtent (pT->GetRemarks ()));
		}
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
	}

	// set member variables

	// compute track cell positions
	int nPosX[NUM_TRACK_TYPES];
	nPosX[TrackNum] = szMaxNum.cx;
	nPosX[TrackLen] = szMaxLen.cx;
	nPosX[TrackTitle] = szMaxTitle.cx;
	nPosX[TrackRemark] = szMaxRem.cx;

	for (int i = 0; i < NUM_TRACK_TYPES + 1; i++)
		m_nPosX[i] = 0;
	for (int i = 0; i < NUM_TRACK_TYPES; i++)
		for (int j = 0; j < NUM_TRACK_TYPES; j++)
			m_nPosX[j + 1] += (style.GetTrackPositionById (i) <= style.GetTrackPositionById (j)) ? nPosX[i] : 0;
	m_nPosX[NUM_TRACK_TYPES + 1] = rdc.GetWidth (rt);

	m_nDeltaY = max (szMaxNum.cy, max (szMaxLen.cy, max (szMaxTitle.cy, szMaxRem.cy)));

	// track font factor
	CRect* pRect;
	switch (rt)
	{
	case Cover:
		pRect = style.GetCoverTrackRect (rdc);
		break;
	case Back:
		pRect = style.GetBackTrackRect (rdc);
		break;
	case Inlay:
		pRect = style.GetInlayTrackRect (rdc);
		break;
	case Label:
		pRect = style.GetLabelTrackRectStraight (rdc);
		break;
	}

	int nMarginLeft = pRect->left;
	m_dTrackFontFactor = min (
		pRect->Height () / (double) (m_nDeltaY * (m_arrTrack.GetSize () + (style.GetTrackTotalTime () ? 2 : 0))),
		pRect->Width () / (double) (sum (NUM_TRACK_TYPES, nPosX) + style.GetTrackInterItemSpacing (rdc, rt) * nInterItemSpacingCount));

	m_nDeltaY = (int) (m_nDeltaY * m_dTrackFontFactor);

	if (rt == Cover)
	{
		// count track groupings
		int nNumTrackGroupings = 0;
		for (int i = 0; i < m_arrTrack.GetSize (); i++)
			if (((CTrack*) m_arrTrack.GetAt (i))->IsTrackGrouping ())
				nNumTrackGroupings++;

		// calculate y offset for track groupings on cover
		switch (style.GetTrackgroupVAlign (Cover))
		{
		case 0:	// top
			m_nOffsetY = 0;
			break;
		case 1:	// bottom
			m_nOffsetY = style.GetCoverTrackRect (rdc)->Height () - nNumTrackGroupings * m_nDeltaY;
			break;
		case 2:	// centered
			{
				CRect* pR = style.GetCoverTrackRect (rdc);
				m_nOffsetY = (pR->Height () - nNumTrackGroupings * m_nDeltaY) / 2;
			}
			break;
		}
	}

	// adjust track cell width
	for (int i = 0; i < NUM_TRACK_TYPES + 1; i++)
		m_nPosX[i] = (int) (m_nPosX[i] * m_dTrackFontFactor) + nMarginLeft;

	// clean up
	pOldFont = rdc.m_pDC->SelectObject (pOldFont);
	if (pOldFont != NULL)
		pOldFont->DeleteObject ();
}

void CTracksInfo::CalcTitleRects(CRenderDC &rdc, RenderType rt, CStyleTemplate &style)
{
	// check whether anything has changed.
	// if not, do not re-calculate
//	if ((m_styleCurrent == style) && !m_bRtChanged && !m_bSizeChanged)
//		return;

	m_styleCurrent = style;

	CFont* pOldFont;
	CRect* pRect;
	CSize szTitle, szSides;

	// title font factor
	switch (rt)
	{
	case Cover:
	case Back:
		pOldFont = style.SelectTitleFont (rdc, Cover);
		szTitle = rdc.m_pDC->GetTextExtent (m_strTitle);
		pRect = style.GetCoverTitleRect (rdc, rt);
		m_dTitleFontFactor = min (
			pRect->Width () / ((double) (szTitle.cx + 1)),
			pRect->Height () / ((double) (szTitle.cy + 1)));
		m_dSideFontFactor = 0;
		break;

	case Inlay:
		pOldFont = style.SelectTitleFont (rdc, Inlay);
		szTitle = rdc.m_pDC->GetTextExtent (m_strTitle);
		pRect = style.GetInlayTitleRect (rdc);
		m_dTitleFontFactor = min (
			pRect->Width () / ((double) (szTitle.cx + 1)),
			pRect->Height () / ((double) (szTitle.cy + 1)));
		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();

		// sides font factor
		pOldFont = style.SelectInlaySideFont (rdc, true);
		szSides = rdc.m_pDC->GetTextExtent (m_strTitle);
		pRect = style.GetInlayRightSideRect (rdc);
		// since the font is rotated (by 90°) and GetTextExtent does not
		// take into account font rotation, "compute" the rotation explicitely, i.e.
		// cx <- cy, cy <- cx.
		m_dSideFontFactor = min (
			pRect->Width () / ((double) (szSides.cy + 1)),
			pRect->Height () / ((double) (szSides.cx + 1)));

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
		break;

	case Label:
		pOldFont = style.SelectTitleFont (rdc, Label);
		szTitle = rdc.m_pDC->GetTextExtent (m_strTitle);
		pRect = style.GetLabelTitleRect (rdc);

		if (style.IsLabelTitleRound ())
		{
			// reference factor: same as cover title font factor
			/*
			double dRefFactor = min (pRect->Width () / ((double) (szTitle.cx + 1)),
				pRect->Height () / ((double) (szTitle.cy + 1)));
			*/

			int y = style.GetTotalLabelRect (rdc)->Height () -
				2 * style.MapY ((int) (style.GetTitleMargin (Label) * 1000), rdc, Label);

/*			// max. text height
			double dRefFactor = fabs ((y - style.GetInnerLabelRect (rdc)->Height ()) / 2.0 / szTitle.cy);

			// ratio length of arc : title text length
			double dMaxRoundFactor = ((y / 2 - szTitle.cy) * 3.14159) / (1.5 * szTitle.cx);

			m_dTitleFontFactor = min (min (dRefFactor, dMaxRoundFactor), y / 100.0);
*/
			m_dTitleFontFactor = y / 160.0;	// y reflects zoom factor. Normal y on a 512x512 approx. 480 (depending on margins), allow magn. factor 3 for that

			//m_dTitleFontFactor = min (dMaxRoundFactor, 2);
		}
		else
		{
			// r^2=x^2+y^2
			// y=height/2 - vmargin - text.cy
			// r=width/2
			// ==> x=sqrt((width/2)^2 - (height/2 - vmargin - text.cy)^2)
			// ==> width=2x

			CRect* pRcTotal = style.GetTotalLabelRect (rdc);

			int y;
			if (style.GetTitleAlign (Label) < 2)	// top, bottom
				y = pRcTotal->Height () / 2 - style.MapY ((int) (style.GetTitleMargin (Label) * 1000), rdc, Label);
			else	// center
				y = pRcTotal->Height () / 3;

			int width = (int) sqrt ((double) (pRcTotal->Width () * pRcTotal->Width () - 4 * y * y));
			m_dTitleFontFactor = min (
				width / ((double) (szTitle.cx + 1)),
				pRect->Height () / ((double) (szTitle.cy + 1)));
			m_dSideFontFactor = 0;
		}

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
		break;
	}

	rdc.m_pDC->SelectObject (pOldFont);
}

void CTracksInfo::CalcTitleRects2(CRenderDC &rdc, RenderType rt, CStyleTemplate &style)
{
	//CalcTitleRects (rdc, rt, style);

	/////////////////////////////////////////////////////////////

	CFont* pOldFont;
	CRect* pRect;
	CSize szTitle, szSides;

	switch (rt)
	{
	case Inlay:
		pRect = style.GetInlayTitleRect (rdc);

		// sides font factor
		pOldFont = style.SelectInlaySideFont (rdc, true);
		szSides = rdc.m_pDC->GetTextExtent (m_strTitle);
		pRect = style.GetInlayRightSideRect (rdc);
		// since the font is rotated (by 90°) and GetTextExtent does not
		// take into account font rotation, "compute" the rotation explicitely, i.e.
		// cx <- cy, cy <- cx.
		m_dSideFontFactor = min (
			pRect->Width () / ((double) (szSides.cy + 1)),
			pRect->Height () / ((double) (szSides.cx + 1)));

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
		break;

	case Label:
		CalcTitleRects (rdc, rt, style);
		break;

	default:
		break;
	}
}

CRect* CTracksInfo::GetInlayTrackRect(int nRow, int nIdx, CRenderDC& rdc, CStyleTemplate& style)
{
	m_rect.right = m_nPosX[nIdx + 1];
	m_rect.left = 0;

	for (int i = 0; i <= 5; i++)
		if (m_nPosX[i] < m_rect.right)
			m_rect.left = max (m_rect.left, m_nPosX[i]);

	m_rect.top = m_nDeltaY * nRow;
	m_rect.bottom = m_nDeltaY * (nRow + 1);

	m_rect.OffsetRect (style.GetTrackInterItemSpacing (rdc, Inlay) * style.GetTrackPositionById (nIdx), m_ptTracksOffset.y);

	return &m_rect;
}

UINT CTracksInfo::GetTrackRectAlignment(CStyleTemplate& style, RenderType rt, TrackType tt)
{
	switch (rt)
	{
	case Cover:
		return style.GetDrawRectAlignment (style.GetTrackgroupHAlign (Cover)) |
			style.GetDrawRectVAlignment (style.GetTrackgroupVAlign (Cover));

	case Label:
		if (!style.IsLabelTrackRound ())
			return style.GetDrawRectAlignment (style.GetTrackTitleHAlign ()) | DT_BOTTOM;

	case Back:
	case Inlay:
		switch (tt)
		{
		case TrackNum:
			return style.GetDrawRectAlignment (style.GetTrackNumHAlign ()) | DT_BOTTOM;
		case TrackLen:
			return style.GetDrawRectAlignment (style.GetTrackLenHAlign ()) | DT_BOTTOM;
		case TrackTitle:
			return style.GetDrawRectAlignment (style.GetTrackTitleHAlign ()) | DT_BOTTOM;
		case TrackRemark:
			return style.GetDrawRectAlignment (style.GetTrackRemarkHAlign ()) | DT_BOTTOM;
		}
	}

	return 0;
}

CString CTracksInfo::CalcTotalTime(CStyleTemplate& style)
{
	int nSec = 0;
	CTrack* pT;

	for (int i = 0; i < m_arrTrack.GetSize (); i++)
	{
		pT = (CTrack*) m_arrTrack.GetAt (i);
		nSec += pT->GetLengthMin () * 60 + pT->GetLengthSec ();
	}

	return CFormatter::Format (style.GetTrackLenFormatting (), nSec / 60, nSec % 60);
}

bool CTracksInfo::operator ==(CTracksInfo &ti)
{
	if (m_strTitle != ti.GetTitle ())
		return false;

	if (m_arrTrack.GetSize () != ti.GetNumTracks ())
		return false;
	for (int i = 0; i < m_arrTrack.GetSize (); i++)
		if (*(CTrack*) m_arrTrack.GetAt (i) != ti.GetTrack (i))
			return false;

	return true;
}

bool CTracksInfo::operator !=(CTracksInfo &ti)
{
	return !(*this == ti);
}

void CTracksInfo::Match(CTracksInfo *pMergedTracks, bool bOverwrite)
{
	CDWordArray arrMatch[10];
	int n;
	CTrack* pT, tm;

	INT_PTR nNumTracks = m_arrTrack.GetSize ();

	// build priority list
	for (INT_PTR i = 0; i < nNumTracks; i++)
	{
		pT = (CTrack*) m_arrTrack.GetAt (i);
		if (pT->IsTrackGrouping ())
			continue;
		if (!pT->GetTitle ().IsEmpty () && !bOverwrite)
			continue;

		for (int j = 0; j < pMergedTracks->GetNumTracks (); j++)
		{
			tm = pMergedTracks->GetTrack (j);
			if (tm.IsTrackGrouping ())
				continue;

			n = *pT - tm;
			if (n < 10)
				arrMatch[n].Add (MAKELONG (i, j));
		}
	}

	// distribute tracks
	bool* bMapNew = new bool[nNumTracks];	// flags indicating whether track [i] has been copied
	bool* bMapOld = new bool[pMergedTracks->GetNumTracks ()];
	for (int i = 0; i < nNumTracks; i++)
		bMapNew[i] = false;
	for (int i = 0; i < pMergedTracks->GetNumTracks (); i++)
		bMapOld[i] = false;

	int nTracksCopied = 0;
	DWORD d;

	for (int i = 0; (i < 10) && (nTracksCopied < nNumTracks); i++)
	{
		for (int j = 0; j < arrMatch[i].GetSize (); j++)
		{
			d = arrMatch[i].GetAt (j);

			if (bMapNew[LOWORD (d)])	// track has already been copied
				continue;
			if (bMapOld[HIWORD (d)])	// track has already been used
				continue;

			// copy track and set flag
			tm = pMergedTracks->GetTrack (HIWORD (d));
			((CTrack*) m_arrTrack.GetAt (LOWORD (d)))->SetText (
				tm.GetTitle (), tm.GetRemarks ());

			bMapNew[LOWORD (d)] = true;
			bMapOld[HIWORD (d)] = true;

			if (++nTracksCopied >= nNumTracks)
				break;
		}
	}

	delete bMapNew;
	delete bMapOld;
}

void CTracksInfo::Serialize(CArchive &ar)
{
	// invoke parent implementation
	CAudioCDTracks::Serialize (ar);

	switch (((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema)
	{
	case 1:
		m_bManualEdit = false;
		break;

	case 2:
	case 3:
		{
			WORD wManualEdit = m_bManualEdit ? 1 : 0;

			if (ar.IsLoading ())
			{
				ar >> wManualEdit;
				m_bManualEdit = wManualEdit ? true : false;
			}
			else
			{
				ar << wManualEdit;
			}
		}

		break;
	}
}

bool CTracksInfo::GetTrackRectFromPoint(CPoint pt, CRenderDC& rdc, RenderType rt, CStyleTemplate& style,
										CRect &r, CString &strText, int &nNum, int &nIdx,
										LOGFONT *pLogfont, COLORREF& col, DWORD& dwAlign)
{
	m_ptTracksOffset.x = 0;
	m_ptTracksOffset.y = rt == Inlay ? style.GetInlayTitleRect (rdc)->bottom : style.GetCoverTitleRect (rdc, rt)->bottom;
	CalcTextRects (rdc, rt, style);

	for (int i = 0; i < m_arrTrack.GetSize (); i++)
		for (int j = 0; j < 4; j++)
			if (GetInlayTrackRect (i, j, rdc, style)->PtInRect (pt))
			{
				r = m_rect;
				nNum = i;
				nIdx = j;

				return GetTrackRectFromIdx (nNum, nIdx, rdc, rt, style, r, strText, pLogfont, col, dwAlign);
			}

	nIdx = -1;
	nNum = -1;
	return false;
}


bool CTracksInfo::SetTrackText(CString strText, int nNum, int nIdx)
{
	bool bModified = false;

	switch (nIdx)
	{
	case 0:	// Num
		bModified = ((CTrack*) m_arrTrack.GetAt (nNum))->GetTrackNum () != strText;
		((CTrack*) m_arrTrack.GetAt (nNum))->SetTrackNum (strText);
		break;
	case 1:	// Len
		bModified = ((CTrack*) m_arrTrack.GetAt (nNum))->GetLength () != strText;
		((CTrack*) m_arrTrack.GetAt (nNum))->SetLength (strText);
		break;
	case 2:	// Title
		bModified = ((CTrack*) m_arrTrack.GetAt (nNum))->GetTitle () != strText;
		((CTrack*) m_arrTrack.GetAt (nNum))->SetTitle (strText);
		break;
	case 3:	// Remarks
		bModified = ((CTrack*) m_arrTrack.GetAt (nNum))->GetRemarks () != strText;
		((CTrack*) m_arrTrack.GetAt (nNum))->SetRemarks (strText);
		break;
	}

	return bModified;
}

bool CTracksInfo::GetTrackRectFromIdx(int nNum, int nIdx, CRenderDC &rdc, RenderType rt,
									  CStyleTemplate &style, CRect &r, CString &strText,
									  LOGFONT *pLogfont, COLORREF &color, DWORD &dwAlign)
{
	CTrack* pTrack = (CTrack*) m_arrTrack.GetAt (nNum);

	if (r.left == -1 && r.top == -1 && r.right == -1 && r.bottom == -1)
	{
		m_ptTracksOffset.x = 0;
		m_ptTracksOffset.y = rt == Inlay ? style.GetInlayTitleRect (rdc)->bottom : style.GetCoverTitleRect (rdc, rt)->bottom;
		CalcTextRects (rdc, rt, style);
	}

	switch (nIdx)
	{
	case 0:	// Num
		strText = CFormatter::Format (
			style.GetTrackNumFormatting (),	pTrack->GetTrackNum ());
		memcpy (pLogfont, style.GetTrackNumFont (), sizeof (LOGFONT));
		color = style.GetTrackNumColor ();
		dwAlign = GetTrackRectAlignment (style, Inlay, TrackNum);
		break;
	case 1:	// Len
		{
			int nSecs = pTrack->GetLengthSec ();
			strText = CFormatter::Format (
				style.GetTrackLenFormatting (),	pTrack->GetLengthMin () + nSecs / 60, nSecs % 60);
			memcpy (pLogfont, style.GetTrackLengthFont (), sizeof (LOGFONT));
			color = style.GetTrackLengthColor ();
			dwAlign = GetTrackRectAlignment (style, Inlay, TrackLen);
		}
		break;
	case 2:	// Title
		strText = pTrack->GetTitle ();
		memcpy (pLogfont, pTrack->IsTrackGrouping () ? style.GetTrackGroupingTitleFont () : style.GetTrackTitleFont (), sizeof (LOGFONT));
		color = pTrack->IsTrackGrouping () ? style.GetTrackGroupingTitleColor () : style.GetTrackTitleColor ();
		dwAlign = GetTrackRectAlignment (style, Inlay, TrackTitle);
		break;
	case 3:	// Remarks
		strText = pTrack->GetRemarks ();
		memcpy (pLogfont, pTrack->IsTrackGrouping () ? style.GetTrackGroupingRemarksFont () : style.GetTrackRemarksFont (), sizeof (LOGFONT));
		color = pTrack->IsTrackGrouping () ? style.GetTrackGroupingRemarksColor () : style.GetTrackRemarksColor ();
		dwAlign = GetTrackRectAlignment (style, Inlay, TrackRemark);
		break;
	}
	
	pLogfont->lfHeight = (int) (pLogfont->lfHeight * m_dTrackFontFactor);
	if (pLogfont->lfHeight == 0)
		pLogfont->lfHeight = 1;

	if (r.left == -1 && r.top == -1 && r.right == -1 && r.bottom == -1)
		r = *GetInlayTrackRect (nNum, nIdx, rdc, style);

	return true;
}