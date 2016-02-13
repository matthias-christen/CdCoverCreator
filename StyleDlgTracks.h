#if !defined(AFX_STYLEDLGTRACKS_H__DF240374_4053_4C41_BC97_9359C4EF1FCC__INCLUDED_)
#define AFX_STYLEDLGTRACKS_H__DF240374_4053_4C41_BC97_9359C4EF1FCC__INCLUDED_

#include "StyleTemplate.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleDlgTracks.h : header file
//

#include "StyleDlgBase.h"
#include "MouseOverButton.h"
#include "CJFlatComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgTracks dialog

class CStyleDlgTracks : public CStyleDlgBase
{
// Construction
public:
	void SetTemplate (CStyleTemplate* pStyle);

	CStyleDlgTracks(CWnd* pParent = NULL);   // standard constructor
	~CStyleDlgTracks ();

// Dialog Data
	//{{AFX_DATA(CStyleDlgTracks)
	enum { IDD = IDD_DIALOG_STYLETRACKS };
	CCJFlatComboBox	m_ctrlTrackFormatting;
	CMouseOverButton	m_btnTrackUp;
	CMouseOverButton	m_btnTrackDown;
	CCJFlatComboBox	m_ctrlTrackAlignment;
	CButton	m_ctrlTrackDisplay;
	CListBox	m_ListDisplay;
//	int		m_nTrackAlign;
	BOOL	m_bDisplayTotalTime;
//	BOOL	m_bTrackGrpNewLine;
	double	m_dTrackGrpSpacingAbove;
	double	m_dTrackGrpSpacingBelow;
	double	m_dTrackMarginLeft;
	double	m_dTrackMarginRight;
	double	m_dTrackInterItemSpacing;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleDlgTracks)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int GetPos (UINT nId);
	bool GetDisplay (UINT nId);
	int GetAlign (UINT nId);
	CString GetFormatting (UINT nId);
	void FillFormattingCombo (CString str);
	void UpdatePosition();
	CToolTipCtrl m_tooltip;
	CMapStringToString m_mapAlignment;
	CStyleTemplate* m_pTemplate;

	HICON m_hIconTrackUp, m_hIconTrackDown;

	// Generated message map functions
	//{{AFX_MSG(CStyleDlgTracks)
	afx_msg void OnSelchangeListDisplay();
	afx_msg void OnButtonTrackUp();
	afx_msg void OnButtonTrackDown();
	afx_msg void OnCheckTrackdisplay();
	afx_msg void OnSelchangeComboTracksalignment();
	afx_msg void OnEditchangeComboTrackformatting();
	afx_msg void OnSelchangeComboTrackformatting();
	afx_msg void OnCheckDisplaytotaltime();
	afx_msg void OnChangeEditTrackhMarginl();
	afx_msg void OnChangeEditTrackhMarginr();
	afx_msg void OnChangeEditTrackhInteritemspacing();
	afx_msg void OnChangeEditTrackgrpSpabove();
	afx_msg void OnChangeEditTrackgrpSpbelow();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusListDisplay();
	afx_msg void OnKillfocusListDisplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDLGTRACKS_H__DF240374_4053_4C41_BC97_9359C4EF1FCC__INCLUDED_)
