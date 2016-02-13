#if !defined(AFX_STYLEDLGALPHA_H__8DCCC7B8_B362_4FDC_8CE7_BEEB655F0A8D__INCLUDED_)
#define AFX_STYLEDLGALPHA_H__8DCCC7B8_B362_4FDC_8CE7_BEEB655F0A8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleDlgAlpha.h : header file
//

#include "StyleDlgBase.h"
#include "BackgroundAlphaTexture.h"
#include "MouseOverButton.h"

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgAlpha dialog

class CStyleDlgAlpha : public CStyleDlgBase
{
// Construction
public:
	void SetBackgroundAlpha (CBackgroundAlphaTexture* pBk);

	CStyleDlgAlpha(CWnd* pParent = NULL);   // standard constructor
	~CStyleDlgAlpha ();

// Dialog Data
	//{{AFX_DATA(CStyleDlgAlpha)
	enum { IDD = IDD_DIALOG_STYLEALPHA };
	CMouseOverButton	m_btnBrowse;
	CString	m_strFilename;
	int		m_nAlphaFormat;
	int		m_nSourceConstantAlpha;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleDlgAlpha)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBackgroundAlphaTexture* m_pBkAlpha;
	HICON m_hIconBrowse;

	// Generated message map functions
	//{{AFX_MSG(CStyleDlgAlpha)
	afx_msg void OnButtonBrowse();
	afx_msg void OnRadioAlphaformatDarken();
	afx_msg void OnRadioAlphaformatLighten();
	afx_msg void OnChangeEditFile();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDLGALPHA_H__8DCCC7B8_B362_4FDC_8CE7_BEEB655F0A8D__INCLUDED_)
