#include "StdAfx.h"
#include "MainFrm.h"
#include ".\floatingbarcode.h"

IMPLEMENT_DYNCREATE(CFloatingBarCode, CFloatingGraphicsObject)

CFloatingBarCode::CFloatingBarCode(void)
{
	m_pCurrentEAN = &m_ean13;
	m_nBarCodeType = 0;	// EAN13
	m_nContextMenuID = IDR_MENU_FLOATINGBARCODE;

	m_tracker.CanResize (false);

	m_pImage = NULL;
}

CFloatingBarCode::~CFloatingBarCode(void)
{
	if (m_pImage != NULL)
		delete m_pImage;
}

void CFloatingBarCode::Render (CRenderDC rdc, RenderType rt, CRect rectRender, bool bIsPrinting)
{
	if (m_pImage == NULL)
		return;

	Rect rcDest (rectRender.left, rectRender.top, rectRender.Width (), rectRender.Height ());

	if (bIsPrinting)
	{
		Graphics graphics (rdc.m_pDC->GetSafeHdc (), rdc.m_hndPrinter);
		graphics.SetPageUnit (UnitPixel);
		graphics.DrawImage (m_pImage, rcDest, 0, 0, m_pImage->GetWidth (), m_pImage->GetHeight (), UnitPixel);
	}
	else
	{
		Graphics graphics (rdc.m_pDC->GetSafeHdc ());
		graphics.SetPageUnit (UnitPixel);
		graphics.DrawImage (m_pImage, rcDest, 0, 0, m_pImage->GetWidth (), m_pImage->GetHeight (), UnitPixel);
	}
}

bool CFloatingBarCode::Add (CWnd* pWnd, CPoint pt, RenderType rt, CRenderDC rdc, CStyleTemplate* pStyle, CAutoRichEditCtrl* pEditCtrl, CSize sizeOffset)
{
	CFloatingGraphicsObject::Add (pWnd, pt, rt, rdc, pStyle, pEditCtrl, sizeOffset);

	m_pCurrentEAN->SetCode ("0000000000000");
	RenderBarCode ();

	return true;
}

bool CFloatingBarCode::Edit (CAutoRichEditCtrl* pEditCtrl)
{
	pEditCtrl->MoveWindow (&m_tracker.m_rect);
	pEditCtrl->ShowWindow (SW_SHOW);

	pEditCtrl->SetWindowText (m_pCurrentEAN->GetCode ());
	pEditCtrl->SetFocus ();

	// attach the control to the RTF toolbar
	((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (pEditCtrl);

	return true;
}

void CFloatingBarCode::EndEdit (CAutoRichEditCtrl* pEditCtrl)
{
	pEditCtrl->ShowWindow (SW_HIDE);

	CString strCode;
	pEditCtrl->GetWindowText (strCode);
	m_ean8.SetCode (strCode);
	m_ean13.SetCode (strCode);

	// detach the control from the RTF toolbar
	((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (NULL);

	RenderBarCode ();
}

void CFloatingBarCode::RenderBarCode ()
{
	if (m_pImage != NULL)
		delete m_pImage;

	CRect r (0, 0, 0, 0);
	CSize size = m_pCurrentEAN->GetSize ();
	r.right = size.cx / 10;
	r.bottom = size.cy / 10;

	m_dWidth = (double) size.cx / 1000.0;
	m_dHeight = (double) size.cy / 1000.0;

	m_pImage = new Bitmap (r.Width (), r.Height (), PixelFormat32bppARGB);
	Graphics g (m_pImage);
	m_pCurrentEAN->Render (&g, r);
}

void CFloatingBarCode::Serialize (CArchive& ar)
{
	CFloatingGraphicsObject::Serialize (ar);

	if (ar.IsStoring ())
	{
		ar << m_nBarCodeType;
		ar << m_pCurrentEAN->GetCode ();
		ar << m_pCurrentEAN->GetForegroundColor () << m_pCurrentEAN->GetBackgroundColor () << m_pCurrentEAN->GetBackgroundMode ();
		ar << m_pCurrentEAN->GetFont ();
	}
	else
	{
		ar >> m_nBarCodeType;

		CString strCode;
		ar >> strCode;

		switch (m_nBarCodeType)
		{
		case 0:	// EAN13
			m_pCurrentEAN = &m_ean13;
			break;
		case 1:	// EAN8
			m_pCurrentEAN = &m_ean8;
			break;
		}

		m_pCurrentEAN->SetCode (strCode);

		COLORREF colFore, colBack;
		int nMode;
		CString strFont;
		ar >> colFore >> colBack >> nMode;
		ar >> strFont;

		SetProperties (m_nBarCodeType, colFore, colBack, nMode == TRANSPARENT, strFont);
	}
}