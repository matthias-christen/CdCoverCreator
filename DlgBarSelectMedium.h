#pragma once

#include "StyleTemplate.h"

// CDlgBarSelectMedium dialog

class CDlgBarSelectMedium : public CDialogBar
{
	DECLARE_DYNAMIC(CDlgBarSelectMedium)

public:
	CDlgBarSelectMedium(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBarSelectMedium();

// Dialog Data
	enum { IDD = IDD_DIALOGBAR_MEDIUM };

	void SetMediumType (MediumType type);
	MediumType GetMediumType ();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboMedium();
};
