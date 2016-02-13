// ProgramSettingsPages.h : header file
//

#ifndef __PROGRAMSETTINGSPAGES_H__
#define __PROGRAMSETTINGSPAGES_H__

#include "CdCoverCreator2.h"

typedef struct tagCheckLibsParam
{
	CCheckListBox* pCoversList, *pLyricsList;
	HWND hwndProgress;
	bool* pbCancelDownload;
	int* pnError;
} CHECKLIBSPARAM;

UINT CheckNewLibs (LPVOID lpParam);
LPSTR ParseHosts(LPSTR lpszBuf, HOSTMAP *pMap, CString strKey, CString strExtension, CHECKLIBSPARAM* pParam);
void AddLibs(CCheckListBox* pList, CString strSuffix, HOSTMAP* pMap);

/////////////////////////////////////////////////////////////////////////////
// CPSHosts dialog

class CPSHosts : public CPropertyPage
{
	DECLARE_DYNCREATE(CPSHosts)

// Construction
public:
	CPSHosts();
	~CPSHosts();

// Dialog Data
	//{{AFX_DATA(CPSHosts)
	enum { IDD = IDD_PROPPAGE_PS_HOSTS };
	CProgressCtrl	m_Progress;
	CCheckListBox	m_ListLyricsHosts;
	CCheckListBox	m_ListCoverHosts;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPSHosts)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nError;
	bool m_bCancelDownload;
	void SetHosts (CString strKey, CCheckListBox* pList, HOSTMAP* pMap);

	// Generated message map functions
	//{{AFX_MSG(CPSHosts)
	afx_msg void OnButtonChecknewlibs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// CPSCheckVersion dialog

class CPSCheckVersion : public CPropertyPage
{
	DECLARE_DYNCREATE(CPSCheckVersion)

// Construction
public:
	CPSCheckVersion();
	~CPSCheckVersion();

// Dialog Data
	//{{AFX_DATA(CPSCheckVersion)
	enum { IDD = IDD_PROPPAGE_PS_VERSIONCHECKING };
	BOOL	m_bDoCheckVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPSCheckVersion)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPSCheckVersion)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // __PROGRAMSETTINGSPAGES_H__
