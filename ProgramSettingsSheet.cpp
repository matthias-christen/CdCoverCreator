// ProgramSettingsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ProgramSettingsSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgramSettingsSheet

IMPLEMENT_DYNAMIC(CProgramSettingsSheet, CPropertySheet)

CProgramSettingsSheet::CProgramSettingsSheet(CWnd* pWndParent)
	 : CPropertySheet(IDS_PROPSHT_CAPTION2, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;

	m_Page1.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page2.m_psp.dwFlags &= ~PSP_HASHELP;
	m_Page3.m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(&m_Page1);

/*
#ifndef PEROUTKA_FEDORISIN
	AddPage(&m_Page2);
#endif
*/

	AddPage (&m_Page3);
}

CProgramSettingsSheet::~CProgramSettingsSheet()
{
}


BEGIN_MESSAGE_MAP(CProgramSettingsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CProgramSettingsSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CProgramSettingsSheet message handlers


