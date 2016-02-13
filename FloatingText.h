#pragma once
#include "floatinggraphicsobject.h"
#include "FormattedTextDraw.h"

class CFloatingText :
	public CFloatingGraphicsObject
{
public:
	CFloatingText(void);
	DECLARE_DYNCREATE(CFloatingText)

	~CFloatingText(void);

	virtual void Render (CRenderDC rdc, RenderType rt, CRect rectRender, bool bIsPrinting);
	virtual bool Edit (CAutoRichEditCtrl* pEditCtrl);
	virtual void EndEdit (CAutoRichEditCtrl* pEditCtrl);

	void Serialize (CArchive& ar);

protected:
	CFormattedTextDraw* m_pFormattedText;
};
