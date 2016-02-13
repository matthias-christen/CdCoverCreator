// ChildFrm.h : interface of the CChildFrame class
//


#pragma once


class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnWindowMinimize ();
	afx_msg void OnWindowRestore ();
	afx_msg void OnWindowClose ();
	afx_msg void OnUpdateWindowMinimize (CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowRestore (CCmdUI* pCmdUI);
	afx_msg void OnUpdateWindowClose (CCmdUI* pCmdUI);
};
