// DlgBarcodeProperties.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "DlgBarcodeProperties.h"
#include ".\dlgbarcodeproperties.h"


// CDlgBarcodeProperties dialog

IMPLEMENT_DYNAMIC(CDlgBarcodeProperties, CDialog)
CDlgBarcodeProperties::CDlgBarcodeProperties (CFloatingBarCode* pBarCode, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBarcodeProperties::IDD, pParent)
	, m_bTransparentBackground(FALSE)
{
	m_pBarcode = pBarCode;
}

CDlgBarcodeProperties::~CDlgBarcodeProperties()
{
}

void CDlgBarcodeProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FGCOLOR, m_cpForeground);
	DDX_Control(pDX, IDC_BUTTON_BKCOLOR, m_cpBackground);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cmbType);
	DDX_Control(pDX, IDC_COMBO_FONT, m_cmbFont);
	DDX_Check(pDX, IDC_CHECK_TRANSPBK, m_bTransparentBackground);
}


BEGIN_MESSAGE_MAP(CDlgBarcodeProperties, CDialog)
END_MESSAGE_MAP()


// CDlgBarcodeProperties message handlers

void CDlgBarcodeProperties::OnOK()
{
	UpdateData ();

	CString strFont;
	m_cmbFont.GetLBText (m_cmbFont.GetCurSel (), strFont);
	m_pBarcode->SetProperties (m_cmbType.GetCurSel (), m_cpForeground.GetColour (), m_cpBackground.GetColour (),
		m_bTransparentBackground, strFont);
	
	CDialog::OnOK();
}

BOOL CDlgBarcodeProperties::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbFont.FillFonts ();

	int nIdx = m_cmbType.SelectString (-1, m_pBarcode->GetType () == 0 ? "EAN13" : "EAN8");
	m_cpForeground.SetColour (m_pBarcode->GetForegroundColor ());
	m_cpBackground.SetColour (m_pBarcode->GetBackgroundColor ());
	m_bTransparentBackground = m_pBarcode->GetBackgroundMode () == TRANSPARENT ? TRUE : FALSE;
	m_cmbFont.SelectString (-1, m_pBarcode->GetFont ());

	UpdateData (FALSE);

	return TRUE;
}
