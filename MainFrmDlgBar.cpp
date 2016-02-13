// MainFrmDlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "MainFrmDlgBar.h"
#include "CdCoverCreator2Doc.h"

#include "StyleTemplates.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrmDlgBar dialog


CMainFrmDlgBar::CMainFrmDlgBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CMainFrmDlgBar)
	//}}AFX_DATA_INIT
}


void CMainFrmDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainFrmDlgBar)
	DDX_Control(pDX, IDC_COMBO_MFR_STYLE, m_Combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainFrmDlgBar, CDialogBar)
	//{{AFX_MSG_MAP(CMainFrmDlgBar)
		// NOTE: the ClassWizard will add message map macros here
	ON_CBN_SELCHANGE(IDC_COMBO_MFR_STYLE, OnSelchangeComboStyle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrmDlgBar message handlers

void CMainFrmDlgBar::OnSelchangeComboStyle ()
{
	CComboBox* pCombo = (CComboBox*) GetDlgItem (IDC_COMBO_MFR_STYLE);
	CString s;

	pCombo->GetLBText (pCombo->GetCurSel (), s);
	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
	if (pChild != NULL)
	{
		CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();

		if (pDoc != NULL)
			pDoc->SetStyle (s);
	}
}

void CMainFrmDlgBar::SetStyle(CString strStyle)
{
	((CComboBox*) GetDlgItem (IDC_COMBO_MFR_STYLE))->SelectString (-1, strStyle);
}

void CMainFrmDlgBar::ListStyles()
{
	CComboBox* pCombo = (CComboBox*) GetDlgItem (IDC_COMBO_MFR_STYLE);
	pCombo->ResetContent ();

	CStringArray& arr = ((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ().GetTemplates ();
	for (int i = 0; i < arr.GetSize (); i++)
		pCombo->AddString (arr.GetAt (i));
}

CString CMainFrmDlgBar::GetCurSel()
{
	CString str;
	GetDlgItemText (IDC_COMBO_MFR_STYLE, str);
	return str;
}
