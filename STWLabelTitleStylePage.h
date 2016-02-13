#if !defined(AFX_STWLABELTITLESTYLEPAGE_H__D873E1B9_1573_456B_A153_C5C8ADC8EAA9__INCLUDED_)
#define AFX_STWLABELTITLESTYLEPAGE_H__D873E1B9_1573_456B_A153_C5C8ADC8EAA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STWLabelTitleStylePage.h : header file
//

#include "CJFlatComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSTWLabelTitleStylePage dialog

class CSTWLabelTitleStylePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSTWLabelTitleStylePage)

// Construction
public:
	CSTWLabelTitleStylePage();
	~CSTWLabelTitleStylePage();

// Dialog Data
	//{{AFX_DATA(CSTWLabelTitleStylePage)
	enum { IDD = IDD_PROPPAGE_STW_LABEL };
	CCJFlatComboBox	m_ComboTracksCenter;
	CCJFlatComboBox	m_ComboTitleCenter;
	CEdit	m_TitleVAlignBottom;
	CEdit	m_TrackVAlignTop;
	CEdit	m_TrackVAlignBottom;
	CEdit	m_TitleVAlignTop;
	BOOL	m_bDisplayTrackgroups;
	BOOL	m_bDisplayTracks;
	int		m_nRoundTitle;
	int		m_nRoundTracks;
	int		m_nTitleVAlign;
	int		m_nTrackVAlign;
	int		m_nTitleCenterWhere;
	int		m_nTrackCenterWhere;
	BOOL	m_bShowTitles;
	//}}AFX_DATA

	double m_dTracksVAlign;
	double m_dTitleVAlign;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSTWLabelTitleStylePage)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableTrackgroupSettings();
	void EnableTitleVAlignmentEdit();
	void EnableTrackVAlignmentEdit();

	double GetVTracksMargin();
	double GetVTitleMargin();
	// Generated message map functions
	//{{AFX_MSG(CSTWLabelTitleStylePage)
	afx_msg void OnRadioLabelTitlevalgntop();
	afx_msg void OnRadioLabelTitlevalignbottom();
	afx_msg void OnRadioLabelTitlevaligncenter();
	afx_msg void OnRadioLabelTrackvalignbottom();
	afx_msg void OnRadioLabelTrackvaligncenter();
	afx_msg void OnRadioLabelTrackvaligntop();
	afx_msg void OnChangeEditLabelTitlevalignbottom();
	afx_msg void OnChangeEditLabelTitlevaligntop();
	afx_msg void OnChangeEditLabelTrackvalignbottom();
	afx_msg void OnChangeEditLabelTrackvaligntop();
	afx_msg void OnRadioLabelTrackround();
	afx_msg void OnRadioLabelTrackstraight();
	afx_msg void OnRadioLabelTitleround();
	afx_msg void OnRadioLabelTitlestraight();
	afx_msg void OnCheckLabelTrackgroups();
	afx_msg void OnCheckLabelTracks();
	afx_msg void OnSelchangeComboLabelTitlecenter();
	afx_msg void OnSelchangeComboLabelTrackcenter();
	afx_msg void OnCheckLabelShowtitles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STWLABELTITLESTYLEPAGE_H__D873E1B9_1573_456B_A153_C5C8ADC8EAA9__INCLUDED_)
