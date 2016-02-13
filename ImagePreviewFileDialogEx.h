#if !defined(AFX_IMAGEPREVIEWFILEDIALOGEX_H__B06C7A3A_B7C9_4697_91C9_957D8C21570B__INCLUDED_)
#define AFX_IMAGEPREVIEWFILEDIALOGEX_H__B06C7A3A_B7C9_4697_91C9_957D8C21570B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImagePreviewFileDialogEx.h : header file
//

//#include "FileDialogEx.h"
#include "Picture.h"

/////////////////////////////////////////////////////////////////////////////
// CImagePreviewFileDialogEx dialog

class CImagePreviewFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CImagePreviewFileDialogEx)

public:
	CImagePreviewFileDialogEx(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	CPicture* m_pPic;
	bool m_bPreview;

	//{{AFX_MSG(CImagePreviewFileDialogEx)
	afx_msg void OnPreview();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	virtual void OnFileNameChange();
	virtual void OnFolderChange();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void PostNcDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPREVIEWFILEDIALOGEX_H__B06C7A3A_B7C9_4697_91C9_957D8C21570B__INCLUDED_)
