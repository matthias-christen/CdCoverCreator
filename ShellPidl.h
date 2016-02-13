#if !defined(AFX_SHELLPIDL_H__INCLUDED_)
#define AFX_SHELLPIDL_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ShellPidl.h : header file
//

#include <shlobj.h>
#define ID_UPDATE_TREESELCHANGE	WM_USER+100
#define ID_UPDATE_SELECTCHILD WM_USER+101
#define ID_UPDATE_NOFOLDER WM_USER+102
/////////////////////////////////////////////////////////////////////////////
// CShellPidl window

class CShellPidl
{
public:
	// STRUCTURES
	typedef struct tagLVID
	{
		LPSHELLFOLDER lpsfParent;
		LPITEMIDLIST  lpi;
		ULONG         ulAttribs;
	} LVITEMDATA, *LPLVITEMDATA;

	typedef struct tagID
	{
		LPSHELLFOLDER lpsfParent;
		LPITEMIDLIST  lpi;
		LPITEMIDLIST  lpifq;
	} TVITEMDATA, *LPTVITEMDATA;

public:

    // Functions that deal with PIDLs
    LPITEMIDLIST	ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
    LPITEMIDLIST	GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi);
    LPITEMIDLIST	CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi);
    BOOL			GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpi, DWORD dwFlags, LPSTR lpFriendlyName);
    LPITEMIDLIST	CreatePidl(UINT cbSize);
    UINT			GetSize(LPCITEMIDLIST pidl);
    LPITEMIDLIST	Next(LPCITEMIDLIST pidl);

	// Utility Functions
	BOOL	DoTheMenuThing(HWND hwnd, LPSHELLFOLDER lpsfParent, LPITEMIDLIST  lpi, LPPOINT lppt);
	int		GetItemIcon(LPITEMIDLIST lpi, UINT uFlags);
};

#endif // !defined(AFX_SHELLTREE_H__INCLUDED_)
