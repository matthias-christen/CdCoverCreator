// StyleDlgFonts.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "StyleDlgFonts.h"
#include "CdCoverCreator2Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgFonts dialog


CStyleDlgFonts::CStyleDlgFonts(CWnd* pParent /*=NULL*/)
	: CStyleDlgBase(CStyleDlgFonts::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStyleDlgFonts)
	//}}AFX_DATA_INIT

	m_hIconFont = AfxGetApp ()->LoadIcon (IDI_ICON_FONT);

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof (NONCLIENTMETRICS);
	SystemParametersInfo (SPI_GETNONCLIENTMETRICS, sizeof (NONCLIENTMETRICS), &ncm, 0);
	memcpy (&m_lfDisabled, &ncm.lfMessageFont, sizeof (LOGFONT));
}

CStyleDlgFonts::~CStyleDlgFonts ()
{
	::DestroyIcon (m_hIconFont);
}

void CStyleDlgFonts::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyleDlgFonts)
	DDX_Control(pDX, IDC_BUTTON_TRACKTITLE_FONT, m_btnFntTrkTitle);
	DDX_Control(pDX, IDC_BUTTON_TRACKTITLE_COLOR, m_btnColTrkTitle);
	DDX_Control(pDX, IDC_BUTTON_TRACKREMARKS_FONT, m_btnFntTrkRemarks);
	DDX_Control(pDX, IDC_BUTTON_TRACKREMARKS_COLOR, m_btnColTrkRemarks);
	DDX_Control(pDX, IDC_BUTTON_TRACKNUM_FONT, m_btnFntTrkNum);
	DDX_Control(pDX, IDC_BUTTON_TRACKNUM_COLOR, m_btnColTrkNum);
	DDX_Control(pDX, IDC_BUTTON_TRACKLENGTH_FONT, m_btnFntTrkLength);
	DDX_Control(pDX, IDC_BUTTON_TRACKLENGTH_COLOR, m_btnColTrkLength);
	DDX_Control(pDX, IDC_BUTTON_TRACKGROUPINGTITLE_FONT, m_btnFntTrkGrpTitle);
	DDX_Control(pDX, IDC_BUTTON_TRACKGROUPINGTITLE_COLOR, m_btnColTrkGrpTitle);
	DDX_Control(pDX, IDC_BUTTON_TRACKGROUPINGREMARKS_FONT, m_btnFntTrkGrpRemarks);
	DDX_Control(pDX, IDC_BUTTON_TRACKGROUPINGREMARKS_COLOR, m_btnColTrkGrpRemarks);
	DDX_Control(pDX, IDC_BUTTON_FONT, m_btnFont);
	DDX_Control(pDX, IDC_BUTTON_COLOR, m_btnColor);
	DDX_Control(pDX, IDC_STATIC_FONT, m_stcFont);
	DDX_Control(pDX, IDC_STATIC_TRACKTITLE_FONT, m_stcTrkTitle);
	DDX_Control(pDX, IDC_STATIC_TRACKREMARKS_FONT, m_stcTrkRemarks);
	DDX_Control(pDX, IDC_STATIC_TRACKNUM_FONT, m_stcTrkNum);
	DDX_Control(pDX, IDC_STATIC_TRACKLENGTH_FONT, m_stcTrkLength);
	DDX_Control(pDX, IDC_STATIC_TRACKGROUPINGTITLE_FONT, m_stcTrkGrpTitle);
	DDX_Control(pDX, IDC_STATIC_TRACKGROUPINGREMARKS_FONT, m_stcTrkgpRemarks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStyleDlgFonts, CDialog)
	//{{AFX_MSG_MAP(CStyleDlgFonts)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKNUM_FONT, OnButtonTracknumFont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKTITLE_FONT, OnButtonTracktitleFont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKREMARKS_FONT, OnButtonTrackremarksFont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKLENGTH_FONT, OnButtonTracklengthFont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKGROUPINGTITLE_FONT, OnButtonTrackgroupingtitleFont)
	ON_BN_CLICKED(IDC_BUTTON_TRACKGROUPINGREMARKS_FONT, OnButtonTrackgroupingremarksFont)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnButtonColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgFonts message handlers

BOOL CStyleDlgFonts::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_btnFont.SetIcon (m_hIconFont);
	m_btnFntTrkTitle.SetIcon (m_hIconFont);
	m_btnFntTrkRemarks.SetIcon (m_hIconFont);
	m_btnFntTrkNum.SetIcon (m_hIconFont);
	m_btnFntTrkLength.SetIcon (m_hIconFont);
	m_btnFntTrkGrpTitle.SetIcon (m_hIconFont);
	m_btnFntTrkGrpRemarks.SetIcon (m_hIconFont);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStyleDlgFonts::OnButtonFont() 
{
	CFontDialog dlg (m_stcFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_stcFont.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_stcFont.SetPreviewFont (&dlg.m_cf);

		memcpy (&m_pStyle->m_lfTitle[m_RenderType], dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			m_pStyle->m_crTitle[m_RenderType] = dlg.m_cf.rgbColors;

		CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
		if (pChild != NULL)
		{
			CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
			if (pDoc != NULL)
			{
//				pDoc->m_pTracks->SetRTFTitle ("", m_RenderType);
				pDoc->ApplyStyleToRTFTitles (m_pStyle, m_RenderType);
				pDoc->UpdateAllViews (NULL);
				pDoc->CheckPoint ();
			}
		}
	}
}

void CStyleDlgFonts::OnButtonTracknumFont() 
{
	CFontDialog dlg (m_stcTrkNum.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_stcTrkNum.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_stcTrkNum.SetPreviewFont (&dlg.m_cf);

		memcpy (&m_pStyle->m_lfTrackNum, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			m_pStyle->m_crTrackNum = dlg.m_cf.rgbColors;

		UpdateDoc ();
	}
}

void CStyleDlgFonts::OnButtonTracktitleFont() 
{
	CFontDialog dlg (m_stcTrkTitle.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_stcTrkTitle.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_stcTrkTitle.SetPreviewFont (&dlg.m_cf);

		memcpy (&m_pStyle->m_lfTrackTitle, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			m_pStyle->m_crTrackTitle = dlg.m_cf.rgbColors;

		UpdateDoc ();
	}
}

void CStyleDlgFonts::OnButtonTrackremarksFont() 
{
	CFontDialog dlg (m_stcTrkRemarks.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_stcTrkRemarks.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_stcTrkRemarks.SetPreviewFont (&dlg.m_cf);

		memcpy (&m_pStyle->m_lfTrackRemarks, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			m_pStyle->m_crTrackRemarks = dlg.m_cf.rgbColors;

		UpdateDoc ();
	}
}

void CStyleDlgFonts::OnButtonTracklengthFont() 
{
	CFontDialog dlg (m_stcTrkLength.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_stcTrkLength.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_stcTrkLength.SetPreviewFont (&dlg.m_cf);

		memcpy (&m_pStyle->m_lfTrackLength, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			m_pStyle->m_crTrackLength = dlg.m_cf.rgbColors;

		UpdateDoc ();
	}
}

void CStyleDlgFonts::OnButtonTrackgroupingtitleFont() 
{
	CFontDialog dlg (m_stcTrkGrpTitle.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_stcTrkGrpTitle.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_stcTrkGrpTitle.SetPreviewFont (&dlg.m_cf);

		memcpy (&m_pStyle->m_lfTrackGroupingTitle, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			m_pStyle->m_crTrackGroupingTitle = dlg.m_cf.rgbColors;

		UpdateDoc ();
	}
}

void CStyleDlgFonts::OnButtonTrackgroupingremarksFont() 
{
	CFontDialog dlg (m_stcTrkgpRemarks.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_stcTrkgpRemarks.GetColor ();

	if (dlg.DoModal () == IDOK)
	{
		m_stcTrkgpRemarks.SetPreviewFont (&dlg.m_cf);

		memcpy (&m_pStyle->m_lfTrackGroupingRemarks, dlg.m_cf.lpLogFont, sizeof (LOGFONT));
		if (dlg.m_cf.rgbColors)
			m_pStyle->m_crTrackGroupingRemarks = dlg.m_cf.rgbColors;

		UpdateDoc ();
	}
}

LRESULT CStyleDlgFonts::OnButtonColor (WPARAM wParam, LPARAM lParam)
{
	bool bResetRTF = false;

	switch (lParam)
	{
	case IDC_BUTTON_COLOR:
		m_pStyle->m_crTitle[m_RenderType] = m_btnColor.GetColour ();
		bResetRTF = true;
		break;
	case IDC_BUTTON_TRACKTITLE_COLOR:
		m_pStyle->m_crTrackTitle = m_btnColTrkTitle.GetColour ();
		break;
	case IDC_BUTTON_TRACKREMARKS_COLOR:
		m_pStyle->m_crTrackRemarks = m_btnColTrkRemarks.GetColour ();
		break;
	case IDC_BUTTON_TRACKNUM_COLOR:
		m_pStyle->m_crTrackNum = m_btnColTrkNum.GetColour ();
		break;
	case IDC_BUTTON_TRACKLENGTH_COLOR:
		m_pStyle->m_crTrackLength = m_btnColTrkLength.GetColour ();
		break;
	case IDC_BUTTON_TRACKGROUPINGTITLE_COLOR:
		m_pStyle->m_crTrackGroupingTitle = m_btnColTrkGrpTitle.GetColour ();
		break;
	case IDC_BUTTON_TRACKGROUPINGREMARKS_COLOR:
		m_pStyle->m_crTrackGroupingRemarks = m_btnColTrkGrpRemarks.GetColour ();
		break;
	}

	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
	if (pChild != NULL)
	{
		CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
		if (pDoc != NULL)
		{
			if (bResetRTF)
			{
//				pDoc->m_pTracks->SetRTFTitle ("", m_RenderType);
				pDoc->ApplyStyleToRTFTitles (m_pStyle, m_RenderType);
			}
			pDoc->UpdateAllViews (NULL);
			pDoc->CheckPoint ();
		}
	}

	return S_OK;
}

void CStyleDlgFonts::SetTemplate(CStyleTemplate *pStyle, RenderType rt)
{
	m_pStyle = pStyle;
	m_RenderType = rt;

	if (pStyle != NULL)
	{
		m_stcFont.SetPreviewFont (&pStyle->m_lfTitle[rt]);
		m_stcTrkTitle.SetPreviewFont (&pStyle->m_lfTrackTitle);
		m_stcTrkRemarks.SetPreviewFont (&pStyle->m_lfTrackRemarks);
		m_stcTrkNum.SetPreviewFont (&pStyle->m_lfTrackNum);
		m_stcTrkLength.SetPreviewFont (&pStyle->m_lfTrackLength);
		m_stcTrkGrpTitle.SetPreviewFont (&pStyle->m_lfTrackGroupingTitle);
		m_stcTrkgpRemarks.SetPreviewFont (&pStyle->m_lfTrackGroupingRemarks);

		m_stcFont.SetColor (pStyle->m_crTitle[rt]);
		m_stcTrkTitle.SetColor (pStyle->m_crTrackTitle);
		m_stcTrkRemarks.SetColor (pStyle->m_crTrackRemarks);
		m_stcTrkNum.SetColor (pStyle->m_crTrackNum);
		m_stcTrkLength.SetColor (pStyle->m_crTrackLength);
		m_stcTrkGrpTitle.SetColor (pStyle->m_crTrackGroupingTitle);
		m_stcTrkgpRemarks.SetColor (pStyle->m_crTrackGroupingRemarks);

		m_btnColor.EnableWindow ();
		m_btnFont.EnableWindow ();
		m_btnColTrkTitle.EnableWindow ();
		m_btnFntTrkTitle.EnableWindow ();
		m_btnColTrkRemarks.EnableWindow ();
		m_btnFntTrkRemarks.EnableWindow ();
		m_btnColTrkNum.EnableWindow ();
		m_btnFntTrkNum.EnableWindow ();
		m_btnColTrkLength.EnableWindow ();
		m_btnFntTrkLength.EnableWindow ();
		m_btnColTrkGrpTitle.EnableWindow ();
		m_btnFntTrkGrpTitle.EnableWindow ();
		m_btnColTrkGrpRemarks.EnableWindow ();
		m_btnFntTrkGrpRemarks.EnableWindow ();

		m_btnColor.SetColour (pStyle->m_crTitle[rt]);
		m_btnColTrkTitle.SetColour (pStyle->m_crTrackTitle);
		m_btnColTrkRemarks.SetColour (pStyle->m_crTrackRemarks);
		m_btnColTrkNum.SetColour (pStyle->m_crTrackNum);
		m_btnColTrkLength.SetColour (pStyle->m_crTrackLength);
		m_btnColTrkGrpTitle.SetColour (pStyle->m_crTrackGroupingTitle);
		m_btnColTrkGrpRemarks.SetColour (pStyle->m_crTrackGroupingRemarks);
	}
	else
	{
		m_stcFont.SetPreviewFont (&m_lfDisabled);
		m_stcTrkTitle.SetPreviewFont (&m_lfDisabled);
		m_stcTrkRemarks.SetPreviewFont (&m_lfDisabled);
		m_stcTrkNum.SetPreviewFont (&m_lfDisabled);
		m_stcTrkLength.SetPreviewFont (&m_lfDisabled);
		m_stcTrkGrpTitle.SetPreviewFont (&m_lfDisabled);
		m_stcTrkgpRemarks.SetPreviewFont (&m_lfDisabled);

		COLORREF col = ::GetSysColor (COLOR_3DDKSHADOW);
		m_stcFont.SetColor (col);
		m_stcTrkTitle.SetColor (col);
		m_stcTrkRemarks.SetColor (col);
		m_stcTrkNum.SetColor (col);
		m_stcTrkLength.SetColor (col);
		m_stcTrkGrpTitle.SetColor (col);
		m_stcTrkgpRemarks.SetColor (col);

		m_btnColor.EnableWindow (false);
		m_btnFont.EnableWindow (false);
		m_btnColTrkTitle.EnableWindow (false);
		m_btnFntTrkTitle.EnableWindow (false);
		m_btnColTrkRemarks.EnableWindow (false);
		m_btnFntTrkRemarks.EnableWindow (false);
		m_btnColTrkNum.EnableWindow (false);
		m_btnFntTrkNum.EnableWindow (false);
		m_btnColTrkLength.EnableWindow (false);
		m_btnFntTrkLength.EnableWindow (false);
		m_btnColTrkGrpTitle.EnableWindow (false);
		m_btnFntTrkGrpTitle.EnableWindow (false);
		m_btnColTrkGrpRemarks.EnableWindow (false);
		m_btnFntTrkGrpRemarks.EnableWindow (false);
	}
}
