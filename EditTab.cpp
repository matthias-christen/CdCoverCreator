// EditTab.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "EditTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditTab

CEditTab::CEditTab()
{
}

CEditTab::~CEditTab()
{
}


BEGIN_MESSAGE_MAP(CEditTab, CEdit)
	//{{AFX_MSG_MAP(CEditTab)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTab message handlers

void CEditTab::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_TAB)
	{
		CWnd* pWndNext = GetNextWindow ((::GetKeyState (VK_SHIFT) & 0x8000) ? GW_HWNDPREV : GW_HWNDNEXT);
		pWndNext->SetFocus ();
		pWndNext->SendMessage (EM_SETSEL, 0, -1);
	}
	else
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditTab::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar != VK_TAB)
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CEditTab::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (::GetKeyState (VK_CONTROL) & 0x8000)
		{
			switch (pMsg->wParam)
			{
			case 'C':
				Copy (); return TRUE;
			case 'V':
				Paste (); return TRUE;
			case 'X':
				Cut (); return TRUE;
			}
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
