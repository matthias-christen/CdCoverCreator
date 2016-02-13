// STWBookletStylePage.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "STWBookletStylePage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplates.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSTWBookletStylePage property page

IMPLEMENT_DYNCREATE(CSTWBookletStylePage, CPropertyPage)

CSTWBookletStylePage::CSTWBookletStylePage() : CPropertyPage(CSTWBookletStylePage::IDD)
{
	//{{AFX_DATA_INIT(CSTWBookletStylePage)
	m_bShowPageNums = FALSE;
	m_nPageNumAlignment = -1;
	m_dMarginBottom = 0.0;
	m_dMarginLeft = 0.0;
	m_dMarginTop = 0.0;
	m_dMarginRight = 0.0;
	//}}AFX_DATA_INIT
}

CSTWBookletStylePage::~CSTWBookletStylePage()
{
	::DestroyIcon (m_hIconFont);
}

void CSTWBookletStylePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWBookletStylePage)
	DDX_Control(pDX, IDC_BUTTON_FONT, m_btnFont);
	DDX_Control(pDX, IDC_BUTTON_COLOR, m_btnColor);
	DDX_Control(pDX, IDC_STATIC_FONT, m_stcPageNumFont);
	DDX_Check(pDX, IDC_CHECK_SHOWPAGENUMS, m_bShowPageNums);
	DDX_Radio(pDX, IDC_RADIO_PAGENUMALIGNOUTER, m_nPageNumAlignment);
	DDX_Text(pDX, IDC_EDIT_BOOKLETMARGBOTTOM, m_dMarginBottom);
	DDV_MinMaxDouble(pDX, m_dMarginBottom, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_BOOKLETMARGLEFT, m_dMarginLeft);
	DDV_MinMaxDouble(pDX, m_dMarginLeft, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_BOOKLETMARGTOP, m_dMarginTop);
	DDV_MinMaxDouble(pDX, m_dMarginTop, 0., 12.);
	DDX_Text(pDX, IDC_EDIT_BOOKLETMARGRIGHT, m_dMarginRight);
	DDV_MinMaxDouble(pDX, m_dMarginRight, 0., 12.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTWBookletStylePage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWBookletStylePage)
	ON_BN_CLICKED(IDC_CHECK_SHOWPAGENUMS, OnCheckShowpagenums)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_SELENDOK, OnButtonColor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTWBookletStylePage message handlers

void CSTWBookletStylePage::OnCheckShowpagenums() 
{
	UpdateData ();

	GetDlgItem (IDC_RADIO_PAGENUMALIGNOUTER)->EnableWindow (m_bShowPageNums);
	GetDlgItem (IDC_RADIO_PAGENUMALIGNCENTER)->EnableWindow (m_bShowPageNums);
	GetDlgItem (IDC_RADIO_PAGENUMALIGNINNER)->EnableWindow (m_bShowPageNums);

	m_btnFont.EnableWindow (m_bShowPageNums);
	m_btnColor.EnableWindow (m_bShowPageNums);
}

void CSTWBookletStylePage::OnButtonFont() 
{
	CFontDialog dlg (m_stcPageNumFont.GetLogfont (), CF_EFFECTS | CF_BOTH, NULL, this);
	dlg.m_cf.rgbColors = m_stcPageNumFont.GetColor ();

	if (dlg.DoModal () == IDOK)
		m_stcPageNumFont.SetPreviewFont (&dlg.m_cf);
}

LRESULT CSTWBookletStylePage::OnButtonColor(WPARAM wParam, LPARAM lParam) 
{
	m_stcPageNumFont.SetColor (m_btnColor.GetColour ());
	return S_OK;
}

BOOL CSTWBookletStylePage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_BACK);
	
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;

	m_dMarginLeft = pTmp->GetBookletMarginLeft ();
	m_dMarginTop = pTmp->GetBookletMarginTop ();
	m_dMarginRight = pTmp->GetBookletMarginRight ();
	m_dMarginBottom = pTmp->GetBookletMarginBottom ();

	m_bShowPageNums = pTmp->ShowBookletPageNums ();
	m_nPageNumAlignment = pTmp->GetBookletPageNumAlign ();

	m_stcPageNumFont.SetPreviewFont (pTmp->GetBookletPageNumFont ());
	m_stcPageNumFont.SetColor (pTmp->GetBookletPageNumColor ());
	m_btnColor.SetColour (pTmp->GetBookletPageNumColor ());

	UpdateData (false);
	
	OnCheckShowpagenums ();

	return CPropertyPage::OnSetActive();
}

BOOL CSTWBookletStylePage::OnKillActive() 
{
	SetData ();	
	return CPropertyPage::OnKillActive();
}

BOOL CSTWBookletStylePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CString str;
	str.LoadString (IDS_COL_DEFAULT);
	m_btnColor.SetDefaultText (str);
	str.LoadString (IDS_COL_CUSTOM);
	m_btnColor.SetCustomText (str);
	m_btnFont.SetIcon (m_hIconFont = AfxGetApp ()->LoadIcon (IDI_ICON_FONT));

	return TRUE;
}

BOOL CSTWBookletStylePage::OnWizardFinish() 
{
	SetData ();	
	return CPropertyPage::OnWizardFinish();
}

void CSTWBookletStylePage::SetData()
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
	
	CStyleTemplate* pTmp = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;

	UpdateData ();

	pTmp->m_dBookletMarginLeft = m_dMarginLeft;
	pTmp->m_dBookletMarginTop = m_dMarginTop;
	pTmp->m_dBookletMarginRight = m_dMarginRight;
	pTmp->m_dBookletMarginBottom = m_dMarginBottom;

	pTmp->m_bBookletShowPageNums = m_bShowPageNums ? true : false;
	pTmp->m_nBookletPageNumAlign = m_nPageNumAlignment;

	memcpy (&pTmp->m_lfBookletPageNumFont, m_stcPageNumFont.GetLogfont (), sizeof (LOGFONT));
	pTmp->m_crBookletPageNumColor = m_stcPageNumFont.GetColor ();
}
