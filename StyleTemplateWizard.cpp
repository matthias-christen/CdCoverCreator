// StyleTemplateWizard.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "resource.h"
#include "StyleTemplateWizard.h"

#include "StyleTemplates.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleTemplateWizard

IMPLEMENT_DYNAMIC(CStyleTemplateWizard, CPropertySheet)

CStyleTemplateWizard::CStyleTemplateWizard(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION1, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	m_pTemplate = NULL;

	AddPage(&m_Page0);
	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_Page5);
	AddPage(&m_Page6);
	AddPage(&m_Page7);
	AddPage(&m_Page8);

	SetWizardMode();
	m_psh.dwFlags |= PSH_WIZARDHASFINISH;
}

CStyleTemplateWizard::~CStyleTemplateWizard()
{
	if (m_pTemplate != NULL)
		delete m_pTemplate;
}


BEGIN_MESSAGE_MAP(CStyleTemplateWizard, CPropertySheet)
	//{{AFX_MSG_MAP(CStyleTemplateWizard)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CStyleTemplateWizard message handlers

BOOL CStyleTemplateWizard::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// add a preview window to the property sheet.
	CRect rectWnd;
	GetWindowRect (rectWnd);
	SetWindowPos (NULL, 0, 0, rectWnd.Width () + 180, rectWnd.Height (), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	CRect rectPreview (rectWnd.Width (), 20, rectWnd.Width () + 160, rectWnd.Height () - 35);

	m_wndPreview.CreateEx (WS_EX_STATICEDGE, NULL, NULL, WS_CHILD | WS_VISIBLE, rectPreview, this, 0x1000);
	m_wndPreview.SetParent (this);

	CenterWindow ();
	return bResult;
}

int CStyleTemplateWizard::DoModal() 
{
	int nRet = CPropertySheet::DoModal ();

	if (nRet == ID_WIZFINISH)
		((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ().StoreTemplate (*m_pTemplate);

	return nRet;
}

LRESULT CStyleTemplateWizard::OnCommandHelp (WPARAM wParam, LPARAM lParam)
{
	HtmlHelp (/*AfxGetMainWnd ()->GetSafeHwnd (), AfxGetApp ()->m_pszHelpFilePath,*/
		HH_HELP_CONTEXT, HID_STYLEWIZARD);
	return TRUE;
}
