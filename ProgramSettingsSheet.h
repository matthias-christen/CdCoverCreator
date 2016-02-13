// ProgramSettingsSheet.h : header file
//
// This class defines custom modal property sheet 
// CProgramSettingsSheet.
 
#ifndef __PROGRAMSETTINGSSHEET_H__
#define __PROGRAMSETTINGSSHEET_H__

#include "ProgramSettingsPages.h"
#include "PSImageEditing.h"

/////////////////////////////////////////////////////////////////////////////
// CProgramSettingsSheet

class CProgramSettingsSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CProgramSettingsSheet)

// Construction
public:
	CProgramSettingsSheet(CWnd* pWndParent = NULL);

// Attributes
public:
	CPSHosts m_Page1;
	CPSCheckVersion m_Page2;
	CPSImageEditing m_Page3;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgramSettingsSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CProgramSettingsSheet();

// Generated message map functions
protected:
	//{{AFX_MSG(CProgramSettingsSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __PROGRAMSETTINGSSHEET_H__
