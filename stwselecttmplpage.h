// STWSelectTmplPage.h : header file 
//

#include "CJFlatComboBox.h"

#ifndef __STWSELECTTMPLPAGE_H__
#define __STWSELECTTMPLPAGE_H__

/////////////////////////////////////////////////////////////////////////////
// CSTWSelectTmplPage dialog

class CSTWSelectTmplPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSTWSelectTmplPage)

// Construction
public:
	CString GetTemplateName();
	CSTWSelectTmplPage();
	~CSTWSelectTmplPage();

// Dialog Data
	//{{AFX_DATA(CSTWSelectTmplPage)
	enum { IDD = IDD_PROPPAGE_STW_SELECTTEMPLATE };
	CCJFlatComboBox	m_Combo;
	BOOL	m_bCreateNewTemplate;
	CString	m_strNewTemplateName;
	CString	m_strTemplateName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSTWSelectTmplPage)
	public:
	virtual LRESULT OnWizardNext();
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool CreateTemplate();

	// Generated message map functions
	//{{AFX_MSG(CSTWSelectTmplPage)
	afx_msg void OnCheckCreatenewtemplate();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboTemplate();
	afx_msg void OnButtonDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // __STWSELECTTMPLPAGE_H__
