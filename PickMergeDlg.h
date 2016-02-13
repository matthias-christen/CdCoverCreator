#if !defined(AFX_PICKMERGEDLG_H__F196AAED_8E16_4E9C_8E83_77B8468CD4EA__INCLUDED_)
#define AFX_PICKMERGEDLG_H__F196AAED_8E16_4E9C_8E83_77B8468CD4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PickMergeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPickMergeDlg dialog

class CPickMergeDlg : public CDialog
{
// Construction
public:
	CObArray m_arrTrackInfo;
	CPickMergeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPickMergeDlg)
	enum { IDD = IDD_DIALOG_MATCHMERGE };
	CListBox	m_List;
	BOOL	m_DontDeleteExisting;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPickMergeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPickMergeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICKMERGEDLG_H__F196AAED_8E16_4E9C_8E83_77B8468CD4EA__INCLUDED_)
