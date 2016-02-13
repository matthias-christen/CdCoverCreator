// StyleDlgTracks.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"
#include "StyleDlgTracks.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgTracks dialog


CStyleDlgTracks::CStyleDlgTracks(CWnd* pParent /*=NULL*/)
	: CStyleDlgBase(CStyleDlgTracks::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStyleDlgTracks)
//	m_nTrackAlign = 0;
	m_bDisplayTotalTime = TRUE;
//	m_bTrackGrpNewLine = TRUE;
	m_dTrackGrpSpacingAbove = 0.5;
	m_dTrackGrpSpacingBelow = 0.3;
	m_dTrackMarginLeft = 0.0;
	m_dTrackMarginRight = 0.0;
	m_dTrackInterItemSpacing = 0.0;
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStyleDlgTracks::~CStyleDlgTracks ()
{
	::DestroyIcon (m_hIconTrackUp);
	::DestroyIcon (m_hIconTrackDown);
}

void CStyleDlgTracks::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyleDlgTracks)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_COMBO_TRACKFORMATTING, m_ctrlTrackFormatting);
	DDX_Control(pDX, IDC_BUTTON_TRACK_UP, m_btnTrackUp);
	DDX_Control(pDX, IDC_BUTTON_TRACK_DOWN, m_btnTrackDown);
	DDX_Control(pDX, IDC_COMBO_TRACKSALIGNMENT, m_ctrlTrackAlignment);
	DDX_Control(pDX, IDC_CHECK_TRACKDISPLAY, m_ctrlTrackDisplay);
	DDX_Control(pDX, IDC_LIST_DISPLAY, m_ListDisplay);
//	DDX_Radio(pDX, IDC_RADIO_TRACKALIGN, m_nTrackAlign);
	DDX_Check(pDX, IDC_CHECK_DISPLAYTOTALTIME, m_bDisplayTotalTime);
//	DDX_Check(pDX, IDC_CHECK_TRACKGRP_NEWLINE, m_bTrackGrpNewLine);
	DDX_Text(pDX, IDC_EDIT_TRACKGRP_SPABOVE, m_dTrackGrpSpacingAbove);
	DDV_MinMaxDouble(pDX, m_dTrackGrpSpacingAbove, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_TRACKGRP_SPBELOW, m_dTrackGrpSpacingBelow);
	DDV_MinMaxDouble(pDX, m_dTrackGrpSpacingBelow, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_TRACKH_MARGINL, m_dTrackMarginLeft);
	DDV_MinMaxDouble(pDX, m_dTrackMarginLeft, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_TRACKH_MARGINR, m_dTrackMarginRight);
	DDV_MinMaxDouble(pDX, m_dTrackMarginRight, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_TRACKH_INTERITEMSPACING, m_dTrackInterItemSpacing);
	DDV_MinMaxDouble(pDX, m_dTrackInterItemSpacing, 0., 12.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStyleDlgTracks, CDialog)
	//{{AFX_MSG_MAP(CStyleDlgTracks)
	ON_LBN_SELCHANGE(IDC_LIST_DISPLAY, OnSelchangeListDisplay)
	ON_BN_CLICKED(IDC_BUTTON_TRACK_UP, OnButtonTrackUp)
	ON_BN_CLICKED(IDC_BUTTON_TRACK_DOWN, OnButtonTrackDown)
	ON_BN_CLICKED(IDC_CHECK_TRACKDISPLAY, OnCheckTrackdisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACKSALIGNMENT, OnSelchangeComboTracksalignment)
	ON_CBN_EDITCHANGE(IDC_COMBO_TRACKFORMATTING, OnEditchangeComboTrackformatting)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACKFORMATTING, OnSelchangeComboTrackformatting)
	ON_BN_CLICKED(IDC_CHECK_DISPLAYTOTALTIME, OnCheckDisplaytotaltime)
	ON_EN_CHANGE(IDC_EDIT_TRACKH_MARGINL, OnChangeEditTrackhMarginl)
	ON_EN_CHANGE(IDC_EDIT_TRACKH_MARGINR, OnChangeEditTrackhMarginr)
	ON_EN_CHANGE(IDC_EDIT_TRACKH_INTERITEMSPACING, OnChangeEditTrackhInteritemspacing)
	ON_EN_CHANGE(IDC_EDIT_TRACKGRP_SPABOVE, OnChangeEditTrackgrpSpabove)
	ON_EN_CHANGE(IDC_EDIT_TRACKGRP_SPBELOW, OnChangeEditTrackgrpSpbelow)
	ON_LBN_SETFOCUS(IDC_LIST_DISPLAY, OnSetfocusListDisplay)
	ON_LBN_KILLFOCUS(IDC_LIST_DISPLAY, OnKillfocusListDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgTracks message handlers

void CStyleDlgTracks::OnSelchangeListDisplay() 
{
	CString s, strVal;
	m_ListDisplay.GetText (m_ListDisplay.GetCurSel (), s);
	m_mapAlignment.Lookup (s, strVal);

	m_ctrlTrackDisplay.SetCheck (strVal[0] - '0');
	m_ctrlTrackAlignment.SetCurSel (strVal[1] - '0');
	FillFormattingCombo (s);

	// enable the controls
	m_btnTrackUp.EnableWindow ();
	m_btnTrackDown.EnableWindow ();
	m_ctrlTrackAlignment.EnableWindow ();
	m_ctrlTrackDisplay.EnableWindow ();
}

void CStyleDlgTracks::OnButtonTrackUp() 
{
	int n = m_ListDisplay.GetCurSel ();
	if (n > 0)
	{
		CString s;
		m_ListDisplay.GetText (n, s);
		m_ListDisplay.DeleteString (n);
		m_ListDisplay.InsertString (n - 1, s);
		m_ListDisplay.SetCurSel (n - 1);

		UpdatePosition ();
	}
}

void CStyleDlgTracks::OnButtonTrackDown() 
{
	int n = m_ListDisplay.GetCurSel ();
	if ((n >= 0) && (n < m_ListDisplay.GetCount () - 1))
	{
		CString s;
		m_ListDisplay.GetText (n, s);
		m_ListDisplay.DeleteString (n);
		m_ListDisplay.InsertString (n + 1, s);
		m_ListDisplay.SetCurSel (n + 1);

		UpdatePosition ();
	}
}

void CStyleDlgTracks::UpdatePosition()
{
	CString s, strVal, s1;
	int n;

	for (int i = 0; i < 4; i++)
	{
		s.LoadString (IDS_TRACK_DISPLAY + i);
		m_mapAlignment.Lookup (s, strVal);

		s1 = strVal.Mid (3);
		n = m_ListDisplay.FindStringExact (-1, s);
		strVal.Format ("%c%c%d", strVal[0], strVal[1], n);
		m_mapAlignment.SetAt (s, strVal + s1);

		m_pTemplate->m_htaTrack[i].nPosition = n;
	}

	UpdateDoc ();
}

void CStyleDlgTracks::OnCheckTrackdisplay() 
{
	CString s, strVal, s1;
	m_ListDisplay.GetText (m_ListDisplay.GetCurSel (), s);
	m_mapAlignment.Lookup (s, strVal);

	s1 = strVal.Mid (1);
	strVal.Format ("%d", m_ctrlTrackDisplay.GetCheck ());
	m_mapAlignment.SetAt (s, strVal + s1);

	// update template
	for (int i = 0; i < NUM_TRACK_TYPES; i++)
		if (m_pTemplate->m_htaTrack[i].nPosition == s1[1] - '0')	// s1[1] is 3rd character of strVal -> position
		{
			m_pTemplate->m_htaTrack[i].bDisplay = strVal[0] == '1' ? true : false;
			break;
		}

	UpdateDoc ();
}

void CStyleDlgTracks::OnSelchangeComboTracksalignment() 
{
	CString s, strVal, s1;
	m_ListDisplay.GetText (m_ListDisplay.GetCurSel (), s);
	m_mapAlignment.Lookup (s, strVal);

	s1 = strVal.Mid (2);
	strVal.Format ("%c%d", strVal[0], m_ctrlTrackAlignment.GetCurSel ());
	m_mapAlignment.SetAt (s, strVal + s1);

	// update template
	for (int i = 0; i < NUM_TRACK_TYPES; i++)
		if (m_pTemplate->m_htaTrack[i].nPosition == s1[0] - '0')	// s1[0] is 3rd character of strVal -> position
		{
			m_pTemplate->m_htaTrack[i].nHAlign = strVal[1] - '0';
			break;
		}

	UpdateDoc ();
}

void CStyleDlgTracks::OnEditchangeComboTrackformatting() 
{
	CString s, strVal, strTxt;
	m_ListDisplay.GetText (m_ListDisplay.GetCurSel (), s);
	m_mapAlignment.Lookup (s, strVal);

	m_ctrlTrackFormatting.GetWindowText (strTxt);
	m_mapAlignment.SetAt (s, strVal.Left (3) + strTxt);

	// update template
	for (int i = 0; i < NUM_TRACK_TYPES; i++)
		if (m_pTemplate->m_htaTrack[i].nPosition == strVal[2] - '0')	// 3rd character of strVal -> position
		{
			m_pTemplate->m_htaTrack[i].strFormatting = strTxt;
			break;
		}

	UpdateDoc ();
}

void CStyleDlgTracks::OnSelchangeComboTrackformatting() 
{
	CString s;
	m_ctrlTrackFormatting.GetLBText (m_ctrlTrackFormatting.GetCurSel (), s);
	m_ctrlTrackFormatting.SetWindowText (s);
	OnEditchangeComboTrackformatting ();
}

int CStyleDlgTracks::GetPos(UINT nId)
{
	CString s, strVal;
	s.LoadString (nId);
	m_mapAlignment.Lookup (s, strVal);

	return strVal[2] - '0';
}

bool CStyleDlgTracks::GetDisplay(UINT nId)
{
	CString s, strVal;
	s.LoadString (nId);
	m_mapAlignment.Lookup (s, strVal);

	return strVal[0] == '0' ? false : true;
}

int CStyleDlgTracks::GetAlign(UINT nId)
{
	CString s, strVal;
	s.LoadString (nId);
	m_mapAlignment.Lookup (s, strVal);

	return strVal[1] - '0';
}

CString CStyleDlgTracks::GetFormatting(UINT nId)
{
	CString s, strVal;
	s.LoadString (nId);
	m_mapAlignment.Lookup (s, strVal);

	return strVal.Mid (3);
}

void CStyleDlgTracks::FillFormattingCombo(CString str)
{
	m_ctrlTrackFormatting.ResetContent ();

	CString s1, s2;
	s1.LoadString (IDS_TRACK_DISPLAY);
	s2.LoadString (IDS_TRACK_LENGTH);

	CString s, strFormat;
	if (str == s1)
	{
		for (int i = 0; ; i++)
		{
			if (!s.LoadString (IDS_TRACK_DISPLAY_FORMATTING + i))
				break;
			m_ctrlTrackFormatting.AddString (s);
		}

		m_ctrlTrackFormatting.EnableWindow ();
		strFormat = GetFormatting (IDS_TRACK_DISPLAY);
		m_ctrlTrackFormatting.SetWindowText (strFormat);
	}
	else if (str == s2)
	{
		for (int i = 0; ; i++)
		{
			if (!s.LoadString (IDS_TRACK_LENGTH_FORMATTING + i))
				break;
			m_ctrlTrackFormatting.AddString (s);
		}

		m_ctrlTrackFormatting.EnableWindow ();
		strFormat = GetFormatting (IDS_TRACK_LENGTH);
		m_ctrlTrackFormatting.SetWindowText (strFormat);
	}
	else
		m_ctrlTrackFormatting.EnableWindow (false);
}

void CStyleDlgTracks::OnCheckDisplaytotaltime() 
{
	m_pTemplate->m_bTrackTotalTime = ((CButton*) GetDlgItem (IDC_CHECK_DISPLAYTOTALTIME))->GetCheck () == BST_CHECKED;
	UpdateDoc ();
}
/*
void CStyleDlgTracks::OnCheckTrackgrpNewline() 
{
	m_pTemplate->m_bTrackGrpNewLine = ((CButton*) GetDlgItem (IDC_CHECK_TRACKGRP_NEWLINE))->GetCheck () == BST_CHECKED;
	UpdateDoc ();
}

void CStyleDlgTracks::OnRadioTrackalign() 
{
	m_pTemplate->m_bTrackFloating = ((CButton*) GetDlgItem (IDC_RADIO_TRACKALIGN))->GetCheck () == BST_UNCHECKED;
	UpdateDoc ();
}

void CStyleDlgTracks::OnRadioTrackFloating() 
{
	m_pTemplate->m_bTrackFloating =	((CButton*) GetDlgItem (IDC_RADIO2))->GetCheck () == BST_CHECKED;
	UpdateDoc ();
}*/

void CStyleDlgTracks::OnChangeEditTrackhMarginl() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKH_MARGINL, s);
	m_pTemplate->m_dTrackMarginLeft = atof (s);

	UpdateDoc ();
}

void CStyleDlgTracks::OnChangeEditTrackhMarginr() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKH_MARGINR, s);
	m_pTemplate->m_dTrackMarginRight = atof (s);

	UpdateDoc ();
}

void CStyleDlgTracks::OnChangeEditTrackhInteritemspacing() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKH_INTERITEMSPACING, s);
	m_pTemplate->m_dTrackInterItemSpacing = atof (s);

	UpdateDoc ();
}

void CStyleDlgTracks::OnChangeEditTrackgrpSpabove() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKGRP_SPABOVE, s);
	m_pTemplate->m_dTrackGrpSpacingAbove = atof (s);

	UpdateDoc ();
}

void CStyleDlgTracks::OnChangeEditTrackgrpSpbelow() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKGRP_SPBELOW, s);
	m_pTemplate->m_dTrackGrpSpacingBelow = atof (s);

	UpdateDoc ();
}

void CStyleDlgTracks::SetTemplate(CStyleTemplate *pStyle)
{
	m_pTemplate = pStyle;

	if (pStyle != NULL)
	{
		CString s, s1;

//		m_nTrackAlign = pStyle->GetTrackFloating () ? 1 : 0;
		m_bDisplayTotalTime = pStyle->GetTrackTotalTime ();

		// track alignment list box
		m_ListDisplay.ResetContent ();
		for (int i = 0; i < NUM_TRACK_TYPES; i++)
			m_ListDisplay.AddString ("");

		s.LoadString (IDS_TRACK_DISPLAY);
		m_ListDisplay.InsertString (pStyle->GetTrackNumPosition (), s);
		m_ListDisplay.DeleteString (pStyle->GetTrackNumPosition () + 1);
		s1.Format ("%d%d%d", pStyle->GetTrackNumDisplay () ? 1 : 0, pStyle->GetTrackNumHAlign (), pStyle->GetTrackNumPosition ());
		m_mapAlignment.SetAt (s, s1 + pStyle->GetTrackNumFormatting ());

		s.LoadString (IDS_TRACK_LENGTH);
		m_ListDisplay.InsertString (pStyle->GetTrackLenPosition (), s);
		m_ListDisplay.DeleteString (pStyle->GetTrackLenPosition () + 1);
		s1.Format ("%d%d%d", pStyle->GetTrackLenDisplay () ? 1 : 0, pStyle->GetTrackLenHAlign (), pStyle->GetTrackLenPosition ());
		m_mapAlignment.SetAt (s, s1 + pStyle->GetTrackLenFormatting ());

		s.LoadString (IDS_TRACK_TITLE);
		m_ListDisplay.InsertString (pStyle->GetTrackTitlePosition (), s);
		m_ListDisplay.DeleteString (pStyle->GetTrackTitlePosition () + 1);
		s1.Format ("%d%d%d", pStyle->GetTrackTitleDisplay () ? 1 : 0, pStyle->GetTrackTitleHAlign (), pStyle->GetTrackTitlePosition ());
		m_mapAlignment.SetAt (s, s1);

		s.LoadString (IDS_TRACK_REMARKS);
		m_ListDisplay.InsertString (pStyle->GetTrackRemarkPosition (), s);
		m_ListDisplay.DeleteString (pStyle->GetTrackRemarkPosition () + 1);
		s1.Format ("%d%d%d", pStyle->GetTrackRemarkDisplay () ? 1 : 0, pStyle->GetTrackRemarkHAlign (), pStyle->GetTrackRemarkPosition ());
		m_mapAlignment.SetAt (s, s1);

		// spacing
		m_dTrackMarginLeft = pStyle->GetTrackMarginLeft ();
		m_dTrackMarginRight = pStyle->GetTrackMarginRight ();
		m_dTrackInterItemSpacing = pStyle->GetTrackInterItemSpacing ();

		// track groupings
		m_dTrackGrpSpacingAbove = pStyle->GetTrackGrpSpacingAbove ();
		m_dTrackGrpSpacingBelow = pStyle->GetTrackGrpSpacingBelow ();
//		m_bTrackGrpNewLine = pStyle->GetTrackGrpNewLine ();

		UpdateData (false);

		m_ListDisplay.EnableWindow ();
		GetDlgItem (IDC_CHECK_DISPLAYTOTALTIME)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TRACKGRP_SPABOVE)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TRACKGRP_SPBELOW)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TRACKH_MARGINL)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TRACKH_MARGINR)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TRACKH_INTERITEMSPACING)->EnableWindow ();

		m_ListDisplay.SetCurSel (-1);
		OnKillfocusListDisplay ();
	}
	else
	{
		m_ListDisplay.EnableWindow (false);
		GetDlgItem (IDC_CHECK_DISPLAYTOTALTIME)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TRACKGRP_SPABOVE)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TRACKGRP_SPBELOW)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TRACKH_MARGINL)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TRACKH_MARGINR)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TRACKH_INTERITEMSPACING)->EnableWindow (false);
		OnKillfocusListDisplay ();		
	}
}

BOOL CStyleDlgTracks::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString s, s1;

	for (int i = 0; i < 4; i++)
	{
		s.LoadString (IDS_TRACK_DISPLAY + i);
		m_ListDisplay.AddString (s);

		// Track Align String format:
		// abcF
		//
		// where a, b, c are 1-digit values, F is a string
		// - a:	Display (1 if to be displayed, 0 if not)
		// - b:	Horizontal alignment (0: left, 1: right, 2: centered)
		// - c:	Horizontal Position (0 is the first position)
		// - F:	Number format
		s1.LoadString (IDS_TRACK_ALIGN + i);
		m_mapAlignment.SetAt (s, s1);
	}

	m_btnTrackUp.SetIcon (m_hIconTrackUp = AfxGetApp ()->LoadIcon (IDI_ICON_UP));
	m_btnTrackDown.SetIcon (m_hIconTrackDown = AfxGetApp ()->LoadIcon (IDI_ICON_DOWN));
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool (&m_btnTrackUp, IDS_TT_MOVEUP);
		m_tooltip.AddTool (&m_btnTrackDown, IDS_TT_MOVEDOWN);
	}
	
	return TRUE;
}

void CStyleDlgTracks::OnSetfocusListDisplay() 
{
	if (m_ListDisplay.GetCurSel () == LB_ERR)
		return;

	m_ctrlTrackFormatting.EnableWindow ();
	m_btnTrackUp.EnableWindow ();
	m_btnTrackDown.EnableWindow ();
	m_ctrlTrackAlignment.EnableWindow ();
	m_ctrlTrackDisplay.EnableWindow ();
	GetDlgItem (IDC_COMBO_TRACKSALIGNMENT)->EnableWindow ();
}

void CStyleDlgTracks::OnKillfocusListDisplay() 
{
	if (m_ListDisplay.GetCurSel () != LB_ERR)
		return;

	m_ctrlTrackFormatting.EnableWindow (false);
	m_btnTrackUp.EnableWindow (false);
	m_btnTrackDown.EnableWindow (false);
	m_ctrlTrackAlignment.EnableWindow (false);
	m_ctrlTrackDisplay.EnableWindow (false);
	GetDlgItem (IDC_COMBO_TRACKSALIGNMENT)->EnableWindow (false);
}
