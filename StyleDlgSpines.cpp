// StyleDlgSpines.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "StyleDlgSpines.h"


// CStyleDlgSpines dialog

IMPLEMENT_DYNAMIC(CStyleDlgSpines, CDialog)
CStyleDlgSpines::CStyleDlgSpines(CWnd* pParent /*=NULL*/)
	: CStyleDlgBase (CStyleDlgSpines::IDD, pParent)
	, m_bShowTitle(false)
	, m_nVerticalAlign(0)
	, m_dMarginTop(0)
	, m_dMarginBottom(0)
	, m_nTextDirection(0)
	, m_bInlaySideAntiparallel(0)
{
	m_pStyle = NULL;
}

CStyleDlgSpines::~CStyleDlgSpines()
{
}

void CStyleDlgSpines::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_SHOWTITLES, m_bShowTitle);
	DDX_Text(pDX, IDC_EDIT_VATOP, m_dMarginTop);
	DDX_Text(pDX, IDC_EDIT_VABOTTOM, m_dMarginBottom);
	DDX_Radio(pDX, IDC_RADIO_VATOP, m_nVerticalAlign);
	DDX_Radio(pDX, IDC_RADIO_TDUP, m_nTextDirection);
	DDX_Check(pDX, IDC_CHECK_TDANTIPARALLEL, m_bInlaySideAntiparallel);
}

void CStyleDlgSpines::SetTemplate(CStyleTemplate *pStyle, RenderType rt)
{
	m_pStyle = pStyle;
	m_RenderType = rt;

	if ((rt == Inlay) && (pStyle != NULL))
	{
		m_bShowTitle = pStyle->GetInlayDisplaySidebarTitles ();
		m_nVerticalAlign = pStyle->GetInlaySideVAlign ();
		m_dMarginTop = pStyle->GetInlaySideVMargin ();
		m_dMarginBottom = pStyle->GetInlaySideVMargin ();
		m_nTextDirection = pStyle->GetInlaySideTextDirection () ? 0 : 1;
		m_bInlaySideAntiparallel = pStyle->IsInlaySideAntiparallel () ? 1 : 0;

		UpdateData (false);

		GetDlgItem (IDC_CHECK_SHOWTITLES)->EnableWindow ();
		GetDlgItem (IDC_RADIO_VATOP)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_RADIO_VABOTTOM)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_RADIO_VACENTER)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_EDIT_VATOP)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_EDIT_VABOTTOM)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_RADIO_TDUP)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_RADIO_TDDOWN)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_CHECK_TDANTIPARALLEL)->EnableWindow (m_bShowTitle);

		EnableTitleAlignmentEdit ();
	}
	else
	{
		GetDlgItem (IDC_CHECK_SHOWTITLES)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_VATOP)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_VABOTTOM)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_VACENTER)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_VATOP)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_VABOTTOM)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_TDUP)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_TDDOWN)->EnableWindow (false);
		GetDlgItem (IDC_CHECK_TDANTIPARALLEL)->EnableWindow (false);
	}
}


BEGIN_MESSAGE_MAP(CStyleDlgSpines, CDialog)
	ON_BN_CLICKED(IDC_RADIO_VACENTER, OnRadioAlignCenter)
	ON_BN_CLICKED(IDC_RADIO_VABOTTOM, OnRadioAlignBottom)
	ON_BN_CLICKED(IDC_RADIO_VATOP, OnRadioAlignTop)
	ON_EN_CHANGE(IDC_EDIT_VATOP, OnChangeEditAlignTop)
	ON_EN_CHANGE(IDC_EDIT_VABOTTOM, OnChangeEditAlignBottom)
	ON_BN_CLICKED(IDC_CHECK_SHOWTITLES, OnCheckTitleShow)
	ON_BN_CLICKED(IDC_RADIO_TDUP, OnRadioTextDirUp)
	ON_BN_CLICKED(IDC_RADIO_TDDOWN, OnRadioTextDirDown)
	ON_BN_CLICKED(IDC_CHECK_TDANTIPARALLEL, OnCheckAntiparallel)
END_MESSAGE_MAP()


// CStyleDlgSpines message handlers
void CStyleDlgSpines::OnRadioAlignCenter() 
{
	if (m_nVerticalAlign == 2)
		return;	// is already set

	m_nVerticalAlign = 2;
	EnableTitleAlignmentEdit ();
	UpdateDoc (IDC_RADIO_VACENTER);
}

void CStyleDlgSpines::OnRadioAlignTop() 
{
	if (m_nVerticalAlign == 0)
		return;	// is already set

	m_nVerticalAlign = 0;
	EnableTitleAlignmentEdit ();
	UpdateDoc (IDC_RADIO_VATOP);
}

void CStyleDlgSpines::OnRadioAlignBottom() 
{
	if (m_nVerticalAlign == 1)
		return;	// is already set

	m_nVerticalAlign = 1;
	EnableTitleAlignmentEdit ();
	UpdateDoc (IDC_RADIO_VABOTTOM);
}

void CStyleDlgSpines::EnableTitleAlignmentEdit()
{
	if (m_pStyle == NULL)
		return;

	UpdateData ();

	switch (m_nVerticalAlign)
	{
	case 0:
		GetDlgItem (IDC_EDIT_VATOP)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_EDIT_VABOTTOM)->EnableWindow (false);
		OnChangeEditAlignTop ();
		break;
	case 1:
		GetDlgItem (IDC_EDIT_VATOP)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_VABOTTOM)->EnableWindow (m_bShowTitle);
		OnChangeEditAlignBottom ();
		break;
	case 2:
		GetDlgItem (IDC_EDIT_VATOP)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_VABOTTOM)->EnableWindow (false);

		m_pStyle->m_nInlaySideVAlign = 2;
	}
}

void CStyleDlgSpines::OnChangeEditAlignTop () 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_VATOP, s);
	if (s.IsEmpty ())
		return;

	m_pStyle->m_dInlaySideVMargin = atof (s);
	m_pStyle->m_nInlaySideVAlign = m_nVerticalAlign;

	UpdateDoc (IDC_EDIT_VATOP);
}

void CStyleDlgSpines::OnChangeEditAlignBottom() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_VABOTTOM, s);
	if (s.IsEmpty ())
		return;

	m_pStyle->m_dInlaySideVMargin = atof (s);
	m_pStyle->m_nInlaySideVAlign = m_nVerticalAlign;

	// update (but only if this edit control has the focus
	UpdateDoc (IDC_EDIT_VABOTTOM);
}

void CStyleDlgSpines::OnCheckTitleShow() 
{
	UpdateData ();

	GetDlgItem (IDC_RADIO_VATOP)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_RADIO_VABOTTOM)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_RADIO_VACENTER)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_EDIT_VATOP)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_EDIT_VABOTTOM)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_RADIO_TDUP)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_RADIO_TDDOWN)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_CHECK_TDANTIPARALLEL)->EnableWindow (m_bShowTitle);

	EnableTitleAlignmentEdit ();

	if (m_pStyle != NULL)
		m_pStyle->m_bInlayDisplaySidebarTitles = m_bShowTitle ? true : false;

	UpdateDoc ();
}

void CStyleDlgSpines::OnRadioTextDirUp () 
{
	if (m_pStyle == NULL)
		return;

	if (!m_pStyle->GetInlaySideTextDirection ())
	{
		UpdateData ();

		m_pStyle->m_bInlaySideTextDirection = true;
		UpdateDoc ();
	}
}

void CStyleDlgSpines::OnRadioTextDirDown () 
{
	if (m_pStyle == NULL)
		return;

	if (m_pStyle->GetInlaySideTextDirection ())
	{
		UpdateData ();

		m_pStyle->m_bInlaySideTextDirection = false;
		UpdateDoc ();
	}
}

void CStyleDlgSpines::OnCheckAntiparallel ()
{
	if (m_pStyle == NULL)
		return;

	UpdateData ();

	m_pStyle->m_bInlaySideAntiparallel = m_bInlaySideAntiparallel ? true : false;;
	UpdateDoc ();
}