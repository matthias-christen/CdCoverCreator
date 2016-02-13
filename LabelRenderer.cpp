// LabelRenderer.cpp: implementation of the CLabelRenderer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "LabelRenderer.h"

#include "TextRenderer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLabelRenderer::CLabelRenderer(CTracksInfo* pTracks, CDataCDInfo* pDataCDInfo, CBackgroundInfo** bkgrnd, std::list<CFloatingGraphicsObject*>* pListFloatObj, CStyleTemplate& style)
{
	m_pTracks = pTracks;
	m_pDataCDInfo = pDataCDInfo;

	for (int i = 0; i < NUM_RENDER_TYPES; i++)
	{
		m_pBackground[i] = bkgrnd[i];
		m_pListFloatingObjs[i] = pListFloatObj != NULL ? &pListFloatObj[i] : NULL;
	}

	m_pStyle = &style;
}

CLabelRenderer::~CLabelRenderer()
{

}

void CLabelRenderer::Render(CRenderDC rdc, RenderType rt, bool bIsPreviewing, bool bIsPrinting, bool bDrawTicks, bool bDrawCutLinesWhenPrinting)
{
	if (rt == Booklet)
		return;

	rdc.m_pDC->SetBkMode (TRANSPARENT);
	
	CPoint ptViewOrg = rdc.m_pDC->GetViewportOrg ();
	CRect r1 (*m_pStyle->GetTotalRect (rdc, rt));
	CRect r2 (*m_pStyle->GetInner2LabelRect (rdc));

	// *** MFC Bug Workaround: ***
	// Now if we are in print preview mode then the clipping
	// rectangle needs to be adjusted before creating the
	// clipping region
	if (rdc.m_pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC)))
	{
		CPreviewDC* pPrevDC = (CPreviewDC*) rdc.m_pDC;

		pPrevDC->PrinterDPtoScreenDP (&r1.TopLeft ());
		pPrevDC->PrinterDPtoScreenDP (&r1.BottomRight ());
		pPrevDC->PrinterDPtoScreenDP (&r2.TopLeft ());
		pPrevDC->PrinterDPtoScreenDP (&r2.BottomRight ());

		// Now offset the result by the viewport origin of
		// the print preview window...
		CPoint ptOrg;
		::GetViewportOrgEx (rdc.m_pDC->m_hDC, &ptOrg);
		r1 += ptOrg - ptViewOrg;
		r2 += ptOrg - ptViewOrg;
	}
	// *** END Workaround ***

	r1.OffsetRect (ptViewOrg);
	r2.OffsetRect (ptViewOrg);

	// create clipping region
	CRgn rgn1, rgn2;
	if (rt == Label)
	{
		if (!bIsPrinting)
		{
			// draw shade
			int nOffset = max (r1.Width () / 50, 2);
			r1.OffsetRect (nOffset, nOffset);
			rgn1.CreateEllipticRgn (r1.left, r1.top, r1.right, r1.bottom);
			rdc.m_pDC->SelectClipRgn (&rgn1);
			rgn2.CreateEllipticRgn (r2.left + nOffset, r2.top + nOffset, r2.right + nOffset, r2.bottom + nOffset);
			rdc.m_pDC->SelectClipRgn (&rgn2, RGN_DIFF);
			CBrush brush (RGB (0x60, 0x60, 0x60));
			rdc.m_pDC->FillRect (&r1, &brush);
			brush.DeleteObject ();
			rgn1.DeleteObject ();
			rgn2.DeleteObject ();
			r1.OffsetRect (-nOffset, -nOffset);
		}

		rgn1.CreateEllipticRgn (r1.left, r1.top, r1.right, r1.bottom);
		rdc.m_pDC->SelectClipRgn (&rgn1);

		rgn2.CreateEllipticRgn (r2.left, r2.top, r2.right, r2.bottom);
		rdc.m_pDC->SelectClipRgn (&rgn2, RGN_DIFF);
	}

	// set clipping region
	if (rt != Label)
	{
		rgn1.CreateRectRgn (r1.left, r1.top, r1.right, r1.bottom);
		rdc.m_pDC->SelectClipRgn (&rgn1);
	}

	// render background
	if (m_pBackground[rt] != NULL)
		m_pBackground[rt]->Render (rdc, *m_pStyle, bIsPrinting);

	// render the floating objects
	if (m_pListFloatingObjs[rt] != NULL)
		for (std::list<CFloatingGraphicsObject*>::iterator it = m_pListFloatingObjs[rt]->begin (); it != m_pListFloatingObjs[rt]->end (); it++)
			(*it)->Render (rdc, rt, m_pStyle, bIsPreviewing, bIsPrinting);

	// render tracks
	if (m_pTracks != NULL)
		m_pTracks->Render (rdc, rt, *m_pStyle);

	if (m_pDataCDInfo != NULL)
		m_pDataCDInfo->Render (rdc, rt, *m_pStyle);

	// clean up
	rdc.m_pDC->SelectClipRgn (NULL);
	rgn1.DeleteObject ();
	rgn2.DeleteObject ();

	CPen penSolid (PS_SOLID, 0, RGB (0, 0, 0));

	// draw label margins
	switch (rt)
	{
	case Inlay:
		if (bIsPrinting)
		{
			if (bDrawCutLinesWhenPrinting)
				DrawInlayNonprintMargins (rdc, penSolid);
			if (bDrawTicks)
				DrawInlayPrintMargins (rdc);
		}
		else
		{
			DrawInlayNonprintMargins (rdc, penSolid);
			DrawShade (rdc, Inlay);
		}
		break;

	case Cover:
	case Back:
		if (bIsPrinting)
		{
			if (bDrawCutLinesWhenPrinting)
				DrawCoverNonprintMargins (rdc, penSolid);
			if (bDrawTicks)
				DrawCoverPrintMargins (rdc, rt);
		}
		else
		{
			DrawCoverNonprintMargins (rdc, penSolid);
			DrawShade (rdc, Cover);
		}
		break;

	case Label:
		if (!bIsPrinting)
		{
			// outline
			rdc.m_pDC->SelectStockObject (HOLLOW_BRUSH);
			rdc.m_pDC->Ellipse (m_pStyle->GetTotalLabelRect (rdc));
			rdc.m_pDC->Ellipse (m_pStyle->GetInner2LabelRect (rdc));
		}
		break;
	}

	penSolid.DeleteObject ();
}

void CLabelRenderer::DrawInlayPrintMargins(CRenderDC &rdc)
{
	CRect* pR = m_pStyle->GetTotalInlayRect ();

	int nLeft = m_pStyle->MapX (pR->left, rdc, Inlay);
	int nRight = m_pStyle->MapX (pR->right, rdc, Inlay);
	int nTop = m_pStyle->MapY (pR->top, rdc, Inlay);
	int nBottom = m_pStyle->MapY (pR->bottom, rdc, Inlay);

	CRect r (pR);

	switch (m_pStyle->GetMediumType ())
	{
	case MediumJewelCase:
		{
			pR = m_pStyle->GetInlayLeftSideRect (rdc);
			int nLeftX1 = pR->left;
			int nLeftX2 = pR->right;
			pR = m_pStyle->GetInlayRightSideRect (rdc);
			int nRightX1 = pR->left;
			int nRightX2 = pR->right;

			int nTopY1 = m_pStyle->MapY (r.top - 500, rdc, Inlay);
			int nTopY2 = m_pStyle->MapY (r.top - 200, rdc, Inlay);
			int nTopY2a = m_pStyle->MapY (r.top + 200, rdc, Inlay);
			int nBottomY1 = m_pStyle->MapY (r.bottom + 200, rdc, Inlay);
			int nBottomY1a = m_pStyle->MapY (r.bottom - 200, rdc, Inlay);
			int nBottomY2 = m_pStyle->MapY (r.bottom + 500, rdc, Inlay);

			int nTopY3 = m_pStyle->MapY (r.top, rdc, Inlay);
			int nLeftX3 = m_pStyle->MapX (r.left - 500, rdc, Inlay);
			int nLeftX4 = m_pStyle->MapX (r.left - 200, rdc, Inlay);
			int nBottomY3 = m_pStyle->MapY (r.bottom, rdc, Inlay);
			int nRightX3 = m_pStyle->MapX (r.right + 200, rdc, Inlay);
			int nRightX4 = m_pStyle->MapY (r.right + 500, rdc, Inlay);

			rdc.m_pDC->MoveTo (nLeftX1, nTopY1);
			rdc.m_pDC->LineTo (nLeftX1, nTopY2);
			rdc.m_pDC->MoveTo (nLeftX2, nTopY1);
			rdc.m_pDC->LineTo (nLeftX2, nTopY2a);
			rdc.m_pDC->MoveTo (nRightX1, nTopY1);
			rdc.m_pDC->LineTo (nRightX1, nTopY2a);
			rdc.m_pDC->MoveTo (nRightX2, nTopY1);
			rdc.m_pDC->LineTo (nRightX2, nTopY2);

			rdc.m_pDC->MoveTo (nLeftX1, nBottomY1);
			rdc.m_pDC->LineTo (nLeftX1, nBottomY2);
			rdc.m_pDC->MoveTo (nLeftX2, nBottomY1a);
			rdc.m_pDC->LineTo (nLeftX2, nBottomY2);
			rdc.m_pDC->MoveTo (nRightX1, nBottomY1a);
			rdc.m_pDC->LineTo (nRightX1, nBottomY2);
			rdc.m_pDC->MoveTo (nRightX2, nBottomY1);
			rdc.m_pDC->LineTo (nRightX2, nBottomY2);

			rdc.m_pDC->MoveTo (nLeftX3, nTopY3);
			rdc.m_pDC->LineTo (nLeftX4, nTopY3);
			rdc.m_pDC->MoveTo (nRightX3, nTopY3);
			rdc.m_pDC->LineTo (nRightX4, nTopY3);
			rdc.m_pDC->MoveTo (nLeftX3, nBottomY3);
			rdc.m_pDC->LineTo (nLeftX4, nBottomY3);
			rdc.m_pDC->MoveTo (nRightX3, nBottomY3);
			rdc.m_pDC->LineTo (nRightX4, nBottomY3);
		}
		break;

	case MediumSlimCase:
		{
			CPen pen (PS_DOT, 1, RGB (0x00, 0x00, 0x00));
			CPen* pOldPen = (CPen*) rdc.m_pDC->SelectObject (&pen);

			pR = m_pStyle->GetInlayRightSideRect (rdc);
			int nRight1 = pR->left;
			int nRight2 = pR->right;
			
			rdc.m_pDC->MoveTo (nRight1, nTop);
			rdc.m_pDC->LineTo (nRight1, nBottom);

			rdc.m_pDC->MoveTo (nRight2, nTop);
			rdc.m_pDC->LineTo (nRight2, nBottom);

			// draw the frame
			int nLeft1 = m_pStyle->MapX (r.left + 600, rdc, Inlay);

			rdc.m_pDC->MoveTo (nRight2, nTop);
			rdc.m_pDC->LineTo (nLeft1, nTop);
			rdc.m_pDC->LineTo (nLeft, m_pStyle->MapY (r.top + 200, rdc, Inlay));
			rdc.m_pDC->LineTo (nLeft, m_pStyle->MapY (r.bottom - 200, rdc, Inlay));
			rdc.m_pDC->LineTo (nLeft1, nBottom);
			rdc.m_pDC->LineTo (nRight2, nBottom);

			rdc.m_pDC->SelectObject (pOldPen);
		}
		break;

	case MediumPaperSleeve:
		break;

	case MediumDVDBox:
		{
			pR = m_pStyle->GetInlayRightSideRect (rdc);
			int nRightX1 = pR->left;
			int nRightX2 = pR->right;

			int nTopY1 = m_pStyle->MapY (r.top - 500, rdc, Inlay);
			int nTopY2 = m_pStyle->MapY (r.top - 200, rdc, Inlay);
			int nTopY2a = m_pStyle->MapY (r.top + 200, rdc, Inlay);
			int nBottomY1 = m_pStyle->MapY (r.bottom + 200, rdc, Inlay);
			int nBottomY1a = m_pStyle->MapY (r.bottom - 200, rdc, Inlay);
			int nBottomY2 = m_pStyle->MapY (r.bottom + 500, rdc, Inlay);

			int nTopY3 = m_pStyle->MapY (r.top, rdc, Inlay);
			int nLeftX3 = m_pStyle->MapX (r.left - 500, rdc, Inlay);
			int nLeftX4 = m_pStyle->MapX (r.left - 200, rdc, Inlay);
			int nBottomY3 = m_pStyle->MapY (r.bottom, rdc, Inlay);

			// vertical marks
			rdc.m_pDC->MoveTo (nLeft, nTopY1);
			rdc.m_pDC->LineTo (nLeft, nTopY2);
			rdc.m_pDC->MoveTo (nRightX1, nTopY1);
			rdc.m_pDC->LineTo (nRightX1, nTopY2a);
			rdc.m_pDC->MoveTo (nRightX2, nTopY1);
			rdc.m_pDC->LineTo (nRightX2, nTopY2a);

			rdc.m_pDC->MoveTo (nLeft, nBottomY1);
			rdc.m_pDC->LineTo (nLeft, nBottomY2);
			rdc.m_pDC->MoveTo (nRightX1, nBottomY1a);
			rdc.m_pDC->LineTo (nRightX1, nBottomY2);
			rdc.m_pDC->MoveTo (nRightX2, nBottomY1a);
			rdc.m_pDC->LineTo (nRightX2, nBottomY2);

			// horizontal marks
			rdc.m_pDC->MoveTo (nLeftX3, nTopY3);
			rdc.m_pDC->LineTo (nLeftX4, nTopY3);
			rdc.m_pDC->MoveTo (nLeftX3, nBottomY3);
			rdc.m_pDC->LineTo (nLeftX4, nBottomY3);
		}
		break;
	}
}

void CLabelRenderer::DrawInlayNonprintMargins(CRenderDC &rdc, CPen& penSolid)
{
	CPen penDots (PS_DOT, 0, RGB (0, 0, 0));

	CPen* pOldPen = rdc.m_pDC->SelectObject (&penSolid);
	CBrush* pOldBrush = (CBrush*) rdc.m_pDC->SelectStockObject (HOLLOW_BRUSH);
	rdc.m_pDC->Rectangle (m_pStyle->GetTotalInlayRect (rdc));

	rdc.m_pDC->SelectObject (&penDots);
	rdc.m_pDC->Rectangle (m_pStyle->GetInlayLeftSideRect (rdc));
	rdc.m_pDC->Rectangle (m_pStyle->GetInlayRightSideRect (rdc));

	rdc.m_pDC->SelectObject (pOldPen);
	rdc.m_pDC->SelectObject (pOldBrush);
	penDots.DeleteObject ();
}

void CLabelRenderer::DrawCoverPrintMargins(CRenderDC &rdc, RenderType rt)
{
	CRect* pR = m_pStyle->GetTotalCoverRect ();

	int nLeft = m_pStyle->MapX (pR->left, rdc, Cover);
	int nRight = m_pStyle->MapX (pR->right, rdc, Cover);
	int nTop = m_pStyle->MapY (pR->top, rdc, Cover);
	int nBottom = m_pStyle->MapY (pR->bottom, rdc, Cover);

	CRect r (pR);

	switch (m_pStyle->GetMediumType ())
	{
	case MediumJewelCase:
		{
			int nLeft1 = m_pStyle->MapX (pR->left - 500, rdc, Cover);	// left tick
			int nLeft2 = m_pStyle->MapX (pR->left - 200, rdc, Cover);

			int nRight1 = m_pStyle->MapX (pR->right + 200, rdc, Cover);	// right tick
			int nRight2 = m_pStyle->MapX (pR->right + 500, rdc, Cover);

			int nTop1 = m_pStyle->MapY (pR->top - 500, rdc, Cover);		// top tick
			int nTop2 = m_pStyle->MapY (pR->top - 200, rdc, Cover);

			int nBottom1 = m_pStyle->MapY (pR->bottom + 200, rdc, Cover);	// bottom tick
			int nBottom2 = m_pStyle->MapY (pR->bottom + 500, rdc, Cover);

			// top left
			rdc.m_pDC->MoveTo (nLeft1, nTop);
			rdc.m_pDC->LineTo (nLeft2, nTop);
			rdc.m_pDC->MoveTo (nLeft, nTop1);
			rdc.m_pDC->LineTo (nLeft, nTop2);

			// top right
			rdc.m_pDC->MoveTo (nRight1, nTop);
			rdc.m_pDC->LineTo (nRight2, nTop);
			rdc.m_pDC->MoveTo (nRight, nTop1);
			rdc.m_pDC->LineTo (nRight, nTop2);

			// bottom left
			rdc.m_pDC->MoveTo (nLeft1, nBottom);
			rdc.m_pDC->LineTo (nLeft2, nBottom);
			rdc.m_pDC->MoveTo (nLeft, nBottom1);
			rdc.m_pDC->LineTo (nLeft, nBottom2);

			// bottom right
			rdc.m_pDC->MoveTo (nRight1, nBottom);
			rdc.m_pDC->LineTo (nRight2, nBottom);
			rdc.m_pDC->MoveTo (nRight, nBottom1);
			rdc.m_pDC->LineTo (nRight, nBottom2);
		}
		break;

	case MediumSlimCase:
		if (rt == Cover)
		{
			CPen pen (PS_DOT, 1, RGB (0x00, 0x00, 0x00));
			CPen* pOldPen = (CPen*) rdc.m_pDC->SelectObject (&pen);
			CBrush* pOldBrush = (CBrush*) rdc.m_pDC->SelectStockObject (HOLLOW_BRUSH);

			rdc.m_pDC->Rectangle (nLeft, nTop, nRight, nBottom);

			// draw upper notch
			int nTop1 = m_pStyle->MapY (r.top + 200, rdc, Cover);
			int nLeft1 = m_pStyle->MapX (r.left + 1800, rdc, Cover);
			int nLeft2 = m_pStyle->MapX (r.left + 2000, rdc, Cover);
			rdc.m_pDC->MoveTo (nLeft, nTop1);
			rdc.m_pDC->LineTo (nLeft1, nTop1);
			rdc.m_pDC->LineTo (nLeft2, nTop);

			// draw lower notch
			int nBottom1 = m_pStyle->MapY (r.bottom - 200, rdc, Cover);
			rdc.m_pDC->MoveTo (nLeft, nBottom1);
			rdc.m_pDC->LineTo (nLeft1, nBottom1);
			rdc.m_pDC->LineTo (nLeft2, nBottom);

			rdc.m_pDC->SelectObject (pOldPen);
			rdc.m_pDC->SelectObject (pOldBrush);
		}
		else if (rt == Back)
		{
			CPen pen (PS_DOT, 1, RGB (0x00, 0x00, 0x00));
			CPen* pOldPen = (CPen*) rdc.m_pDC->SelectObject (&pen);
			CBrush* pOldBrush = (CBrush*) rdc.m_pDC->SelectStockObject (HOLLOW_BRUSH);

			rdc.m_pDC->Rectangle (nLeft, nTop, nRight, nBottom);

			// draw upper notch
			int nTop1 = m_pStyle->MapY (r.top + 200, rdc, Cover);
			int nRight1 = m_pStyle->MapX (r.right - 1800, rdc, Cover);
			int nRight2 = m_pStyle->MapX (r.right - 2000, rdc, Cover);
			rdc.m_pDC->MoveTo (nRight, nTop1);
			rdc.m_pDC->LineTo (nRight1, nTop1);
			rdc.m_pDC->LineTo (nRight2, nTop);

			// draw lower notch
			int nBottom1 = m_pStyle->MapY (r.bottom - 200, rdc, Cover);
			rdc.m_pDC->MoveTo (nRight, nBottom1);
			rdc.m_pDC->LineTo (nRight1, nBottom1);
			rdc.m_pDC->LineTo (nRight2, nBottom);

			rdc.m_pDC->SelectObject (pOldPen);
			rdc.m_pDC->SelectObject (pOldBrush);
		}
		break;

	case MediumPaperSleeve:
		{
			if (rt == Back)
			{
				int nLeft1 = m_pStyle->MapX (pR->left + 1000, rdc, Cover);
				int nRight1 = m_pStyle->MapX (pR->right - 1000, rdc, Cover);
				int nTop1 = m_pStyle->MapY (pR->top - 1500, rdc, Cover);
				int nBottom1 = m_pStyle->MapY (pR->bottom + 1500, rdc, Cover);

				// draw upper fold lash
				rdc.m_pDC->MoveTo (nLeft, nTop);
				rdc.m_pDC->LineTo (nLeft1, nTop1);
				rdc.m_pDC->LineTo (nRight1, nTop1);
				rdc.m_pDC->LineTo (nRight, nTop);

				// draw lower lash
				rdc.m_pDC->MoveTo (nLeft, nBottom);
				rdc.m_pDC->LineTo (nLeft1, nBottom1);
				rdc.m_pDC->LineTo (nRight1, nBottom1);
				rdc.m_pDC->LineTo (nRight, nBottom);
			}

			CPen pen (PS_DOT, 1, RGB (0x00, 0x00, 0x00));
			CPen* pOldPen = (CPen*) rdc.m_pDC->SelectObject (&pen);
			CBrush* pOldBrush = (CBrush*) rdc.m_pDC->SelectStockObject (HOLLOW_BRUSH);
			rdc.m_pDC->Rectangle (nLeft, nTop, nRight, nBottom);

			if (rt == Cover)
			{
				// draw optional closing lash
				int nRight1 = m_pStyle->MapX (pR->right + 1800, rdc, Cover);
				int nTop1 = m_pStyle->MapY (pR->top + 800, rdc, Cover);
				int nBottom1 = m_pStyle->MapY (pR->bottom - 800, rdc, Cover);

				rdc.m_pDC->MoveTo (nRight, nTop);
				rdc.m_pDC->LineTo (nRight1, nTop1);
				rdc.m_pDC->LineTo (nRight1, nBottom1);
				rdc.m_pDC->LineTo (nRight, nBottom);
			}

			rdc.m_pDC->SelectObject (pOldPen);
			rdc.m_pDC->SelectObject (pOldBrush);
		}

		break;

	case MediumDVDBox:
		{
			int nRight1 = m_pStyle->MapX (pR->right + 200, rdc, Cover);	// right tick
			int nRight2 = m_pStyle->MapX (pR->right + 500, rdc, Cover);

			int nTop1 = m_pStyle->MapY (pR->top - 500, rdc, Cover);		// top tick
			int nTop2 = m_pStyle->MapY (pR->top - 200, rdc, Cover);

			int nBottom1 = m_pStyle->MapY (pR->bottom + 200, rdc, Cover);	// bottom tick
			int nBottom2 = m_pStyle->MapY (pR->bottom + 500, rdc, Cover);

			// top right
			rdc.m_pDC->MoveTo (nRight1, nTop);
			rdc.m_pDC->LineTo (nRight2, nTop);
			rdc.m_pDC->MoveTo (nRight, nTop1);
			rdc.m_pDC->LineTo (nRight, nTop2);

			// bottom right
			rdc.m_pDC->MoveTo (nRight1, nBottom);
			rdc.m_pDC->LineTo (nRight2, nBottom);
			rdc.m_pDC->MoveTo (nRight, nBottom1);
			rdc.m_pDC->LineTo (nRight, nBottom2);
		}
		break;
	}
}

void CLabelRenderer::DrawCoverNonprintMargins(CRenderDC &rdc, CPen &penSolid)
{
	CPen* pOldPen = rdc.m_pDC->SelectObject (&penSolid);
	CBrush* pOldBrush = (CBrush*) rdc.m_pDC->SelectStockObject (HOLLOW_BRUSH);
	rdc.m_pDC->Rectangle (m_pStyle->GetTotalCoverRect (rdc));

	rdc.m_pDC->SelectObject (pOldPen);
	rdc.m_pDC->SelectObject (pOldBrush);
}

void CLabelRenderer::DrawShade(CRenderDC &rdc, RenderType rt)
{
	CRect* pR1;
	switch (rt)
	{
	case Cover:
	case Back:
		pR1 = m_pStyle->GetTotalCoverRect (rdc);
		break;
	case Inlay:
		pR1 = m_pStyle->GetTotalInlayRect (rdc);
		break;
	}

	CBrush brush (RGB (0x60, 0x60, 0x60));

	int nOffset = max (pR1->Width () / 50, 2);
	CRect r;
	
	r.left = pR1->right;
	r.top = pR1->top + nOffset;
	r.right = r.left + nOffset;
	r.bottom = pR1->bottom + nOffset;
	rdc.m_pDC->FillRect (&r, &brush);

	r.left = pR1->left + nOffset;
	r.top = pR1->bottom;
	r.right = pR1->right + nOffset;
	r.bottom = r.top + nOffset;
	rdc.m_pDC->FillRect (&r, &brush);

	brush.DeleteObject ();
}
