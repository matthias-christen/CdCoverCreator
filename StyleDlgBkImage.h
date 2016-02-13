#if !defined(AFX_STYLEDLGBKIMAGE_H__C4BB0AC2_1339_4275_9293_F57BEDE80764__INCLUDED_)
#define AFX_STYLEDLGBKIMAGE_H__C4BB0AC2_1339_4275_9293_F57BEDE80764__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StyleDlgBkImage.h : header file
//

#include "StyleDlgBase.h"
#include "MouseOverButton.h"
#include "GradientStatic.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgBkImage dialog

class CStyleDlgBkImage : public CStyleDlgBase
{
// Construction
public:
	CStyleDlgBkImage(CWnd* pParent = NULL);   // standard constructor
	~CStyleDlgBkImage ();

	void SetBackground (CBackgroundInfo *pBk, RenderType rt);

// Dialog Data
	//{{AFX_DATA(CStyleDlgBkImage)
	enum { IDD = IDD_DIALOG_STYLEBKIMG };
	CMouseOverButton	m_btnScan;
	CMouseOverButton	m_btnRemoveTransparent;
	CMouseOverButton	m_btnSetTransparent;
	CGradientStatic	m_StaticTransparentCol;
	CMouseOverButton	m_BrowseBkImg;
	CString	m_strImgFilename;
	int		m_enumMode;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleDlgBkImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool m_bSettingTransparentColor;
	HCURSOR m_hPickCursor;
	CToolTipCtrl m_tooltip;
	COLORREF m_crTransparent;
	CBackgroundInfo* m_pBackgroundInfo;
	RenderType m_RenderType;
//	CPicture* m_pPic;

	HICON m_hIconBrowse, m_hIconScan;
	HICON m_hIconSetTransparent, m_hIconRemoveTransparent;
	HICON m_hIconTL, m_hIconTC, m_hIconTR, m_hIconCL, m_hIconCC, m_hIconCR, m_hIconBL, m_hIconBC, m_hIconBR;

	// Generated message map functions
	//{{AFX_MSG(CStyleDlgBkImage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonTransparentcolor();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonRemovetransparent();
	afx_msg void OnBrowseBkImg();
	afx_msg void OnScan();
	afx_msg void OnChangeEditBkimgfilename();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	void afx_msg OnAlignment (UINT nId);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAdjustColor();
	CButton m_btnAdjustColor;
	afx_msg void OnBnClickedButtonEditimage();
	afx_msg void OnBnClickedButtonReloadimage();
	CButton m_btnEditImage;
	CButton m_btnReloadImage;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDLGBKIMAGE_H__C4BB0AC2_1339_4275_9293_F57BEDE80764__INCLUDED_)
