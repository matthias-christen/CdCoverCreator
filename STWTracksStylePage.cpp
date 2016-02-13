// STWTracksStylePage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "STWTracksStylePage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplates.h"

#include "Formatter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSTWTracksStylePage, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CSTWTracksStylePage property page

CSTWTracksStylePage::CSTWTracksStylePage() : CPropertyPage(CSTWTracksStylePage::IDD)
{
	//{{AFX_DATA_INIT(CSTWTracksStylePage)
	m_nTrackAlign = 0;
	m_bDisplayTotalTime = TRUE;
	m_bTrackGrpNewLine = TRUE;
	m_dTrackGrpSpacingAbove = 0.5;
	m_dTrackGrpSpacingBelow = 0.3;
	m_dTrackMarginLeft = 0.0;
	m_dTrackMarginRight = 0.0;
	m_dTrackInterItemSpacing = 0.0;
	//}}AFX_DATA_INIT
}

CSTWTracksStylePage::~CSTWTracksStylePage()
{
	::DestroyIcon (m_hIconTrackUp);
	::DestroyIcon (m_hIconTrackDown);
}

void CSTWTracksStylePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWTracksStylePage)
	DDX_Control(pDX, IDC_STATIC_TRACKFORMATTINGSAMPLE, m_ctrlTrackFormattingSample);
	DDX_Control(pDX, IDC_COMBO_TRACKFORMATTING, m_ctrlTrackFormatting);
	DDX_Control(pDX, IDC_BUTTON_TRACK_UP, m_btnTrackUp);
	DDX_Control(pDX, IDC_BUTTON_TRACK_DOWN, m_btnTrackDown);
	DDX_Control(pDX, IDC_COMBO_TRACKSALIGNMENT, m_ctrlTrackAlignment);
	DDX_Control(pDX, IDC_CHECK_TRACKDISPLAY, m_ctrlTrackDisplay);
	DDX_Control(pDX, IDC_LIST_DISPLAY, m_ListDisplay);
	DDX_Radio(pDX, IDC_RADIO_TRACKALIGN, m_nTrackAlign);
	DDX_Check(pDX, IDC_CHECK_DISPLAYTOTALTIME, m_bDisplayTotalTime);
	DDX_Check(pDX, IDC_CHECK_TRACKGRP_NEWLINE, m_bTrackGrpNewLine);
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


BEGIN_MESSAGE_MAP(CSTWTracksStylePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWTracksStylePage)
	ON_LBN_SELCHANGE(IDC_LIST_DISPLAY, OnSelchangeListDisplay)
	ON_BN_CLICKED(IDC_BUTTON_TRACK_UP, OnButtonTrackUp)
	ON_BN_CLICKED(IDC_BUTTON_TRACK_DOWN, OnButtonTrackDown)
	ON_BN_CLICKED(IDC_CHECK_TRACKDISPLAY, OnCheckTrackdisplay)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACKSALIGNMENT, OnSelchangeComboTracksalignment)
	ON_CBN_EDITCHANGE(IDC_COMBO_TRACKFORMATTING, OnEditchangeComboTrackformatting)
	ON_CBN_SELCHANGE(IDC_COMBO_TRACKFORMATTING, OnSelchangeComboTrackformatting)
	ON_BN_CLICKED(IDC_CHECK_DISPLAYTOTALTIME, OnCheckDisplaytotaltime)
	ON_BN_CLICKED(IDC_CHECK_TRACKGRP_NEWLINE, OnCheckTrackgrpNewline)
	ON_BN_CLICKED(IDC_RADIO_TRACKALIGN, OnRadioTrackalign)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioTrackFloating)
	ON_EN_CHANGE(IDC_EDIT_TRACKH_MARGINL, OnChangeEditTrackhMarginl)
	ON_EN_CHANGE(IDC_EDIT_TRACKH_MARGINR, OnChangeEditTrackhMarginr)
	ON_EN_CHANGE(IDC_EDIT_TRACKH_INTERITEMSPACING, OnChangeEditTrackhInteritemspacing)
	ON_EN_CHANGE(IDC_EDIT_TRACKGRP_SPABOVE, OnChangeEditTrackgrpSpabove)
	ON_EN_CHANGE(IDC_EDIT_TRACKGRP_SPBELOW, OnChangeEditTrackgrpSpbelow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSTWTracksStylePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

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
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSTWTracksStylePage::OnSelchangeListDisplay() 
{
	CString s, strVal;
	m_ListDisplay.GetText (m_ListDisplay.GetCurSel (), s);
	m_mapAlignment.Lookup (s, strVal);

	m_ctrlTrackDisplay.SetCheck (strVal[0] - '0');
	m_ctrlTrackAlignment.SetCurSel (strVal[1] - '0');
	FillFormattingCombo (s);
}

void CSTWTracksStylePage::OnButtonTrackUp() 
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

void CSTWTracksStylePage::OnButtonTrackDown() 
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

void CSTWTracksStylePage::UpdatePosition()
{
	CString s, strVal, s1;
	int n;
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;

	for (int i = 0; i < 4; i++)
	{
		s.LoadString (IDS_TRACK_DISPLAY + i);
		m_mapAlignment.Lookup (s, strVal);

		s1 = strVal.Mid (3);
		n = m_ListDisplay.FindStringExact (-1, s);
		strVal.Format ("%c%c%d", strVal[0], strVal[1], n);
		m_mapAlignment.SetAt (s, strVal + s1);

		pTmp->m_htaTrack[i].nPosition = n;
	}

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnCheckTrackdisplay() 
{
	CString s, strVal, s1;
	m_ListDisplay.GetText (m_ListDisplay.GetCurSel (), s);
	m_mapAlignment.Lookup (s, strVal);

	s1 = strVal.Mid (1);
	strVal.Format ("%d", m_ctrlTrackDisplay.GetCheck ());
	m_mapAlignment.SetAt (s, strVal + s1);

	// update template
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	for (int i = 0; i < NUM_TRACK_TYPES; i++)
		if (pTmp->m_htaTrack[i].nPosition == s1[1] - '0')	// s1[1] is 3rd character of strVal -> position
		{
			pTmp->m_htaTrack[i].bDisplay = strVal[0] == '1' ? true : false;
			break;
		}
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnSelchangeComboTracksalignment() 
{
	CString s, strVal, s1;
	m_ListDisplay.GetText (m_ListDisplay.GetCurSel (), s);
	m_mapAlignment.Lookup (s, strVal);

	s1 = strVal.Mid (2);
	strVal.Format ("%c%d", strVal[0], m_ctrlTrackAlignment.GetCurSel ());
	m_mapAlignment.SetAt (s, strVal + s1);

	// update template
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	for (int i = 0; i < NUM_TRACK_TYPES; i++)
		if (pTmp->m_htaTrack[i].nPosition == s1[0] - '0')	// s1[0] is 3rd character of strVal -> position
		{
			pTmp->m_htaTrack[i].nHAlign = strVal[1] - '0';
			break;
		}
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnEditchangeComboTrackformatting() 
{
	CString s, strVal, strTxt;
	m_ListDisplay.GetText (m_ListDisplay.GetCurSel (), s);
	m_mapAlignment.Lookup (s, strVal);

	m_ctrlTrackFormatting.GetWindowText (strTxt);
	m_mapAlignment.SetAt (s, strVal.Left (3) + strTxt);

	m_ctrlTrackFormattingSample.SetWindowText (CFormatter::Format (strTxt, 9, 5));

	// update template
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	for (int i = 0; i < NUM_TRACK_TYPES; i++)
		if (pTmp->m_htaTrack[i].nPosition == strVal[2] - '0')	// 3rd character of strVal -> position
		{
			pTmp->m_htaTrack[i].strFormatting = strTxt;
			break;
		}
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnSelchangeComboTrackformatting() 
{
	CString s;
	m_ctrlTrackFormatting.GetLBText (m_ctrlTrackFormatting.GetCurSel (), s);
	m_ctrlTrackFormatting.SetWindowText (s);
	OnEditchangeComboTrackformatting ();
}

int CSTWTracksStylePage::GetPos(UINT nId)
{
	CString s, strVal;
	s.LoadString (nId);
	m_mapAlignment.Lookup (s, strVal);

	return strVal[2] - '0';
}

bool CSTWTracksStylePage::GetDisplay(UINT nId)
{
	CString s, strVal;
	s.LoadString (nId);
	m_mapAlignment.Lookup (s, strVal);

	return strVal[0] == '0' ? false : true;
}

int CSTWTracksStylePage::GetAlign(UINT nId)
{
	CString s, strVal;
	s.LoadString (nId);
	m_mapAlignment.Lookup (s, strVal);

	return strVal[1] - '0';
}

CString CSTWTracksStylePage::GetFormatting(UINT nId)
{
	CString s, strVal;
	s.LoadString (nId);
	m_mapAlignment.Lookup (s, strVal);

	return strVal.Mid (3);
}

BOOL CSTWTracksStylePage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
		
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	CString s, s1;

	if (pTmp != NULL)
	{
		m_nTrackAlign = pTmp->GetTrackFloating () ? 1 : 0;
		m_bDisplayTotalTime = pTmp->GetTrackTotalTime ();

		// track alignment list box
		m_ListDisplay.ResetContent ();
		for (int i = 0; i < NUM_TRACK_TYPES; i++)
			m_ListDisplay.AddString ("");

		s.LoadString (IDS_TRACK_DISPLAY);
		m_ListDisplay.InsertString (pTmp->GetTrackNumPosition (), s);
		m_ListDisplay.DeleteString (pTmp->GetTrackNumPosition () + 1);
		s1.Format ("%d%d%d", pTmp->GetTrackNumDisplay () ? 1 : 0, pTmp->GetTrackNumHAlign (), pTmp->GetTrackNumPosition ());
		m_mapAlignment.SetAt (s, s1 + pTmp->GetTrackNumFormatting ());

		s.LoadString (IDS_TRACK_LENGTH);
		m_ListDisplay.InsertString (pTmp->GetTrackLenPosition (), s);
		m_ListDisplay.DeleteString (pTmp->GetTrackLenPosition () + 1);
		s1.Format ("%d%d%d", pTmp->GetTrackLenDisplay () ? 1 : 0, pTmp->GetTrackLenHAlign (), pTmp->GetTrackLenPosition ());
		m_mapAlignment.SetAt (s, s1 + pTmp->GetTrackLenFormatting ());

		s.LoadString (IDS_TRACK_TITLE);
		m_ListDisplay.InsertString (pTmp->GetTrackTitlePosition (), s);
		m_ListDisplay.DeleteString (pTmp->GetTrackTitlePosition () + 1);
		s1.Format ("%d%d%d", pTmp->GetTrackTitleDisplay () ? 1 : 0, pTmp->GetTrackTitleHAlign (), pTmp->GetTrackTitlePosition ());
		m_mapAlignment.SetAt (s, s1);

		s.LoadString (IDS_TRACK_REMARKS);
		m_ListDisplay.InsertString (pTmp->GetTrackRemarkPosition (), s);
		m_ListDisplay.DeleteString (pTmp->GetTrackRemarkPosition () + 1);
		s1.Format ("%d%d%d", pTmp->GetTrackRemarkDisplay () ? 1 : 0, pTmp->GetTrackRemarkHAlign (), pTmp->GetTrackRemarkPosition ());
		m_mapAlignment.SetAt (s, s1);

		// spacing
		m_dTrackMarginLeft = pTmp->GetTrackMarginLeft ();
		m_dTrackMarginRight = pTmp->GetTrackMarginRight ();
		m_dTrackInterItemSpacing = pTmp->GetTrackInterItemSpacing ();

		// track groupings
		m_dTrackGrpSpacingAbove = pTmp->GetTrackGrpSpacingAbove ();
		m_dTrackGrpSpacingBelow = pTmp->GetTrackGrpSpacingBelow ();
		m_bTrackGrpNewLine = pTmp->GetTrackGrpNewLine ();

		UpdateData (false);
	}

	m_ListDisplay.SetCurSel (0);
	OnSelchangeListDisplay ();

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CSTWTracksStylePage::OnKillActive() 
{
	UpdateData ();
	return CPropertyPage::OnKillActive();
}

BOOL CSTWTracksStylePage::OnWizardFinish() 
{
	UpdateData ();	
	return CPropertyPage::OnWizardFinish();
}

void CSTWTracksStylePage::FillFormattingCombo(CString str)
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
		m_ctrlTrackFormattingSample.SetWindowText (CFormatter::Format (strFormat, 9));
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
		m_ctrlTrackFormattingSample.SetWindowText (CFormatter::Format (strFormat, 9, 5));
	}
	else
	{
		m_ctrlTrackFormatting.EnableWindow (false);
		m_ctrlTrackFormattingSample.SetWindowText ("");
	}
}

void CSTWTracksStylePage::OnCheckDisplaytotaltime() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bTrackTotalTime =
		((CButton*) GetDlgItem (IDC_CHECK_DISPLAYTOTALTIME))->GetCheck () == BST_CHECKED;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnCheckTrackgrpNewline() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bTrackGrpNewLine =
		((CButton*) GetDlgItem (IDC_CHECK_TRACKGRP_NEWLINE))->GetCheck () == BST_CHECKED;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnRadioTrackalign() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bTrackFloating =
		((CButton*) GetDlgItem (IDC_RADIO_TRACKALIGN))->GetCheck () == BST_UNCHECKED;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnRadioTrackFloating() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bTrackFloating =
		((CButton*) GetDlgItem (IDC_RADIO2))->GetCheck () == BST_CHECKED;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnChangeEditTrackhMarginl() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKH_MARGINL, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackMarginLeft = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnChangeEditTrackhMarginr() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKH_MARGINR, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackMarginRight = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnChangeEditTrackhInteritemspacing() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKH_INTERITEMSPACING, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackInterItemSpacing = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnChangeEditTrackgrpSpabove() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKGRP_SPABOVE, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackGrpSpacingAbove = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWTracksStylePage::OnChangeEditTrackgrpSpbelow() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TRACKGRP_SPBELOW, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackGrpSpacingBelow = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}


BOOL CSTWTracksStylePage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
