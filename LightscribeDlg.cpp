// LightscribeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "LightscribeDlg.h"


// CLightscribeDlg dialog

IMPLEMENT_DYNAMIC(CLightscribeDlg, CDialog)
CLightscribeDlg::CLightscribeDlg(CWnd* pParent, CStringList* pListDevices)
	: CDialog(CLightscribeDlg::IDD, pParent)
	, m_nPrintQuality (0)
	, m_pListDevices (pListDevices)
	, m_strLightscribeDevice(_T(""))
{
	CString strPreviewPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetImageCacheDir () + "lspreview.bmp";
	BSTR bstrPreviewPath = strPreviewPath.AllocSysString ();

	m_pImage = new Image (bstrPreviewPath);

	::SysFreeString (bstrPreviewPath);
}

CLightscribeDlg::~CLightscribeDlg()
{
	delete m_pImage;
}

void CLightscribeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_QUALITY, m_nPrintQuality);
	DDX_CBStringExact(pDX, IDC_COMBO_LIGHTSCRIBEDEVICE, m_strLightscribeDevice);
}


BEGIN_MESSAGE_MAP(CLightscribeDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CLightscribeDlg message handlers

void CLightscribeDlg::OnPaint()
{
	CPaintDC dc (this); // device context for painting

	CRect r;
	GetDlgItem (IDC_STATIC_PREVIEW)->GetWindowRect (&r);
	ScreenToClient (&r);

	Graphics g (dc.GetSafeHdc ());
	g.DrawImage (m_pImage, (r.left + r.right - 200) / 2, (r.top + r.bottom - 200) / 2, 200, 200);
}

BOOL CLightscribeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pCb = (CComboBox*) GetDlgItem (IDC_COMBO_LIGHTSCRIBEDEVICE);
	POSITION pos = m_pListDevices->GetHeadPosition ();
	while (pos != NULL)
		pCb->AddString (m_pListDevices->GetNext (pos));
	pCb->SetCurSel (0);

	return TRUE;
}
