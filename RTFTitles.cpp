#include "StdAfx.h"
#include "rtftitles.h"
#include "FormattedTextDraw.h"

CRTFTitles::CRTFTitles(void)
{
}

CRTFTitles::~CRTFTitles(void)
{
}

void CRTFTitles::ApplyStyleToRTFTitles(CStyleTemplate *pStyle, RenderType rt)
{
	int nEnd = (rt == -1) ? NUM_RENDER_TYPES : rt + 1;
	for (int i = rt == -1 ? 0 : rt; i < nEnd; i++)
	{
		// colors
		//////////////////////////////////////////////////////////////

		int nPos = m_strRTFTitle[i].Find ("\\colortbl");
		if (nPos == -1)
		{
			nPos = m_strRTFTitle[i].Find ("\\fonttbl");
			if (nPos == -1)
				continue;

			nPos = m_strRTFTitle[i].Find ("}}", nPos);
			m_strRTFTitle[i].Insert (nPos + 2, "{\\colortbl }");
			nPos += 3;
		}

		CString str;
		str.Format (";\\red%d\\green%d\\blue%d",
			GetRValue (pStyle->GetTitleColor ((RenderType) i)),
			GetGValue (pStyle->GetTitleColor ((RenderType) i)),
			GetBValue (pStyle->GetTitleColor ((RenderType) i)));
		m_strRTFTitle[i].Insert (nPos + 10, str);

		// alignment
		//////////////////////////////////////////////////////////////

		nPos = m_strRTFTitle[i].Find ("\\pard");
		if (nPos > -1)
		{
			// delete any alignment that is already there
			char c = m_strRTFTitle[i].GetAt (nPos + 7);
			if (m_strRTFTitle[i].GetAt (nPos + 5) == '\\' &&
				m_strRTFTitle[i].GetAt (nPos + 6) == 'q' &&
				(c == 'l' || c == 'r' || c == 'c'))
			{
				m_strRTFTitle[i].Delete (nPos + 5, 3);
			}

			if ((RenderType) i == Label)
				m_strRTFTitle[i].Insert (nPos + 5, "\\qc");	// always center label titles!
			else
			{
				switch (pStyle->GetTitleAlign ((RenderType) i))
				{
				case 0:	// left
					m_strRTFTitle[i].Insert (nPos + 5, "\\ql"); break;
				case 1:	// right
					m_strRTFTitle[i].Insert (nPos + 5, "\\qr"); break;
				case 2:	// center
					m_strRTFTitle[i].Insert (nPos + 5, "\\qc"); break;
				}
			}
		}

		// font size
		//////////////////////////////////////////////////////////////

		// replace all font sizes relative to the largest font size found,
		// which is assumed to be the title font size for the render type
		int nSize = 1;

		// determine the maximum font size used
		for (nPos = 0; ; )
		{
			nPos = m_strRTFTitle[i].Find ("\\fs", nPos + 3);
			if (nPos == -1)
				break;
			else if (isdigit (m_strRTFTitle[i].GetAt (nPos + 3)))
			{
				int nPosEnd = nPos + 3;
				while (isdigit (m_strRTFTitle[i].GetAt (nPosEnd)))
					nPosEnd++;

				nSize = max (nSize, atoi (m_strRTFTitle[i].Mid (nPos + 3).GetBuffer (-1)));
			}
		}

		// compute the ratio
		double dRatio = (double) abs (2 * pStyle->GetTitleFont ((RenderType) i)->lfHeight) / (double) nSize;

		// replace the font sizes
		for (nPos = 0; ; )
		{
			nPos = m_strRTFTitle[i].Find ("\\fs", nPos + 3);
			if (nPos == -1)
				break;
			else if (isdigit (m_strRTFTitle[i].GetAt (nPos + 3)))
			{
				int nPosEnd = nPos + 3;
				while (isdigit (m_strRTFTitle[i].GetAt (nPosEnd)))
					nPosEnd++;

				nSize = atoi (m_strRTFTitle[i].Mid (nPos + 3).GetBuffer (-1));
				m_strRTFTitle[i].Delete (nPos + 3, nPosEnd - nPos - 3);
				str.Format ("%d", (int) (dRatio * nSize));
				m_strRTFTitle[i].Insert (nPos + 3, str);
			}
		}

		// font
		//////////////////////////////////////////////////////////////

		nPos = m_strRTFTitle[i].Find ("\\fcharset0");
		if (nPos == -1)
			continue;
		int nPos2 = m_strRTFTitle[i].Find (";}", nPos);
		if (nPos2 == -1)
			continue;

		m_strRTFTitle[i] = m_strRTFTitle[i].Left (nPos + 11) +
			pStyle->GetTitleFontName ((RenderType) i) + m_strRTFTitle[i].Mid (nPos2);
	}
}

void CRTFTitles::DrawTitle2 (CRenderDC &rdc, RenderType rt, CStyleTemplate &style, CPoint& ptTracksOffset)
{
	CRect r;
	long lHeight = (long) (style.GetTitleHeight (rt) * 1000);

	IFormattedTextDraw* pFormattedText;
	if ((rt != Label) || !style.IsLabelTitleRound ())
	{
		pFormattedText = new CFormattedTextDraw ();
		pFormattedText->Create ();
		BSTR bstrTitle = m_strRTFTitle[rt].AllocSysString ();
		pFormattedText->put_RTFText (bstrTitle);
		SysFreeString (bstrTitle);

		pFormattedText->put_Height (lHeight);

		CRect* pRect = style.GetTotalRect (rt);
		long lExtWidth = pRect->Width (), lExtHeight = pRect->Height ();
		pFormattedText->get_NaturalExtent (rdc.m_pDC->GetSafeHdc (), &lExtWidth, &lExtHeight);
		CSize size (style.MapX (lExtWidth, rdc, rt), style.MapY (lExtHeight, rdc, rt));
//		CSize size (lExtWidth, lExtHeight);
		style.SetTitleSize (size, rt);
	}

	switch (rt)
	{
	case Cover:
	case Back:
		r = *style.GetCoverTitleRect (rdc, rt);
		ptTracksOffset.x = 0;
		ptTracksOffset.y = r.bottom;
		break;           

	case Inlay:
		r = *style.GetInlayTitleRect (rdc);
		ptTracksOffset.x = 0;
		ptTracksOffset.y = r.bottom;
		break;

	case Label:
		if (style.IsLabelTitleRound ())
		{
//			DrawTitle (rdc, rt, style);
			return;
		}
		else
		{
			r = *style.GetLabelTitleRect (rdc);
			ptTracksOffset.x = 0;
			ptTracksOffset.y = 0;
		}
		break;
	}

	if (style.GetDisplayTitles (rt))
		pFormattedText->Draw (rdc.m_pDC->GetSafeHdc (), &r);

	delete pFormattedText;
}
