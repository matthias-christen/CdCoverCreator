// ImagePreviewFileDialogEx.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "ImagePreviewFileDialogEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImagePreviewFileDialogEx

IMPLEMENT_DYNAMIC(CImagePreviewFileDialogEx, CFileDialog)

CImagePreviewFileDialogEx::CImagePreviewFileDialogEx(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog (bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_ofn.Flags |= OFN_ENABLETEMPLATE;
	m_ofn.lpTemplateName = MAKEINTRESOURCE (IDD_FILEOPENPREVIEW);

	m_bPreview = true;
	m_pPic = NULL;
}


BEGIN_MESSAGE_MAP(CImagePreviewFileDialogEx, CFileDialog)
	//{{AFX_MSG_MAP(CImagePreviewFileDialogEx)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CImagePreviewFileDialogEx::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
//	m_DIBStaticCtrl.SubclassDlgItem(IDC_IMAGE, this);
	GetDlgItem (IDC_PREVIEW)->SendMessage (BM_SETCHECK, (m_bPreview) ? 1 : 0);
	
	return TRUE;
}

void CImagePreviewFileDialogEx::OnFileNameChange() 
{
	CFileDialog::OnFileNameChange();

	if (m_bPreview)
	{
		if (m_pPic != NULL)
			delete m_pPic;

		m_pPic = new CPicture (GetPathName ());

		CRect r;
		GetDlgItem (IDC_IMAGE)->GetWindowRect (&r);
		ScreenToClient (&r);
		InvalidateRect (&r);
	}
}

void CImagePreviewFileDialogEx::OnFolderChange() 
{
	CFileDialog::OnFolderChange ();
	
	if (m_pPic != NULL)
	{
		delete m_pPic;
		m_pPic = NULL;

		CRect r;
		GetDlgItem (IDC_IMAGE)->GetWindowRect (&r);
		ScreenToClient (&r);
		InvalidateRect (&r);
	}
}

void CImagePreviewFileDialogEx::OnPreview() 
{
	m_bPreview = !m_bPreview;

	if (!m_bPreview)
	{
		if (m_pPic != NULL)
			delete m_pPic;
		m_pPic = NULL;
	}
	else
		m_pPic = new CPicture (GetPathName ());

	CRect r;
	GetDlgItem (IDC_IMAGE)->GetWindowRect (&r);
	ScreenToClient (&r);
	InvalidateRect (&r);
}

void CImagePreviewFileDialogEx::OnPaint() 
{
	CPaintDC dc (this); // device context for painting
	
	if (m_pPic != NULL)
	{
		CRect r;
		GetDlgItem (IDC_IMAGE)->GetWindowRect (&r);
		
		CWnd* pWnd = GetActiveWindow ();
		if (pWnd == NULL)
			return;
		if (!::IsWindow (pWnd->GetSafeHwnd ()))
			return;

		pWnd->ScreenToClient (&r);
		r.DeflateRect (5, 5, 5, 5);

		CSize sz = m_pPic->GetSize ();
		if (sz.cx > r.Width ())
		{
			sz.cy = (sz.cy * r.Width ()) / sz.cx;
			sz.cx = r.Width ();
		}
		if (sz.cy > r.Height ())
		{
			sz.cx = (sz.cx * r.Height ()) / sz.cy;
			sz.cy = r.Height ();
		}

		m_pPic->Render (&dc, NULL,
			(r.left + r.right - sz.cx) / 2,
			(r.top + r.bottom + sz.cy) / 2,
			sz.cx, -sz.cy, false);
	}
}

void CImagePreviewFileDialogEx::PostNcDestroy()
{
	if (m_pPic != NULL)
		delete m_pPic;

	CFileDialog::PostNcDestroy();
}
