// PickMergeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "PickMergeDlg.h"

#include "TracksInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPickMergeDlg dialog


CPickMergeDlg::CPickMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPickMergeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPickMergeDlg)
	m_DontDeleteExisting = FALSE;
	//}}AFX_DATA_INIT
}


void CPickMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPickMergeDlg)
	DDX_Control(pDX, IDC_LIST_MERGE, m_List);
	DDX_Check(pDX, IDC_CHECK_DELEXISTING, m_DontDeleteExisting);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPickMergeDlg, CDialog)
	//{{AFX_MSG_MAP(CPickMergeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPickMergeDlg message handlers

BOOL CPickMergeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int n;
	CTracksInfo* pT;

	for (int i = 0; i < m_arrTrackInfo.GetSize (); i++)
	{
		pT = (CTracksInfo*) m_arrTrackInfo.GetAt (i);
		if (pT == NULL)
			continue;

		n = m_List.AddString (pT->GetTitle ());
		m_List.SetItemDataPtr (n, (void*) pT);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPickMergeDlg::OnOK() 
{
	m_arrTrackInfo.RemoveAll ();
	for (int i = 0; i < m_List.GetCount (); i++)
		if (m_List.GetSel (i))
			m_arrTrackInfo.Add ((CObject*) m_List.GetItemDataPtr (i));
	
	CDialog::OnOK();
}
