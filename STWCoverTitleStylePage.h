#if !defined(AFX_STWCOVERTITLESTYLEPAGE_H__4427F255_FC24_4E3F_A284_96FB42B30220__INCLUDED_)
#define AFX_STWCOVERTITLESTYLEPAGE_H__4427F255_FC24_4E3F_A284_96FB42B30220__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// STWCoverTitleStylePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSTWCoverTitleStylePage dialog

class CSTWCoverTitleStylePage : public CPropertyPage
{
// Construction
public:
	double GetVTracksMargin();
	double GetHTracksMargin();
	double m_dTitleHAlign;
	double GetHTitleMargin();
	void EnableTrackgroupHAlignmentEdit();
	void EnableTrackgroupVAlignmentEdit();
	void EnableTitleAlignmentEdit();
	CSTWCoverTitleStylePage ();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSTWCoverTitleStylePage)
	enum { IDD = IDD_PROPPAGE_STW_COVER };
	CEdit	m_EditCoverTrackgroupMarginT;
	CEdit	m_EditCoverTrackgroupMarginR;
	CEdit	m_EditCoverTrackgroupMarginL;
	CEdit	m_EditCoverTrackgroupMarginB;
	CEdit	m_EditCoverTitleHAlignR;
	CEdit	m_EditCoverTitleHAlignL;
	int		m_nTitleMargin;
	int		m_nTrackgroupVMargin;
	int		m_nTrackgroupHMargin;
	BOOL	m_bShowTrackgroups;
	double	m_dTitleVMarginTop;
	double	m_dTitleVHeight;
	BOOL	m_bShowTitles;
	//}}AFX_DATA

	double m_dTitleVAlign;
	double m_dTracksHAlign, m_dTracksVAlign;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSTWCoverTitleStylePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSTWCoverTitleStylePage)
	afx_msg void OnRadioCoverTitlemargincenter();
	afx_msg void OnRadioCoverTitlemarginleft();
	afx_msg void OnRadioCoverTitlemarginright();
	afx_msg void OnRadioTitleTghcentered();
	afx_msg void OnRadioTitleTgmarginbottom();
	afx_msg void OnRadioTitleTgmarginleft();
	afx_msg void OnRadioTitleTgmarginright();
	afx_msg void OnRadioTitleTgmargintop();
	afx_msg void OnRadioTitleTgvcenter();
	afx_msg void OnChangeEditCoverTitlehalignleft();
	afx_msg void OnChangeEditCoverTitlehalignright();
	afx_msg void OnChangeEditCoverTitlevalignheight();
	afx_msg void OnChangeEditCoverTitlevaligntop();
	afx_msg void OnChangeEditTitleTgmarginbottom();
	afx_msg void OnChangeEditTitleTgmarginleft();
	afx_msg void OnChangeEditTitleTgmarginright();
	afx_msg void OnChangeEditTitleTgmargintop();
	afx_msg void OnCheckCoverTitleTgshow();
	afx_msg void OnCheckCoverTitleShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STWCOVERTITLESTYLEPAGE_H__4427F255_FC24_4E3F_A284_96FB42B30220__INCLUDED_)
