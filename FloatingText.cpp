#include "StdAfx.h"
#include "floatingtext.h"
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CFloatingText, CFloatingGraphicsObject)

CFloatingText::CFloatingText(void)
{
	// create the RTF
	m_pFormattedText = new CFormattedTextDraw ();
	m_pFormattedText->Create ();

	m_nContextMenuID = IDR_MENU_FLOATINGTEXT;
}

CFloatingText::~CFloatingText(void)
{
	delete m_pFormattedText;
}

void CFloatingText::Render (CRenderDC rdc, RenderType rt, CRect rectRender, bool bIsPrinting)
{
	m_pFormattedText->put_Height (m_dHeight * 1000);
	m_pFormattedText->Draw (rdc.m_pDC->GetSafeHdc (), &rectRender);
}

bool CFloatingText::Edit (CAutoRichEditCtrl* pEditCtrl)
{
	pEditCtrl->MoveWindow (&m_tracker.m_rect);
	pEditCtrl->ShowWindow (SW_SHOW);

	BSTR bstrRTF;
	m_pFormattedText->get_RTFText (&bstrRTF);
	CString strRTF (bstrRTF);

	pEditCtrl->SetRTF (strRTF);
	pEditCtrl->SetFocus ();

	// attach the control to the RTF toolbar
	((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (pEditCtrl);

	return true;
}

void CFloatingText::EndEdit (CAutoRichEditCtrl* pEditCtrl)
{
	pEditCtrl->ShowWindow (SW_HIDE);

	BSTR bstrText = pEditCtrl->GetRTF ().AllocSysString ();
	m_pFormattedText->put_RTFText (bstrText);
	::SysFreeString (bstrText);

	// detach the control from the RTF toolbar
	((CMainFrame*) AfxGetMainWnd ())->m_wndRTFToolbar.SetRTFControl (NULL);
}

void CFloatingText::Serialize (CArchive& ar)
{
	CFloatingGraphicsObject::Serialize (ar);

	if (ar.IsStoring ())
	{
		BSTR bstrText;
		m_pFormattedText->get_RTFText (&bstrText);
		CString strText (bstrText);

		ar << strText;
	}
	else
	{
		CString strText;
		ar >> strText;

		BSTR bstrText = strText.AllocSysString ();
		m_pFormattedText->put_RTFText (bstrText);
		::SysFreeString (bstrText);
	}
}