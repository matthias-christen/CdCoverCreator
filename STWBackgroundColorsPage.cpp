// STWBackgroundColorsPage.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "STWBackgroundColorsPage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplate.h"
#include "StyleTemplates.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTWBackgroundColorsPage property page

IMPLEMENT_DYNCREATE(CSTWBackgroundColorsPage, CPropertyPage)

CSTWBackgroundColorsPage::CSTWBackgroundColorsPage() : CPropertyPage(CSTWBackgroundColorsPage::IDD)
{
	//{{AFX_DATA_INIT(CSTWBackgroundColorsPage)
	//}}AFX_DATA_INIT

	m_bDegCoverChanged = false;
	m_bDegBackChanged = false;
	m_bDegLabelChanged = false;
	m_bDegInlayChanged = false;
}

CSTWBackgroundColorsPage::~CSTWBackgroundColorsPage()
{
}

void CSTWBackgroundColorsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWBackgroundColorsPage)
	DDX_Control(pDX, IDC_SPIN_DEGLABEL, m_SpinDegLabel);
	DDX_Control(pDX, IDC_SPIN_DEGINLAY, m_SpinDegInlay);
	DDX_Control(pDX, IDC_SPIN_DEGCOVER, m_SpinDegCover);
	DDX_Control(pDX, IDC_SPIN_DEGBACK, m_SpinDegBack);
	DDX_Control(pDX, IDC_EDIT_DEGLABEL, m_EditDegLabel);
	DDX_Control(pDX, IDC_EDIT_DEGINLAY, m_EditDegInlay);
	DDX_Control(pDX, IDC_EDIT_DEGCOVER, m_EditDegCover);
	DDX_Control(pDX, IDC_EDIT_DEGBACK, m_EditDegBack);
	DDX_Control(pDX, IDC_DEGM_LABEL, m_DegLabel);
	DDX_Control(pDX, IDC_DEGM_INLAY, m_DegInlay);
	DDX_Control(pDX, IDC_DEGM_COVER, m_DegCover);
	DDX_Control(pDX, IDC_DEGM_BACK, m_DegBack);
	DDX_Control(pDX, IDC_STATIC_LABEL_BKCOLOR, m_BkColLabel);
	DDX_Control(pDX, IDC_STATIC_INLAY_BKCOLOR, m_BkColInlay);
	DDX_Control(pDX, IDC_STATIC_COVER_BKCOLOR, m_BkColCover);
	DDX_Control(pDX, IDC_STATIC_BACK_BKCOLOR, m_BkColBack);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTWBackgroundColorsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWBackgroundColorsPage)
	ON_LGPN_UPDATE(IDC_STATIC_LABEL_BKCOLOR, OnBkColorLabel)
	ON_LGPN_UPDATE(IDC_STATIC_INLAY_BKCOLOR, OnBkColorInlay)
	ON_LGPN_UPDATE(IDC_STATIC_COVER_BKCOLOR, OnBkColorCover)
	ON_LGPN_UPDATE(IDC_STATIC_BACK_BKCOLOR, OnBkColorBack)

	ON_DSN_UPDATE(IDC_DEGM_BACK, OnDegmBack)
	ON_DSN_UPDATE(IDC_DEGM_COVER, OnDegmCover)
	ON_DSN_UPDATE(IDC_DEGM_LABEL, OnDegmLabel)
	ON_DSN_UPDATE(IDC_DEGM_INLAY, OnDegmInlay)

	ON_DSN_BUTTONUP(IDC_DEGM_BACK, OnDegmBackBtnUp)
	ON_DSN_BUTTONUP(IDC_DEGM_COVER, OnDegmCoverBtnUp)
	ON_DSN_BUTTONUP(IDC_DEGM_LABEL, OnDegmLabelBtnUp)
	ON_DSN_BUTTONUP(IDC_DEGM_INLAY, OnDegmInlayBtnUp)

	ON_EN_CHANGE(IDC_EDIT_DEGCOVER, OnChangeEditDegcover)
	ON_EN_CHANGE(IDC_EDIT_DEGBACK, OnChangeEditDegback)
	ON_EN_CHANGE(IDC_EDIT_DEGLABEL, OnChangeEditDeglabel)
	ON_EN_CHANGE(IDC_EDIT_DEGINLAY, OnChangeEditDeginlay)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnButtonColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTWBackgroundColorsPage message handlers

BOOL CSTWBackgroundColorsPage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
	
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	if (pTmp != NULL)
	{
		CString s;

//		m_BkColCover.SetStartColor (pTmp->GetBkColor (Cover));
//		m_BkColCover.SetEndColor (pTmp->GetBkColor2 (Cover));
//		m_BkColCover.SetAngle (pTmp->GetBkAngle (Cover));
		m_BkColCover.SetData (*(pTmp->GetGradientData (Cover)));
		m_DegCover.SetDegAngle (pTmp->GetBkAngle (Cover));
		s.Format ("%d", pTmp->GetBkAngle (Cover) / 10);
		m_EditDegCover.SetWindowText (s);

//		m_BkColBack.SetStartColor (pTmp->GetBkColor (Back));
//		m_BkColBack.SetEndColor (pTmp->GetBkColor2 (Back));
//		m_BkColBack.SetAngle (pTmp->GetBkAngle (Back));
		m_BkColBack.SetData (*(pTmp->GetGradientData (Back)));
		m_DegBack.SetDegAngle (pTmp->GetBkAngle (Back));
		s.Format ("%d", pTmp->GetBkAngle (Back) / 10);
		m_EditDegBack.SetWindowText (s);

//		m_BkColLabel.SetStartColor (pTmp->GetBkColor (Label));
//		m_BkColLabel.SetEndColor (pTmp->GetBkColor2 (Label));
//		m_BkColLabel.SetAngle (pTmp->GetBkAngle (Label));
		m_BkColLabel.SetData (*(pTmp->GetGradientData (Label)));
		m_DegLabel.SetDegAngle (pTmp->GetBkAngle (Label));
		s.Format ("%d", pTmp->GetBkAngle (Label) / 10);
		m_EditDegLabel.SetWindowText (s);

//		m_BkColInlay.SetStartColor (pTmp->GetBkColor (Inlay));
//		m_BkColInlay.SetEndColor (pTmp->GetBkColor2 (Inlay));
//		m_BkColInlay.SetAngle (pTmp->GetBkAngle (Inlay));
		m_BkColInlay.SetData (*(pTmp->GetGradientData (Inlay)));
		m_DegInlay.SetDegAngle (pTmp->GetBkAngle (Inlay));
		s.Format ("%d", pTmp->GetBkAngle (Inlay) / 10);
		m_EditDegInlay.SetWindowText (s);
	}

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
	
	return CPropertyPage::OnSetActive();
}

BOOL CSTWBackgroundColorsPage::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertyPage::OnKillActive();
}

BOOL CSTWBackgroundColorsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_SpinDegCover.SetRange (0, 360);
	m_SpinDegBack.SetRange (0, 360);
	m_SpinDegLabel.SetRange (0, 360);
	m_SpinDegInlay.SetRange (0, 360);

	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool (&m_DegCover, IDS_TT_DIRECTION);
		m_tooltip.AddTool (&m_DegBack, IDS_TT_DIRECTION);
		m_tooltip.AddTool (&m_DegLabel, IDS_TT_DIRECTION);
		m_tooltip.AddTool (&m_DegInlay, IDS_TT_DIRECTION);
	}

	return TRUE;
}

LRESULT CSTWBackgroundColorsPage::OnButtonColor(WPARAM wParam, LPARAM lParam)
{
	/*
	switch (lParam)
	{
	case IDC_BUTTON_COVER_BKCOLOR:
		m_BkColCover.SetStartColor (m_ButtonColCover.GetColour ());
		m_BkColCover.InvalidateRect (NULL);
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crBkStart[Cover] = m_ButtonColCover.GetColour ();
		break;
	case IDC_BUTTON_COVER_BKCOLOR2:
		m_BkColCover.SetEndColor (m_ButtonColCover2.GetColour ());
		m_BkColCover.InvalidateRect (NULL);
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crBkEnd[Cover] = m_ButtonColCover2.GetColour ();
		break;
	case IDC_BUTTON_BACK_BKCOLOR:
		m_BkColBack.SetStartColor (m_ButtonColBack.GetColour ());
		m_BkColBack.InvalidateRect (NULL);
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crBkStart[Back] = m_ButtonColBack.GetColour ();
		break;
	case IDC_BUTTON_BACK_BKCOLOR2:
		m_BkColBack.SetEndColor (m_ButtonColBack2.GetColour ());
		m_BkColBack.InvalidateRect (NULL);
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crBkEnd[Back] = m_ButtonColBack2.GetColour ();
		break;
	case IDC_BUTTON_LABEL_BKCOLOR:
		m_BkColLabel.SetStartColor (m_ButtonColLabel.GetColour ());
		m_BkColLabel.InvalidateRect (NULL);
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crBkStart[Label] = m_ButtonColLabel.GetColour ();
		break;
	case IDC_BUTTON_LABEL_BKCOLOR2:
		m_BkColLabel.SetEndColor (m_ButtonColLabel2.GetColour ());
		m_BkColLabel.InvalidateRect (NULL);
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crBkEnd[Label] = m_ButtonColLabel2.GetColour ();
		break;
	case IDC_BUTTON_INLAY_BKCOLOR:
		m_BkColInlay.SetStartColor (m_ButtonColInlay.GetColour ());
		m_BkColInlay.InvalidateRect (NULL);
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crBkStart[Inlay] = m_ButtonColInlay.GetColour ();
		break;
	case IDC_BUTTON_INLAY_BKCOLOR2:
		m_BkColInlay.SetEndColor (m_ButtonColInlay2.GetColour ());
		m_BkColInlay.InvalidateRect (NULL);
		((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_crBkEnd[Inlay] = m_ButtonColInlay2.GetColour ();
		break;
	}*/

	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);

	return S_OK;
}

void CSTWBackgroundColorsPage::OnBkColorLabel()
{
	m_BkColLabel.GetData (&((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Label]);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnBkColorInlay()
{
	m_BkColInlay.GetData (&((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Inlay]);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnBkColorCover()
{
	m_BkColCover.GetData (&((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Cover]);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnBkColorBack()
{
	m_BkColBack.GetData (&((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Back]);
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnDegmCover() 
{
	m_bDegCoverChanged = true;

	CString s;
	s.Format ("%d", m_DegCover.GetDegAngle () / 10);
	m_EditDegCover.SetWindowText (s);
	m_SpinDegCover.SetPos (m_DegCover.GetDegAngle () / 10);

	m_BkColCover.SetAngle (m_DegCover.GetDegAngle ());
	m_BkColCover.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnDegmBack() 
{
	m_bDegBackChanged = true;

	CString s;
	s.Format ("%d", m_DegBack.GetDegAngle () / 10);
	m_EditDegBack.SetWindowText (s);
	m_SpinDegBack.SetPos (m_DegBack.GetDegAngle () / 10);

	m_BkColBack.SetAngle (m_DegBack.GetDegAngle ());
	m_BkColBack.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnDegmLabel() 
{
	m_bDegLabelChanged = true;

	CString s;
	s.Format ("%d", m_DegLabel.GetDegAngle () / 10);
	m_EditDegLabel.SetWindowText (s);
	m_SpinDegLabel.SetPos (m_DegLabel.GetDegAngle () / 10);

	m_BkColLabel.SetAngle (m_DegLabel.GetDegAngle ());
	m_BkColLabel.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnDegmInlay() 
{
	m_bDegInlayChanged = true;

	CString s;
	s.Format ("%d", m_DegInlay.GetDegAngle () / 10);
	m_EditDegInlay.SetWindowText (s);
	m_SpinDegInlay.SetPos (m_DegInlay.GetDegAngle () / 10);

	m_BkColInlay.SetAngle (m_DegInlay.GetDegAngle ());
	m_BkColInlay.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnDegmCoverBtnUp ()
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Cover].dAngle = deg2rad (m_DegCover.GetDegAngle ());
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnDegmBackBtnUp ()
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Back].dAngle = deg2rad (m_DegBack.GetDegAngle ());
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnDegmLabelBtnUp ()
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Label].dAngle = deg2rad (m_DegLabel.GetDegAngle ());
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnDegmInlayBtnUp ()
{
	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Inlay].dAngle = deg2rad (m_DegInlay.GetDegAngle ());
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnChangeEditDegcover() 
{
	if (m_bDegCoverChanged)
	{
		m_bDegCoverChanged = false;
		return;
	}

	if (!::IsWindow (m_EditDegCover.GetSafeHwnd ()))
		return;

	CString s;
	m_EditDegCover.GetWindowText (s);
	m_DegCover.SetDegAngle (atoi (s) * 10);

	m_BkColCover.SetAngle (m_DegCover.GetDegAngle ());
	if (::IsWindow (m_BkColCover.GetSafeHwnd ()))
		m_BkColCover.InvalidateRect (NULL);

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Cover].dAngle = deg2rad (m_DegCover.GetDegAngle ());
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnChangeEditDegback() 
{
	if (m_bDegBackChanged)
	{
		m_bDegBackChanged = false;
		return;
	}

	if (!::IsWindow (m_EditDegBack.GetSafeHwnd ()))
		return;

	CString s;
	m_EditDegBack.GetWindowText (s);
	m_DegBack.SetDegAngle (atoi (s) * 10);

	m_BkColBack.SetAngle (m_DegBack.GetDegAngle ());
	if (::IsWindow (m_BkColBack.GetSafeHwnd ()))
		m_BkColBack.InvalidateRect (NULL);

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Back].dAngle = deg2rad (m_DegBack.GetDegAngle ());
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnChangeEditDeglabel() 
{
	if (m_bDegLabelChanged)
	{
		m_bDegLabelChanged = false;
		return;
	}

	if (!::IsWindow (m_EditDegLabel.GetSafeHwnd ()))
		return;

	CString s;
	m_EditDegLabel.GetWindowText (s);
	m_DegLabel.SetDegAngle (atoi (s) * 10);

	m_BkColLabel.SetAngle (m_DegLabel.GetDegAngle ());
	if (::IsWindow (m_BkColLabel.GetSafeHwnd ()))
		m_BkColLabel.InvalidateRect (NULL);

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Label].dAngle = deg2rad (m_DegLabel.GetDegAngle ());
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

void CSTWBackgroundColorsPage::OnChangeEditDeginlay() 
{
	if (m_bDegInlayChanged)
	{
		m_bDegInlayChanged = false;
		return;
	}

	if (!::IsWindow (m_EditDegInlay.GetSafeHwnd ()))
		return;

	CString s;
	m_EditDegInlay.GetWindowText (s);
	m_DegInlay.SetDegAngle (atoi (s) * 10);

	m_BkColInlay.SetAngle (m_DegInlay.GetDegAngle ());
	if (::IsWindow (m_BkColInlay.GetSafeHwnd ()))
		m_BkColInlay.InvalidateRect (NULL);

	((CStyleTemplateWizard*) GetParent ())->m_pTemplate->m_lgdBackground[Inlay].dAngle = deg2rad (m_DegInlay.GetDegAngle ());
	((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);
}

BOOL CSTWBackgroundColorsPage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}
