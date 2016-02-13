// STWTrackStylePage.h : header file 
//

#ifndef __STWTRACKSSTYLEPAGE_H__
#define __STWTRACKSSTYLEPAGE_H__

#include "MouseOverButton.h"
#include "CJFlatComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSTWTracksStylePage dialog

class CSTWTracksStylePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSTWTracksStylePage)

// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CSTWTracksStylePage();
	~CSTWTracksStylePage();

	int GetAlign (UINT nId);
	bool GetDisplay (UINT nId);
	int GetPos (UINT nId);
	CString GetFormatting(UINT nId);

// Dialog Data
	//{{AFX_DATA(CSTWTracksStylePage)
	enum { IDD = IDD_PROPPAGE_STW_TRACKS };
	CStatic	m_ctrlTrackFormattingSample;
	CCJFlatComboBox	m_ctrlTrackFormatting;
	CMouseOverButton	m_btnTrackUp;
	CMouseOverButton	m_btnTrackDown;
	CCJFlatComboBox	m_ctrlTrackAlignment;
	CButton	m_ctrlTrackDisplay;
	CListBox	m_ListDisplay;
	int		m_nTrackAlign;
	BOOL	m_bDisplayTotalTime;
	BOOL	m_bTrackGrpNewLine;
	double	m_dTrackGrpSpacingAbove;
	double	m_dTrackGrpSpacingBelow;
	double	m_dTrackMarginLeft;
	double	m_dTrackMarginRight;
	double	m_dTrackInterItemSpacing;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSTWTracksStylePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnWizardFinish();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	void FillFormattingCombo(CString str);
	void UpdatePosition();
	CMapStringToString m_mapAlignment;

	HICON m_hIconTrackUp, m_hIconTrackDown;

	// Generated message map functions
	//{{AFX_MSG(CSTWTracksStylePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListDisplay();
	afx_msg void OnButtonTrackUp();
	afx_msg void OnButtonTrackDown();
	afx_msg void OnCheckTrackdisplay();
	afx_msg void OnSelchangeComboTracksalignment();
	afx_msg void OnEditchangeComboTrackformatting();
	afx_msg void OnSelchangeComboTrackformatting();
	afx_msg void OnCheckDisplaytotaltime();
	afx_msg void OnCheckTrackgrpNewline();
	afx_msg void OnRadioTrackalign();
	afx_msg void OnRadioTrackFloating();
	afx_msg void OnChangeEditTrackhMarginl();
	afx_msg void OnChangeEditTrackhMarginr();
	afx_msg void OnChangeEditTrackhInteritemspacing();
	afx_msg void OnChangeEditTrackgrpSpabove();
	afx_msg void OnChangeEditTrackgrpSpbelow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // __STWTRACKSSTYLEPAGE_H__
