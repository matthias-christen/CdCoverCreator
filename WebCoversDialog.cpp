// WebCoversDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "WebCoversDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebCoversDialog dialog


CWebCoversDialog::CWebCoversDialog(UINT nId, CWnd* pParent /*=NULL*/)
	: CDialog(nId, pParent)
{
	m_pPic = NULL;
	m_pSearchThread = NULL;
	m_pImgsThread = NULL;

	m_arrFilenames.SetSize (4);

	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof (NONCLIENTMETRICS);
	SystemParametersInfo (SPI_GETNONCLIENTMETRICS, sizeof (NONCLIENTMETRICS), &ncm, 0);
	m_font.CreateFontIndirect (&ncm.lfMessageFont);
}

CWebCoversDialog::~CWebCoversDialog ()
{
	if (m_pPic != NULL)
		delete m_pPic;

	m_font.DeleteObject ();

	for (int i = 0; i < m_ImageList.GetImageCount (); i++)
		DestroyIcon (m_ImageList.ExtractIcon (i));
	m_ImageList.DeleteImageList ();
}

void CWebCoversDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWebCoversDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWebCoversDialog, CDialog)
	//{{AFX_MSG_MAP(CWebCoversDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWebCoversDialog message handlers

BOOL CWebCoversDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWinApp* pApp = AfxGetApp ();

	m_ImageList.Create (16, 16, ILC_COLOR4 | ILC_MASK, 4, 1);
	m_ImageList.Add (pApp->LoadIcon (IDI_ICON_COVER));
	m_ImageList.Add (pApp->LoadIcon (IDI_ICON_BACK));
	m_ImageList.Add (pApp->LoadIcon (IDI_ICON_INLAY));
	m_ImageList.Add (pApp->LoadIcon (IDI_ICON_LABEL));

	return TRUE;
}

void CWebCoversDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect r;
	GetDlgItem (IDC_STATIC_IMAGE)->GetWindowRect (&r);
	ScreenToClient (&r);

	dc.Draw3dRect (&r, GetSysColor (COLOR_3DSHADOW), GetSysColor (COLOR_3DHILIGHT));

	if (m_pPic != NULL)
	{
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
	else
	{
		CString strPreview;
		strPreview.LoadString (IDS_PREVIEW);
		CFont* pOld = dc.SelectObject (&m_font);
		dc.SetBkMode (TRANSPARENT);
		dc.DrawText (strPreview, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		dc.SelectObject (pOld);
	}
}

void CWebCoversDialog::OnOK() 
{
	// search first non-empty file name
	CString strFilename;
	for (int i = 0; i < m_arrFilenames.GetSize (); i++)
		if (!m_arrFilenames.GetAt (i).IsEmpty ())
		{
			strFilename = m_arrFilenames.GetAt (i);
			break;
		}

	// set empty entries to this file name
	if (!strFilename.IsEmpty ())
		for (i = 0; i < m_arrFilenames.GetSize (); i++)
			if (m_arrFilenames.GetAt (i).IsEmpty ())
				m_arrFilenames.SetAt (i, strFilename);

	CDialog::OnOK();
}