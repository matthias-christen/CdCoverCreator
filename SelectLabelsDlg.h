#pragma once
#include "selectlabelstatic.h"

#ifdef PEROUTKA_FEDORISIN

// CSelectLabelsDlg dialog

class CSelectLabelsDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectLabelsDlg)

public:
	CSelectLabelsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectLabelsDlg();

	void SetPreview (CWnd* pPreviewWnd);
	bool PrintLabel (int nIdx);

// Dialog Data
	enum { IDD = IDD_DIALOG_SELECTLABELS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnLabelSelected ();
	DECLARE_MESSAGE_MAP()

public:
	CSelectLabelStatic m_LabelSelector;
	CWnd* m_pPreviewWnd;
};

#endif