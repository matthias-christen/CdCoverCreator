// STWInlayTitleStylePage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "STWInlayTitleStylePage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplates.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSTWInlayTitleStylePage, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CSTWInlayTitleStylePage property page

CSTWInlayTitleStylePage::CSTWInlayTitleStylePage() : CPropertyPage(CSTWInlayTitleStylePage::IDD)
, m_nSpineTextDirection(0)
{
	//{{AFX_DATA_INIT(CSTWInlayTitleStylePage)
	m_nSidebarVAlign = 1;
	m_nTitleHAlign = 2;
	m_dTitleHeight = 2.0;
	m_dTop = 1.0;
	m_bShowSidebarTitles = FALSE;
	m_bShowTitles = FALSE;
	m_bInlaySideAntiparallel = TRUE;
	//}}AFX_DATA_INIT
}

CSTWInlayTitleStylePage::~CSTWInlayTitleStylePage()
{
}

void CSTWInlayTitleStylePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWInlayTitleStylePage)
	DDX_Control(pDX, IDC_EDIT_SIDE_VALIGNT, m_EditSideVAlignT);
	DDX_Control(pDX, IDC_EDIT_SIDE_VALIGNB, m_EditSideVAlignB);
	DDX_Control(pDX, IDC_EDIT_TITLE_HALIGNR, m_EditTitleHAlignR);
	DDX_Control(pDX, IDC_EDIT_TITLE_HALIGNL, m_EditTitleHAlignL);
	DDX_Radio(pDX, IDC_RADIO_SIDEBAR_VALIGN1, m_nSidebarVAlign);
	DDX_Radio(pDX, IDC_RADIO_TITLEHALIGN1, m_nTitleHAlign);
	DDX_Text(pDX, IDC_EDIT_TITLE_HEIGHT, m_dTitleHeight);
	DDV_MinMaxDouble(pDX, m_dTitleHeight, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_TITLE_TOP, m_dTop);
	DDV_MinMaxDouble(pDX, m_dTop, 0., 12.);
	DDX_Check(pDX, IDC_CHECK_INLAY_SHOWSIDEBARS, m_bShowSidebarTitles);
	DDX_Check(pDX, IDC_CHECK_INLAY_SHOWTITLES, m_bShowTitles);
	DDX_Check(pDX, IDC_CHECK_TDANTIPARALLEL2, m_bInlaySideAntiparallel);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_SPINETEXTDIR_UP, m_nSpineTextDirection);
}


BEGIN_MESSAGE_MAP(CSTWInlayTitleStylePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWInlayTitleStylePage)
	ON_BN_CLICKED(IDC_RADIO_TITLEHALIGN1, OnRadioTitlehalign1)
	ON_BN_CLICKED(IDC_RADIO_TITLEHALIGN2, OnRadioTitlehalign2)
	ON_BN_CLICKED(IDC_RADIO_TITLEHALIGN3, OnRadioTitlehalign3)
	ON_EN_CHANGE(IDC_EDIT_TITLE_HALIGNL, OnChangeEditTitleHalignl)
	ON_EN_CHANGE(IDC_EDIT_TITLE_HALIGNR, OnChangeEditTitleHalignr)
	ON_EN_CHANGE(IDC_EDIT_TITLE_HEIGHT, OnChangeEditTitleHeight)
	ON_EN_CHANGE(IDC_EDIT_TITLE_TOP, OnChangeEditTitleTop)
	ON_BN_CLICKED(IDC_RADIO_SIDEBAR_VALIGN1, OnRadioSidebarValign1)
	ON_BN_CLICKED(IDC_RADIO_SIDEBAR_VALIGN2, OnRadioSidebarValign2)
	ON_BN_CLICKED(IDC_RADIO_SIDEBAR_VALIGN3, OnRadioSidebarValign3)
	ON_EN_CHANGE(IDC_EDIT_SIDE_VALIGNB, OnChangeEditSideValignb)
	ON_EN_CHANGE(IDC_EDIT_SIDE_VALIGNT, OnChangeEditSideValignt)
	ON_BN_CLICKED(IDC_CHECK_INLAY_SHOWTITLES, OnCheckInlayShowtitles)
	ON_BN_CLICKED(IDC_CHECK_INLAY_SHOWSIDEBARS, OnCheckInlayShowsidebars)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_SPINETEXTDIR_UP, OnRadioSpineUp)
	ON_BN_CLICKED(IDC_RADIO_SPINETEXTDIR_DOWN, OnRadioSpineDown)
	ON_BN_CLICKED(IDC_CHECK_TDANTIPARALLEL2, OnCheckAntiparallel)
END_MESSAGE_MAP()

BOOL CSTWInlayTitleStylePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSTWInlayTitleStylePage::OnRadioTitlehalign1() 
{
	EnableAlignmentEdit ();
}

void CSTWInlayTitleStylePage::OnRadioTitlehalign2() 
{
	EnableAlignmentEdit ();
}

void CSTWInlayTitleStylePage::OnRadioTitlehalign3() 
{
	EnableAlignmentEdit ();
}

void CSTWInlayTitleStylePage::EnableAlignmentEdit()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nTitleAlign[Inlay] = m_nTitleHAlign;

	switch (m_nTitleHAlign)
	{
	case 0:
		m_EditTitleHAlignL.EnableWindow ();
		m_EditTitleHAlignR.EnableWindow (false);
		m_EditTitleHAlignL.SetFocus ();
		OnChangeEditTitleHalignl ();
		break;
	case 1:
		m_EditTitleHAlignL.EnableWindow (false);
		m_EditTitleHAlignR.EnableWindow ();
		m_EditTitleHAlignR.SetFocus ();
		OnChangeEditTitleHalignr ();
		break;
	default:
		m_EditTitleHAlignL.EnableWindow (false);
		m_EditTitleHAlignR.EnableWindow (false);
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

BOOL CSTWInlayTitleStylePage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
	
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	if (pTmp != NULL)
	{
		m_nTitleHAlign = pTmp->GetTitleAlign (Inlay);

		CString s;
		s.Format ("%f", pTmp->GetTitleMargin (Inlay));
		switch (m_nTitleHAlign)
		{
		case 0:
			m_EditTitleHAlignL.SetWindowText (s);
			m_EditTitleHAlignL.EnableWindow ();
			break;
		case 1:
			m_EditTitleHAlignR.SetWindowText (s);
			m_EditTitleHAlignR.EnableWindow ();
			break;
		}

		m_dTop = pTmp->GetTitleTop (Inlay);
		m_dTitleHeight = pTmp->GetTitleHeight (Inlay);

		m_nSidebarVAlign = pTmp->GetInlaySideVAlign ();
		s.Format ("%f", pTmp->GetInlaySideVMargin ());
		switch (m_nSidebarVAlign)
		{
		case 0:
			m_EditSideVAlignT.SetWindowText (s);
			m_EditSideVAlignT.EnableWindow ();
			break;
		case 1:
			m_EditSideVAlignB.SetWindowText (s);
			m_EditSideVAlignB.EnableWindow ();
			break;
		}

		m_bShowTitles = pTmp->GetDisplayTitles (Inlay);
		m_bShowSidebarTitles = pTmp->GetInlayDisplaySidebarTitles ();
		m_nSpineTextDirection = pTmp->GetInlaySideTextDirection () ? 0 : 1;
		m_bInlaySideAntiparallel = pTmp->IsInlaySideAntiparallel () ? TRUE : FALSE;

		UpdateData (false);

		OnCheckInlayShowtitles ();
		OnCheckInlayShowsidebars ();
	}

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CSTWInlayTitleStylePage::OnKillActive() 
{
	m_dTitleHAlign = GetHTitleMargin ();
	return CPropertyPage::OnKillActive();
}

double CSTWInlayTitleStylePage::GetHTitleMargin()
{
	UpdateData ();
	CString s = "0";

	switch (m_nTitleHAlign)
	{
	case 0:
		m_EditTitleHAlignL.GetWindowText (s);
		break;
	case 1:
		m_EditTitleHAlignR.GetWindowText (s);
		break;
	}

	return atof (s);
}

void CSTWInlayTitleStylePage::OnChangeEditTitleHalignl() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TITLE_HALIGNL, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleMargin[Inlay] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnChangeEditTitleHalignr() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TITLE_HALIGNR, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleMargin[Inlay] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnChangeEditTitleHeight() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TITLE_HEIGHT, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleHeight[Inlay] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnChangeEditTitleTop() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_TITLE_TOP, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dTitleTop[Inlay] = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnChangeEditSideValignb() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_SIDE_VALIGNB, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dInlaySideVMargin = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnChangeEditSideValignt() 
{
	CString s;
	GetDlgItemText (IDC_EDIT_SIDE_VALIGNT, s);
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_dInlaySideVMargin = atof (s);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::SetSidebarVAlign()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_nInlaySideVAlign = m_nSidebarVAlign;

	switch (m_nSidebarVAlign)
	{
	case 0:
		m_EditSideVAlignT.EnableWindow ();
		m_EditSideVAlignB.EnableWindow (false);
		m_EditSideVAlignT.SetFocus ();
		OnChangeEditSideValignt ();
		break;
	case 1:
		m_EditSideVAlignT.EnableWindow (false);
		m_EditSideVAlignB.EnableWindow ();
		m_EditSideVAlignB.SetFocus ();
		OnChangeEditSideValignb ();
		break;
	default:
		m_EditSideVAlignT.EnableWindow (false);
		m_EditSideVAlignB.EnableWindow (false);
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWInlayTitleStylePage::OnRadioSidebarValign1() 
{
	SetSidebarVAlign ();
}

void CSTWInlayTitleStylePage::OnRadioSidebarValign2() 
{
	SetSidebarVAlign ();
}

void CSTWInlayTitleStylePage::OnRadioSidebarValign3() 
{
	SetSidebarVAlign ();
}

void CSTWInlayTitleStylePage::OnCheckInlayShowtitles() 
{
	UpdateData ();

	m_EditTitleHAlignL.EnableWindow (m_bShowTitles);
	m_EditTitleHAlignR.EnableWindow (m_bShowTitles);

	GetDlgItem (IDC_RADIO_TITLEHALIGN1)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_RADIO_TITLEHALIGN2)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_RADIO_TITLEHALIGN3)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_EDIT_TITLE_TOP)->EnableWindow (m_bShowTitles);
	GetDlgItem (IDC_EDIT_TITLE_HEIGHT)->EnableWindow (m_bShowTitles);

	if (m_bShowTitles)
		EnableAlignmentEdit ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bDisplayTitles[Inlay] = m_bShowTitles ? true : false;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnCheckInlayShowsidebars() 
{
	UpdateData ();

	m_EditSideVAlignT.EnableWindow (m_bShowSidebarTitles);
	m_EditSideVAlignT.EnableWindow (m_bShowSidebarTitles);

	GetDlgItem (IDC_RADIO_SIDEBAR_VALIGN1)->EnableWindow (m_bShowSidebarTitles);
	GetDlgItem (IDC_RADIO_SIDEBAR_VALIGN2)->EnableWindow (m_bShowSidebarTitles);
	GetDlgItem (IDC_RADIO_SIDEBAR_VALIGN3)->EnableWindow (m_bShowSidebarTitles);

	if (m_bShowSidebarTitles)
		SetSidebarVAlign ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bInlayDisplaySidebarTitles = m_bShowSidebarTitles ? true : false;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnRadioSpineUp ()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bInlaySideTextDirection = m_nSpineTextDirection == 0;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnRadioSpineDown ()
{
	UpdateData ();

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bInlaySideTextDirection = m_nSpineTextDirection == 0;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWInlayTitleStylePage::OnCheckAntiparallel ()
{
	UpdateData ();
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_bInlaySideAntiparallel = m_bInlaySideAntiparallel ? true : false;
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}