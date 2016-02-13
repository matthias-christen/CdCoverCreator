// STWCoverTitleStylePage.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "STWCoverTitleStylePage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplates.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTWCoverTitleStylePage dialog


CSTWCoverTitleStylePage::CSTWCoverTitleStylePage () : CPropertyPage(CSTWCoverTitleStylePage::IDD)
{
	//{{AFX_DATA_INIT(CSTWCoverTitleStylePage)
	m_nTitleMargin = 2;
	m_nTrackgroupVMargin = 0;
	m_nTrackgroupHMargin = 2;
	m_bShowTrackgroups = TRUE;
	m_dTitleVMarginTop = 4.0;
	m_dTitleVHeight = 2.0;
	m_bShowTitles = TRUE;
	//}}AFX_DATA_INIT
}


void CSTWCoverTitleStylePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWCoverTitleStylePage)
	DDX_Control(pDX, IDC_EDIT_TITLE_TGMARGINTOP, m_EditCoverTrackgroupMarginT);
	DDX_Control(pDX, IDC_EDIT_TITLE_TGMARGINRIGHT, m_EditCoverTrackgroupMarginR);
	DDX_Control(pDX, IDC_EDIT_TITLE_TGMARGINLEFT, m_EditCoverTrackgroupMarginL);
	DDX_Control(pDX, IDC_EDIT_TITLE_TGMARGINBOTTOM, m_EditCoverTrackgroupMarginB);
	DDX_Control(pDX, IDC_EDIT_COVER_TITLEHALIGNRIGHT, m_EditCoverTitleHAlignR);
	DDX_Control(pDX, IDC_EDIT_COVER_TITLEHALIGNLEFT, m_EditCoverTitleHAlignL);
	DDX_Radio(pDX, IDC_RADIO_COVER_TITLEMARGINLEFT, m_nTitleMargin);
	DDX_Radio(pDX, IDC_RADIO_TITLE_TGMARGINTOP, m_nTrackgroupVMargin);
	DDX_Radio(pDX, IDC_RADIO_TITLE_TGMARGINLEFT, m_nTrackgroupHMargin);
	DDX_Check(pDX, IDC_CHECK_COVER_TITLE_TGSHOW, m_bShowTrackgroups);
	DDX_Text(pDX, IDC_EDIT_COVER_TITLEVALIGNTOP, m_dTitleVMarginTop);
	DDV_MinMaxDouble(pDX, m_dTitleVMarginTop, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_COVER_TITLEVALIGNHEIGHT, m_dTitleVHeight);
	DDV_MinMaxDouble(pDX, m_dTitleVHeight, 0., 12.);
	DDX_Check(pDX, IDC_CHECK_COVER_TITLE_SHOW, m_bShowTitles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTWCoverTitleStylePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWCoverTitleStylePage)
	ON_BN_CLICKED(IDC_RADIO_COVER_TITLEMARGINCENTER, OnRadioCoverTitlemargincenter)
	ON_BN_CLICKED(IDC_RADIO_COVER_TITLEMARGINLEFT, OnRadioCoverTitlemarginleft)
	ON_BN_CLICKED(IDC_RADIO_COVER_TITLEMARGINRIGHT, OnRadioCoverTitlemarginright)
	ON_BN_CLICKED(IDC_RADIO_TITLE_TGHCENTERED, OnRadioTitleTghcentered)
	ON_BN_CLICKED(IDC_RADIO_TITLE_TGMARGINBOTTOM, OnRadioTitleTgmarginbottom)
	ON_BN_CLICKED(IDC_RADIO_TITLE_TGMARGINLEFT, OnRadioTitleTgmarginleft)
	ON_BN_CLICKED(IDC_RADIO_TITLE_TGMARGINRIGHT, OnRadioTitleTgmarginright)
	ON_BN_CLICKED(IDC_RADIO_TITLE_TGMARGINTOP, OnRadioTitleTgmargintop)
	ON_BN_CLICKED(IDC_RADIO_TITLE_TGVCENTER, OnRadioTitleTgvcenter)
	ON_EN_CHANGE(IDC_EDIT_COVER_TITLEHALIGNLEFT, OnChangeEditCoverTitlehalignleft)
	ON_EN_CHANGE(IDC_EDIT_COVER_TITLEHALIGNRIGHT, OnChangeEditCoverTitlehalignright)
	ON_EN_CHANGE(IDC_EDIT_COVER_TITLEVALIGNHEIGHT, OnChangeEditCoverTitlevalignheight)
	ON_EN_CHANGE(IDC_EDIT_COVER_TITLEVALIGNTOP, OnChangeEditCoverTitlevaligntop)
	ON_EN_CHANGE(IDC_EDIT_TITLE_TGMARGINBOTTOM, OnChangeEditTitleTgmarginbottom)
	ON_EN_CHANGE(IDC_EDIT_TITLE_TGMARGINLEFT, OnChangeEditTitleTgmarginleft)
	ON_EN_CHANGE(IDC_EDIT_TITLE_TGMARGINRIGHT, OnChangeEditTitleTgmarginright)
	ON_EN_CHANGE(IDC_EDIT_TITLE_TGMARGINTOP, OnChangeEditTitleTgmargintop)
	ON_BN_CLICKED(IDC_CHECK_COVER_TITLE_TGSHOW, OnCheckCoverTitleTgshow)
	ON_BN_CLICKED(IDC_CHECK_COVER_TITLE_SHOW, OnCheckCoverTitleShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTWCoverTitleStylePage message handlers

BOOL CSTWCoverTitleStylePage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
	
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	if (pTmp != NULL)
	{
		m_nTitleMargin = pTmp->GetTitleAlign (Cover);

		CString s;
		s.Format ("%f", pTmp->GetTitleMargin (Cover));
		switch (m_nTitleMargin)
		{
		case 0:
			m_EditCoverTitleHAlignL.SetWindowText (s);
			m_EditCoverTitleHAlignL.EnableWindow ();
			break;
		case 1:
			m_EditCoverTitleHAlignR.SetWindowText (s);
			m_EditCoverTitleHAlignR.EnableWindow ();
			break;
		}

		m_dTitleVMarginTop = pTmp->GetTitleTop (Cover);
		m_dTitleVHeight = pTmp->GetTitleHeight (Cover);

		m_bShowTrackgroups = pTmp->GetDisplayTrackGroups (Cover);
		m_bShowTitles = pTmp->GetDisplayTitles (Cover);

		m_nTrackgroupHMargin = pTmp->GetTrackgroupHAlign (Cover);
		s.Format ("%f", pTmp->GetTrackgroupHMargin (Cover));
		switch (m_nTrackgroupHMargin)
		{
		case 0:
			m_EditCoverTrackgroupMarginL.SetWindowText (s);
			m_EditCoverTrackgroupMarginL.EnableWindow ();
			break;
		case 1:
			m_EditCoverTrackgroupMarginR.SetWindowText (s);
			m_EditCoverTrackgroupMarginR.EnableWindow ();
			break;
		}

		m_nTrackgroupVMargin = pTmp->GetTrackgroupVAlign (Cover);
		s.Format ("%f", pTmp->GetTrackgroupVMargin (Cover));
		switch (m_nTrackgroupVMargin)
		{
		case 0:
			m_EditCoverTrackgroupMarginT.SetWindowText (s);
			m_EditCoverTrackgroupMarginT.EnableWindow ();
			break;
		case 1:
			m_EditCoverTrackgroupMarginB.SetWindowText (s);
			m_EditCoverTrackgroupMarginB.EnableWindow ();
			break;
		}

		UpdateData (false);
	
		OnCheckCoverTitleTgshow ();
		OnCheckCoverTitleShow ();
	}

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CSTWCoverTitleStylePage::OnKillActive() 
{
	m_dTitleHAlign = GetHTitleMargin ();
	m_dTracksHAlign = GetHTracksMargin ();
	m_dTracksVAlign = GetVTracksMargin ();

	return CPropertyPage::OnKillActive();
}

void CSTWCoverTitleStylePage::OnRadioCoverTitlemargincenter() 
{
	EnableTitleAlignmentEdit ();
}

void CSTWCoverTitleStylePage::OnRadioCoverTitlemarginleft() 
{
	EnableTitleAlignmentEdit ();
}

void CSTWCoverTitleStylePage::OnRadioCoverTitlemarginright() 
{
	EnableTitleAlignmentEdit ();
}

void CSTWCoverTitleStylePage::EnableTitleAlignmentEdit()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nTitleAlign[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nTitleAlign[Back] =
		m_nTitleMargin;

	switch (m_nTitleMargin)
	{
	case 0:
		m_EditCoverTitleHAlignL.EnableWindow ();
		m_EditCoverTitleHAlignR.EnableWindow (false);
		m_EditCoverTitleHAlignL.SetFocus ();
		OnChangeEditCoverTitlehalignleft ();
		break;
	case 1:
		m_EditCoverTitleHAlignL.EnableWindow (false);
		m_EditCoverTitleHAlignR.EnableWindow ();
		m_EditCoverTitleHAlignR.SetFocus ();
		OnChangeEditCoverTitlehalignright ();
		break;
	default:
		m_EditCoverTitleHAlignL.EnableWindow (false);
		m_EditCoverTitleHAlignR.EnableWindow (false);
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWCoverTitleStylePage::OnRadioTitleTgmargintop() 
{
	EnableTrackgroupVAlignmentEdit ();
}

void CSTWCoverTitleStylePage::OnRadioTitleTgmarginbottom() 
{
	EnableTrackgroupVAlignmentEdit ();
}

void CSTWCoverTitleStylePage::OnRadioTitleTgvcenter() 
{
	EnableTrackgroupVAlignmentEdit ();
}

void CSTWCoverTitleStylePage::EnableTrackgroupVAlignmentEdit()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nTrackgroupVAlign[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nTrackgroupVAlign[Back] =
		m_nTrackgroupVMargin;

	switch (m_nTrackgroupVMargin)
	{
	case 0:
		m_EditCoverTrackgroupMarginB.EnableWindow (false);
		m_EditCoverTrackgroupMarginT.EnableWindow ();
		m_EditCoverTrackgroupMarginT.SetFocus ();
		OnChangeEditTitleTgmargintop ();
		break;
	case 1:
		m_EditCoverTrackgroupMarginB.EnableWindow ();
		m_EditCoverTrackgroupMarginT.EnableWindow (false);
		m_EditCoverTrackgroupMarginB.SetFocus ();
		OnChangeEditTitleTgmarginbottom ();
		break;
	default:
		m_EditCoverTrackgroupMarginB.EnableWindow (false);
		m_EditCoverTrackgroupMarginT.EnableWindow (false);
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWCoverTitleStylePage::OnRadioTitleTgmarginleft() 
{
	EnableTrackgroupHAlignmentEdit ();
}

void CSTWCoverTitleStylePage::OnRadioTitleTgmarginright() 
{
	EnableTrackgroupHAlignmentEdit ();
}

void CSTWCoverTitleStylePage::OnRadioTitleTghcentered() 
{
	EnableTrackgroupHAlignmentEdit ();
}

void CSTWCoverTitleStylePage::EnableTrackgroupHAlignmentEdit()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nTrackgroupHAlign[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nTrackgroupHAlign[Back] =
		m_nTrackgroupHMargin;

	switch (m_nTrackgroupHMargin)
	{
	case 0:
		m_EditCoverTrackgroupMarginL.EnableWindow ();
		m_EditCoverTrackgroupMarginR.EnableWindow (false);
		m_EditCoverTrackgroupMarginL.SetFocus ();
		OnChangeEditTitleTgmarginleft ();
		break;
	case 1:
		m_EditCoverTrackgroupMarginL.EnableWindow (false);
		m_EditCoverTrackgroupMarginR.EnableWindow ();
		m_EditCoverTrackgroupMarginR.SetFocus ();
		OnChangeEditTitleTgmarginright ();
		break;
	default:
		m_EditCoverTrackgroupMarginL.EnableWindow (false);
		m_EditCoverTrackgroupMarginR.EnableWindow (false);
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWCoverTitleStylePage::OnChangeEditCoverTitlehalignleft() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_COVER_TITLEHALIGNLEFT, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleMargin[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleMargin[Back] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWCoverTitleStylePage::OnChangeEditCoverTitlehalignright() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_COVER_TITLEHALIGNRIGHT, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleMargin[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleMargin[Back] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWCoverTitleStylePage::OnChangeEditCoverTitlevalignheight() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_COVER_TITLEVALIGNHEIGHT, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleHeight[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleHeight[Back] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWCoverTitleStylePage::OnChangeEditCoverTitlevaligntop() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_COVER_TITLEVALIGNTOP, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleTop[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleTop[Back] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWCoverTitleStylePage::OnChangeEditTitleTgmarginbottom() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TITLE_TGMARGINBOTTOM, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackgroupVMargin[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackgroupVMargin[Back] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWCoverTitleStylePage::OnChangeEditTitleTgmargintop() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TITLE_TGMARGINTOP, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackgroupVMargin[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackgroupVMargin[Back] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWCoverTitleStylePage::OnChangeEditTitleTgmarginleft() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TITLE_TGMARGINLEFT, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackgroupHMargin[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackgroupHMargin[Back] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWCoverTitleStylePage::OnChangeEditTitleTgmarginright() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TITLE_TGMARGINRIGHT, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackgroupHMargin[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTrackgroupHMargin[Back] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWCoverTitleStylePage::OnCheckCoverTitleTgshow() 
{
	UpdateData ();

	m_EditCoverTrackgroupMarginB.EnableWindow (m_bShowTrackgroups);
	m_EditCoverTrackgroupMarginT.EnableWindow (m_bShowTrackgroups);
	m_EditCoverTrackgroupMarginL.EnableWindow (m_bShowTrackgroups);
	m_EditCoverTrackgroupMarginR.EnableWindow (m_bShowTrackgroups);

	GetDlgItem (IDC_RADIO_TITLE_TGMARGINBOTTOM)->EnableWindow (m_bShowTrackgroups);
	GetDlgItem (IDC_RADIO_TITLE_TGMARGINTOP)->EnableWindow (m_bShowTrackgroups);
	GetDlgItem (IDC_RADIO_TITLE_TGVCENTER)->EnableWindow (m_bShowTrackgroups);
	GetDlgItem (IDC_RADIO_TITLE_TGMARGINLEFT)->EnableWindow (m_bShowTrackgroups);
	GetDlgItem (IDC_RADIO_TITLE_TGMARGINRIGHT)->EnableWindow (m_bShowTrackgroups);
	GetDlgItem (IDC_RADIO_TITLE_TGHCENTERED)->EnableWindow (m_bShowTrackgroups);

	if (m_bShowTrackgroups)
	{
		EnableTrackgroupHAlignmentEdit ();
		EnableTrackgroupVAlignmentEdit ();
	}

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bDisplayTrackgroups[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bDisplayTrackgroups[Back] =
		m_bShowTrackgroups ? true : false;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

double CSTWCoverTitleStylePage::GetHTitleMargin()
{
	UpdateData ();
	CString s = "0";

	switch (m_nTitleMargin)
	{
	case 0:
		m_EditCoverTitleHAlignL.GetWindowText (s);
		break;
	case 1:
		m_EditCoverTitleHAlignR.GetWindowText (s);
		break;
	}

	return atof (s);
}

double CSTWCoverTitleStylePage::GetHTracksMargin()
{
	UpdateData ();
	CString s = "0";

	switch (m_nTrackgroupHMargin)
	{
	case 0:
		m_EditCoverTrackgroupMarginL.GetWindowText (s);
		break;
	case 1:
		m_EditCoverTrackgroupMarginR.GetWindowText (s);
		break;
	}

	return atof (s);
}

double CSTWCoverTitleStylePage::GetVTracksMargin()
{
	UpdateData ();
	CString s = "0";

	switch (m_nTrackgroupVMargin)
	{
	case 0:
		m_EditCoverTrackgroupMarginT.GetWindowText (s);
		break;
	case 1:
		m_EditCoverTrackgroupMarginB.GetWindowText (s);
		break;
	}

	return atof (s);
}

void CSTWCoverTitleStylePage::OnCheckCoverTitleShow() 
{
	UpdateData ();

	m_EditCoverTitleHAlignL.EnableWindow (m_bShowTitles);
	m_EditCoverTitleHAlignR.EnableWindow (m_bShowTitles);

	GetDlgItem (IDC_RADIO_COVER_TITLEMARGINLEFT)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_RADIO_COVER_TITLEMARGINRIGHT)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_RADIO_COVER_TITLEMARGINCENTER)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_EDIT_COVER_TITLEVALIGNTOP)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_EDIT_COVER_TITLEVALIGNHEIGHT)->EnableWindow (m_bShowTitles);

	if (m_bShowTitles)
		EnableTitleAlignmentEdit ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bDisplayTitles[Cover] =
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bDisplayTitles[Back] =
		m_bShowTitles ? true : false;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}
