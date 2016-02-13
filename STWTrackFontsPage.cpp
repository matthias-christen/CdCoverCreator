// STWTrackFontsPage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "STWTrackFontsPage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplates.h"

#include "Formatter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSTWTrackFontsPage, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CSTWTrackFontsPage property page

CSTWTrackFontsPage::CSTWTrackFontsPage() : CPropertyPage(CSTWTrackFontsPage::IDD)
{
	//{{AFX_DATA_INIT(CSTWTrackFontsPage)
	//}}AFX_DATA_INIT

	m_hIconFont = AfxGetApp ()->LoadIcon (IDI_ICON_FONT);
	m_hIconColor = AfxGetApp ()->LoadIcon (IDI_ICON_COLORS);
}

CSTWTrackFontsPage::~CSTWTrackFontsPage()
{
	::DestroyIcon (m_hIconFont);
	::DestroyIcon (m_hIconColor);
}

void CSTWTrackFontsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWTrackFontsPage)
	DDX_Control(pDX, IDC_BUTTON_TRACKTITLE_FONT, m_btnTrackTitleFont);
	DDX_Control(pDX, IDC_BUTTON_TRACKTITLE_COLOR, m_btnTrackTitleColor);
	DDX_Control(pDX, IDC_BUTTON_TRACKREMARKS_FONT, m_btnTrackRemFont);
	DDX_Control(pDX, IDC_BUTTON_TRACKREMARKS_COLOR, m_btnTrackRemColor);
	DDX_Control(pDX, IDC_BUTTON_TRACKNUM_FONT, m_btnTrackNumFont);
	DDX_Control(pDX, IDC_BUTTON_TRACKNUM_COLOR, m_btnTrackNumColor);
	DDX_Control(pDX, IDC_BUTTON_TRACKLENGTH_FONT, m_btnTrackLenFont);
	DDX_Control(pDX, IDC_BUTTON_TRACKLENGTH_COLOR, m_btnTrackLenColor);
	DDX_Control(pDX, IDC_BUTTON_TRACKGROUPINGTITLE_FONT, m_btnTrackGrpTitleFont);
	DDX_Control(pDX, IDC_BUTTON_TRACKGROUPINGTITLE_COLOR, m_btnTrackGrpTitleColor);
	DDX_Control(pDX, IDC_BUTTON_TRACKGROUPINGREMARKS_FONT, m_btnTrackGrpRemFont);
	DDX_Control(pDX, IDC_BUTTON_TRACKGROUPINGREMARKS_COLOR, m_btnTrackGrpRemColor);
	DDX_Control(pDX, IDC_STATIC_TRACKTITLE_FONT, m_TrackTitleFont);
	DDX_Control(pDX, IDC_STATIC_TRACKREMARKS_FONT, m_TrackRemarksFont);
	DDX_Control(pDX, IDC_STATIC_TRACKNUM_FONT, m_TrackNumFont);
	DDX_Control(pDX, IDC_STATIC_TRACKLENGTH_FONT, m_TrackLengthFont);
	DDX_Control(pDX, IDC_STATIC_TRACKGROUPINGTITLE_FONT, m_TrackGroupingTitleFont);
	DDX_Control(pDX, IDC_STATIC_TRACKGROUPINGREMARKS_FONT, m_TrackGroupingRemarksFont);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTWTrackFontsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWTrackFontsPage)
	ON_BN_CLICKED(IDC_BUTTON_TRACKNUM_FONT, OnButtonTrackNumfont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKLENGTH_FONT, OnButtonTrackLengthfont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKTITLE_FONT, OnButtonTrackTitlefont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKREMARKS_FONT, OnButtonTrackRemarksfont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKGROUPINGTITLE_FONT, OnButtonTrackGroupingTitlefont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKGROUPINGREMARKS_FONT, OnButtonTrackGroupingRemarksfont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKNUM_COLOR, OnButtonTrackNumcolor)
	ON_BN_CLICKED(IDC_BUTTON_TRACKLENGTH_COLOR, OnButtonTrackLengthcolor)
	ON_BN_CLICKED(IDC_BUTTON_TRACKTITLE_COLOR, OnButtonTrackTitlecolor)
	ON_BN_CLICKED(IDC_BUTTON_TRACKREMARKS_COLOR, OnButtonTrackRemarkscolor)
	ON_BN_CLICKED(IDC_BUTTON_TRACKGROUPINGTITLE_COLOR, OnButtonTrackGroupingTitlecolor)
	ON_BN_CLICKED(IDC_BUTTON_TRACKGROUPINGREMARKS_COLOR, OnButtonTrackGroupingRemarkscolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSTWTrackFontsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_btnTrackTitleFont.SetIcon (m_hIconFont);
	m_btnTrackTitleColor.SetIcon (m_hIconColor);
	m_btnTrackRemFont.SetIcon (m_hIconFont);
	m_btnTrackRemColor.SetIcon (m_hIconColor);
	m_btnTrackNumFont.SetIcon (m_hIconFont);
	m_btnTrackNumColor.SetIcon (m_hIconColor);
	m_btnTrackLenFont.SetIcon (m_hIconFont);
	m_btnTrackLenColor.SetIcon (m_hIconColor);
	m_btnTrackGrpTitleFont.SetIcon (m_hIconFont);
	m_btnTrackGrpTitleColor.SetIcon (m_hIconColor);
	m_btnTrackGrpRemFont.SetIcon (m_hIconFont);
	m_btnTrackGrpRemColor.SetIcon (m_hIconColor);
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool (&m_btnTrackTitleFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnTrackTitleColor, IDS_TT_COLOR);
		m_tooltip.AddTool (&m_btnTrackRemFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnTrackRemColor, IDS_TT_COLOR);
		m_tooltip.AddTool (&m_btnTrackNumFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnTrackNumColor, IDS_TT_COLOR);
		m_tooltip.AddTool (&m_btnTrackLenFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnTrackLenColor, IDS_TT_COLOR);
		m_tooltip.AddTool (&m_btnTrackGrpTitleFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnTrackGrpTitleColor, IDS_TT_COLOR);
		m_tooltip.AddTool (&m_btnTrackGrpRemFont, IDS_TT_FONT);
		m_tooltip.AddTool (&m_btnTrackGrpRemColor, IDS_TT_COLOR);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSTWTrackFontsPage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
	
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	if (pTmp != NULL)
	{
		m_TrackNumFont.SetPreviewFont (pTmp->GetTrackNumFont ());
		m_TrackNumFont.SetColor (pTmp->GetTrackNumColor ());
		m_TrackLengthFont.SetPreviewFont (pTmp->GetTrackLengthFont ());
		m_TrackLengthFont.SetColor (pTmp->GetTrackLengthColor ());
		m_TrackTitleFont.SetPreviewFont (pTmp->GetTrackTitleFont ());
		m_TrackTitleFont.SetColor (pTmp->GetTrackTitleColor ());
		m_TrackRemarksFont.SetPreviewFont (pTmp->GetTrackRemarksFont ());
		m_TrackRemarksFont.SetColor (pTmp->GetTrackRemarksColor ());
		m_TrackGroupingTitleFont.SetPreviewFont (pTmp->GetTrackGroupingTitleFont ());
		m_TrackGroupingTitleFont.SetColor (pTmp->GetTrackGroupingTitleColor ());
		m_TrackGroupingRemarksFont.SetPreviewFont (pTmp->GetTrackGroupingRemarksFont ());
		m_TrackGroupingRemarksFont.SetColor (pTmp->GetTrackGroupingRemarksColor ());
	}

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);

	return CPropertyPage::OnSetActive();
}

BOOL CSTWTrackFontsPage::OnKillActive() 
{
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	return CPropertyPage::OnKillActive();
}

void CSTWTrackFontsPage::OnButtonTrackNumfont() 
{
	CFontDialog dlg (m_TrackNumFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_TrackNumFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_TrackNumFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTrackNum, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTrackNum = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackLengthfont() 
{
	CFontDialog dlg (m_TrackLengthFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_TrackLengthFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_TrackLengthFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTrackLength, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTrackLength = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackTitlefont() 
{
	CFontDialog dlg (m_TrackTitleFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_TrackTitleFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_TrackTitleFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTrackTitle, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTrackTitle = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackRemarksfont() 
{
	CFontDialog dlg (m_TrackRemarksFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_TrackRemarksFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_TrackRemarksFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTrackRemarks, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTrackRemarks = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackGroupingTitlefont() 
{
	CFontDialog dlg (m_TrackGroupingTitleFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_TrackGroupingTitleFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_TrackGroupingTitleFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTrackGroupingTitle, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTrackGroupingTitle = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackGroupingRemarksfont() 
{
	CFontDialog dlg (m_TrackGroupingRemarksFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_TrackGroupingRemarksFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_TrackGroupingRemarksFont.SetPreviewFont (&dlg.m_cf);

		CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
		memcpy (&pTmp->m_lfTrackGroupingRemarks, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			pTmp->m_crTrackGroupingRemarks = dlg.m_cf.rgbColors;
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackNumcolor() 
{
	CColorDialog dlg (m_TrackNumFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_TrackNumFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTrackNum = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackLengthcolor() 
{
	CColorDialog dlg (m_TrackLengthFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_TrackLengthFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTrackLength = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackTitlecolor() 
{
	CColorDialog dlg (m_TrackTitleFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_TrackTitleFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTrackTitle = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackRemarkscolor() 
{
	CColorDialog dlg (m_TrackRemarksFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_TrackRemarksFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTrackRemarks = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackGroupingTitlecolor() 
{
	CColorDialog dlg (m_TrackGroupingTitleFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_TrackGroupingTitleFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTrackGroupingTitle = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWTrackFontsPage::OnButtonTrackGroupingRemarkscolor() 
{
	CColorDialog dlg (m_TrackGroupingRemarksFont.GetColor (), CC_FULLOPEN, this);
	if (dlg.DoModal () == IDOK)
	{
		m_TrackGroupingRemarksFont.SetColor (dlg.GetColor ());

		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crTrackGroupingRemarks = dlg.GetColor ();
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	}
}

BOOL CSTWTrackFontsPage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
