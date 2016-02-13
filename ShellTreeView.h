#if !defined(AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_)
#define AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ShellTree.h : header file
//

#include <afxcview.h>
#include <shlobj.h>
#include "ShellPidl.h"

/////////////////////////////////////////////////////////////////////////////
// CShellTreeView window

class CShellTreeView : public CTreeCtrl, public CShellPidl
{
public:

// Construction
public:
	CShellTreeView();

// Attributes
public:
    BOOL m_bOutaHere;

// Operations
public:
	void	PopulateTree();
	void	OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	void	GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL	OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath);
	void	EnableImages();
	BOOL	GetSelectedFolderPath(CString &szFolderPath);
	LPSHELLFOLDER	GetParentShellFolder(HTREEITEM folderNode);
	LPITEMIDLIST	GetRelativeIDLIST(HTREEITEM folderNode);
	LPITEMIDLIST	GetFullyQualifiedID(HTREEITEM folderNode);
	void	FindTreePidl(	HTREEITEM nextNode,
							LPLVITEMDATA lplvid, 
							bool& valid);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellTreeView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShellTreeView();

	// Generated message map functions
protected:
	void	FillTreeView(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq, HTREEITEM     hParent);
	void	GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem);
 	static int CALLBACK TreeViewCompareProc(LPARAM, LPARAM, LPARAM);
	
	//{{AFX_MSG(CShellTreeView)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_)
