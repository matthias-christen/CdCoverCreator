// CdCoverCreator2.h : main header file for the CdCoverCreator2 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include <map>
#include "BCMenu.h"
#include "StyleTemplates.h"

typedef struct tagResourceHost
{
	bool bEnabled;
	CString strLibFilename;
	int nVersion;
	HINSTANCE hInst;
	CString strCookie;
} RESOURCEHOST;

typedef std::map<CString, RESOURCEHOST*> HOSTMAP;

enum NewDocMode
{
	Empty, AudioCD, DataCD, Web, ImportM3U
};

// CCdCoverCreator2App:
// See CdCoverCreator2.cpp for the implementation of this class
//

class CCdCoverCreator2App : public CWinApp
{
public:
	CCdCoverCreator2App();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual void HtmlHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);

// Implementation
	afx_msg void OnAppAbout();
	//afx_msg void OnUpdateWindowNew(CCmdUI* pCmdUI);
	afx_msg void OnFileProgramoptions();
	afx_msg void OnCheckversion();
	afx_msg void OnFileNewEmptyproject();
	afx_msg void OnFileNewWizard();
	afx_msg void OnFileNewDataCD ();
	afx_msg void OnFileNewListweb();
	afx_msg void OnFileNewImportM3U();
	//afx_msg void OnUpdateRecentFileMenu(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	CString GetAppDir ();
	CString GetApplicationDirectory ();
	CString CheckDirExists (CString strDir);

	CString GetTextureDir ();
	CString GetAlphaTextureDir ();
	CString GetImageCacheDir ();
	CString GetListCacheDir ();
	CString GetTemplateDir ();
	
	double GetPaperLength(int nOrientation);
	void SetPaperOrientation (int nOrientation);

	inline CStyleTemplates& GetTemplates () { return *m_pTemplates; }

	bool m_bVersionChecked;
	bool m_bFirstDoc;
	HOSTMAP m_mapCoverHosts, m_mapLyricsHosts;

	inline bool DoCheckVersion () { return m_bDoCheckVersion; }
	void SetCheckVersion (BOOL bCheck);

	void FillRecentFileMenu (CMenu* pMenu, UINT& nIdx, UINT& nId);

	NewDocMode m_NewDocMode;

	CString GetVersion ();

protected:
	void GetHosts (HOSTMAP* pMap, CString strKey, CString strSuffix);

	bool m_bDoCheckVersion;
	ULONG_PTR m_gdiplusToken;
	CStyleTemplates* m_pTemplates;

public:
	afx_msg void OnHelpCdcovercreatorontheweb();
};

extern CCdCoverCreator2App theApp;