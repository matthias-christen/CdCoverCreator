#if !defined(AFX_PAGESETUPDLG_H__3A5F03C9_1AA0_42A7_A5C0_AA9539052457__INCLUDED_)
#define AFX_PAGESETUPDLG_H__3A5F03C9_1AA0_42A7_A5C0_AA9539052457__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSetupDlg.h : header file
//

#include "CJFlatComboBox.h"
#include "MouseOverButton.h"

#include "pugxml.h"

/////////////////////////////////////////////////////////////////////////////
// CPageSetupDlg dialog

class CPageSetupDlg : public CDialog
{
// Construction
public:
	CPageSetupDlg(CWnd* pParent = NULL);   // standard constructor
	~CPageSetupDlg ();

// Dialog Data
	//{{AFX_DATA(CPageSetupDlg)
	enum {
#ifdef PEROUTKA_FEDORISIN
		IDD = IDD_PAGESETUPDLG_PF
#else
		IDD = IDD_PAGESETUPDLG
#endif
	};

#ifndef PEROUTKA_FEDORISIN
	CMouseOverButton	m_btnFunPrint;
	CMouseOverButton	m_btnPrintPower;
#endif

	CCJFlatComboBox	m_ComboCoverbackLabelType;
	CCJFlatComboBox	m_ComboCoverLabelType;
	CCJFlatComboBox	m_ComboInlayLabelType;
	CCJFlatComboBox	m_ComboLabelLabelType;
	CCJFlatComboBox	m_ComboPrintInlay;
	CCJFlatComboBox	m_ComboPrintCoverback;
	double	m_dCoverBackMarginLeft;
	double	m_dCoverBackMarginTop;
	double	m_dCoverMarginLeft;
	double	m_dCoverMarginTop;
	double	m_dInlayMarginLeft;
	double	m_dInlayMarginTop;
	double	m_dLabelMarginLeft;
	double	m_dLabelMarginTop;
	int		m_nCoverPO;
	int		m_nCoverbackPO;
	int		m_nInlayPO;
	int		m_nLabelPO;
	BOOL	m_b2Labels;
	double	m_dBookletMarginLeft;
	double	m_dBookletMarginTop;
	int		m_nBookletPO;
	int m_nIncludeCutLines;
	//}}AFX_DATA

	CString m_strCoverLabelType;
	CString m_strCoverbackLabelType;
	CString m_strLabelLabelType;
	CString m_strInlayLabelType;

	RenderType m_rtRenderCoverbackWith;
	RenderType m_rtRenderInlayWith;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadMeasures (CComboBox* pCombo, UINT nId);
	void LoadMeasures (CComboBox* pCombo, pug::xml_node& nodeType);

	// Generated message map functions
	//{{AFX_MSG(CPageSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeComboCoverlabeltype();
	afx_msg void OnSelchangeComboCoverbacklabeltype();
	afx_msg void OnSelchangeComboLabellabeltype();
	afx_msg void OnSelchangeComboInlaylabeltype();
	virtual void OnOK();
	afx_msg void OnSelchangeComboPrintcoverbacktogether();
	afx_msg void OnSelchangeComboPrintinlaytogether();
	afx_msg void OnButtonPrintpower();
	afx_msg void OnButtonFunprint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
	HINSTANCE GotoURL (LPCTSTR url, int showcmd);

	HICON m_hIconPrintPower, m_hIconFunPrint;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESETUPDLG_H__3A5F03C9_1AA0_42A7_A5C0_AA9539052457__INCLUDED_)
