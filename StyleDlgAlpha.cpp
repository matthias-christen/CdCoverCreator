// StyleDlgAlpha.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "MainFrm.h"
#include "CdCoverCreator2Doc.h"
#include "StyleDlgAlpha.h"
#include "ImagePreviewFileDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgAlpha dialog


CStyleDlgAlpha::CStyleDlgAlpha(CWnd* pParent /*=NULL*/)
	: CStyleDlgBase(CStyleDlgAlpha::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStyleDlgAlpha)
	m_strFilename = _T("");
	m_nAlphaFormat = -1;
	m_nSourceConstantAlpha = 0;
	//}}AFX_DATA_INIT
}

CStyleDlgAlpha::~CStyleDlgAlpha ()
{
	::DestroyIcon (m_hIconBrowse);
}

void CStyleDlgAlpha::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyleDlgAlpha)
	DDX_Control(pDX, IDC_BUTTON_BROWSE, m_btnBrowse);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFilename);
	DDX_Radio(pDX, IDC_RADIO_ALPHAFORMAT_DARKEN, m_nAlphaFormat);
	DDX_Slider(pDX, IDC_SLIDER_OPACITY, m_nSourceConstantAlpha);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStyleDlgAlpha, CDialog)
	//{{AFX_MSG_MAP(CStyleDlgAlpha)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_RADIO_ALPHAFORMAT_DARKEN, OnRadioAlphaformatDarken)
	ON_BN_CLICKED(IDC_RADIO_ALPHAFORMAT_LIGHTEN, OnRadioAlphaformatLighten)
	ON_EN_CHANGE(IDC_EDIT_FILE, OnChangeEditFile)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgAlpha message handlers

BOOL CStyleDlgAlpha::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_btnBrowse.SetIcon (m_hIconBrowse = AfxGetApp ()->LoadIcon (IDI_ICON_BROWSE));
	((CSliderCtrl*) GetDlgItem (IDC_SLIDER_OPACITY))->SetRange (0x00, 0xFF);
	
	return TRUE;
}

void CStyleDlgAlpha::OnButtonBrowse() 
{
	CString strFilename;
	strFilename.LoadString (IDS_OPENBKALPHAIMGFILE_FILENAME);
	CString strFilter;
	strFilter.LoadString (IDS_OPENBKALPHAIMGFILE_FILTER);

	CString strInitialDir = ((CCdCoverCreator2App*) AfxGetApp ())->GetAlphaTextureDir ();
	CImagePreviewFileDialogEx dlg (TRUE, NULL, strInitialDir + strFilename, 0, strFilter, this);
	dlg.m_ofn.lpstrInitialDir = strInitialDir.Left (strInitialDir.GetLength () - 1);

	if (dlg.DoModal () == IDOK)
		SetDlgItemText (IDC_EDIT_FILE, dlg.GetPathName ());
}

void CStyleDlgAlpha::OnChangeEditFile() 
{
	UpdateData ();

	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
	if (pChild != NULL)
	{
		CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
		if (pDoc != NULL)
			pDoc->SetBkAlphaFilename (m_strFilename);
	}	
}

void CStyleDlgAlpha::OnRadioAlphaformatDarken() 
{
	m_pBkAlpha->m_nAlphaFormat = 0;

	// update image
	UpdateDoc ();
}

void CStyleDlgAlpha::OnRadioAlphaformatLighten() 
{
	m_pBkAlpha->m_nAlphaFormat = 1;

	// update image
	UpdateDoc ();
}

void CStyleDlgAlpha::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if ((pScrollBar->GetDlgCtrlID () == IDC_SLIDER_OPACITY) && (nSBCode == SB_ENDSCROLL))
	{
		UpdateData ();
		m_pBkAlpha->m_nSourceConstantAlpha = m_nSourceConstantAlpha;

		// update image
		UpdateDoc ();
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CStyleDlgAlpha::SetBackgroundAlpha(CBackgroundAlphaTexture *pBk)
{
	m_pBkAlpha = pBk;

	if (pBk != NULL)
	{
		GetDlgItem (IDC_EDIT_FILE)->EnableWindow (true);
		GetDlgItem (IDC_BUTTON_BROWSE)->EnableWindow (true);
		GetDlgItem (IDC_RADIO_ALPHAFORMAT_DARKEN)->EnableWindow (true);
		GetDlgItem (IDC_RADIO_ALPHAFORMAT_LIGHTEN)->EnableWindow (true);
		GetDlgItem (IDC_SLIDER_OPACITY)->EnableWindow (true);

		m_strFilename = pBk->GetImageFilename ();
		m_nAlphaFormat = pBk->m_nAlphaFormat;
		m_nSourceConstantAlpha = pBk->m_nSourceConstantAlpha;

		UpdateData (false);
	}
	else
	{
		GetDlgItem (IDC_EDIT_FILE)->EnableWindow (false);
		GetDlgItem (IDC_BUTTON_BROWSE)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_ALPHAFORMAT_DARKEN)->EnableWindow (false);
		GetDlgItem (IDC_RADIO_ALPHAFORMAT_LIGHTEN)->EnableWindow (false);
		GetDlgItem (IDC_SLIDER_OPACITY)->EnableWindow (false);
	}
}
