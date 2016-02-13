#pragma once

#include "SelectLabelStatic.h"


// CSelectLabelPrintDialog

class CSelectLabelPrintDialog : public CPrintDialog
{
	DECLARE_DYNAMIC(CSelectLabelPrintDialog)

public:
	CSelectLabelPrintDialog(BOOL bPrintSetupOnly, CSelectLabelStatic* pSelectLabelReference,
			// TRUE for Print Setup, FALSE for Print Dialog
			DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS
				| PD_HIDEPRINTTOFILE | PD_NOSELECTION,
			CWnd* pParentWnd = NULL);
	virtual ~CSelectLabelPrintDialog();

void DoDataExchange(CDataExchange* pDX);

protected:
	afx_msg void OnLabelSelected ();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

	CSelectLabelStatic m_LabelSelector;
	CSelectLabelStatic* m_pLabelSelectorReference;	// reference (the static in the dialog shown whilst previewing)
};


