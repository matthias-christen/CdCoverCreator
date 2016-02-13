#pragma once
#include "floatinggraphicsobject.h"
#include "Picture.h"

class CFloatingImage : public CFloatingGraphicsObject
{
public:
	CFloatingImage(void);
	DECLARE_DYNCREATE(CFloatingImage)

	~CFloatingImage(void);

	virtual void Render (CRenderDC rdc, RenderType rt, CRect rectRender, bool bIsPrinting);
	bool Add (CWnd* pWnd, CPoint pt, RenderType rt, CRenderDC rdc, CStyleTemplate* pStyle, CAutoRichEditCtrl* pEditCtrl, CSize sizeOffset);

	void SetTransparentColor (COLORREF crTransparent);

	void Serialize (CArchive& ar);

	inline CPicture* GetPicture () { return m_pPicture; }

protected:
	CPicture* m_pPicture;
	CString m_strImgFilename;
	COLORREF m_crTransparent;
};
