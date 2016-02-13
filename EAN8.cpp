#include "StdAfx.h"
#include "ean8.h"

#include <math.h>

CEAN8::CEAN8 ()
	: CEANBase (8)
{
}

CEAN8::~CEAN8(void)
{
}

void CEAN8::SetCode (CString strCode, CString)
{
	CEANBase::SetCode (strCode, "");

	m_strBars = LEFT_GUARD;
	
	for (int i = 0; i < 4; i++) 
		m_strBars += Explode (m_strCode[i] - '0', 'A');	// set A
	
	m_strBars += CENTER_GUARD;
	
	for (int i = 4; i < 8; i++) 
		m_strBars += Explode (m_strCode[i] - '0', 'C');	// set C

	m_strBars += RIGHT_GUARD;
}

CSize CEAN8::GetSize ()
{
	int nModuleSize = (int) (MODULE * 100);

	int nTop = 1;	// nTop margin
	int nLeft = 7;	// EAN-13 - 11, EAN-8 - 7
	int nRight = 7;	// always 7				

	return CSize ((nLeft + 67 + nRight) * nModuleSize, (int) (EAN8_HEIGHT * 100 + (nTop + 9) * nModuleSize));
}

// nPPM - pixels per module, nHeight - bar nHeight in pixels
void CEAN8::Render (Graphics* pGraphics, CRect rect)
{
	if (m_strBars.IsEmpty ())
		SetCode (m_strCode);

	CSize size = GetSize ();
	int nPPM = (int) (((double) size.cy / (double) rect.Height ()) * (double) MODULE);

	int nHeight = (int) (((EAN8_HEIGHT - 1) * 100) / (double) size.cy * rect.Height ());
	int nTop = 1;	// nTop margin
	int nLeft = 7;	// EAN-13 - 11, EAN-8 - 7
	int nRight = 7;	// always 7				

	if (m_nBackgroundMode == OPAQUE)
	{
		Color col;
		col.SetFromCOLORREF (m_colBackgroundColor);
		SolidBrush brush (col);
		pGraphics->FillRectangle (&brush, rect.left, rect.top, rect.Width (), rect.Height ());
	}

	Color col;
	col.SetFromCOLORREF (m_colForegroundColor);
	SolidBrush brush (col);

	for (int i = 0; i < 67; i++)
	{
		int h = nHeight + 5 * nPPM;
		if ((i >= 3) && (i < 31))
			h = nHeight;
		if ((i >= 36) && (i < 64))
			h = nHeight;

		if (m_strBars.GetAt (i) == '0')
			continue;
		
		pGraphics->FillRectangle (&brush, (nLeft + i + 4) * nPPM + rect.left, nTop * nPPM + rect.top, nPPM, h - 1);
	}

	int nLevel = nHeight + (nTop + /*7*/0) * nPPM + rect.left;

	BSTR bstrFont = m_strFont.AllocSysString ();
	Gdiplus::Font font (bstrFont, 10 * nPPM);
	::SysFreeString (bstrFont);

	int nCount = 0;
	for (int i = 0; i < 8; i++)
	{
		if (i == 4)
			nCount = 0;

		BSTR bstrText = m_strCode.Mid (i, 1).AllocSysString ();
		PointF pt (((i < 4 ? 5 : 37 )+ nLeft + 7 * nCount++) * nPPM + rect.left, nLevel);
		pGraphics->DrawString (bstrText, 1, &font, pt, &brush);
		::SysFreeString (bstrText);
	}
}
