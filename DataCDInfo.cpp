#include "StdAfx.h"
#include "CdCoverCreator2.h"

#include "datacdinfo.h"

#include "TextRenderer.h"
#include <math.h>

IMPLEMENT_SERIAL (CDataCDInfo, CObject, VERSIONABLE_SCHEMA | 1)


CDataCDInfo::CDataCDInfo (void)
{
	m_pDirTree = NULL;
}

CDataCDInfo::CDataCDInfo (CDataCDInfo& info)
{
	m_pDirTree = NULL;
	*this = info;
}

CDataCDInfo::~CDataCDInfo(void)
{
	if (m_pDirTree != NULL)
		delete m_pDirTree;
}

void CDataCDInfo::Render (CRenderDC& rdc, RenderType rt, CStyleTemplate& style)
{
	if (m_pDirTree == NULL)
		return;

	// render the title
	CPoint pt;

	if (rt != Inlay || style.GetMediumType () != MediumSlimCase)	// draw the title on inlays only if this is no slim case
		CRTFTitles::DrawTitle2 (rdc, rt, style, pt);

	if (rt == Label)
	{
		CalcLabelTitleRect (rdc, style);
		DrawLabelTitle (rdc, style);
	}

	if (rt == Inlay)
	{
		CalcInlaySideBarRect (rdc, style);
		DrawSideBars (rdc, style);
	}

	// render the directory tree

	CRect r;

	switch (rt)
	{
//	case Cover:
//		r = *(style.GetCoverTrackRect (rdc));
//		break;
	case Back:
		r = *(style.GetBackTrackRect (rdc));
		break;
	case Inlay:
		r = *(style.GetInlayTrackRect (rdc));
		break;
//	case Label:
//		r = *(style.GetLabelTrackRect (rdc));
//		break;
	}

	if (style.GetMediumType () == MediumSlimCase && rt == Inlay)
		return;		// don't draw stuff on Inlay of slim case

	if (style.GetDisplayTrackGroups (rt) && (rt != Label) && (rt != Cover))	// don't display on label and on cover
		m_pDirTree->Render (rdc, r, style, rdc.m_hndPrinter != NULL);
}

void CDataCDInfo::DrawLabelTitle (CRenderDC& rdc, CStyleTemplate& style)
{
	if (style.GetDisplayTitles (Label))
	{
		if (style.IsLabelTitleRound ())
		{
			bool bFlip = style.GetTitleAlign (Label) == 0 ||
				(style.GetTitleAlign (Label) == 2 && style.GetLabelTitlePosition () == 0);

			CRect r = *style.GetLabelTitleRect (rdc);
			CFont* pOldFont = style.SelectTitleFont (rdc, Label, m_dTitleFontFactor);
			int nHeight = rdc.m_pDC->GetTextExtent ("X").cy;

			// split lines
			int nPos;
			for (int nOldPos = 0; ; )
			{
				nPos = m_strCDTitle.Find ('\n', nOldPos);
				if (nPos != -1)
				{
					CTextRenderer::DrawRoundText (rdc.m_pDC, rdc.m_hndPrinter, r, !bFlip,
						m_strCDTitle.Mid (nOldPos, nPos - nOldPos - 1),
						style.GetDisplayTrackGroups (Label) || style.IsLabelTrackDisplayed () ? PI : 2 * PI - 0.2);
					r.DeflateRect (nHeight, nHeight);
				}
				else
				{
					CTextRenderer::DrawRoundText (rdc.m_pDC, rdc.m_hndPrinter, r, !bFlip,
						m_strCDTitle.Mid (nOldPos),
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

			CFont* pOldFont = style.SelectTitleFont (rdc, Label, m_dTitleFontFactor);
			rdc.m_pDC->DrawText (m_strCDTitle, &r1, nFormat);
			pOldFont = rdc.m_pDC->SelectObject (pOldFont);
			if (pOldFont != NULL)
				pOldFont->DeleteObject ();
		}
	}
}

void CDataCDInfo::CalcLabelTitleRect (CRenderDC& rdc, CStyleTemplate& style)
{
	CFont* pOldFont = style.SelectTitleFont (rdc, Label);
	CSize szTitle = rdc.m_pDC->GetTextExtent (m_strCDTitle);
	CRect* pRect = style.GetLabelTitleRect (rdc);

	if (style.IsLabelTitleRound ())
	{
		int y = style.GetTotalLabelRect (rdc)->Height () -
			2 * style.MapY ((int) (style.GetTitleMargin (Label) * 1000), rdc, Label);
		m_dTitleFontFactor = y / 160.0;	// y reflects zoom factor. Normal y on a 512x512 approx. 480 (depending on margins), allow magn. factor 3 for that
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
	}
}

void CDataCDInfo::DrawSideBars(CRenderDC &rdc, CStyleTemplate &style)
{
	if (style.GetInlayDisplaySidebarTitles ())
	{
		// draw left side
		CFont* pOldFont = style.SelectInlaySideFont (rdc, true, m_dSideFontFactor);

		if (style.GetInlaySideTextDirection ())
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strCDTitle).cx);

			CRect* pRect = style.GetInlayLeftSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->left, pRect->bottom - nTop, ETO_CLIPPED, NULL, m_strCDTitle, NULL);
		}
		else
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strCDTitle).cx);

			CRect* pRect = style.GetInlayLeftSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->right, nTop, ETO_CLIPPED, NULL, m_strCDTitle, NULL);
		}

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();

		// draw right side
		pOldFont = style.SelectInlaySideFont (rdc, false, m_dSideFontFactor);

		if ((style.GetInlaySideTextDirection () && !style.IsInlaySideAntiparallel ()) || (!style.GetInlaySideTextDirection () && style.IsInlaySideAntiparallel ()))
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strCDTitle).cx);

			CRect* pRect = style.GetInlayRightSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->left, pRect->bottom - nTop, ETO_CLIPPED, NULL, m_strCDTitle, NULL);
		}
		else
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strCDTitle).cx);

			CRect* pRect = style.GetInlayRightSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->right, nTop, ETO_CLIPPED, NULL, m_strCDTitle, NULL);
		}

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();

/*
		// draw sides
		CFont* pOldFont = style.SelectInlaySideFont (rdc, m_dSideFontFactor);

		if (style.GetInlaySideTextDirection ())
		{
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strCDTitle).cx);

			CRect* pRect = style.GetInlayLeftSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->left, pRect->bottom - nTop, ETO_CLIPPED, NULL, m_strCDTitle, NULL);

			pRect = style.GetInlayRightSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->left, pRect->bottom - nTop, ETO_CLIPPED, NULL, m_strCDTitle, NULL);
		}
		else
		{
			CRect* pRect = style.GetInlayLeftSideRect (rdc);
			int nTop = style.GetInlaySideVAlignTop (rdc, rdc.m_pDC->GetTextExtent (m_strCDTitle).cx);

			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->right, nTop, ETO_CLIPPED, NULL, m_strCDTitle, NULL);
			
			pRect = style.GetInlayRightSideRect (rdc);
			if (pRect->Width () > 0)
				rdc.m_pDC->ExtTextOut (pRect->right, nTop, ETO_CLIPPED, NULL, m_strCDTitle, NULL);
		}

		pOldFont = rdc.m_pDC->SelectObject (pOldFont);
		if (pOldFont != NULL)
			pOldFont->DeleteObject ();
*/
	}
}

void CDataCDInfo::CalcInlaySideBarRect (CRenderDC& rdc, CStyleTemplate& style)
{
	// sides font factor
	CFont* pOldFont = style.SelectInlaySideFont (rdc, true);
	CSize szSides = rdc.m_pDC->GetTextExtent (m_strCDTitle);
	CRect* pRect = style.GetInlayRightSideRect (rdc);
	// since the font is rotated (by 90°) and GetTextExtent does not
	// take into account font rotation, "compute" the rotation explicitely, i.e.
	// cx <- cy, cy <- cx.
	m_dSideFontFactor = min (
		pRect->Width () / ((double) (szSides.cy + 1)),
		pRect->Height () / ((double) (szSides.cx + 1)));

	pOldFont = rdc.m_pDC->SelectObject (pOldFont);
	if (pOldFont != NULL)
		pOldFont->DeleteObject ();
}

void CDataCDInfo::Serialize (CArchive& ar)
{
	if (ar.IsLoading ())
	{
		ar >> m_strCDTitle;
		for (int i = 0; i < NUM_RENDER_TYPES; i++)
			ar >> m_strRTFTitle[i];
	}
	else
	{
		ar << m_strCDTitle;
		for (int i = 0; i < NUM_RENDER_TYPES; i++)
			ar << m_strRTFTitle[i];
	}

	if (m_pDirTree == NULL)
		m_pDirTree = new CDirTree ();
	m_pDirTree->Serialize (ar);
}

CDataCDInfo& CDataCDInfo::operator = (CDataCDInfo& info)
{
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		m_strRTFTitle[i] = info.GetRTFTitle ((RenderType) i);

	m_strCDTitle = info.m_strCDTitle;

	if (m_pDirTree == NULL)
		m_pDirTree = new CDirTree ();
	*m_pDirTree = *(info.m_pDirTree);

	return *this;
}

bool CDataCDInfo::operator == (CDataCDInfo& info)
{
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		if (m_strRTFTitle[i] != info.GetRTFTitle ((RenderType) i))
			return false;

	if (m_strCDTitle != info.m_strCDTitle)
		return false;

	if (*m_pDirTree != *(info.m_pDirTree))
		return false;

	return true;
}

bool CDataCDInfo::operator != (CDataCDInfo& info)
{
	return !(*this == info);
}