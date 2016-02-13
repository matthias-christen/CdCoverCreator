// FontPreviewStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "FontPreviewStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFontPreviewStatic

CFontPreviewStatic::CFontPreviewStatic()
{
	m_hBkBrush = ::GetSysColorBrush (COLOR_3DFACE);
	m_pFont = NULL;
	m_color = 0;
}

CFontPreviewStatic::~CFontPreviewStatic()
{
	if (m_pFont != NULL)
		m_pFont->DeleteObject ();
	delete m_pFont;
}


BEGIN_MESSAGE_MAP(CFontPreviewStatic, CStatic)
	//{{AFX_MSG_MAP(CFontPreviewStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontPreviewStatic message handlers

void CFontPreviewStatic::SetPreviewFont(CHOOSEFONT *pCf)
{
	if (m_pFont == NULL)
		m_pFont = new CFont ();

	m_pFont->DeleteObject ();
	m_pFont->CreateFontIndirect (pCf->lpLogFont);
	if (pCf->rgbColors)
		m_color = pCf->rgbColors;

	SetFont (m_pFont);
}

HBRUSH CFontPreviewStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor (m_color);
	pDC->SetBkMode (TRANSPARENT);
	
	return m_hBkBrush;
}

LOGFONT* CFontPreviewStatic::GetLogfont()
{
	if (m_pFont == NULL)
		GetFont ()->GetLogFont (&m_lf);
	else
		m_pFont->GetLogFont (&m_lf);

	return &m_lf;
}

void CFontPreviewStatic::SetPreviewFont(LOGFONT *pLf)
{
	if (m_pFont == NULL)
		m_pFont = new CFont ();

	m_pFont->DeleteObject ();
	m_pFont->CreateFontIndirect (pLf);

	SetFont (m_pFont);
}

void CFontPreviewStatic::SetColor(COLORREF color)
{
	m_color = color;
	InvalidateRect (NULL);
}
