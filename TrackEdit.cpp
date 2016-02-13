// TrackEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "TrackEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackEdit

CTrackEdit::CTrackEdit()
{
	m_col = 0;
}

CTrackEdit::~CTrackEdit()
{
	m_font.DeleteObject ();
}


BEGIN_MESSAGE_MAP(CTrackEdit, CAutoRichEditCtrl)
	//{{AFX_MSG_MAP(CTrackEdit)
	ON_WM_KEYDOWN()
//	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackEdit message handlers

void CTrackEdit::SetAppearance(COLORREF col, LOGFONT *pLogfont, DWORD dwAlign)
{
	m_font.DeleteObject ();
	m_font.CreateFontIndirect (pLogfont);
	SetFont (&m_font);

	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = col;
	cf.dwEffects = 0;
	SetSelectionCharFormat (cf);

	if (dwAlign & DT_CENTER)
		SetParagraphCenter ();
	else if (dwAlign & DT_RIGHT)
		SetParagraphRight ();
	else
		SetParagraphLeft ();

//	SetMargins (0, 0);
}

CString CTrackEdit::GetText()
{
	CString s;
	GetWindowText (s);
	return s;
}

void CTrackEdit::SetText(CString strText)
{
	SetWindowText (strText);
}

void CTrackEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_ESCAPE:
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), EN_ESCAPE), (LPARAM) GetSafeHwnd ());
		break;
	case VK_UP:
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), EN_UP), (LPARAM) GetSafeHwnd ());
		break;
	case VK_DOWN:
		GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), EN_DOWN), (LPARAM) GetSafeHwnd ());
		break;
	case VK_TAB:
		if (::GetKeyState (VK_SHIFT) & 0x8000)
			GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), EN_PREV), (LPARAM) GetSafeHwnd ());
		else
			GetParent ()->SendMessage (WM_COMMAND, MAKELONG (GetDlgCtrlID (), EN_NEXT), (LPARAM) GetSafeHwnd ());
		break;
	default:
		CAutoRichEditCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}
/*
HBRUSH CTrackEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor (::GetSysColor (COLOR_WINDOW));
	pDC->SetTextColor (m_col);
	return ::GetSysColorBrush (COLOR_WINDOW);
}*/

void CTrackEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_ESCAPE:
	case VK_UP:
	case VK_DOWN:
	case VK_TAB:
		return;
	default:
		CAutoRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	}
}
