// StyleDlgTrackgroupings.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "StyleDlgTrackgroupings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgTrackgroupings dialog


CStyleDlgTrackgroupings::CStyleDlgTrackgroupings(CWnd* pParent /*=NULL*/)
	: CStyleDlgBase(CStyleDlgTrackgroupings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStyleDlgTrackgroupings)
	m_bShowTrackGroupings = FALSE;
	m_bShowTracks = FALSE;
	m_dHAlignLeft = 0.0;
	m_dHAlignRight = 0.0;
	m_dVAlignBottom = 0.0;
	m_dVAlignTop = 0.0;
	m_nHAlign = -1;
	m_nTextMode = -1;
	m_nVAlign = -1;
	//}}AFX_DATA_INIT
}


void CStyleDlgTrackgroupings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyleDlgTrackgroupings)
	DDX_Control(pDX, IDC_BUTTON_HALIGN_CENTER, m_btnHAlignCenter);
	DDX_Check(pDX, IDC_CHECK_SHOWGROUPINGS, m_bShowTrackGroupings);
	DDX_Check(pDX, IDC_CHECK_SHOWTRACKS, m_bShowTracks);
	DDX_Text(pDX, IDC_EDIT_HALIGN_LEFT, m_dHAlignLeft);
	DDX_Text(pDX, IDC_EDIT_HALIGNRIGHT, m_dHAlignRight);
	DDX_Text(pDX, IDC_EDIT_VALIGN_BOTTOM, m_dVAlignBottom);
	DDX_Text(pDX, IDC_EDIT_VALIGN_TOP, m_dVAlignTop);
	DDX_Radio(pDX, IDC_RADIO_HALIGN_LEFT, m_nHAlign);
	DDX_Radio(pDX, IDC_RADIO_ROUNDTEXT, m_nTextMode);
	DDX_Radio(pDX, IDC_RADIO_VALIGN_TOP, m_nVAlign);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStyleDlgTrackgroupings, CDialog)
	//{{AFX_MSG_MAP(CStyleDlgTrackgroupings)
	ON_BN_CLICKED(IDC_BUTTON_HALIGN_CENTER, OnButtonHalignCenter)
	ON_BN_CLICKED(IDC_CHECK_SHOWGROUPINGS, OnCheckShowgroupings)
	ON_BN_CLICKED(IDC_CHECK_SHOWTRACKS, OnCheckShowtracks)
	ON_EN_CHANGE(IDC_EDIT_HALIGN_LEFT, OnChangeEditHalignLeft)
	ON_EN_CHANGE(IDC_EDIT_HALIGNRIGHT, OnChangeEditHalignright)
	ON_EN_CHANGE(IDC_EDIT_VALIGN_BOTTOM, OnChangeEditValignBottom)
	ON_EN_CHANGE(IDC_EDIT_VALIGN_TOP, OnChangeEditValignTop)
	ON_BN_CLICKED(IDC_RADIO_HALIGN_CENTER, OnRadioHalignCenter)
	ON_BN_CLICKED(IDC_RADIO_HALIGN_LEFT, OnRadioHalignLeft)
	ON_BN_CLICKED(IDC_RADIO_HALIGN_RIGHT, OnRadioHalignRight)
	ON_BN_CLICKED(IDC_RADIO_ROUNDTEXT, OnRadioRoundtext)
	ON_BN_CLICKED(IDC_RADIO_STRAIGHTTEXT, OnRadioStraighttext)
	ON_BN_CLICKED(IDC_RADIO_VALIGN_BOTTOM, OnRadioValignBottom)
	ON_BN_CLICKED(IDC_RADIO_VALIGN_CENTER, OnRadioValignCenter)
	ON_BN_CLICKED(IDC_RADIO_VALIGN_TOP, OnRadioValignTop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgTrackgroupings message handlers

void CStyleDlgTrackgroupings::SetTemplate(CStyleTemplate *pStyle, RenderType rt)
{
	m_pStyle = pStyle;
	m_RenderType = rt;

	if ((rt == Cover) || (rt == Label))
	{
		if (pStyle != NULL)
		{
			m_nHAlign = rt == Label ? 2 : pStyle->GetTrackgroupHAlign (rt);
			if ((0 > m_nHAlign) || (m_nHAlign > 2))
				m_nHAlign = 0;

			if (m_nHAlign == 0)
				m_dHAlignLeft = pStyle->GetTrackgroupHMargin (rt);
			else if (m_nHAlign == 1)
				m_dHAlignRight = pStyle->GetTrackgroupHMargin (rt);

			m_nVAlign = pStyle->GetTrackgroupVAlign (rt);
			if ((0 > m_nVAlign) || (m_nVAlign > 2))
				m_nVAlign = 0;

			if (m_nVAlign == 0)
				m_dVAlignTop = pStyle->GetTrackgroupVMargin (rt);
			else if (m_nVAlign == 1)
				m_dVAlignBottom = pStyle->GetTrackgroupVMargin (rt);

			m_bShowTrackGroupings = pStyle->GetDisplayTrackGroups (rt) ? TRUE : FALSE;
			switch (rt)
			{
			case Cover:
			case Booklet:
				m_bShowTracks = false; break;
			case Back:
			case Inlay:
				m_bShowTracks = true;
				m_bShowTrackGroupings = true;
				break;
			case Label:
				m_bShowTracks = pStyle->IsLabelTrackDisplayed (); break;
			}

			m_nTextMode = pStyle->IsLabelTrackRound () ? 0 : 1;

			UpdateData (false);
		}
/*
		CString strCaption;
		strCaption.LoadString (rt == Label ? IDS_VERTMARGIN : IDS_HORZMARGIN);
		m_grpHorz.SetWindowText (strCaption);
		strCaption.LoadString (rt == Label ? IDS_MEASURES : IDS_VERTMARGIN);
		m_grpVert.SetWindowText (strCaption);
		strCaption.LoadString (rt == Label ? IDS_TOP : IDS_LEFT);
		GetDlgItem (IDC_RADIO_TITLEMARGINLEFT)->SetWindowText (strCaption);
		strCaption.LoadString (rt == Label ? IDS_BOTTOM : IDS_RIGHT);
		GetDlgItem (IDC_RADIO_TITLEMARGINRIGHT)->SetWindowText (strCaption);
*/
		GetDlgItem (IDC_BUTTON_HALIGN_CENTER)->ShowWindow (rt == Label ? SW_SHOW : SW_HIDE);
		GetDlgItem (IDC_RADIO_ROUNDTEXT)->ShowWindow (rt == Label ? SW_SHOW : SW_HIDE);
		GetDlgItem (IDC_RADIO_STRAIGHTTEXT)->ShowWindow (rt == Label ? SW_SHOW : SW_HIDE);

		GetDlgItem (IDC_RADIO_VALIGN_TOP)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		GetDlgItem (IDC_EDIT_VALIGN_TOP)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		GetDlgItem (IDC_STATIC_CM1)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		GetDlgItem (IDC_RADIO_VALIGN_BOTTOM)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		GetDlgItem (IDC_EDIT_VALIGN_BOTTOM)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		GetDlgItem (IDC_STATIC_CM2)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		GetDlgItem (IDC_RADIO_VALIGN_CENTER)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
	
		m_btnHAlignCenter.EnableWindow ();
		GetDlgItem (IDC_CHECK_SHOWGROUPINGS)->EnableWindow ();
		GetDlgItem (IDC_CHECK_SHOWTRACKS)->EnableWindow (rt == Label);
		GetDlgItem (IDC_RADIO_HALIGN_LEFT)->EnableWindow (rt != Label);
//		GetDlgItem (IDC_EDIT_HALIGN_LEFT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
		GetDlgItem (IDC_RADIO_HALIGN_RIGHT)->EnableWindow (rt != Label);
//		GetDlgItem (IDC_EDIT_HALIGNRIGHT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
		GetDlgItem (IDC_RADIO_HALIGN_CENTER)->EnableWindow ();
		GetDlgItem (IDC_RADIO_VALIGN_TOP)->EnableWindow ();
		GetDlgItem (IDC_RADIO_VALIGN_BOTTOM)->EnableWindow ();
		GetDlgItem (IDC_RADIO_VALIGN_CENTER)->EnableWindow ();
//		GetDlgItem (IDC_EDIT_VALIGN_TOP)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
//		GetDlgItem (IDC_EDIT_VALIGN_BOTTOM)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
		GetDlgItem (IDC_RADIO_ROUNDTEXT)->EnableWindow ();
		GetDlgItem (IDC_RADIO_STRAIGHTTEXT)->EnableWindow ();

		EnableHAlignmentEdit ();
		EnableVAlignmentEdit ();		
	}
	else
	{
		m_btnHAlignCenter.EnableWindow (false);
		GetDlgItem (IDC_CHECK_SHOWGROUPINGS)->EnableWindow (false);
		GetDlgItem (IDC_CHECK_SHOWTRACKS)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_HALIGN_LEFT)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_HALIGN_LEFT)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_HALIGN_RIGHT)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_HALIGNRIGHT)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_HALIGN_CENTER)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_VALIGN_TOP)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_VALIGN_BOTTOM)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_VALIGN_CENTER)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_VALIGN_TOP)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_VALIGN_BOTTOM)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_ROUNDTEXT)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_STRAIGHTTEXT)->EnableWindow (false);
	}
}

void CStyleDlgTrackgroupings::OnButtonHalignCenter() 
{
	if (m_pStyle == NULL)
		return;

	m_pStyle->m_nLabelTrackPosition = ~m_pStyle->m_nLabelTrackPosition & 1;

	CString s;
	s.LoadString (m_pStyle->m_nLabelTrackPosition == 0 ? IDS_LABELTITLEPOS_ABOVE : IDS_LABELTITLEPOS_BELOW);
	m_btnHAlignCenter.SetWindowText (s);

	UpdateDoc ();
}

void CStyleDlgTrackgroupings::OnCheckShowgroupings() 
{
	UpdateData ();

	m_btnHAlignCenter.EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_HALIGN_LEFT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_HALIGN_RIGHT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_VALIGN_TOP)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_VALIGN_BOTTOM)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_VALIGN_CENTER)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_ROUNDTEXT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_STRAIGHTTEXT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);

//	if (m_bShowTitle)
	EnableHAlignmentEdit ();
	EnableVAlignmentEdit ();

	if (m_pStyle != NULL)
		m_pStyle->m_bDisplayTrackgroups[m_RenderType] = m_bShowTrackGroupings || m_bShowTracks ? true : false;

	UpdateDoc ();
}

void CStyleDlgTrackgroupings::OnCheckShowtracks() 
{
	UpdateData ();

	m_btnHAlignCenter.EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_HALIGN_LEFT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_HALIGN_RIGHT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_VALIGN_TOP)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_VALIGN_BOTTOM)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_VALIGN_CENTER)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_EDIT_VALIGN_BOTTOM)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
	GetDlgItem (IDC_RADIO_STRAIGHTTEXT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);

//	if (m_bShowTitle)
	EnableHAlignmentEdit ();
	EnableVAlignmentEdit ();

	if (m_pStyle != NULL)
		m_pStyle->m_bLabelDisplayTracks = m_bShowTracks ? true : false;

	UpdateDoc ();
}

void CStyleDlgTrackgroupings::OnChangeEditHalignLeft() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_HALIGN_LEFT, s);
	if (s.IsEmpty ())
		return;

	m_pStyle->m_dTrackgroupHMargin[m_RenderType] = atof (s);
	m_pStyle->m_nTrackgroupHAlign[m_RenderType] = m_nHAlign;

	UpdateDoc (IDC_EDIT_HALIGN_LEFT);
}

void CStyleDlgTrackgroupings::OnChangeEditHalignright() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_HALIGNRIGHT, s);
	if (s.IsEmpty ())
		return;

	m_pStyle->m_dTrackgroupHMargin[m_RenderType] = atof (s);
	m_pStyle->m_nTrackgroupHAlign[m_RenderType] = m_nHAlign;

	UpdateDoc (IDC_EDIT_HALIGNRIGHT);
}

void CStyleDlgTrackgroupings::OnChangeEditValignBottom() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_VALIGN_BOTTOM, s);
	if (s.IsEmpty ())
		return;

	m_pStyle->m_dTrackgroupVMargin[m_RenderType] = atof (s);
	m_pStyle->m_nTrackgroupVAlign[m_RenderType] = m_nVAlign;

	UpdateDoc (IDC_EDIT_VALIGN_BOTTOM);
}

void CStyleDlgTrackgroupings::OnChangeEditValignTop() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_VALIGN_TOP, s);
	if (s.IsEmpty ())
		return;

	m_pStyle->m_dTrackgroupVMargin[m_RenderType] = atof (s);
	m_pStyle->m_nTrackgroupVAlign[m_RenderType] = m_nVAlign;

	UpdateDoc (IDC_EDIT_VALIGN_TOP);
}

void CStyleDlgTrackgroupings::OnRadioHalignCenter() 
{
	if (m_nHAlign == 2)
		return;	// is already set

	m_nHAlign = 2;
	EnableHAlignmentEdit ();
	UpdateDoc (IDC_RADIO_HALIGN_CENTER);
}

void CStyleDlgTrackgroupings::OnRadioHalignLeft() 
{
	if (m_nHAlign == 0)
		return;	// is already set

	m_nHAlign = 0;
	EnableHAlignmentEdit ();
	UpdateDoc (IDC_RADIO_HALIGN_LEFT);
}

void CStyleDlgTrackgroupings::OnRadioHalignRight() 
{
	if (m_nHAlign == 1)
		return;	// is already set

	m_nHAlign = 1;
	EnableHAlignmentEdit ();
	UpdateDoc (IDC_RADIO_HALIGN_RIGHT);
}

void CStyleDlgTrackgroupings::OnRadioRoundtext() 
{
	if (m_pStyle == NULL)
		return;

	if (!m_pStyle->IsLabelTrackRound ())
	{
		UpdateData ();
		//EnableLabelTitleEdit ();

		m_pStyle->m_bLabelTrackRound = true;
		UpdateDoc ();
	}
}

void CStyleDlgTrackgroupings::OnRadioStraighttext() 
{
	if (m_pStyle == NULL)
		return;

	if (m_pStyle->IsLabelTrackRound ())
	{
		UpdateData ();
		//EnableLabelTitleEdit ();

		m_pStyle->m_bLabelTrackRound = false;
		UpdateDoc ();
	}
}

void CStyleDlgTrackgroupings::OnRadioValignBottom() 
{
	if (m_nVAlign == 1)
		return;	// is already set

	m_nVAlign = 1;
	EnableVAlignmentEdit ();
	UpdateDoc (IDC_RADIO_VALIGN_BOTTOM);
}

void CStyleDlgTrackgroupings::OnRadioValignCenter() 
{
	if (m_nVAlign == 2)
		return;	// is already set

	m_nVAlign = 2;
	EnableVAlignmentEdit ();
	UpdateDoc (IDC_RADIO_VALIGN_CENTER);	
}

void CStyleDlgTrackgroupings::OnRadioValignTop() 
{
	if (m_nVAlign == 0)
		return;	// is already set

	m_nVAlign = 0;
	EnableVAlignmentEdit ();
	UpdateDoc (IDC_RADIO_VALIGN_TOP);	
}

void CStyleDlgTrackgroupings::EnableHAlignmentEdit()
{
	UpdateData ();

	switch (m_nHAlign)
	{
	case 0:
		GetDlgItem (IDC_EDIT_HALIGN_LEFT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
		GetDlgItem (IDC_EDIT_HALIGNRIGHT)->EnableWindow (false);
		m_btnHAlignCenter.EnableWindow (false);
		OnChangeEditHalignLeft ();
		break;
	case 1:
		GetDlgItem (IDC_EDIT_HALIGN_LEFT)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_HALIGNRIGHT)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
		m_btnHAlignCenter.EnableWindow (false);
		OnChangeEditHalignright ();
		break;
	case 2:
		GetDlgItem (IDC_EDIT_HALIGN_LEFT)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_HALIGNRIGHT)->EnableWindow (false);
		m_btnHAlignCenter.EnableWindow ();

		m_pStyle->m_nTrackgroupHAlign[m_RenderType] = 2;
	}
}

void CStyleDlgTrackgroupings::EnableVAlignmentEdit()
{
	UpdateData ();

	switch (m_nVAlign)
	{
	case 0:
		GetDlgItem (IDC_EDIT_VALIGN_TOP)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
		GetDlgItem (IDC_EDIT_VALIGN_BOTTOM)->EnableWindow (false);
		OnChangeEditValignTop ();
		break;
	case 1:
		GetDlgItem (IDC_EDIT_VALIGN_TOP)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_VALIGN_BOTTOM)->EnableWindow (m_bShowTrackGroupings || m_bShowTracks);
		OnChangeEditValignBottom ();
		break;
	case 2:
		GetDlgItem (IDC_EDIT_VALIGN_TOP)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_VALIGN_BOTTOM)->EnableWindow (false);

		m_pStyle->m_nTrackgroupVAlign[m_RenderType] = 2;
	}
}
