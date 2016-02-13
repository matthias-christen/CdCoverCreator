#include "StdAfx.h"
#include "ean13.h"

#include <math.h>

CEAN13::CEAN13 ()
	: CEANBase (13)
{
}

CEAN13::~CEAN13(void)
{
}

void CEAN13::SetCode (CString strCode, CString strAddon)
{
	CEANBase::SetCode (strCode, strAddon);

	m_strBars = LEFT_GUARD;
	CString strSet = m_strSet[m_strCode[0] - '0'];

	for (int i = 1; i < 7; i++)
		m_strBars += Explode (m_strCode[i] - '0', strSet[i - 1]);
	
	m_strBars += CENTER_GUARD;
	
	for (int i = 7; i < 13; i++) 
		m_strBars += Explode (m_strCode[i] - '0', 'C'); // set C
	
	m_strBars += RIGHT_GUARD;

	if (m_nAddonType == 2)
	{
		int nMod = m_nAddonType % 4;
		int c1 = m_strAddon[0] - '0';
		int c2 = m_strAddon[1] - '0';

		m_strAddon = ADDON_GUARD + Explode (c1, m_strSetAddon2[nMod].GetAt (0)) +
			ADDON_DELIN + Explode (c2, m_strSetAddon2[nMod].GetAt (1));
	}
	else if (m_nAddonType == 5)
	{
		int nMod = (3 * ((m_strAddon[0] - '0') + (m_strAddon[2] - '0') + (m_strAddon[4] - '0')) +
			9 * ((m_strAddon[1] - '0') + (m_strAddon[3] - '0'))) % 10;
		
		m_strAddBars = ADDON_GUARD;
		for (int i = 1; i <= 5; i++)
		{
			m_strAddBars += Explode (m_strAddon[i - 1], m_strSetAddon5[nMod].GetAt (i - 1));
			if (i != 5)
				m_strAddBars += ADDON_DELIN;
		}
	}
}

// returns the size in 100th of millimeters
CSize CEAN13::GetSize ()
{
	int nModuleSize = (int) (MODULE * 100);

	int nTop = 1;	// nTop margin
	int nLeft = 7;	// EAN-13 - 11, EAN-8 - 7
	int nRight = 7;	// always 7				

	int nRightAdd = 0;
	if (m_nAddonType == 2)
		nRightAdd = 25;
	if (m_nAddonType == 5)
		nRightAdd = 52;

	return CSize ((nLeft + 95 + nRight + nRightAdd) * nModuleSize, (int) (EAN13_HEIGHT * 100 + (nTop + 9) * nModuleSize));
}

// nPPM - pixels per module, nHeight - bar nHeight in pixels
void CEAN13::Render (Gdiplus::Graphics* pGraphics, CRect rect)
{
	if (m_strBars.IsEmpty ())
		SetCode (m_strCode, m_strAddon);

	CSize size = GetSize ();
	int nPPM = (int) (((double) size.cy / (double) rect.Height ()) * (double) MODULE);

	int nHeight = (int) (((EAN13_HEIGHT - 1) * 100) / (double) size.cy * rect.Height ());
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

	for (int i = 0; i < 95; i++)
	{
		int h = nHeight + 5 * nPPM;
		if ((i >= 3) && (i < 45))
			h = nHeight;
		if ((i >= 50) && (i < 92))
			h = nHeight;

		if (m_strBars.GetAt (i) == '0')
			continue;
		
		pGraphics->FillRectangle (&brush, (nLeft + i + 3) * nPPM + rect.left, nTop * nPPM + rect.top, nPPM, h - 1);
	}

	BSTR bstrFont = m_strFont.AllocSysString ();
	Gdiplus::Font font (bstrFont, 8 * nPPM);
	::SysFreeString (bstrFont);

	if (m_nAddonType)
	{
		int nLeftAdd = 95 + nRight + nLeft;
		int nMax = m_nAddonType == 2 ? 20 : 47;

		for (int i = 0; i < nMax; i++)
		{
			int h = nHeight + 5 * nPPM;
			if (m_strAddBars.GetAt (i) == '0')
				continue;

			pGraphics->FillRectangle (&brush, (nLeftAdd + i) * nPPM + rect.left, (nTop + 9) * nPPM + rect.top, nPPM, h - 9 * nPPM - 1);
		}

		int nCount = 0;
		int nTxtFix = m_nAddonType == 2 ? 5 : 7;	// fix for text output
		for (int i = 0; i < m_nAddonType; i++)
		{
			BSTR bstrText = m_strAddon.Mid (i, 1).AllocSysString ();
			PointF pt ((nTxtFix + nLeftAdd + 7 * nCount++) * nPPM, 8 * nPPM);
			pGraphics->DrawString (bstrText, 1, &font, pt, &brush);
			::SysFreeString (bstrText);
		}

		//pDC->TextOut ((nLeftAdd + 1 + 9 * m_nAddonType) * nPPM, 8 * nPPM, ">");
		pGraphics->DrawString (L">", 1, &font, PointF ((nLeftAdd + 1 + 9 * m_nAddonType) * nPPM, 8 * nPPM), &brush);
	}

	int nLevel = nHeight + (nTop + /*7*/0) * nPPM + rect.top;

//	pDC->TextOut (/*3 * nPPM + */rect.left, nLevel, m_strCode.Left (1));
	BSTR bstrText = m_strCode.Left (1).AllocSysString ();
	pGraphics->DrawString (bstrText, 1, &font, PointF (/*-3 * nPPM +*/ rect.left, nLevel), &brush);
	::SysFreeString (bstrText);

	int nCount = 0;
	for (int i = 1; i < 13; i++)
	{
		if (i == 7)
			nCount = 0;

		bstrText = m_strCode.Mid (i, 1).AllocSysString ();
		PointF pt (((i < 7 ? 5 : 51) + nLeft + 7 * nCount++) * nPPM + rect.left, nLevel);
		//PointF pt (((i < 7 ? 2 : 48) + nLeft + 7 * nCount++) * nPPM + rect.left, nLevel);
		pGraphics->DrawString (bstrText, 1, &font, pt, &brush);
		::SysFreeString (bstrText);
	}

	if (!m_nAddonType)
		pGraphics->DrawString (L">", 1, &font, PointF ((95 + nLeft) * nPPM + rect.left, nLevel), &brush);
}