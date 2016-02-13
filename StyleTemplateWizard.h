// StyleTemplateWizard.h : header file
//
// This class defines custom modal property sheet 
// CStyleTemplateWizard.
 // CStyleTemplateWizard has been customized to include
// a preview window.
 
#ifndef __STYLETEMPLATEWIZARD_H__
#define __STYLETEMPLATEWIZARD_H__

#include "StyleTemplate.h"

#include "STWSelectTmplPage.h"
#include "STWTitleFontsPage.h"
#include "STWTrackFontsPage.h"
#include "STWBackgroundColorsPage.h"
#include "STWCoverTitleStylePage.h"
#include "STWLabelTitleStylePage.h"
#include "STWInlayTitleStylePage.h"
#include "STWTracksStylePage.h"
#include "STWBookletStylePage.h"

#include "STWPreviewWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CStyleTemplateWizard

class CStyleTemplateWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CStyleTemplateWizard)

// Construction
public:
	CStyleTemplateWizard(CWnd* pWndParent = NULL);

// Attributes
public:
	CSTWSelectTmplPage m_Page0;
	CSTWTitleFontsPage m_Page1;
	CSTWTrackFontsPage m_Page2;
	CSTWBackgroundColorsPage m_Page3;
	CSTWCoverTitleStylePage m_Page4;
	CSTWLabelTitleStylePage m_Page5;
	CSTWInlayTitleStylePage m_Page6;
	CSTWTracksStylePage m_Page7;
	CSTWBookletStylePage m_Page8;

	CSTWPreviewWnd m_wndPreview;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleTemplateWizard)
	public:
	virtual int DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_strCurrentStyleName;
	CStyleTemplate* m_pTemplate;
	virtual ~CStyleTemplateWizard();
		 virtual BOOL OnInitDialog();

// Generated message map functions
protected:
	//{{AFX_MSG(CStyleTemplateWizard)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __STYLETEMPLATEWIZARD_H__
