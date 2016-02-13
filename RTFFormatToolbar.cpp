// RTFFormatToolbar.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "RTFFormatToolbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRTFFormatToolbar

CRTFFormatToolbar::CRTFFormatToolbar()
{
}

CRTFFormatToolbar::~CRTFFormatToolbar()
{
	m_font.DeleteObject ();
}


BEGIN_MESSAGE_MAP(CRTFFormatToolbar, CToolBar)
	//{{AFX_MSG_MAP(CRTFFormatToolbar)
	ON_UPDATE_COMMAND_UI(ID_RTFF_BOLD, OnUpdateBold)
	ON_UPDATE_COMMAND_UI(ID_RTFF_ITALIC, OnUpdateItalic)
	ON_UPDATE_COMMAND_UI(ID_RTFF_UNDERLINE, OnUpdateUnderline)
	ON_UPDATE_COMMAND_UI(ID_RTFF_UNDERLINE_DOT, OnUpdateUnderlineDot)
	ON_UPDATE_COMMAND_UI(ID_RTFF_STRIKETHRU, OnUpdateStrikethru)
	ON_UPDATE_COMMAND_UI(ID_RTFF_ALIGNLEFT, OnUpdateAlignleft)
	ON_UPDATE_COMMAND_UI(ID_RTFF_ALIGNCENTER, OnUpdateAligncenter)
	ON_UPDATE_COMMAND_UI(ID_RTFF_ALIGNRIGHT, OnUpdateAlignright)
	ON_UPDATE_COMMAND_UI(ID_RTFF_SUP, OnUpdateSup)
	ON_UPDATE_COMMAND_UI(ID_RTFF_SUB, OnUpdateSub)
	ON_UPDATE_COMMAND_UI(ID_RTFF_FONT, OnUpdateFont)
	ON_UPDATE_COMMAND_UI(ID_RTFF_FONTSIZE, OnUpdateFontsize)
	ON_UPDATE_COMMAND_UI(ID_RTFF_COLOR, OnUpdateColor)
	ON_UPDATE_COMMAND_UI(ID_RTFF_BKCOLOR, OnUpdateBkColor)
	ON_UPDATE_COMMAND_UI(ID_RTFF_BORDER, OnUpdateBorder)
	ON_UPDATE_COMMAND_UI(ID_RTFF_BULLETS, OnUpdateBullets)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(ID_RTFF_FONT, OnFontChanged)
	ON_CBN_SELCHANGE(ID_RTFF_FONTSIZE, OnFontSizeChanged)
	ON_CBN_SELENDOK(ID_RTFF_FONTSIZE, OnFontSizeSelendok)
	ON_CBN_SELENDCANCEL(ID_RTFF_FONTSIZE, OnFontSizeSelendcancel)
	ON_CBN_EDITCHANGE(ID_RTFF_FONTSIZE, OnFontSizeEditChange)
	ON_MESSAGE(CPN_SELENDOK, OnColorChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRTFFormatToolbar message handlers

bool CRTFFormatToolbar::CreateBar(CWnd *pParentWnd, DWORD dwStyle, UINT nId)
{
	CreateEx (pParentWnd, TBSTYLE_FLAT, dwStyle, CRect (0, 0, 0, 0), nId);
	LoadToolBar (IDR_TOOLBAR_RTFFORMAT);
	GetToolBarCtrl ().SetExtendedStyle (TBSTYLE_EX_DRAWDDARROWS);

	// add arrows
	TBBUTTONINFO tbi;
	tbi.dwMask = TBIF_STYLE;
	tbi.cbSize = sizeof (TBBUTTONINFO);

/*	GetToolBarCtrl ().GetButtonInfo (ID_RTFF_BULLETS, &tbi);
	tbi.fsStyle |= TBSTYLE_DROPDOWN;
	GetToolBarCtrl ().SetButtonInfo (ID_RTFF_BULLETS, &tbi);

	GetToolBarCtrl ().GetButtonInfo (ID_RTFF_BORDER, &tbi);
	tbi.fsStyle |= TBSTYLE_DROPDOWN;
	GetToolBarCtrl ().SetButtonInfo (ID_RTFF_BORDER, &tbi);
*/
	// add controls
	SetButtonInfo (CommandToIndex (ID_RTFF_FONT), ID_RTFF_FONT, TBBS_SEPARATOR, 150);
	SetButtonInfo (CommandToIndex (ID_RTFF_FONTSIZE), ID_RTFF_FONTSIZE, TBBS_SEPARATOR, 50);
	SetButtonInfo (CommandToIndex (ID_RTFF_COLOR), ID_RTFF_COLOR, TBBS_SEPARATOR, 70);
	SetButtonInfo (CommandToIndex (ID_RTFF_BKCOLOR), ID_RTFF_BKCOLOR, TBBS_SEPARATOR, 70);

	CRect rect;

	GetItemRect (CommandToIndex (ID_RTFF_FONT), &rect);
	rect.bottom += 150;
	m_wndComboFont.Create (WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN |
		CBS_OWNERDRAWFIXED | CBS_HASSTRINGS | CBS_SORT, rect, this, ID_RTFF_FONT);
	
	GetItemRect (CommandToIndex (ID_RTFF_FONTSIZE), &rect);
	rect.bottom += 150;
	m_wndComboFontSize.Create (WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN, rect, this, ID_RTFF_FONTSIZE);
	
	GetItemRect (CommandToIndex (ID_RTFF_COLOR), &rect);
	CString str, strDefault, strCustom;
	str.LoadString (IDS_TEXT);
	strDefault.LoadString (IDS_COL_DEFAULT);
	strCustom.LoadString (IDS_COL_CUSTOM);
	m_wndColorPicker.Create (str, WS_CHILD | WS_VISIBLE, rect, this, ID_RTFF_COLOR);
	m_wndColorPicker.SetDefaultText (strDefault);
	m_wndColorPicker.SetCustomText (strCustom);

	GetItemRect (CommandToIndex (ID_RTFF_BKCOLOR), &rect);
	str.LoadString (IDS_BACKGROUND);
	m_wndColorPickerBk.Create (str, WS_CHILD | WS_VISIBLE, rect, this, ID_RTFF_BKCOLOR);
	m_wndColorPickerBk.SetDefaultText (strDefault);
	m_wndColorPickerBk.SetCustomText (strCustom);

    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof (NONCLIENTMETRICS);
    VERIFY (SystemParametersInfo (SPI_GETNONCLIENTMETRICS, sizeof (NONCLIENTMETRICS), &ncm, 0));
    m_font.CreateFontIndirect (&ncm.lfMessageFont);

	m_wndComboFont.SetFont (&m_font);
	m_wndComboFontSize.SetFont (&m_font);
	m_wndColorPicker.SetFont (&m_font);
	m_wndColorPickerBk.SetFont (&m_font);

	m_wndComboFont.SetItemHeight (-1, m_wndComboFontSize.GetItemHeight (-1));

	char* szFontSizes[] = {"8", "9", "10", "11", "12", "14", "16", "18",
		"20", "22", "24", "26", "28", "32", "36", "40", "48", "60", "72", 0};
	for (int i = 0; szFontSizes[i]; i++)
		m_wndComboFontSize.AddString (szFontSizes[i]);

	return true;
}

void CRTFFormatToolbar::SetRTFControl(CRichEditCtrl *pEdit)
{
	m_pEdit = pEdit;
	//((CReBarCtrl*) GetParent ())->ShowBand (2, pEdit != NULL);
}

void CRTFFormatToolbar::OnUnderline() 
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
	m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);

	cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
	cf.dwEffects = ((cf.bUnderlineType == CFU_UNDERLINE) && (cf.dwEffects & CFE_UNDERLINE)) ? 0 : CFE_UNDERLINE;
	cf.bUnderlineType = CFU_UNDERLINE;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

void CRTFFormatToolbar::OnUnderlineDot() 
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
	m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);

	cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
	cf.dwEffects = ((cf.bUnderlineType == CFU_UNDERLINEDOTTED) && (cf.dwEffects & CFE_UNDERLINE)) ? 0 : CFU_UNDERLINEDOTTED;
	cf.bUnderlineType = CFU_UNDERLINEDOTTED;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}
/*
void CRTFFormatToolbar::OnUnderlineDouble() 
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
	m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);

	cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
	cf.dwEffects = ((cf.bUnderlineType == CFU_UNDERLINEDOUBLE) && (cf.dwEffects & CFE_UNDERLINE)) ? 0 : CFU_UNDERLINEDOUBLE;
	cf.bUnderlineType = CFU_UNDERLINEDOUBLE;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}*/

void CRTFFormatToolbar::OnAlignleft() 
{
	PARAFORMAT pf;
	pf.cbSize = sizeof (PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT;
	pf.wAlignment = PFA_LEFT;

	m_pEdit->SendMessage (EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}

void CRTFFormatToolbar::OnAligncenter() 
{
	PARAFORMAT pf;
	pf.cbSize = sizeof (PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT;
	pf.wAlignment = PFA_CENTER;

	m_pEdit->SendMessage (EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}

void CRTFFormatToolbar::OnAlignright() 
{
	PARAFORMAT pf;
	pf.cbSize = sizeof (PARAFORMAT);
	pf.dwMask = PFM_ALIGNMENT;
	pf.wAlignment = PFA_RIGHT;

	m_pEdit->SendMessage (EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}

void CRTFFormatToolbar::OnStrikethru() 
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_STRIKEOUT;
	m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);

	cf.dwMask = CFM_STRIKEOUT;
	cf.dwEffects = cf.dwEffects & CFE_STRIKEOUT ? 0 : CFE_STRIKEOUT;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

void CRTFFormatToolbar::OnSuperScript() 
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_SUPERSCRIPT;
	m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);

	cf.dwMask = CFM_SUPERSCRIPT;
	cf.dwEffects = cf.dwEffects & CFE_SUPERSCRIPT ? 0 : CFE_SUPERSCRIPT;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

void CRTFFormatToolbar::OnSubScript() 
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_SUBSCRIPT;
	m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);

	cf.dwMask = CFM_SUBSCRIPT;
	cf.dwEffects = cf.dwEffects & CFE_SUBSCRIPT ? 0 : CFE_SUBSCRIPT;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

void CRTFFormatToolbar::OnBold() 
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_WEIGHT;
	m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);

	cf.dwMask = CFM_WEIGHT;
	cf.wWeight = cf.wWeight == FW_BOLD ? FW_NORMAL : FW_BOLD;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

void CRTFFormatToolbar::OnItalic() 
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_ITALIC;
	m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);

	cf.dwMask = CFM_ITALIC;
	cf.dwEffects = cf.dwEffects & CFE_ITALIC ? 0 : CFE_ITALIC;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

void CRTFFormatToolbar::OnUpdateBold(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_WEIGHT;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		pCmdUI->SetCheck (cf.wWeight == FW_BOLD);
	}
}

void CRTFFormatToolbar::OnUpdateItalic(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_ITALIC;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		pCmdUI->SetCheck (cf.dwEffects & CFE_ITALIC ? 1 : 0);
	}
}

void CRTFFormatToolbar::OnUpdateUnderline(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		pCmdUI->SetCheck ((cf.bUnderlineType == CFU_UNDERLINE) && (cf.dwEffects & CFE_UNDERLINE));
	}
}

void CRTFFormatToolbar::OnUpdateUnderlineDot(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		pCmdUI->SetCheck ((cf.bUnderlineType == CFU_UNDERLINEDOTTED) && (cf.dwEffects & CFE_UNDERLINE));
	}
}
/*
void CRTFFormatToolbar::OnUpdateUnderlineDouble(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_UNDERLINETYPE | CFM_UNDERLINE;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		pCmdUI->SetCheck ((cf.bUnderlineType == CFU_UNDERLINEDOUBLE) && (cf.dwEffects & CFE_UNDERLINE));
	}
}*/

void CRTFFormatToolbar::OnUpdateStrikethru(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_STRIKEOUT;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		pCmdUI->SetCheck (cf.dwEffects & CFE_STRIKEOUT ? 1 : 0);
	}
}

void CRTFFormatToolbar::OnUpdateAlignleft(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		PARAFORMAT pf;
		pf.cbSize = sizeof (PARAFORMAT);
		pf.dwMask = PFM_ALIGNMENT;
		m_pEdit->SendMessage (EM_GETPARAFORMAT, 0, (LPARAM) &pf);
		pCmdUI->SetCheck (pf.wAlignment == PFA_LEFT);
	}
}

void CRTFFormatToolbar::OnUpdateAligncenter(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		PARAFORMAT pf;
		pf.cbSize = sizeof (PARAFORMAT);
		pf.dwMask = PFM_ALIGNMENT;
		m_pEdit->SendMessage (EM_GETPARAFORMAT, 0, (LPARAM) &pf);
		pCmdUI->SetCheck (pf.wAlignment == PFA_CENTER);
	}
}

void CRTFFormatToolbar::OnUpdateAlignright(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		PARAFORMAT pf;
		pf.cbSize = sizeof (PARAFORMAT);
		pf.dwMask = PFM_ALIGNMENT;
		m_pEdit->SendMessage (EM_GETPARAFORMAT, 0, (LPARAM) &pf);
		pCmdUI->SetCheck (pf.wAlignment == PFA_RIGHT);
	}
}

void CRTFFormatToolbar::OnUpdateSup(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_SUPERSCRIPT;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		pCmdUI->SetCheck (cf.dwEffects & CFE_SUPERSCRIPT ? 1 : 0);
	}
}

void CRTFFormatToolbar::OnUpdateSub(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_SUBSCRIPT;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		pCmdUI->SetCheck (cf.dwEffects & CFE_SUBSCRIPT ? 1 : 0);
	}
}

void CRTFFormatToolbar::OnUpdateFont(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();
	if (GetFocus ()->GetSafeHwnd () != m_pEdit->GetSafeHwnd ())
		return;

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_FACE;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		m_wndComboFont.SelectString (-1, cf.szFaceName);
	}
}

void CRTFFormatToolbar::OnUpdateFontsize(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();
	if (GetFocus ()->GetSafeHwnd () != m_pEdit->GetSafeHwnd ())
		return;

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_SIZE;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, 1, (LPARAM) &cf);
		SetDlgItemInt (ID_RTFF_FONTSIZE, cf.yHeight / 20);
	}
}

void CRTFFormatToolbar::OnUpdateColor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();
	if (GetFocus ()->GetSafeHwnd () != m_pEdit->GetSafeHwnd ())
		return;

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_COLOR;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);

		// only set new color if neccessary (avoids flickering)
		COLORREF colNew = (cf.dwEffects & CFE_AUTOCOLOR) ?	// bk color has not changed
			CLR_DEFAULT : cf.crTextColor;
		if (m_wndColorPicker.GetColour () != colNew)
			m_wndColorPicker.SetColour (colNew);
	}
}

void CRTFFormatToolbar::OnUpdateBkColor (CCmdUI* pCmdUI)
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();
	if (GetFocus ()->GetSafeHwnd () != m_pEdit->GetSafeHwnd ())
		return;

	if (m_pEdit != NULL)
	{
		CHARFORMAT2 cf;
		cf.cbSize = sizeof (CHARFORMAT2);
		cf.dwMask = CFM_BACKCOLOR;
		cf.crBackColor = 0xffffffff;
		m_pEdit->SendMessage (EM_GETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);

		// only set new color if neccessary (avoids flickering)
		COLORREF colNew = (cf.dwEffects & CFE_AUTOBACKCOLOR) ?
			CLR_DEFAULT : cf.crBackColor;
		if (m_wndColorPickerBk.GetColour () != colNew)
			m_wndColorPickerBk.SetColour (colNew);
	}
}

void CRTFFormatToolbar::OnFontChanged()
{
	LOGFONT lf;
	m_wndComboFont.GetSelFont (lf);

	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_FACE;
	strcpy (cf.szFaceName, lf.lfFaceName);

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
	m_pEdit->SetFocus ();
}

void CRTFFormatToolbar::OnFontSizeChanged ()
{
	CString s;
	m_wndComboFontSize.GetLBText (m_wndComboFontSize.GetCurSel (), s);
	int nHeight = atoi (s) * 20;

	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_SIZE;
	cf.yHeight = nHeight;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
	//m_pEdit->SetFocus ();
}

void CRTFFormatToolbar::OnFontSizeSelendok ()
{
	m_pEdit->SetFocus ();
}

void CRTFFormatToolbar::OnFontSizeSelendcancel ()
{
	m_pEdit->SetFocus ();
}

void CRTFFormatToolbar::OnFontSizeEditChange ()
{
	CString s;
	m_wndComboFontSize.GetWindowText (s);
	int nHeight = atoi (s) * 20;

	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);
	cf.dwMask = CFM_SIZE;
	cf.yHeight = nHeight;

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
}

LRESULT CRTFFormatToolbar::OnColorChanged (WPARAM wParam, LPARAM lParam)
{
	CHARFORMAT2 cf;
	cf.cbSize = sizeof (CHARFORMAT2);

	switch (lParam)
	{
	case ID_RTFF_COLOR:
		cf.dwMask = CFM_COLOR;
		cf.crTextColor = m_wndColorPicker.GetColour ();
		cf.dwEffects = cf.crTextColor == CLR_DEFAULT ? CFE_AUTOCOLOR : 0;
		break;

	case ID_RTFF_BKCOLOR:
		cf.dwMask = CFM_BACKCOLOR;
		cf.crBackColor = m_wndColorPickerBk.GetColour ();
		//cf.crBackColor = cf.crBackColor == CLR_DEFAULT ? ::GetSysColor (COLOR_WINDOW) : cf.crBackColor;
		//cf.dwEffects = 0;
		cf.dwEffects = cf.crBackColor == CLR_DEFAULT ? CFE_AUTOBACKCOLOR : 0;
		break;
	}

	m_pEdit->SendMessage (EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
	m_pEdit->SetFocus ();

	return S_OK;
}

void CRTFFormatToolbar::OnBullets ()
{
	RECT r;
	GetItemRect (CommandToIndex (ID_RTFF_BULLETS), &r);
	ClientToScreen (&r);
	
	CMenu menu, *pPopup;
	VERIFY (menu.LoadMenu (IDR_MENU_RTF_BULLETS));
	
	pPopup = (CMenu*) menu.GetSubMenu (0);
	ASSERT (pPopup != NULL);

	PARAFORMAT pf;
	pf.cbSize = sizeof (PARAFORMAT);
	pf.dwMask = PFM_NUMBERING;
	m_pEdit->SendMessage (EM_GETPARAFORMAT, 0, (LPARAM) &pf);
	pPopup->CheckMenuItem (pf.wNumbering + ID_RTFF_BULLETS_NONE, MF_BYCOMMAND | MF_CHECKED);
	
	pPopup->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, r.left, r.bottom, this);
}

void CRTFFormatToolbar::OnBorder ()
{
}

void CRTFFormatToolbar::OnPagebreak ()
{
	TRACE ("RichEditVer: %d\n", _RICHEDIT_VER);

	PARAFORMAT2 pf;
	pf.cbSize = sizeof (PARAFORMAT2);
	pf.dwMask = PFM_PAGEBREAKBEFORE;
//	pf.wEffects = PFE_PAGEBREAKBEFORE;
	pf.wReserved = PFE_PAGEBREAKBEFORE;
	m_pEdit->SetParaFormat (pf);
}

void CRTFFormatToolbar::OnUpdateBorder(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		PARAFORMAT2 pf;
		pf.cbSize = sizeof (PARAFORMAT2);
		pf.dwMask = PFM_BORDER;
		m_pEdit->SendMessage (EM_GETPARAFORMAT, 0, (LPARAM) &pf);
		pCmdUI->SetCheck (pf.wBorders != 0);
	}
}

void CRTFFormatToolbar::OnUpdateBullets(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (m_pEdit != NULL);
	UpdateVisibility ();

	if (m_pEdit != NULL)
	{
		PARAFORMAT pf;
		pf.cbSize = sizeof (PARAFORMAT);
		pf.dwMask = PFM_NUMBERING;
		m_pEdit->SendMessage (EM_GETPARAFORMAT, 0, (LPARAM) &pf);
		pCmdUI->SetCheck (pf.wNumbering != 0);
	}
}

void CRTFFormatToolbar::UpdateVisibility()
{
	if (m_pEdit == NULL)
		SetRTFControl (NULL);
}

void CRTFFormatToolbar::OnBulletsArrow()
{
}

void CRTFFormatToolbar::OnBorderArrow()
{
	RECT r;
	GetItemRect (CommandToIndex (ID_RTFF_BORDER), &r);
	ClientToScreen (&r);
	
	BCMenu menu, *pPopup;
	VERIFY (menu.LoadMenu (IDR_MENU_RTF_BORDER));
	
	pPopup = (BCMenu*) menu.GetSubMenu (0);
	ASSERT (pPopup != NULL);
	pPopup->LoadToolbar (IDR_TOOLBAR_RTF_BORDER);
	pPopup->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, r.left, r.bottom, this);
}

void CRTFFormatToolbar::OnBorderStyle(UINT nId)
{
	PARAFORMAT2 pf;
	pf.cbSize = sizeof (PARAFORMAT2);
	pf.dwMask = PFM_BORDER;
	pf.wBorders ^= 1 << (nId - ID_RTFF_BORDER_LEFT);

	m_pEdit->SendMessage (EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}

void CRTFFormatToolbar::OnBulletsStyle(UINT nId)
{
	PARAFORMAT2 pf;
	pf.cbSize = sizeof (PARAFORMAT2);
	pf.dwMask = PFM_NUMBERING | PFM_NUMBERINGSTART;
	pf.wNumbering = nId - ID_RTFF_BULLETS_NONE;
	pf.wNumberingStart = 1;

	m_pEdit->SendMessage (EM_SETPARAFORMAT, 0, (LPARAM) &pf);
}

void CRTFFormatToolbar::OnUpdateMenuBullets(CCmdUI *pCmdUI)
{
	if (m_pEdit != NULL)
	{
		PARAFORMAT pf;
		pf.cbSize = sizeof (PARAFORMAT);
		pf.dwMask = PFM_NUMBERING;
		m_pEdit->SendMessage (EM_GETPARAFORMAT, 0, (LPARAM) &pf);
		pCmdUI->SetRadio (pf.wNumbering == pCmdUI->m_nID - ID_RTFF_BULLETS_NONE);
	}
}
