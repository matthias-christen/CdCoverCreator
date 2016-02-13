// CreationWizard.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CreationWizard.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreationWizard

IMPLEMENT_DYNAMIC(CCreationWizard, CPropertySheet)

CCreationWizard::CCreationWizard (NewDocMode mode, CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		m_pBackground[i] = NULL;
	m_pTracks = NULL;
	m_pDataCDInfo = NULL;
	m_pStyle = NULL;

	m_Page2a.m_nBkImageType = Cover;
	m_Page2b.m_nBkImageType = Back;
	m_Page2c.m_nBkImageType = Label;
	m_Page2d.m_nBkImageType = Inlay;

	switch (mode)
	{
	case AudioCD:
		AddPage (&m_Page1);
		break;
	case DataCD:
		AddPage (&m_PageDataCD);
		break;
	}

	AddPage (&m_Page2a);
	AddPage (&m_Page2b);
	AddPage (&m_Page2c);
	AddPage (&m_Page2d);
	AddPage (&m_Page3);

	SetWizardMode ();

	m_psh.dwFlags |= PSH_WIZARDHASFINISH;
    //m_psh.dwFlags |= PSH_WIZARD97 | /*PSH_WATERMARK |*/ PSH_HEADER | PSH_WIZARDHASFINISH;
    ////m_psh.pszbmWatermark = MAKEINTRESOURCE (IDB_WATERMARK);
    //m_psh.pszbmHeader = MAKEINTRESOURCE (IDB_BITMAP_AUDIOCD);
	//m_psh.hInstance = AfxGetInstanceHandle ();
}

CCreationWizard::~CCreationWizard()
{
}


BEGIN_MESSAGE_MAP(CCreationWizard, CPropertySheet)
	//{{AFX_MSG_MAP(CCreationWizard)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COMMANDHELP, OnCommandHelp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCreationWizard message handlers



CTracksInfo& CCreationWizard::GetTracksInfo()
{
	return *m_pTracks;
}

void CCreationWizard::SetTracksInfo(CTracksInfo &tracks)
{
	m_pTracks = &tracks;
	m_Page1.SetTracksInfo (tracks);
}

CDataCDInfo& CCreationWizard::GetDataCDInfo ()
{
	return *m_pDataCDInfo;
}

void CCreationWizard::SetDataCDInfo (CDataCDInfo& info)
{
	m_pDataCDInfo = &info;
	m_PageDataCD.SetDataCDInfo (info);
}

CBackgroundInfo& CCreationWizard::GetBackgroundInfo(RenderType rt)
{
	return *m_pBackground[rt];
}

CBackgroundInfo** CCreationWizard::GetBackgroundInfo()
{
	return m_pBackground;	
}

void CCreationWizard::SetBackgroundInfo(CBackgroundInfo &bkgrnd, RenderType rt)
{
	m_pBackground[rt] = &bkgrnd;

	switch (rt)
	{
	case Cover:
		m_Page2a.SetBackgroundInfo (bkgrnd);
		break;
	case Back:
		m_Page2b.SetBackgroundInfo (bkgrnd);
		break;
	case Label:
		m_Page2c.SetBackgroundInfo (bkgrnd);
		break;
	case Inlay:
		m_Page2d.SetBackgroundInfo (bkgrnd);
		break;
	}
}

CStyleTemplate& CCreationWizard::GetStyleTemplate()
{
	if (m_pStyle == NULL)
		return *(new CStyleTemplate ("Azur"));

	return *m_pStyle;
}

void CCreationWizard::SetStyleTemplate(CStyleTemplate &style)
{
	m_pStyle = &style;
	m_Page3.SetStyleTemplate (style);
}

LRESULT CCreationWizard::OnCommandHelp (WPARAM wParam, LPARAM lParam)
{
	HtmlHelp (/*AfxGetMainWnd ()->GetSafeHwnd (), AfxGetApp ()->m_pszHelpFilePath,*/
		HH_HELP_CONTEXT, HID_CREATIONWIZARD);
	return TRUE;
}

BOOL CCreationWizard::PreTranslateMessage (MSG* pMsg)
{
	// disable the ESC key on the Creation Wizard
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		pMsg->wParam = NULL;

	return CPropertySheet::PreTranslateMessage(pMsg);
}