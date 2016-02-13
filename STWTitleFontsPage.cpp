// STWTitleFontsPage.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "STWTitleFontsPage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTWTitleFontsPage dialog


CSTWTitleFontsPage::CSTWTitleFontsPage () : CPropertyPage(CSTWTitleFontsPage::IDD)
{
	//{{AFX_DATA_INIT(CSTWTitleFontsPage)
	//}}AFX_DATA_INIT

	m_hIconFont = AfxGetApp ()->LoadIcon (IDI_ICON_FONT);
	m_hIconColor = AfxGetApp ()->LoadIcon (IDI_ICON_COLORS);
}

CSTWTitleFontsPage::~CSTWTitleFontsPage ()
{
	::DestroyIcon (m_hIconFont);
	::DestroyIcon (m_hIconColor);
}

void CSTWTitleFontsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWTitleFontsPage)
	DDX_Control(pDX, IDC_TITLEFONT_LABEL, m_LabelFont);
	DDX_Control(pDX, IDC_TITLEFONT_INLAYTITLE, m_InlayTitleFont);
	DDX_Control(pDX, IDC_TITLEFONT_INLAYSIDE, m_InlaySideFont);
	DDX_Control(pDX, IDC_TITLEFONT_COVER, m_CoverFont);
	DDX_Control(pDX, IDC_BUTTON_TITLEFONT_LABELFONT, m_btnLabelFont);
	DDX_Control(pDX, IDC_BUTTON_TITLEFONT_LABELCOLOR, m_btnLabelColor);
	DDX_Control(pDX, IDC_BUTTON_TITLEFONT_INLAYTITLEFONT, m_btnInlayTitleFont);
	DDX_Control(pDX, IDC_BUTTON_TITLEFONT_INLAYTITLECOLOR, m_btnInlayTitleColor);
	DDX_Control(pDX, IDC_BUTTON_TITLEFONT_INLAYSIDEFONT, m_btnInlaySideFont);
	DDX_Control(pDX, IDC_BUTTON_TITLEFONT_INLAYSIDECOLOR, m_btnInlaySideColor);
	DDX_Control(pDX, IDC_BUTTON_TITLEFONT_COVERFONT, m_btnCoverFont);
	DDX_Control(pDX, IDC_BUTTON_TITLEFONT_COVERCOLOR, m_btnCoverColor);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTWTitleFontsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWTitleFontsPage)
	ON_BN_CLICKED(IDC_BUTTON_TITLEFONT_COVERFONT, OnButtonTitlefontCoverfont)
	ON_BN_CLICKED(IDC_BUTTON_TITLEFONT_LABELFONT, OnButtonTitlefontLabelfont)
	ON_BN_CLICKED(IDC_BUTTON_TITLEFONT_INLAYSIDEFONT, OnButtonTitlefontInlaysidefont)
	ON_BN_CLICKED(IDC_BUTTON_TITLEFONT_INLAYTITLEFONT, OnButtonTitlefontInlaytitlefont)
	ON_BN_CLICKED(IDC_BUTTON_TITLEFONT_COVERCOLOR, OnButtonTitlefontCovercolor)
	ON_BN_CLICKED(IDC_BUTTON_TITLEFONT_LABELCOLOR, OnButtonTitlefontLabelcolor)
	ON_BN_CLICKED(IDC_BUTTON_TITLEFONT_INLAYSIDECOLOR, OnButtonTitlefontInlaysidecolor)
	ON_BN_CLICKED(IDC_BUTTON_TITLEFONT_INLAYTITLECOLOR, OnButtonTitlefontInlaytitlecolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTWTitleFontsPage message handlers

BOOL CSTWTitleFontsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_btnCoverFont.SetIcon (m_hIconFont);
	m_btnCoverColor.SetIcon (m_hIconColor);
	m_btnLabelFont.SetIcon (m_hIconFont);
	m_btnLabelColor.SetIcon (m_hIconColor);
	m_btnInlayTitleFont.SetIcon (m_hIconFont);
	m_btnInlayTitleColor.SetIcon (m_hIconColor);
	m_btnInlaySideFont.SetIcon (m_hIconFont);
	m_btnInlaySideColor.SetIcon (m_hIconColor);
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool (&m_btnCoverFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnCoverColor, IDS_TT_COLOR);
		m_tooltip.AddTool (&m_btnLabelFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnLabelColor, IDS_TT_COLOR);
		m_tooltip.AddTool (&m_btnInlayTitleFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnInlayTitleColor, IDS_TT_COLOR);
		m_tooltip.AddTool (&m_btnInlaySideFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnInlaySideColor, IDS_TT_COLOR);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSTWTitleFontsPage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
	
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	if (pTmp != NULL)
	{
		m_CoverFont.SetPreviewFont (pTmp->GetTitleFont (Cover));
		m_CoverFont.SetColor (pTmp->GetTitleColor (Cover));
		m_LabelFont.SetPreviewFont (pTmp->GetTitleFont (Label));
		m_LabelFont.SetColor (pTmp->GetTitleColor (Label));
		m_InlayTitleFont.SetPreviewFont (pTmp->GetTitleFont (Inlay));
		m_InlayTitleFont.SetColor (pTmp->GetTitleColor (Inlay));
		m_InlaySideFont.SetPreviewFont (pTmp->GetInlaySideFont ());
		m_InlaySideFont.SetColor (pTmp->GetInlaySideColor ());
	}

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CSTWTitleFontsPage::OnKillActive() 
{
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	return CPropertyPage::OnKillActive();
}

void CSTWTitleFontsPage::OnButtonTitlefontCoverfont() 
{
	CFontDialog dlg (m_CoverFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_CoverFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_CoverFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTitle[Cover], dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		memcpy (&pTmp->m_lfTitle[Back], dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTitle[Cover] = pTmp->m_crTitle[Back] = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTitleFontsPage::OnButtonTitlefontLabelfont() 
{
	CFontDialog dlg (m_LabelFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_LabelFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_LabelFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTitle[Label], dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTitle[Label] = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTitleFontsPage::OnButtonTitlefontInlaysidefont() 
{
	CFontDialog dlg (m_InlaySideFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_InlaySideFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_InlaySideFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfInlaySide, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crInlaySide = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTitleFontsPage::OnButtonTitlefontInlaytitlefont() 
{
	CFontDialog dlg (m_InlayTitleFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_InlayTitleFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_InlayTitleFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTitle[Inlay], dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTitle[Inlay] = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTitleFontsPage::OnButtonTitlefontCovercolor() 
{
	CColorDialog dlg (m_CoverFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_CoverFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTitle[Cover] = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTitle[Back] = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTitleFontsPage::OnButtonTitlefontLabelcolor() 
{
	CColorDialog dlg (m_LabelFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_LabelFont.SetColor (dlg.GetColor ());

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTitle[Label] = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTitleFontsPage::OnButtonTitlefontInlaysidecolor() 
{
	CColorDialog dlg (m_InlaySideFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_InlaySideFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crInlaySide = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTitleFontsPage::OnButtonTitlefontInlaytitlecolor() 
{
	CColorDialog dlg (m_InlayTitleFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_InlayTitleFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTitle[Inlay] = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

BOOL CSTWTitleFontsPage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
