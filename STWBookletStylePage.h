#if !defined(AFX_STWBOOKLETSTYLEPAGE_H__884E26D2_D481_451E_9ECA_D2F83B816F44__INCLUDED_)
#define AFX_STWBOOKLETSTYLEPAGE_H__884E26D2_D481_451E_9ECA_D2F83B816F44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STWBookletStylePage.h : header file
//

#include "ColourPicker.h"
#include "MouseOverButton.h"
#include "FontPreviewStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CSTWBookletStylePage dialog

class CSTWBookletStylePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSTWBookletStylePage)

// Construction
public:
	CSTWBookletStylePage();
	~CSTWBookletStylePage();

// Dialog Data
	//{{AFX_DATA(CSTWBookletStylePage)
	enum { IDD = IDD_PROPPAGE_STW_BOOKLET };
	CMouseOverButton	m_btnFont;
	CColourPicker	m_btnColor;
	CFontPreviewStatic	m_stcPageNumFont;
	BOOL	m_bShowPageNums;
	int		m_nPageNumAlignment;
	double	m_dMarginBottom;
	double	m_dMarginLeft;
	double	m_dMarginTop;
	double	m_dMarginRight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSTWBookletStylePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetData();
	// Generated message map functions
	//{{AFX_MSG(CSTWBookletStylePage)
	afx_msg void OnCheckShowpagenums();
	afx_msg void OnButtonFont();
	afx_msg LRESULT OnButtonColor(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	HICON m_hIconFont;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STWBOOKLETSTYLEPAGE_H__884E26D2_D481_451E_9ECA_D2F83B816F44__INCLUDED_)
