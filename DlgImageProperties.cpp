// DlgImageProperties.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "DlgImageProperties.h"


// CDlgImageProperties dialog

IMPLEMENT_DYNAMIC(CDlgImageProperties, CDialog)
CDlgImageProperties::CDlgImageProperties(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgImageProperties::IDD, pParent)
	, m_nContrast(100)
	, m_nBrightness(0)
	, m_nSaturation(100)
	, m_nHue(0)
	, m_nGamma(100)
{
}

void CDlgImageProperties::SetPicture (CPicture* pPicture)
{
	m_pPicture = pPicture;

	m_nContrast = (int) ((m_fOldContrast = pPicture->GetContrast ()) * 100);
	m_nBrightness = (int) ((m_fOldBrightness = pPicture->GetBrightness ()) * 100);
	m_nSaturation = (int) ((m_fOldSaturation = pPicture->GetSaturation ()) * 100);
	m_nHue = (int) (m_fOldHue = pPicture->GetHue ());
	m_nGamma = (int) ((m_fOldGamma = pPicture->GetGamma ()) * 100);
}

CDlgImageProperties::~CDlgImageProperties()
{
}

void CDlgImageProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER_CONTRAST, m_nContrast);
	DDX_Slider(pDX, IDC_SLIDER_BRIGHTNESS, m_nBrightness);
	DDX_Slider(pDX, IDC_SLIDER_SATURATION, m_nSaturation);
	DDX_Slider(pDX, IDC_SLIDER_HUE, m_nHue);
	DDX_Slider(pDX, IDC_SLIDER_GAMMA, m_nGamma);
}


BEGIN_MESSAGE_MAP(CDlgImageProperties, CDialog)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CDlgImageProperties message handlers

void CDlgImageProperties::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

	UpdateData ();
	UpdateDisplay ();
}

void CDlgImageProperties::UpdateDisplay ()
{
	CString s;

	s.Format ("%.2f", (double) m_nContrast / 100.0f);
	SetDlgItemText (IDC_STATIC_CONTRAST, s);

	s.Format ("%.2f", (double) m_nBrightness / 100.0f);
	SetDlgItemText (IDC_STATIC_BRIGHTNESS, s);

	s.Format ("%.2f", (double) m_nSaturation / 100.0f);
	SetDlgItemText (IDC_STATIC_SATURATION, s);

	SetDlgItemInt (IDC_STATIC_HUE, m_nHue);

	s.Format ("%.2f", (double) m_nGamma / 100.0f);
	SetDlgItemText (IDC_STATIC_GAMMA, s);

	// set the parameters on the picture
	m_pPicture->SetGamma ((double) m_nGamma / 100.0f);
	m_pPicture->AdjustColors ((double) m_nContrast / 100.0f, (double) m_nBrightness / 100.0f,
		(double) m_nSaturation / 100.0f, m_nHue);

	// update the views
	if (m_pDocument != NULL)
		m_pDocument->UpdateAllViews (NULL);
}

BOOL CDlgImageProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_CONTRAST))->SetRange (0, 400);
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_BRIGHTNESS))->SetRange (-100, 100);
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_SATURATION))->SetRange (0, 300);
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_HUE))->SetRange (-180, 180);
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_GAMMA))->SetRange (0, 400);

	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_CONTRAST))->SetTicFreq (50);
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_BRIGHTNESS))->SetTicFreq (20);
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_SATURATION))->SetTicFreq (20);
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_HUE))->SetTicFreq (30);
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_GAMMA))->SetTicFreq (50);

	UpdateData (FALSE);
	UpdateDisplay ();

	return TRUE;
}

void CDlgImageProperties::OnBnClickedButtonReset()
{
	m_nContrast = 100;
	m_nBrightness = 0;
	m_nSaturation = 100;
	m_nHue = 0;
	m_nGamma = 100;

	UpdateData (FALSE);
	UpdateDisplay ();
}

void CDlgImageProperties::OnCancel()
{
	// reset the image
	m_pPicture->AdjustColors (m_fOldContrast, m_fOldBrightness, m_fOldSaturation, m_fOldHue);
	m_pPicture->SetGamma (m_fOldGamma);

	if (m_pDocument != NULL)
		m_pDocument->UpdateAllViews (NULL);

	CDialog::OnCancel();
}
