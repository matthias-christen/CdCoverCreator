// StyleDlgBkImage.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "MainFrm.h"
#include "CdCoverCreator2Doc.h"
#include "CdCoverCreator2View.h"
#include "StyleDlgBkImage.h"
#include "ImagePreviewFileDialogEx.h"
#include "DlgImageProperties.h"
#include "PSImageEditing.h"
#include "cccTwain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgBkImage dialog


CStyleDlgBkImage::CStyleDlgBkImage(CWnd* pParent /*=NULL*/)
	: CStyleDlgBase(CStyleDlgBkImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStyleDlgBkImage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bSettingTransparentColor = false;
	m_crTransparent = -1;

	CWinApp* pApp = AfxGetApp ();

	m_hIconBrowse = pApp->LoadIcon (IDI_ICON_BROWSE);
	m_hIconScan = pApp->LoadIcon (IDI_ICON_SCAN);
	m_hIconSetTransparent = pApp->LoadIcon (IDI_SETTRANSPARENT);
	m_hIconRemoveTransparent = pApp->LoadIcon (IDI_REMOVETRANSPARENT);

	m_hPickCursor = pApp->LoadCursor (IDC_CURSOR_PICKER);
}

CStyleDlgBkImage::~CStyleDlgBkImage ()
{
	::DestroyIcon (m_hIconBrowse);
	::DestroyIcon (m_hIconScan);
	::DestroyIcon (m_hIconSetTransparent);
	::DestroyIcon (m_hIconRemoveTransparent);

	::DestroyIcon (m_hIconTL);
	::DestroyIcon (m_hIconTC);
	::DestroyIcon (m_hIconTR);
	::DestroyIcon (m_hIconCL);
	::DestroyIcon (m_hIconCC);
	::DestroyIcon (m_hIconCR);
	::DestroyIcon (m_hIconBL);
	::DestroyIcon (m_hIconBC);
	::DestroyIcon (m_hIconBR);

	::DestroyCursor (m_hPickCursor);
}

void CStyleDlgBkImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStyleDlgBkImage)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_SCAN, m_btnScan);
	DDX_Control(pDX, IDC_BUTTON_REMOVETRANSPARENT, m_btnRemoveTransparent);
	DDX_Control(pDX, IDC_BUTTON_TRANSPARENTCOLOR, m_btnSetTransparent);
	DDX_Control(pDX, IDC_STATIC_TRANSPARENT, m_StaticTransparentCol);
	DDX_Control(pDX, IDC_BROWSEBKIMG, m_BrowseBkImg);
	DDX_Text(pDX, IDC_EDIT_BKIMGFILENAME, m_strImgFilename);
	DDX_Radio(pDX, IDC_RADIO1, m_enumMode);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_ADJUSTCOLOR, m_btnAdjustColor);
	DDX_Control(pDX, IDC_BUTTON_EDITIMAGE, m_btnEditImage);
	DDX_Control(pDX, IDC_BUTTON_RELOADIMAGE, m_btnReloadImage);
}


BEGIN_MESSAGE_MAP(CStyleDlgBkImage, CDialog)
	//{{AFX_MSG_MAP(CStyleDlgBkImage)
	ON_BN_CLICKED(IDC_BUTTON_TRANSPARENTCOLOR, OnButtonTransparentcolor)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_REMOVETRANSPARENT, OnButtonRemovetransparent)
	ON_BN_CLICKED(IDC_BROWSEBKIMG, OnBrowseBkImg)
	ON_BN_CLICKED(IDC_SCAN, OnScan)
	ON_EN_CHANGE(IDC_EDIT_BKIMGFILENAME, OnChangeEditBkimgfilename)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO11, OnAlignment)
	ON_BN_CLICKED(IDC_BUTTON_ADJUSTCOLOR, OnAdjustColor)
	ON_BN_CLICKED(IDC_BUTTON_EDITIMAGE, OnBnClickedButtonEditimage)
	ON_BN_CLICKED(IDC_BUTTON_RELOADIMAGE, OnBnClickedButtonReloadimage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgBkImage message handlers

BOOL CStyleDlgBkImage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_BrowseBkImg.SetIcon (m_hIconBrowse);
	m_btnScan.SetIcon (m_hIconScan);

	CWinApp* pApp = AfxGetApp ();

	GetDlgItem (IDC_RADIO1)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconTL = pApp->LoadIcon (IDI_ICON_TL)));
	GetDlgItem (IDC_RADIO2)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconTC = pApp->LoadIcon (IDI_ICON_TC)));
	GetDlgItem (IDC_RADIO3)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconTR = pApp->LoadIcon (IDI_ICON_TR)));
	GetDlgItem (IDC_RADIO4)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconCL = pApp->LoadIcon (IDI_ICON_CL)));
	GetDlgItem (IDC_RADIO5)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconCC = pApp->LoadIcon (IDI_ICON_CC)));
	GetDlgItem (IDC_RADIO6)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconCR = pApp->LoadIcon (IDI_ICON_CR)));
	GetDlgItem (IDC_RADIO7)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconBL = pApp->LoadIcon (IDI_ICON_BL)));
	GetDlgItem (IDC_RADIO8)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconBC = pApp->LoadIcon (IDI_ICON_BC)));
	GetDlgItem (IDC_RADIO9)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconBR = pApp->LoadIcon (IDI_ICON_BR)));

	GetDlgItem (IDC_RADIO5)->SendMessage (BM_SETCHECK, BST_CHECKED, 0);

	m_btnSetTransparent.SetIcon (m_hIconSetTransparent);
	m_btnRemoveTransparent.SetIcon (m_hIconRemoveTransparent);
	m_StaticTransparentCol.SetAngle (0);
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool (&m_BrowseBkImg, IDS_TT_BROWSE);
		m_tooltip.AddTool (&m_btnScan, IDS_TT_SCAN);
		m_tooltip.AddTool (&m_btnSetTransparent, IDS_TT_TRANSPARENT);
		m_tooltip.AddTool (&m_btnRemoveTransparent, IDS_TT_REMOVETRANSPARENT);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStyleDlgBkImage::OnBrowseBkImg() 
{
	CString strFilename;
	strFilename.LoadString (IDS_OPENBKIMGFILE_FILENAME);
	CString strFilter;
	strFilter.LoadString (IDS_OPENBKIMGFILE_FILTER);

	CImagePreviewFileDialogEx dlg (TRUE, NULL, strFilename, 0, strFilter, this);
	if (dlg.DoModal () == IDOK)
	{
		SetDlgItemText (IDC_EDIT_BKIMGFILENAME, dlg.GetPathName ());
//		OnButtonPreview ();
	}
}

void CStyleDlgBkImage::OnScan() 
{
	CcccTwain* pTwain = &((CMainFrame*) AfxGetApp ()->GetMainWnd ())->m_twain;
	pTwain->m_pBkImgPage = NULL;//this;
	pTwain->SelectSource ();
	if (pTwain->SourceSelected ())
		pTwain->Acquire ();
}

void CStyleDlgBkImage::OnButtonTransparentcolor() 
{
	m_bSettingTransparentColor = true;
	SetCapture ();
	SetCursor (m_hPickCursor);
}

void CStyleDlgBkImage::OnButtonRemovetransparent() 
{
	m_crTransparent = m_pBackgroundInfo->m_crTransparent = -1;
	m_StaticTransparentCol.ShowWindow (SW_HIDE);

	// update image
	UpdateDoc ();
}

BOOL CStyleDlgBkImage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bSettingTransparentColor)
	{
		::SetCursor (m_hPickCursor);
		return TRUE;
	}
	else
		return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CStyleDlgBkImage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bSettingTransparentColor)
	{
		POINT pt;
		::GetCursorPos (&pt);

		HDC hdc = ::GetDC (NULL);
		m_crTransparent = m_pBackgroundInfo->m_crTransparent = ::GetPixel (hdc, pt.x, pt.y);
		::ReleaseDC (NULL, hdc);

		m_StaticTransparentCol.SetStartColor (m_crTransparent);
		m_StaticTransparentCol.SetEndColor (m_crTransparent);
		m_StaticTransparentCol.ShowWindow (SW_SHOW);

		m_bSettingTransparentColor = false;

		ReleaseCapture ();

		// update image
		UpdateDoc ();
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CStyleDlgBkImage::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bSettingTransparentColor)
	{
		POINT pt;
		::GetCursorPos (&pt);

		HDC hdc = ::GetDC (NULL);
		m_crTransparent = ::GetPixel (hdc, pt.x, pt.y);
		::ReleaseDC (NULL, hdc);

		m_StaticTransparentCol.SetStartColor (m_crTransparent);
		m_StaticTransparentCol.SetEndColor (m_crTransparent);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CStyleDlgBkImage::SetBackground(CBackgroundInfo *pBk, RenderType rt)
{
	m_pBackgroundInfo = pBk;
	m_RenderType = rt;

	if (pBk != NULL)
	{
		m_strImgFilename = pBk->m_strImgFilename;
		m_enumMode = pBk->m_enumMode;
		m_crTransparent = pBk->m_crTransparent;

		UpdateData (false);

		if (m_crTransparent == -1)
			m_StaticTransparentCol.ShowWindow (SW_HIDE);
		else
		{
			m_StaticTransparentCol.SetStartColor (m_crTransparent);
			m_StaticTransparentCol.SetEndColor (m_crTransparent);
			m_StaticTransparentCol.ShowWindow (SW_SHOW);
		}

		m_btnScan.EnableWindow (true);
		m_btnRemoveTransparent.EnableWindow (true);
		m_btnSetTransparent.EnableWindow (true);
		m_StaticTransparentCol.EnableWindow (true);
		m_BrowseBkImg.EnableWindow (true);
		m_btnAdjustColor.EnableWindow (true);
		m_btnEditImage.EnableWindow (true);
		m_btnReloadImage.EnableWindow (true);
		GetDlgItem (IDC_EDIT_BKIMGFILENAME)->EnableWindow (true);
		GetDlgItem (IDC_RADIO1)->EnableWindow (true);
		GetDlgItem (IDC_RADIO2)->EnableWindow (true);
		GetDlgItem (IDC_RADIO3)->EnableWindow (true);
		GetDlgItem (IDC_RADIO4)->EnableWindow (true);
		GetDlgItem (IDC_RADIO5)->EnableWindow (true);
		GetDlgItem (IDC_RADIO6)->EnableWindow (true);
		GetDlgItem (IDC_RADIO7)->EnableWindow (true);
		GetDlgItem (IDC_RADIO8)->EnableWindow (true);
		GetDlgItem (IDC_RADIO9)->EnableWindow (true);
		GetDlgItem (IDC_RADIO10)->EnableWindow (true);
		GetDlgItem (IDC_RADIO11)->EnableWindow (true);
	}
	else
	{
		m_btnScan.EnableWindow (false);
		m_btnRemoveTransparent.EnableWindow (false);
		m_btnSetTransparent.EnableWindow (false);
		m_StaticTransparentCol.EnableWindow (false);
		m_BrowseBkImg.EnableWindow (false);
		m_btnAdjustColor.EnableWindow (false);
		m_btnEditImage.EnableWindow (false);
		m_btnReloadImage.EnableWindow (false);
		GetDlgItem (IDC_EDIT_BKIMGFILENAME)->EnableWindow (false);
		GetDlgItem (IDC_RADIO1)->EnableWindow (false);
		GetDlgItem (IDC_RADIO2)->EnableWindow (false);
		GetDlgItem (IDC_RADIO3)->EnableWindow (false);
		GetDlgItem (IDC_RADIO4)->EnableWindow (false);
		GetDlgItem (IDC_RADIO5)->EnableWindow (false);
		GetDlgItem (IDC_RADIO6)->EnableWindow (false);
		GetDlgItem (IDC_RADIO7)->EnableWindow (false);
		GetDlgItem (IDC_RADIO8)->EnableWindow (false);
		GetDlgItem (IDC_RADIO9)->EnableWindow (false);
		GetDlgItem (IDC_RADIO10)->EnableWindow (false);
		GetDlgItem (IDC_RADIO11)->EnableWindow (false);
	}
}

void CStyleDlgBkImage::OnAlignment(UINT nId)
{
	UpdateData ();
	m_pBackgroundInfo->m_enumMode = (BkImageAlignmentMode) m_enumMode;
	UpdateDoc ();
}

void CStyleDlgBkImage::OnChangeEditBkimgfilename() 
{
	UpdateData ();

	bool bImageValid = false;

	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
	if (pChild != NULL)
	{
		CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
		if (pDoc != NULL)
			bImageValid = pDoc->SetBkImageFilename (m_strImgFilename);
	}
	
	m_btnAdjustColor.EnableWindow (bImageValid);
	m_btnEditImage.EnableWindow (bImageValid);
	m_btnReloadImage.EnableWindow (bImageValid);
}

void CStyleDlgBkImage::OnAdjustColor()
{
	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
	if (pChild != NULL)
	{
		// get doc and view
		CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
		if (pDoc == NULL)
			return;

		CCdCoverCreator2View* pView = (CCdCoverCreator2View*) pChild->GetActiveView ();
		if (pView == NULL)
			return;

		// get the background image
		CPicture* pPicture = pDoc->m_pBackground[pView->GetRenderType ()]->GetPicture ();
		if (pPicture == NULL)
			return;

		// set up the dialog
		CDlgImageProperties dlg;
		dlg.m_pDocument = pDoc;
		dlg.SetPicture (pPicture);

		if (dlg.DoModal () == IDOK)
			pDoc->CheckPoint ();
	}
}

void CStyleDlgBkImage::OnBnClickedButtonEditimage()
{
	CString strAppPath = AfxGetApp ()->GetProfileString ("ImageEditing", "app", "");
	if (strAppPath.IsEmpty ())
	{
		CPSImageEditing dlg;
		if (dlg.DoModal () == IDOK)
		{
			strAppPath = dlg.m_strAppName;
			if (strAppPath.IsEmpty ())
				return;
		}
	}

	UpdateData ();
	WinExec (strAppPath + " \"" + m_strImgFilename + "\"", SW_SHOW);
}

void CStyleDlgBkImage::OnBnClickedButtonReloadimage()
{
	OnChangeEditBkimgfilename ();
}
