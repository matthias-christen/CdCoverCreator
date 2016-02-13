#if !defined(AFX_WEBCOVERSDIALOG_H__44369A2B_F637_4AD4_A3F8_4663ED576CC6__INCLUDED_)
#define AFX_WEBCOVERSDIALOG_H__44369A2B_F637_4AD4_A3F8_4663ED576CC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebCoversDialog.h : header file
//

#include "Picture.h"

/////////////////////////////////////////////////////////////////////////////
// CWebCoversDialog dialog

class CWebCoversDialog : public CDialog
{
// Construction
public:
	CWebCoversDialog(UINT nId, CWnd* pParent = NULL);   // standard constructor
	~CWebCoversDialog ();

	CStringArray m_arrFilenames;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebCoversDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;
	CFont m_font;
	CPicture* m_pPic;
	bool m_bCancelDownload;
	CWinThread* m_pSearchThread, *m_pImgsThread;

	// Generated message map functions
	//{{AFX_MSG(CWebCoversDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBCOVERSDIALOG_H__44369A2B_F637_4AD4_A3F8_4663ED576CC6__INCLUDED_)
