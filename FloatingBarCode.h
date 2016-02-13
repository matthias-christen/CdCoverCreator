#pragma once
#include "floatinggraphicsobject.h"
#include "EAN8.h"
#include "EAN13.h"

using namespace Gdiplus;

class CFloatingBarCode : public CFloatingGraphicsObject
{
public:
	CFloatingBarCode(void);
	DECLARE_DYNCREATE(CFloatingBarCode)

	~CFloatingBarCode(void);

	virtual void Render (CRenderDC rdc, RenderType rt, CRect rectRender, bool bIsPrinting);
	bool Add (CWnd* pWnd, CPoint pt, RenderType rt, CRenderDC rdc, CStyleTemplate* pStyle, CAutoRichEditCtrl* pEditCtrl, CSize sizeOffset);
	virtual bool Edit (CAutoRichEditCtrl* pEditCtrl);
	virtual void EndEdit (CAutoRichEditCtrl* pEditCtrl);

	void Serialize (CArchive& ar);

	inline void SetProperties (int nType, COLORREF colForeground, COLORREF colBackground, bool bTransparentBackground, CString strFont)
	{
		m_ean8.SetForegroundColor (colForeground);
		m_ean8.SetBackground (bTransparentBackground ? TRANSPARENT : OPAQUE, colBackground);
		m_ean8.SetFont (strFont);

		m_ean13.SetForegroundColor (colForeground);
		m_ean13.SetBackground (bTransparentBackground ? TRANSPARENT : OPAQUE, colBackground);
		m_ean13.SetFont (strFont);

		m_nBarCodeType = nType;
		switch (nType)
		{
		case 0:
			m_pCurrentEAN = &m_ean13;
			break;
		case 1:
			m_pCurrentEAN = &m_ean8;
			break;
		}

		RenderBarCode ();
	}

	inline int GetType () { return m_nBarCodeType; }
	inline COLORREF GetForegroundColor () { return m_pCurrentEAN->GetForegroundColor (); }
	inline COLORREF GetBackgroundColor () { return m_pCurrentEAN->GetBackgroundColor (); }
	inline int GetBackgroundMode () { return m_pCurrentEAN->GetBackgroundMode (); }
	inline CString GetFont () { return m_pCurrentEAN->GetFont (); }

protected:
	void RenderBarCode ();

	CEAN8 m_ean8;
	CEAN13 m_ean13;
	CEANBase* m_pCurrentEAN;
	int m_nBarCodeType;

	Bitmap* m_pImage;
};
