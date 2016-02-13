// RenderDC.cpp: implementation of the CRenderDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "RenderDC.h"
#include "StyleTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRenderDC::CRenderDC(CDC* pDC, int nHeight, CObject* pStyle)
{
	m_pDC = pDC;
	m_hndPrinter = NULL;

	/*
	m_nWidthInlay = (nHeight * 5) / 4;
	m_nWidthCover = nHeight;
	m_nWidthLabel = nHeight;

	m_nHeightInlay = nHeight;
	m_nHeightCover = nHeight;
	m_nHeightLabel = nHeight;
	*/

	SetMeasures (nHeight, pStyle);
}

CRenderDC::CRenderDC(CDC* pDC, HANDLE hndPrinter, int nHeight, CObject* pStyle)
{
	m_pDC = pDC;
	m_hndPrinter = hndPrinter;

	SetMeasures (nHeight, pStyle);
}

void CRenderDC::SetMeasures (int nHeight, CObject* pStyle)
{
	CStyleTemplate* pStl = (CStyleTemplate*) pStyle;

	m_nHeightCover = nHeight;

	CRect* pRect = pStl->GetTotalCoverRect ();
	int nVirtCoverWidth = pRect->Width ();
	int nVirtCoverHeight = pRect->Height ();
	m_nWidthCover = (nVirtCoverWidth * nHeight) / nVirtCoverHeight;

	pRect = pStl->GetTotalInlayRect ();
	m_nHeightInlay = (pRect->Height () * nHeight) / nVirtCoverHeight;
	m_nWidthInlay = (pRect->Width () * m_nHeightInlay) / pRect->Height ();

	pRect = pStl->GetTotalLabelRect ();
	m_nHeightLabel = (pRect->Height () * nHeight) / nVirtCoverHeight;
	m_nWidthLabel = (pRect->Width () * m_nHeightLabel) / pRect->Height ();
}

CRenderDC::~CRenderDC()
{

}

int CRenderDC::GetWidth (RenderType rt)
{
	switch (rt)
	{
	case Inlay:
		return m_nWidthInlay;
	case Cover:
	case Back:
		return m_nWidthCover;
	case Label:
		return m_nWidthLabel;
	}

	return 0;
}

int CRenderDC::GetHeight (RenderType rt)
{
	switch (rt)
	{
	case Inlay:
		return m_nHeightInlay;
	case Cover:
	case Back:
		return m_nHeightCover;
	case Label:
		return m_nHeightLabel;
	}

	return 0;
}