// CreationWizard.h : header file
//
// This class defines custom modal property sheet 
// CCreationWizard.
 
#ifndef __CREATIONWIZARD_H__
#define __CREATIONWIZARD_H__

#include "CdCoverCreator2.h"
#include "CreationWizardPages.h"
#include "CWDataCDPage.h"
#include "TracksInfo.h"	// Added by ClassView
#include "DataCDInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CCreationWizard

class CCreationWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(CCreationWizard)

// Construction
public:
	CCreationWizard (NewDocMode mode, CWnd* pWndParent = NULL);

// Attributes
public:
	CCWArrangePage m_Page1;
	CCWBkImgPage m_Page2a, m_Page2b, m_Page2c, m_Page2d;
	CCWFormatPage m_Page3;

	CCWDataCDPage m_PageDataCD;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreationWizard)
	//}}AFX_VIRTUAL

// Implementation
public:
	CBackgroundInfo** GetBackgroundInfo();
	void SetStyleTemplate (CStyleTemplate& style);
	CStyleTemplate& GetStyleTemplate();
	void SetBackgroundInfo (CBackgroundInfo& bkgrnd, RenderType rt);
	CBackgroundInfo& GetBackgroundInfo(RenderType rt);
	void SetTracksInfo (CTracksInfo& tracks);
	CTracksInfo& GetTracksInfo();
	void SetDataCDInfo (CDataCDInfo& info);
	CDataCDInfo& GetDataCDInfo ();

	virtual ~CCreationWizard();

	CTracksInfo* m_pTracks;
	CDataCDInfo* m_pDataCDInfo;
	CBackgroundInfo* m_pBackground[NUM_RENDER_TYPES];
	CStyleTemplate* m_pStyle;

// Generated message map functions
protected:
	//{{AFX_MSG(CCreationWizard)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg LRESULT OnCommandHelp(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG *pMsg);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

#endif	// __CREATIONWIZARD_H__
