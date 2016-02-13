#pragma once

#include "StyleDlgBase.h"
#include "StyleTemplate.h"

// CStyleDlgSpines dialog

class CStyleDlgSpines : public CStyleDlgBase
{
	DECLARE_DYNAMIC(CStyleDlgSpines)

public:
	CStyleDlgSpines(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStyleDlgSpines();

	void SetTemplate (CStyleTemplate* pStyle, RenderType rt);
	void EnableTitleAlignmentEdit();

	// Dialog Data
	enum { IDD = IDD_DIALOG_STYLESPINES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnRadioAlignCenter();
	afx_msg void OnRadioAlignTop();
	afx_msg void OnRadioAlignBottom();
	afx_msg void OnChangeEditAlignTop();
	afx_msg void OnChangeEditAlignBottom();
	afx_msg void OnCheckTitleShow();
	afx_msg void OnRadioTextDirUp();
	afx_msg void OnRadioTextDirDown();
	afx_msg void OnCheckAntiparallel ();
	DECLARE_MESSAGE_MAP()

	double m_dTitleVAlign;

	CStyleTemplate* m_pStyle;
	RenderType m_RenderType;
public:
	BOOL m_bShowTitle;
	int m_nVerticalAlign;
	double m_dMarginTop;
	double m_dMarginBottom;
	int m_nTextDirection;
	BOOL m_bInlaySideAntiparallel;
};
