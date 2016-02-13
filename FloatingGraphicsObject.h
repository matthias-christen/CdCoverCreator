#pragma once
#include "afx.h"

#include "StyleTemplate.h"
#include "RenderDC.h"
#include "AutoRichEditCtrl.h"
#include "FloatObjRectTracker.h"

class CFloatingGraphicsObject :	public CObject
{
public:
	CFloatingGraphicsObject(void);
	DECLARE_DYNCREATE(CFloatingGraphicsObject)

	~CFloatingGraphicsObject(void);

	void Render (CRenderDC rdc, RenderType rt, CStyleTemplate* pStyle, bool bIsPreviewing = false, bool bIsPrinting = false);
	virtual void Render (CRenderDC rdc, RenderType rt, CRect rectRender, bool bIsPrinting);

	void DrawTracker (CRenderDC rdc, RenderType rt, CStyleTemplate* pStyle, CSize sizeOffset);

	// adds an object (rubber banding)
	virtual bool Add (CWnd* pWnd, CPoint pt, RenderType rt, CRenderDC rdc, CStyleTemplate* pStyle, CAutoRichEditCtrl* pEditCtrl, CSize sizeOffset);

	// un-selects the object
	virtual void Deselect ();

	// track this object
	int HitTest (CPoint pt);
	virtual bool Track (CWnd* pWnd, CPoint pt, RenderType rt, CRenderDC rdc, CStyleTemplate* pStyle, CSize sizeOffset);
	inline CRectTracker* GetTracker () { return &m_tracker; }

	// hit test
	virtual bool SetCursor (CWnd* pWnd, UINT nHitTest);

	// edit the object
	virtual bool Edit (CAutoRichEditCtrl* pEditCtrl);
	virtual void EndEdit (CAutoRichEditCtrl* pEditCtrl);

	virtual bool ContextMenu (CView* pView);

	void Serialize (CArchive& ar);

	// measures functions
	void SetTopLeft (double x, double y) { m_dLeft = x; m_dTop = y; }
	void SetSize (double w, double h) { m_dWidth = w; m_dHeight = h; }

	double GetLeft () { return m_dLeft; }
	double GetTop () { return m_dTop; }
	double GetWidth () { return m_dWidth; }
	double GetHeight () { return m_dHeight; }

protected:
	// measures
	double m_dLeft, m_dTop, m_dWidth, m_dHeight;

	UINT m_nContextMenuID;

	// the rect tracker
	CFloatObjRectTracker m_tracker;
};
