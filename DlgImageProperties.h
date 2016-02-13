#pragma once
#include "CdCoverCreator2Doc.h"
#include "Picture.h"

// CDlgImageProperties dialog

class CDlgImageProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgImageProperties)

public:
	CDlgImageProperties(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgImageProperties();

	void SetPicture (CPicture* pPicture);

// Dialog Data
	enum { IDD = IDD_DIALOG_IMAGEPROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void UpdateDisplay ();

	DECLARE_MESSAGE_MAP()

public:
	int m_nContrast;
	int m_nBrightness;
	int m_nSaturation;
	int m_nHue;
	int m_nGamma;

	CCdCoverCreator2Doc* m_pDocument;
	CPicture* m_pPicture;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonReset();
	virtual BOOL OnInitDialog();

protected:
	double m_fOldContrast;
	double m_fOldBrightness;
	double m_fOldSaturation;
	double m_fOldHue;
	double m_fOldGamma;
	virtual void OnCancel();
};
