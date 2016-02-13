// StyleDlgMeasures.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"
#include "StyleDlgMeasures.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgMeasures dialog


CStyleDlgMeasures::CStyleDlgMeasures(CWnd* pParent /*=NULL*/)
	: CStyleDlgBase(CStyleDlgMeasures::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStyleDlgMeasures)
	m_bShowTitle = FALSE;
	m_dHAlignLeft = 0.0;
	m_dHAlignRight = 0.0;
	m_dVAlignHeight = 0.0;
	m_dVAlignTop = 0.0;
	m_nLabelTitleMode = -1;
	m_nTitleMargin = -1;
	//}}AFX_DATA_INIT

	m_pStyle = NULL;
}


void CStyleDlgMeasures::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyleDlgMeasures)
	DDX_Control(pDX, IDC_GROUP_VERT, m_grpVert);
	DDX_Control(pDX, IDC_GROUP_HORZ, m_grpHorz);
	DDX_Control(pDX, IDC_STATIC_TOP, m_stcTop);
	DDX_Control(pDX, IDC_STATIC_HEIGHT, m_stcHeight);
	DDX_Control(pDX, IDC_STATIC_CM2, m_stcCm2);
	DDX_Control(pDX, IDC_STATIC_CM1, m_stcCm1);
	DDX_Control(pDX, IDC_BUTTON_LABELTRACKTITLECENTER, m_btnLabelTrackTitleCenter);
	DDX_Check(pDX, IDC_CHECK_TITLE_SHOW, m_bShowTitle);
	DDX_Text(pDX, IDC_EDIT_TITLEHALIGNLEFT, m_dHAlignLeft);
	DDX_Text(pDX, IDC_EDIT_TITLEHALIGNRIGHT, m_dHAlignRight);
	DDX_Text(pDX, IDC_EDIT_TITLEVALIGNHEIGHT, m_dVAlignHeight);
	DDX_Text(pDX, IDC_EDIT_TITLEVALIGNTOP, m_dVAlignTop);
	DDX_Radio(pDX, IDC_RADIO_LABEL_TITLEROUND, m_nLabelTitleMode);
	DDX_Radio(pDX, IDC_RADIO_TITLEMARGINLEFT, m_nTitleMargin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStyleDlgMeasures, CDialog)
	//{{AFX_MSG_MAP(CStyleDlgMeasures)
	ON_BN_CLICKED(IDC_RADIO_TITLEMARGINCENTER, OnRadioTitlemargincenter)
	ON_BN_CLICKED(IDC_RADIO_TITLEMARGINLEFT, OnRadioTitlemarginleft)
	ON_BN_CLICKED(IDC_RADIO_TITLEMARGINRIGHT, OnRadioTitlemarginright)
	ON_EN_CHANGE(IDC_EDIT_TITLEHALIGNLEFT, OnChangeEditTitlehalignleft)
	ON_EN_CHANGE(IDC_EDIT_TITLEHALIGNRIGHT, OnChangeEditTitlehalignright)
	ON_EN_CHANGE(IDC_EDIT_TITLEVALIGNHEIGHT, OnChangeEditTitlevalignheight)
	ON_EN_CHANGE(IDC_EDIT_TITLEVALIGNTOP, OnChangeEditTitlevaligntop)
	ON_BN_CLICKED(IDC_CHECK_TITLE_SHOW, OnCheckTitleShow)
	ON_BN_CLICKED(IDC_BUTTON_LABELTRACKTITLECENTER, OnButtonLabeltracktitlecenter)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TITLEROUND, OnRadioLabelTitleround)
	ON_BN_CLICKED(IDC_RADIO_LABEL_TITLESTRAIGHT, OnRadioLabelTitlestraight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgMeasures message handlers

void CStyleDlgMeasures::SetTemplate(CStyleTemplate *pStyle, RenderType rt)
{
	m_pStyle = pStyle;
	m_RenderType = rt;

	if ((rt != Booklet) && (pStyle != NULL))
	{
//		if (pStyle != NULL)
		{
			m_dHAlignLeft = pStyle->GetTitleMargin (rt);
			m_dHAlignRight = m_dHAlignLeft;
			m_nTitleMargin = pStyle->GetTitleAlign (rt);

			m_dVAlignTop = pStyle->GetTitleTop (rt);
			m_dVAlignHeight = pStyle->GetTitleHeight (rt);

			m_bShowTitle = pStyle->GetDisplayTitles (rt) ? TRUE : FALSE;

			m_nLabelTitleMode = pStyle->IsLabelTitleRound () ? 0 : 1;

			UpdateData (false);
		}

		CString strCaption;
		strCaption.LoadString (rt == Label ? IDS_VERTMARGIN : IDS_HORZMARGIN);
		m_grpHorz.SetWindowText (strCaption);
		strCaption.LoadString (rt == Label ? IDS_MEASURES : IDS_VERTMARGIN);
		m_grpVert.SetWindowText (strCaption);
		strCaption.LoadString (rt == Label ? IDS_TOP : IDS_LEFT);
		GetDlgItem (IDC_RADIO_TITLEMARGINLEFT)->SetWindowText (strCaption);
		strCaption.LoadString (rt == Label ? IDS_BOTTOM : IDS_RIGHT);
		GetDlgItem (IDC_RADIO_TITLEMARGINRIGHT)->SetWindowText (strCaption);

		GetDlgItem (IDC_BUTTON_LABELTRACKTITLECENTER)->ShowWindow (rt == Label ? SW_SHOW : SW_HIDE);
		GetDlgItem (IDC_RADIO_LABEL_TITLEROUND)->ShowWindow (rt == Label ? SW_SHOW : SW_HIDE);
		GetDlgItem (IDC_RADIO_LABEL_TITLESTRAIGHT)->ShowWindow (rt == Label ? SW_SHOW : SW_HIDE);
		m_btnLabelTrackTitleCenter.ShowWindow (rt == Label ? SW_SHOW : SW_HIDE);

		GetDlgItem (IDC_EDIT_TITLEVALIGNTOP)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		//GetDlgItem (IDC_EDIT_TITLEVALIGNHEIGHT)->ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		m_stcTop.ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		//m_stcHeight.ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		m_stcCm1.ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
		//m_stcCm2.ShowWindow (rt == Label ? SW_HIDE : SW_SHOW);
	
		m_btnLabelTrackTitleCenter.EnableWindow ();
		GetDlgItem (IDC_CHECK_TITLE_SHOW)->EnableWindow ();
		GetDlgItem (IDC_RADIO_TITLEMARGINLEFT)->EnableWindow ();
		GetDlgItem (IDC_RADIO_TITLEMARGINRIGHT)->EnableWindow ();
		GetDlgItem (IDC_RADIO_TITLEMARGINCENTER)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TITLEHALIGNLEFT)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TITLEHALIGNRIGHT)->EnableWindow ();
		GetDlgItem (IDC_BUTTON_LABELTRACKTITLECENTER)->EnableWindow ();
		GetDlgItem (IDC_RADIO_LABEL_TITLEROUND)->EnableWindow ();
		GetDlgItem (IDC_RADIO_LABEL_TITLESTRAIGHT)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TITLEVALIGNTOP)->EnableWindow ();
		GetDlgItem (IDC_EDIT_TITLEVALIGNHEIGHT)->EnableWindow ();
		m_stcTop.EnableWindow ();
		m_stcHeight.EnableWindow ();
		m_stcCm1.EnableWindow ();
		m_stcCm2.EnableWindow ();

		GetDlgItem (IDC_RADIO_TITLEMARGINLEFT)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_RADIO_TITLEMARGINRIGHT)->EnableWindow (m_bShowTitle);

		GetDlgItem (IDC_RADIO_TITLEMARGINLEFT)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_RADIO_TITLEMARGINRIGHT)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_RADIO_TITLEMARGINCENTER)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_EDIT_TITLEVALIGNTOP)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_EDIT_TITLEVALIGNHEIGHT)->EnableWindow (m_bShowTitle);
/*
		if (m_bShowTitle)
			EnableTitleAlignmentEdit ();
*/
		EnableTitleAlignmentEdit ();
		EnableLabelTitleEdit ();
	}
	else
	{
		m_btnLabelTrackTitleCenter.EnableWindow (false);
		GetDlgItem (IDC_CHECK_TITLE_SHOW)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_TITLEMARGINLEFT)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_TITLEMARGINRIGHT)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_TITLEMARGINCENTER)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TITLEHALIGNLEFT)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TITLEHALIGNRIGHT)->EnableWindow (false);
		GetDlgItem (IDC_BUTTON_LABELTRACKTITLECENTER)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_LABEL_TITLEROUND)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_LABEL_TITLESTRAIGHT)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TITLEVALIGNTOP)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TITLEVALIGNHEIGHT)->EnableWindow (false);
		m_stcTop.EnableWindow (false);
		m_stcHeight.EnableWindow (false);
		m_stcCm1.EnableWindow (false);
		m_stcCm2.EnableWindow (false);
	}
}

void CStyleDlgMeasures::OnRadioTitlemargincenter() 
{
	if (m_nTitleMargin == 2)
		return;	// is already set

	m_nTitleMargin = 2;
	EnableTitleAlignmentEdit ();
	UpdateDoc (IDC_RADIO_TITLEMARGINCENTER);
}

void CStyleDlgMeasures::OnRadioTitlemarginleft() 
{
	if (m_nTitleMargin == 0)
		return;	// is already set

	m_nTitleMargin = 0;
	EnableTitleAlignmentEdit ();
	UpdateDoc (IDC_RADIO_TITLEMARGINLEFT);
}

void CStyleDlgMeasures::OnRadioTitlemarginright() 
{
	if (m_nTitleMargin == 1)
		return;	// is already set

	m_nTitleMargin = 1;
	EnableTitleAlignmentEdit ();
	UpdateDoc (IDC_RADIO_TITLEMARGINRIGHT);
}

void CStyleDlgMeasures::EnableTitleAlignmentEdit()
{
	if (m_pStyle == NULL)
		return;

	UpdateData ();

	switch (m_nTitleMargin)
	{
	case 0:
		GetDlgItem (IDC_EDIT_TITLEHALIGNLEFT)->EnableWindow (m_bShowTitle);
		GetDlgItem (IDC_EDIT_TITLEHALIGNRIGHT)->EnableWindow (false);
		m_btnLabelTrackTitleCenter.EnableWindow (false);
		OnChangeEditTitlehalignleft ();
		break;
	case 1:
		GetDlgItem (IDC_EDIT_TITLEHALIGNLEFT)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TITLEHALIGNRIGHT)->EnableWindow (m_bShowTitle);
		m_btnLabelTrackTitleCenter.EnableWindow (false);
		OnChangeEditTitlehalignright ();
		break;
	case 2:
		GetDlgItem (IDC_EDIT_TITLEHALIGNLEFT)->EnableWindow (false);
		GetDlgItem (IDC_EDIT_TITLEHALIGNRIGHT)->EnableWindow (false);
		m_btnLabelTrackTitleCenter.EnableWindow ();

		m_pStyle->m_nTitleAlign[m_RenderType] = 2;
	}
}

void CStyleDlgMeasures::EnableLabelTitleEdit()
{
	int nShow = (m_RenderType != Label) ? SW_SHOW : SW_HIDE;
	m_stcHeight.ShowWindow (nShow);
	GetDlgItem (IDC_EDIT_TITLEVALIGNHEIGHT)->ShowWindow (nShow);
	m_stcCm2.ShowWindow (nShow);
}

void CStyleDlgMeasures::OnChangeEditTitlehalignleft() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_TITLEHALIGNLEFT, s);
	if (s.IsEmpty ())
		return;

	bool bChanged = false;
	double f = atof (s);
	if (f != m_pStyle->m_dTitleMargin[m_RenderType])
	{
		bChanged = true;
		m_pStyle->m_dTitleMargin[m_RenderType] = f;
	}
	if (m_nTitleMargin != m_pStyle->m_nTitleAlign[m_RenderType])
	{
		bChanged = true;
		m_pStyle->m_nTitleAlign[m_RenderType] = m_nTitleMargin;
	}

	if (bChanged)
		UpdateDoc (IDC_EDIT_TITLEHALIGNLEFT);
}

void CStyleDlgMeasures::OnChangeEditTitlehalignright() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_TITLEHALIGNRIGHT, s);
	if (s.IsEmpty ())
		return;

	bool bChanged = false;
	double f = atof (s);
	if (f != m_pStyle->m_dTitleMargin[m_RenderType])
	{
		bChanged = true;
		m_pStyle->m_dTitleMargin[m_RenderType] = f;
	}
	if (m_nTitleMargin != m_pStyle->m_nTitleAlign[m_RenderType])
	{
		bChanged = true;
		m_pStyle->m_nTitleAlign[m_RenderType] = m_nTitleMargin;
	}

	// update (but only if this edit control has the focus
	if (bChanged)
		UpdateDoc (IDC_EDIT_TITLEHALIGNRIGHT);
}

void CStyleDlgMeasures::OnChangeEditTitlevalignheight() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_TITLEVALIGNHEIGHT, s);
	if (s.IsEmpty ())
		return;

	bool bChanged = false;
	double f = atof (s);
	if (f != m_pStyle->m_dTitleHeight[m_RenderType])
	{
		bChanged = true;
		m_pStyle->m_dTitleHeight[m_RenderType] = f;
	}

	// update (but only if this edit control has the focus
	if (bChanged)
		UpdateDoc (IDC_EDIT_TITLEVALIGNHEIGHT);
}

void CStyleDlgMeasures::OnChangeEditTitlevaligntop() 
{
	if (m_pStyle == NULL)
		return;

	CString s;
	GetDlgItemText (IDC_EDIT_TITLEVALIGNTOP, s);
	if (s.IsEmpty ())
		return;

	bool bChanged = false;
	double f = atof (s);
	if (f != m_pStyle->m_dTitleTop[m_RenderType])
	{
		bChanged = true;
		m_pStyle->m_dTitleTop[m_RenderType] = f;
	}

	// update (but only if this edit control has the focus
	if (bChanged)
		UpdateDoc (IDC_EDIT_TITLEVALIGNTOP);
}

void CStyleDlgMeasures::OnCheckTitleShow() 
{
	UpdateData ();

	GetDlgItem (IDC_RADIO_TITLEMARGINLEFT)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_RADIO_TITLEMARGINRIGHT)->EnableWindow (m_bShowTitle);

	GetDlgItem (IDC_RADIO_TITLEMARGINLEFT)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_RADIO_TITLEMARGINRIGHT)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_RADIO_TITLEMARGINCENTER)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_EDIT_TITLEVALIGNTOP)->EnableWindow (m_bShowTitle);
	GetDlgItem (IDC_EDIT_TITLEVALIGNHEIGHT)->EnableWindow (m_bShowTitle);

//	if (m_bShowTitle)
	EnableTitleAlignmentEdit ();

	if (m_pStyle != NULL)
		m_pStyle->m_bDisplayTitles[m_RenderType] = m_bShowTitle ? true : false;

	UpdateDoc ();
}

void CStyleDlgMeasures::OnButtonLabeltracktitlecenter() 
{
	if (m_pStyle == NULL)
		return;

	m_pStyle->m_nLabelTitlePosition = ~m_pStyle->m_nLabelTitlePosition & 1;

	CString s;
	s.LoadString (m_pStyle->m_nLabelTitlePosition == 0 ? IDS_LABELTITLEPOS_ABOVE : IDS_LABELTITLEPOS_BELOW);
	m_btnLabelTrackTitleCenter.SetWindowText (s);

	UpdateDoc ();
}

void CStyleDlgMeasures::OnRadioLabelTitleround() 
{
	if (m_pStyle == NULL)
		return;

	if (!m_pStyle->IsLabelTitleRound ())
	{
		UpdateData ();
		EnableLabelTitleEdit ();

		m_pStyle->m_bLabelTitleRound = true;
		UpdateDoc ();
	}
}

void CStyleDlgMeasures::OnRadioLabelTitlestraight() 
{
	if (m_pStyle == NULL)
		return;

	if (m_pStyle->IsLabelTitleRound ())
	{
		UpdateData ();
		EnableLabelTitleEdit ();

		m_pStyle->m_bLabelTitleRound = false;
		UpdateDoc ();
	}
}
