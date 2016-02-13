#if !defined(AFX_STYLEDLGBASE_H__BB497C8F_1F4D_48F7_8EA6_0E5E224361B5__INCLUDED_)
#define AFX_STYLEDLGBASE_H__BB497C8F_1F4D_48F7_8EA6_0E5E224361B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleDlgBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgBase dialog

class CStyleDlgBase : public CDialog
{
// Construction
public:
	CStyleDlgBase(UINT uid, CWnd* pParent = NULL);   // standard constructor

// Implementation
protected:
	void UpdateDoc (UINT nId = 0);

	// Generated message map functions
	//{{AFX_MSG(CStyleDlgBase)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDLGBASE_H__BB497C8F_1F4D_48F7_8EA6_0E5E224361B5__INCLUDED_)
