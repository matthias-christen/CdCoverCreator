// StyleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"
#include "StyleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgGradient dialog


CStyleDlgGradient::CStyleDlgGradient(CWnd* pParent /*=NULL*/)
	: CStyleDlgBase(CStyleDlgGradient::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStyleDlgGradient)
	//}}AFX_DATA_INIT

	m_bDegChanged = false;
	m_pStyle = NULL;
}


void CStyleDlgGradient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyleDlgGradient)
	DDX_Control(pDX, IDC_EDIT_DEG, m_editAngle);
	DDX_Control(pDX, IDC_STATIC_BKCOLOR, m_BkColor);
	DDX_Control(pDX, IDC_SPIN_DEG, m_spinAngle);
	DDX_Control(pDX, IDC_DEGM, m_dgAngle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStyleDlgGradient, CDialog)
	ON_LGPN_UPDATE(IDC_STATIC_BKCOLOR, OnGradient)
	ON_DSN_UPDATE(IDC_DEGM, OnDegm)
	ON_DSN_BUTTONUP(IDC_DEGM, OnDegmBtnUp)
	ON_EN_CHANGE(IDC_EDIT_DEG, OnChangeEditDeg)
	ON_MESSAGE(CPN_SELENDOK, OnButtonColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgGradient message handlers

void CStyleDlgGradient::OnGradient ()
{
	if (m_pStyle == NULL)
		return;

	m_BkColor.GetData (&m_pStyle->m_lgdBackground[m_RenderType]);

	m_editAngle.EnableWindow (m_pStyle->m_lgdBackground[m_RenderType].type == Linear);
	m_dgAngle.EnableWindow (m_pStyle->m_lgdBackground[m_RenderType].type == Linear);
	
	UpdateDoc ();
}

void CStyleDlgGradient::OnChangeEditDeg() 
{
	if (m_bDegChanged)
	{
		m_bDegChanged = false;
		return;
	}

	if (!::IsWindow (m_editAngle.GetSafeHwnd ()))
		return;

	CString s;
	m_editAngle.GetWindowText (s);
	m_dgAngle.SetDegAngle (atoi (s) * 10);

	m_BkColor.SetAngle (m_dgAngle.GetDegAngle ());

	double dAngle = deg2rad (m_dgAngle.GetDegAngle ());
	if (m_pStyle->m_lgdBackground[m_RenderType].dAngle != dAngle)
	{
		m_pStyle->m_lgdBackground[m_RenderType].dAngle = dAngle;
		UpdateDoc ();
	}
}

void CStyleDlgGradient::OnDegm() 
{
	m_bDegChanged = true;

	CString s;
	s.Format ("%d", m_dgAngle.GetDegAngle () / 10);
	m_editAngle.SetWindowText (s);
	m_spinAngle.SetPos (m_dgAngle.GetDegAngle () / 10);

	m_BkColor.SetAngle (m_dgAngle.GetDegAngle ());
}

void CStyleDlgGradient::OnDegmBtnUp ()
{
	if (m_pStyle == NULL)
		return;

	m_pStyle->m_lgdBackground[m_RenderType].dAngle = deg2rad (m_dgAngle.GetDegAngle ());	
	UpdateDoc ();
}

LRESULT CStyleDlgGradient::OnButtonColor(WPARAM wParam, LPARAM lParam)
{
	/*
	switch (lParam)
	{
	case IDC_BUTTON_BKCOLOR:
		m_BkColor.SetStartColor (m_btnColStart.GetColour ());
		m_BkColor.InvalidateRect (NULL);
		m_pStyle->m_crBkStart[m_RenderType] = m_btnColStart.GetColour ();
		break;
	case IDC_BUTTON_BKCOLOR2:
		m_BkColor.SetEndColor (m_btnColEnd.GetColour ());
		m_BkColor.InvalidateRect (NULL);
		m_pStyle->m_crBkEnd[m_RenderType] = m_btnColEnd.GetColour ();
		break;
	}*/

	UpdateDoc ();
	return S_OK;
}

void CStyleDlgGradient::SetTemplate(CStyleTemplate *pStyle, RenderType rt)
{
	m_pStyle = pStyle;
	m_RenderType = rt;

	if (pStyle != NULL && rt != Booklet)
	{
		m_BkColor.EnableWindow (true);
		m_BkColor.Reset ();
//		m_BkColor.SetStartColor (pStyle->m_crBkStart[rt]);
//		m_BkColor.SetEndColor (pStyle->m_crBkEnd[rt]);
//		m_BkColor.SetAngle (pStyle->m_nBkAngle[rt]);
		m_BkColor.SetData (pStyle->m_lgdBackground[rt]);

		m_editAngle.EnableWindow (true);
		m_dgAngle.EnableWindow (true);

		int nAngle = rad2deg (pStyle->m_lgdBackground[rt].dAngle);
		SetDlgItemInt (IDC_EDIT_DEG, nAngle / 10);
		m_spinAngle.SetPos (nAngle / 10);
		m_dgAngle.SetDegAngle (nAngle);
	}
	else
	{
		m_BkColor.Reset ();
		m_BkColor.SetStartColor (GetSysColor (COLOR_INACTIVECAPTION));
		m_BkColor.SetEndColor (GetSysColor (COLOR_INACTIVECAPTION));
		m_BkColor.SetAngle (0);

		m_BkColor.EnableWindow (false);
		m_editAngle.EnableWindow (false);
		m_dgAngle.EnableWindow (false);
	}

	m_spinAngle.SetRange (0, 360);
}