// STWLabelTitleStylePage.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "STWLabelTitleStylePage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplates.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTWLabelTitleStylePage property page

IMPLEMENT_DYNCREATE(CSTWLabelTitleStylePage, CPropertyPage)

CSTWLabelTitleStylePage::CSTWLabelTitleStylePage() : CPropertyPage(CSTWLabelTitleStylePage::IDD)
{
	//{{AFX_DATA_INIT(CSTWLabelTitleStylePage)
	m_bDisplayTrackgroups = TRUE;
	m_bDisplayTracks = FALSE;
	m_nRoundTitle = 0;
	m_nRoundTracks = 0;
	m_nTitleVAlign = 2;
	m_nTrackVAlign = 2;
	m_nTitleCenterWhere = 0;
	m_nTrackCenterWhere = 1;
	m_bShowTitles = FALSE;
	//}}AFX_DATA_INIT
}

CSTWLabelTitleStylePage::~CSTWLabelTitleStylePage()
{
}

void CSTWLabelTitleStylePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWLabelTitleStylePage)
	DDX_Control(pDX, IDC_COMBO_LABEL_TRACKCENTER, m_ComboTracksCenter);
	DDX_Control(pDX, IDC_COMBO_LABEL_TITLECENTER, m_ComboTitleCenter);
	DDX_Control(pDX, IDC_EDIT_LABEL_TITLEVALIGNBOTTOM, m_TitleVAlignBottom);
	DDX_Control(pDX, IDC_EDIT_LABEL_TRACKVALIGNTOP, m_TrackVAlignTop);
	DDX_Control(pDX, IDC_EDIT_LABEL_TRACKVALIGNBOTTOM, m_TrackVAlignBottom);
	DDX_Control(pDX, IDC_EDIT_LABEL_TITLEVALIGNTOP, m_TitleVAlignTop);
	DDX_Check(pDX, IDC_CHECK_LABEL_TRACKGROUPS, m_bDisplayTrackgroups);
	DDX_Check(pDX, IDC_CHECK_LABEL_TRACKS, m_bDisplayTracks);
	DDX_Radio(pDX, IDC_RADIO_LABEL_TITLEROUND, m_nRoundTitle);
	DDX_Radio(pDX, IDC_RADIO_LABEL_TRACKROUND, m_nRoundTracks);
	DDX_Radio(pDX, IDC_RADIO_LABEL_TITLEVALGNTOP, m_nTitleVAlign);
	DDX_Radio(pDX, IDC_RADIO_LABEL_TRACKVALIGNTOP, m_nTrackVAlign);
	DDX_CBIndex(pDX, IDC_COMBO_LABEL_TITLECENTER, m_nTitleCenterWhere);
	DDX_CBIndex(pDX, IDC_COMBO_LABEL_TRACKCENTER, m_nTrackCenterWhere);
	DDX_Check(pDX, IDC_CHECK_LABEL_SHOWTITLES, m_bShowTitles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTWLabelTitleStylePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWLabelTitleStylePage)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TITLEVALGNTOP, OnRadioLabelTitlevalgntop)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TITLEVALIGNBOTTOM, OnRadioLabelTitlevalignbottom)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TITLEVALIGNCENTER, OnRadioLabelTitlevaligncenter)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TRACKVALIGNBOTTOM, OnRadioLabelTrackvalignbottom)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TRACKVALIGNCENTER, OnRadioLabelTrackvaligncenter)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TRACKVALIGNTOP, OnRadioLabelTrackvaligntop)
	ON_EN_CHANGE(IDC_EDIT_LABEL_TITLEVALIGNBOTTOM, OnChangeEditLabelTitlevalignbottom)
	ON_EN_CHANGE(IDC_EDIT_LABEL_TITLEVALIGNTOP, OnChangeEditLabelTitlevaligntop)
	ON_EN_CHANGE(IDC_EDIT_LABEL_TRACKVALIGNBOTTOM, OnChangeEditLabelTrackvalignbottom)
	ON_EN_CHANGE(IDC_EDIT_LABEL_TRACKVALIGNTOP, OnChangeEditLabelTrackvaligntop)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TRACKROUND, OnRadioLabelTrackround)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TRACKSTRAIGHT, OnRadioLabelTrackstraight)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TITLEROUND, OnRadioLabelTitleround)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TITLESTRAIGHT, OnRadioLabelTitlestraight)
	ON_BN_CLICKED(IDC_CHECK_LABEL_TRACKGROUPS, OnCheckLabelTrackgroups)
	ON_BN_CLICKED(IDC_CHECK_LABEL_TRACKS, OnCheckLabelTracks)
	ON_CBN_SELCHANGE(IDC_COMBO_LABEL_TITLECENTER, OnSelchangeComboLabelTitlecenter)
	ON_CBN_SELCHANGE(IDC_COMBO_LABEL_TRACKCENTER, OnSelchangeComboLabelTrackcenter)
	ON_BN_CLICKED(IDC_CHECK_LABEL_SHOWTITLES, OnCheckLabelShowtitles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTWLabelTitleStylePage message handlers

BOOL CSTWLabelTitleStylePage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
	
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	if (pTmp != NULL)
	{
		m_nTitleVAlign = pTmp->GetTitleAlign (Label);

		CString s;
		s.Format ("%f", pTmp->GetTitleMargin (Label));
		switch (m_nTitleVAlign)
		{
		case 0:
			m_TitleVAlignTop.SetWindowText (s);
			m_TitleVAlignTop.EnableWindow ();
			break;
		case 1:
			m_TitleVAlignBottom.SetWindowText (s);
			m_TitleVAlignBottom.EnableWindow ();
			break;
		case 2:
			m_nTitleCenterWhere = pTmp->GetLabelTitlePosition ();
			GetDlgItem (IDC_COMBO_LABEL_TITLECENTER)->EnableWindow ();
			break;
		}

		m_nRoundTitle = pTmp->IsLabelTitleRound () ? 0 : 1;

		m_bDisplayTracks = pTmp->GetDisplayTrackGroups (Label);
		m_bDisplayTrackgroups = pTmp->GetDisplayTrackGroups (Label);

		m_nTrackVAlign = pTmp->GetLabelTrackVAlign ();
		s.Format ("%f", pTmp->GetLabelTrackVMargin ());
		switch (m_nTrackVAlign)
		{
		case 0:
			m_TrackVAlignTop.SetWindowText (s);
			m_TrackVAlignTop.EnableWindow ();
			break;
		case 1:
			m_TrackVAlignBottom.SetWindowText (s);
			m_TrackVAlignBottom.EnableWindow ();
			break;
		case 2:
			m_nTrackCenterWhere = pTmp->GetLabelTrackPosition ();
			GetDlgItem (IDC_COMBO_LABEL_TRACKCENTER)->EnableWindow ();
			break;
		}

		m_nRoundTracks = pTmp->IsLabelTrackRound () ? 0 : 1;
		m_bShowTitles = pTmp->GetDisplayTitles (Label);

		UpdateData (false);

		EnableTrackgroupSettings ();
		OnCheckLabelShowtitles ();
	}

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CSTWLabelTitleStylePage::OnKillActive() 
{
	m_dTitleVAlign = GetVTitleMargin ();
	m_dTracksVAlign = GetVTracksMargin ();

	return CPropertyPage::OnKillActive();
}

void CSTWLabelTitleStylePage::OnRadioLabelTitlevalgntop() 
{
	EnableTitleVAlignmentEdit ();
}

void CSTWLabelTitleStylePage::OnRadioLabelTitlevalignbottom() 
{
	EnableTitleVAlignmentEdit ();
}

void CSTWLabelTitleStylePage::OnRadioLabelTitlevaligncenter() 
{
	EnableTitleVAlignmentEdit ();
}

void CSTWLabelTitleStylePage::EnableTitleVAlignmentEdit()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nTitleAlign[Label] = m_nTitleVAlign;

	switch (m_nTitleVAlign)
	{
	case 0:
		m_TitleVAlignTop.EnableWindow ();
		m_TitleVAlignBottom.EnableWindow (false);
		GetDlgItem (IDC_COMBO_LABEL_TITLECENTER)->EnableWindow (false);
		m_TitleVAlignTop.SetFocus ();
		OnChangeEditLabelTitlevaligntop ();
		break;
	case 1:
		m_TitleVAlignTop.EnableWindow (false);
		m_TitleVAlignBottom.EnableWindow ();
		GetDlgItem (IDC_COMBO_LABEL_TITLECENTER)->EnableWindow (false);
		m_TitleVAlignBottom.SetFocus ();
		OnChangeEditLabelTitlevalignbottom ();
		break;
	default:
		m_TitleVAlignTop.EnableWindow (false);
		m_TitleVAlignBottom.EnableWindow (false);
		GetDlgItem (IDC_COMBO_LABEL_TITLECENTER)->EnableWindow ();
		GetDlgItem (IDC_COMBO_LABEL_TITLECENTER)->SetFocus ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWLabelTitleStylePage::OnRadioLabelTrackvalignbottom() 
{
	EnableTrackVAlignmentEdit ();
}

void CSTWLabelTitleStylePage::OnRadioLabelTrackvaligncenter() 
{
	EnableTrackVAlignmentEdit ();
}

void CSTWLabelTitleStylePage::OnRadioLabelTrackvaligntop() 
{
	EnableTrackVAlignmentEdit ();
}

void CSTWLabelTitleStylePage::EnableTrackVAlignmentEdit()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nLabelTrackVAlign = m_nTrackVAlign;

	switch (m_nTrackVAlign)
	{
	case 0:
		m_TrackVAlignTop.EnableWindow ();
		m_TrackVAlignBottom.EnableWindow (false);
		GetDlgItem (IDC_COMBO_LABEL_TRACKCENTER)->EnableWindow (false);
		m_TrackVAlignTop.SetFocus ();
		OnChangeEditLabelTrackvaligntop ();
		break;
	case 1:
		m_TrackVAlignTop.EnableWindow (false);
		m_TrackVAlignBottom.EnableWindow ();
		GetDlgItem (IDC_COMBO_LABEL_TRACKCENTER)->EnableWindow (false);
		m_TrackVAlignBottom.SetFocus ();
		OnChangeEditLabelTrackvalignbottom ();
		break;
	default:
		m_TrackVAlignTop.EnableWindow (false);
		m_TrackVAlignBottom.EnableWindow (false);
		GetDlgItem (IDC_COMBO_LABEL_TRACKCENTER)->EnableWindow ();
		GetDlgItem (IDC_COMBO_LABEL_TRACKCENTER)->SetFocus ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWLabelTitleStylePage::OnChangeEditLabelTitlevalignbottom() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_LABEL_TITLEVALIGNBOTTOM, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleMargin[Label] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWLabelTitleStylePage::OnChangeEditLabelTitlevaligntop() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_LABEL_TITLEVALIGNTOP, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleMargin[Label] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWLabelTitleStylePage::OnChangeEditLabelTrackvalignbottom() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_LABEL_TRACKVALIGNBOTTOM, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dLabelTrackVMargin = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWLabelTitleStylePage::OnChangeEditLabelTrackvaligntop() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_LABEL_TRACKVALIGNTOP, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dLabelTrackVMargin = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWLabelTitleStylePage::OnRadioLabelTrackround() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bLabelTrackRound =
		((CButton*) GetDlgItem (IDC_RADIO_LABEL_TRACKROUND))->GetCheck () == BST_CHECKED;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWLabelTitleStylePage::OnRadioLabelTrackstraight() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bLabelTrackRound =
		((CButton*) GetDlgItem (IDC_RADIO_LABEL_TRACKROUND))->GetCheck () == BST_CHECKED;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWLabelTitleStylePage::OnRadioLabelTitleround() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bLabelTitleRound =
		((CButton*) GetDlgItem (IDC_RADIO_LABEL_TITLEROUND))->GetCheck () == BST_CHECKED;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWLabelTitleStylePage::OnRadioLabelTitlestraight() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bLabelTitleRound =
		((CButton*) GetDlgItem (IDC_RADIO_LABEL_TITLEROUND))->GetCheck () == BST_CHECKED;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWLabelTitleStylePage::OnCheckLabelTrackgroups() 
{
	EnableTrackgroupSettings ();
}

void CSTWLabelTitleStylePage::OnCheckLabelTracks() 
{
	EnableTrackgroupSettings ();
}

void CSTWLabelTitleStylePage::EnableTrackgroupSettings()
{
	UpdateData ();

	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	pTmp->m_bDisplayTrackgroups[Label] = m_bDisplayTrackgroups ? true : false;
	pTmp->m_bLabelDisplayTracks = m_bDisplayTracks ? true : false;

	m_TrackVAlignBottom.EnableWindow (m_bDisplayTracks || m_bDisplayTrackgroups);
	m_TrackVAlignTop.EnableWindow (m_bDisplayTracks || m_bDisplayTrackgroups);

	GetDlgItem (IDC_RADIO_LABEL_TRACKVALIGNTOP)->EnableWindow (m_bDisplayTracks || m_bDisplayTrackgroups);
	GetDlgItem (IDC_RADIO_LABEL_TRACKVALIGNBOTTOM)->EnableWindow (m_bDisplayTracks || m_bDisplayTrackgroups);
	GetDlgItem (IDC_RADIO_LABEL_TRACKVALIGNCENTER)->EnableWindow (m_bDisplayTracks || m_bDisplayTrackgroups);
	GetDlgItem (IDC_RADIO_LABEL_TRACKROUND)->EnableWindow (m_bDisplayTracks || m_bDisplayTrackgroups);
	GetDlgItem (IDC_RADIO_LABEL_TRACKSTRAIGHT)->EnableWindow (m_bDisplayTracks || m_bDisplayTrackgroups);

	if (m_bDisplayTracks || m_bDisplayTrackgroups)
	{
		EnableTitleVAlignmentEdit ();
		EnableTrackVAlignmentEdit ();
	}
}

double CSTWLabelTitleStylePage::GetVTitleMargin()
{
	UpdateData ();
	CString s = "0";

	switch (m_nTitleVAlign)
	{
	case 0:
		m_TitleVAlignTop.GetWindowText (s);
		break;
	case 1:
		m_TitleVAlignBottom.GetWindowText (s);
		break;
	}

	return atof (s);
}

double CSTWLabelTitleStylePage::GetVTracksMargin()
{
	UpdateData ();
	CString s = "0";

	switch (m_nTrackVAlign)
	{
	case 0:
		m_TrackVAlignTop.GetWindowText (s);
		break;
	case 1:
		m_TrackVAlignBottom.GetWindowText (s);
		break;
	}

	return atof (s);
}

void CSTWLabelTitleStylePage::OnSelchangeComboLabelTitlecenter() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nLabelTitlePosition =
		((CComboBox*) GetDlgItem (IDC_COMBO_LABEL_TITLECENTER))->GetCurSel ();
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);		
}

void CSTWLabelTitleStylePage::OnSelchangeComboLabelTrackcenter() 
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nLabelTrackPosition =
		((CComboBox*) GetDlgItem (IDC_COMBO_LABEL_TRACKCENTER))->GetCurSel ();
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);	
}

void CSTWLabelTitleStylePage::OnCheckLabelShowtitles() 
{
	UpdateData ();

	m_TitleVAlignTop.EnableWindow (m_bShowTitles);
	m_TitleVAlignBottom.EnableWindow (m_bShowTitles);
	m_ComboTitleCenter.EnableWindow (m_bShowTitles);

	GetDlgItem (IDC_RADIO_LABEL_TITLEVALGNTOP)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_RADIO_LABEL_TITLEVALIGNBOTTOM)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_RADIO_LABEL_TITLEVALIGNCENTER)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_RADIO_LABEL_TITLEROUND)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_RADIO_LABEL_TITLESTRAIGHT)->EnableWindow (m_bShowTitles);

	if (m_bShowTitles)
		EnableTitleVAlignmentEdit ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bDisplayTitles[Label] = m_bShowTitles ? true : false;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}
