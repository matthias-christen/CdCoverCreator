// PickTitleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "PickTitleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CCDTitleRec
{
public:
	CString m_strTitle;
	CString m_strCategory;
	CString m_strDiscId;

	CCDTitleRec (CString strLine, int& nPos)
	{
		int i = nPos;
		while (!isspace (strLine[nPos]))
			nPos++;
		m_strCategory = strLine.Mid (i, nPos - i + 1);

		i = ++nPos;
		while (!isspace (strLine[nPos]))
			nPos++;
		m_strDiscId = strLine.Mid (i, nPos - i + 1);

		i = nPos;
		nPos = strLine.Find ('\n', nPos);
		m_strTitle = strLine.Mid (i, nPos - i + 1);

		m_strCategory.TrimLeft ();
		m_strCategory.TrimRight ();
		m_strDiscId.TrimLeft ();
		m_strDiscId.TrimRight ();
		m_strTitle.TrimLeft ();
		m_strTitle.TrimRight ();

		nPos += 1;
	}

	CString Get ()
	{
		return m_strCategory + ";" + m_strDiscId;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CPickTitleDlg dialog


CPickTitleDlg::CPickTitleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPickTitleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPickTitleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPickTitleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPickTitleDlg)
	DDX_Control(pDX, IDC_LIST_CDTITLES, m_ListTitles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPickTitleDlg, CDialog)
	//{{AFX_MSG_MAP(CPickTitleDlg)
	ON_WM_DESTROY()
	ON_LBN_DBLCLK(IDC_LIST_CDTITLES, OnDblclkListCdtitles)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPickTitleDlg message handlers

BOOL CPickTitleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CCDTitleRec* pRec;
	int n;

	for (int i = m_strTitle.Find ('\n') + 1; i < m_strTitle.GetLength () - 2; )
	{
		pRec = new CCDTitleRec (m_strTitle, i);

		if (pRec->m_strTitle.IsEmpty ())
			delete pRec;
		else
		{
			n = m_ListTitles.AddString (pRec->m_strTitle);
			m_ListTitles.SetItemDataPtr (n, pRec);
		}
	}

	m_ListTitles.SetCurSel (0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPickTitleDlg::OnOK() 
{
	m_strTitle = ((CCDTitleRec*) m_ListTitles.GetItemDataPtr (m_ListTitles.GetCurSel ()))->Get ();
	CDialog::OnOK();
}

void CPickTitleDlg::OnDestroy() 
{
	for (int i = 0; i < m_ListTitles.GetCount (); i++)
		delete m_ListTitles.GetItemDataPtr (i);

	CDialog::OnDestroy();
}

void CPickTitleDlg::OnDblclkListCdtitles() 
{
	OnOK ();
}
