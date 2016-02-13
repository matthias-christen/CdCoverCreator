// GradientStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "GradientStatic.h"

#include "GradientRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGradientStatic

CGradientStatic::CGradientStatic()
{
}

CGradientStatic::~CGradientStatic()
{
}


BEGIN_MESSAGE_MAP(CGradientStatic, CFontPreviewStatic)
	//{{AFX_MSG_MAP(CGradientStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradientStatic message handlers

void CGradientStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect r;
	GetClientRect (&r);
	CGradientRenderer::DrawGradient (&dc, NULL, r, m_nAngle, m_crStart, m_crEnd, NULL);
}

void CGradientStatic::SetStartColor(COLORREF color)
{
	m_crStart = color;
	InvalidateRect (NULL);
}

COLORREF CGradientStatic::GetStartColor()
{
	return m_crStart;
}

void CGradientStatic::SetEndColor(COLORREF color)
{
	m_crEnd = color;
	InvalidateRect (NULL);
}

COLORREF CGradientStatic::GetEndColor()
{
	return m_crEnd;
}

void CGradientStatic::SetAngle(int nAngle)
{
	m_nAngle = nAngle;
	if (::IsWindow (GetSafeHwnd ()))
		InvalidateRect (NULL);
}

int CGradientStatic::GetAngle()
{
	return m_nAngle;
}
