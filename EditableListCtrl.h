#if !defined(AFX_EDITABLELISTCTRL_H__A3F56ED5_F876_4791_9D18_E31B4CCB8E35__INCLUDED_)
#define AFX_EDITABLELISTCTRL_H__A3F56ED5_F876_4791_9D18_E31B4CCB8E35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditableListCtrl.h : header file
//

#define LVN_ENDDRAG (WM_USER + 1)

// Editor
class CELCEdit : public CEdit
{
// Construction
public:
	CELCEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CELCEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CELCEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CELCEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

public:
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);

protected:
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl window

class CEditableListCtrl : public CListCtrl
{
// Construction
public:
	CEditableListCtrl();

// Attributes
public:

// Operations
public:
	void SetEditText (CString str);
	void SetFixedCol (int nCol);
	void SetFixedRows (bool bIsFixedRows);

	void MoveEdit (int nUp, int nRight);
	void SaveEdit();

	void SetEditPos(int x, int y);
	CPoint GetEditPos();

	void BeginDrag(NMLISTVIEW* pNmlv);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditableListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetDefaultIcon (int nDefaultIcon);
	virtual ~CEditableListCtrl();

	// Generated message map functions
protected:
	int m_nDefaultIcon;
	bool m_bInit;
	int m_nCurEditX, m_nCurEditY;

	CELCEdit m_Edit;
	CELCEdit& GetEdit();

	int GetNextRow (int nRow, bool bForeward = true);
	bool m_bIsFixedRows;
	int GetNextEditableCol (int nCol, bool bForeward = true);
	CMapWordToPtr m_mapFixedCol;

	CImageList* m_pDragImageList;
	bool m_bIsDragging;
	int m_nDragItem;

	//{{AFX_MSG(CEditableListCtrl)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITABLELISTCTRL_H__A3F56ED5_F876_4791_9D18_E31B4CCB8E35__INCLUDED_)
