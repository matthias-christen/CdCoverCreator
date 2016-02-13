// LabelRenderer.h: interface for the CLabelRenderer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LABELRENDERER_H__B7D130D5_66FA_478D_8EF2_9D2743A944BD__INCLUDED_)
#define AFX_LABELRENDERER_H__B7D130D5_66FA_478D_8EF2_9D2743A944BD__INCLUDED_

#include "TracksInfo.h"	// Added by ClassView
#include "DataCDInfo.h"
#include "StyleTemplate.h"	// Added by ClassView
#include "BackgroundInfo.h"	// Added by ClassView
#include "FloatingGraphicsObject.h"
#include <list>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLabelRenderer  
{
public:
	void Render (CRenderDC rdc, RenderType rt, bool bIsPreviewing = false, bool bIsPrinting = false, bool bDrawTicks = true, bool bDrawCutLinesWhenPrinting = false);
	CLabelRenderer(CTracksInfo* pTracks, CDataCDInfo* pDataCDInfo, CBackgroundInfo** bkgrnd, std::list<CFloatingGraphicsObject*>* pListFloatObj, CStyleTemplate& style);
	virtual ~CLabelRenderer();

protected:
	void DrawShade (CRenderDC& rdc, RenderType rt);
	void DrawCoverNonprintMargins (CRenderDC& rdc, CPen& penSolid);
	void DrawCoverPrintMargins(CRenderDC& rdc, RenderType rt);
	void DrawInlayNonprintMargins (CRenderDC& rdc, CPen& penSolid);
	void DrawInlayPrintMargins (CRenderDC& rdc);

	CBackgroundInfo* m_pBackground[NUM_RENDER_TYPES];
	CStyleTemplate* m_pStyle;
	CTracksInfo* m_pTracks;
	CDataCDInfo* m_pDataCDInfo;
	std::list<CFloatingGraphicsObject*>* m_pListFloatingObjs[NUM_RENDER_TYPES];
};

#endif // !defined(AFX_LABELRENDERER_H__B7D130D5_66FA_478D_8EF2_9D2743A944BD__INCLUDED_)
