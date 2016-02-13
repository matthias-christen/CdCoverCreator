// PickDriveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PickDriveDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPickDriveDlg dialog


CPickDriveDlg::CPickDriveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPickDriveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPickDriveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPickDriveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPickDriveDlg)
	DDX_Control(pDX, IDC_LIST_DRIVES, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPickDriveDlg, CDialog)
	//{{AFX_MSG_MAP(CPickDriveDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DRIVES, OnDblclkListDrives)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DRIVES, OnItemchangedListDrives)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPickDriveDlg message handlers

void CPickDriveDlg::OnDblclkListDrives(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnOK ();
	*pResult = 0;
}

BOOL CPickDriveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	m_List.SetImageList (&((CMainFrame*) AfxGetMainWnd ())->m_ilSystemImageList, LVSIL_NORMAL);
	
	SHFILEINFO sfi;
	for (int i = 0; i < m_pArrDrives->GetSize (); i++)
	{
		SHGetFileInfo (m_pArrDrives->GetAt (i), 0, &sfi, sizeof (SHFILEINFO), SHGFI_ICON);
		m_List.InsertItem (i, m_pArrDrives->GetAt (i), sfi.iIcon);
	}

	m_List.SetItemState (0, LVIS_SELECTED, -1);

	return TRUE;
}

void CPickDriveDlg::OnOK() 
{
	POSITION pos = m_List.GetFirstSelectedItemPosition ();
	m_strSelectedDrive = m_List.GetItemText (m_List.GetNextSelectedItem (pos), 0);

	CDialog::OnOK();
}

void CPickDriveDlg::OnItemchangedListDrives(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	GetDlgItem (IDOK)->EnableWindow (m_List.GetSelectedCount () > 0);
	*pResult = 0;
}
