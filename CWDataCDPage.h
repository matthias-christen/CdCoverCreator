#pragma once
#include "afxcmn.h"
#include "DataCDInfo.h"
#include "AutoRichEditCtrl.h"

// CCWDataCDPage dialog

class CCWDataCDPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCWDataCDPage)

public:
	CCWDataCDPage();
	virtual ~CCWDataCDPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_CW_CREATEDATACD };

	void SetDataCDInfo (CDataCDInfo& info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonBrowse();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnWizardFinish();

	CString m_strBaseDir;
	CAutoRichEditCtrl m_editCDTitle;
	BOOL m_bShowFiles;
	int m_nNestingDepth;
	int m_nColumnsCount;

protected:
	void GetDataCDInfo ();

	CDataCDInfo* m_pDataCDInfo;
public:
	BOOL m_bUseSystemIcons;
};
