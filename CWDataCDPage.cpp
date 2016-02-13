// CWDataCDPage.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CWDataCDPage.h"

#include "CreationWizard.h"

// CCWDataCDPage dialog

IMPLEMENT_DYNAMIC(CCWDataCDPage, CPropertyPage)
CCWDataCDPage::CCWDataCDPage()
	: CPropertyPage(CCWDataCDPage::IDD)
	, m_strBaseDir(_T(""))
	, m_bShowFiles(FALSE)
	, m_nNestingDepth(0)
	, m_nColumnsCount(0)
	, m_bUseSystemIcons(FALSE)
{
	m_pDataCDInfo = NULL;
}

CCWDataCDPage::~CCWDataCDPage()
{
	if (m_pDataCDInfo != NULL)
		delete m_pDataCDInfo;
}

void CCWDataCDPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BASEDIR, m_strBaseDir);
	DDX_Control(pDX, 1113, m_editCDTitle);
	DDX_Check(pDX, IDC_CHECK1, m_bShowFiles);
	DDX_Text(pDX, IDC_EDIT_MAXNESTING, m_nNestingDepth);
	DDX_Text(pDX, IDC_EDIT_COLUMNSCOUNT, m_nColumnsCount);
	DDX_Check(pDX, IDC_CHECK_USESYSTEMICONS, m_bUseSystemIcons);
}

void CCWDataCDPage::SetDataCDInfo (CDataCDInfo& info)
{
	if (m_pDataCDInfo != NULL)
		delete m_pDataCDInfo;
	m_pDataCDInfo = new CDataCDInfo (info);
}

BEGIN_MESSAGE_MAP(CCWDataCDPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CCWDataCDPage message handlers

void CCWDataCDPage::OnBnClickedButtonBrowse()
{
	// browse for folder
	LPSTR szDisplayName = m_strBaseDir.GetBuffer (MAX_PATH * 5);

	BROWSEINFO bi;
	memset (&bi, 0x00, sizeof (bi));
	
	bi.hwndOwner = GetSafeHwnd ();
	bi.pszDisplayName = szDisplayName;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;

	// show the shell dialog
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl)
	{
		SHGetPathFromIDList (pidl, szDisplayName);
		m_strBaseDir.ReleaseBuffer ();
		UpdateData (FALSE);

		// free the pidl
		CComPtr<IMalloc> spSHMalloc;
		SHGetMalloc (&spSHMalloc);
		if (spSHMalloc != NULL)
			spSHMalloc->Free (pidl);
	}
}

BOOL CCWDataCDPage::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT);

	if (m_pDataCDInfo == NULL)
		m_pDataCDInfo = new CDataCDInfo ();

	if (m_pDataCDInfo->m_pDirTree == NULL)
	{
		m_editCDTitle.SetRTF ("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2055{\\fonttbl{\\f0\\fswiss\\fcharset0 Arial;}}\\viewkind4\\uc1\\pard\\qc\\f0\\fs48 \\fs20\\par}");

		// get first cd drive
		m_strBaseDir.Empty ();
		DWORD dwDrives = GetLogicalDrives ();
		CString strDrive;
		for (int i = 0; i < 26; i++)
		{
			if (dwDrives & (1 << i))
			{
				strDrive.Format ("%c:\\", 'a' + i);
				if (GetDriveType (strDrive) == DRIVE_CDROM)
				{
					m_strBaseDir = strDrive;
					break;
				}
			}
		}

		m_bShowFiles = FALSE;
		m_nNestingDepth = 2;
		m_nColumnsCount = 3;
		m_bUseSystemIcons = TRUE;
	}
	else
	{
		m_editCDTitle.SetRTF (m_pDataCDInfo->GetRTFTitle (Cover));

		m_strBaseDir = m_pDataCDInfo->m_pDirTree->GetRootDir ();
		m_bShowFiles = m_pDataCDInfo->m_pDirTree->GetDisplayFiles ();
		m_nNestingDepth = m_pDataCDInfo->m_pDirTree->GetMaxNesting ();
		m_nColumnsCount = m_pDataCDInfo->m_pDirTree->GetColumnsCount ();
		m_bUseSystemIcons = m_pDataCDInfo->m_pDirTree->UseSystemIcons ();
	}

	UpdateData (FALSE);

	return CPropertyPage::OnSetActive();
}

void CCWDataCDPage::GetDataCDInfo ()
{
	if (m_pDataCDInfo == NULL)
		m_pDataCDInfo = new CDataCDInfo ();

	UpdateData ();

	m_editCDTitle.GetWindowText (m_pDataCDInfo->m_strCDTitle);

	CString strRTF = m_editCDTitle.GetRTF ();
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		m_pDataCDInfo->SetRTFTitle (strRTF, (RenderType) i);

	if (m_pDataCDInfo->m_pDirTree == NULL)
		m_pDataCDInfo->m_pDirTree = new CDirTree (m_strBaseDir, m_nNestingDepth, m_bShowFiles ? true : false, m_nColumnsCount, m_bUseSystemIcons ? true : false);
	else
		if (!m_pDataCDInfo->m_pDirTree->equals (m_strBaseDir, m_nNestingDepth, m_bShowFiles ? true : false, m_nColumnsCount, m_bUseSystemIcons ? true : false))
			m_pDataCDInfo->m_pDirTree = new CDirTree (m_strBaseDir, m_nNestingDepth, m_bShowFiles ? true : false, m_nColumnsCount, m_bUseSystemIcons ? true : false);

	((CCreationWizard*) GetParent ())->m_pDataCDInfo = m_pDataCDInfo;
}

BOOL CCWDataCDPage::OnKillActive()
{
	GetDataCDInfo ();
	return CPropertyPage::OnKillActive();
}

BOOL CCWDataCDPage::OnWizardFinish()
{
	GetDataCDInfo ();
	return CPropertyPage::OnWizardFinish();
}
