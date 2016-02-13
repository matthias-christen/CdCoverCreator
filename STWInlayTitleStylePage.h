// STWInlayTitleStylePage.h : header file 
//

#ifndef __STWINLAYTITLESTYLEPAGE_H__
#define __STWINLAYTITLESTYLEPAGE_H__

/////////////////////////////////////////////////////////////////////////////
// CSTWInlayTitleStylePage dialog

class CSTWInlayTitleStylePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSTWInlayTitleStylePage)

// Construction
public:
	double GetHTitleMargin();
	CSTWInlayTitleStylePage();
	~CSTWInlayTitleStylePage();

// Dialog Data
	//{{AFX_DATA(CSTWInlayTitleStylePage)
	enum { IDD = IDD_PROPPAGE_STW_INLAY };
	CEdit	m_EditSideVAlignT;
	CEdit	m_EditSideVAlignB;
	CEdit	m_EditTitleHAlignR;
	CEdit	m_EditTitleHAlignL;
	int		m_nSidebarVAlign;
	int		m_nTitleHAlign;
	double	m_dTitleHeight;
	double	m_dTop;
	BOOL	m_bShowSidebarTitles;
	BOOL	m_bShowTitles;
	BOOL	m_bInlaySideAntiparallel;
	//}}AFX_DATA

	double m_dTitleHAlign;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSTWInlayTitleStylePage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetSidebarVAlign();
	void EnableAlignmentEdit();
	// Generated message map functions
	//{{AFX_MSG(CSTWInlayTitleStylePage)
	afx_msg void OnRadioTitlehalign1();
	afx_msg void OnRadioTitlehalign2();
	afx_msg void OnRadioTitlehalign3();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditTitleHalignl();
	afx_msg void OnChangeEditTitleHalignr();
	afx_msg void OnChangeEditTitleHeight();
	afx_msg void OnChangeEditTitleTop();
	afx_msg void OnRadioSidebarValign1();
	afx_msg void OnRadioSidebarValign2();
	afx_msg void OnRadioSidebarValign3();
	afx_msg void OnChangeEditSideValignb();
	afx_msg void OnChangeEditSideValignt();
	afx_msg void OnCheckInlayShowtitles();
	afx_msg void OnCheckInlayShowsidebars();
	//}}AFX_MSG
	afx_msg void OnRadioSpineUp ();
	afx_msg void OnRadioSpineDown ();
	afx_msg void OnCheckAntiparallel ();
	DECLARE_MESSAGE_MAP()

public:
	int m_nSpineTextDirection;
};

#endif // __STWINLAYTITLESTYLEPAGE_H__
