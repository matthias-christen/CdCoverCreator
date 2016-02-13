// PSImageEditing.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "PSImageEditing.h"


// CPSImageEditing dialog

IMPLEMENT_DYNAMIC(CPSImageEditing, CPropertyPage)
CPSImageEditing::CPSImageEditing(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CPSImageEditing::IDD)
	, m_strAppName(_T(""))
{
}

CPSImageEditing::~CPSImageEditing()
{
}

void CPSImageEditing::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strAppName);
}


BEGIN_MESSAGE_MAP(CPSImageEditing, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
END_MESSAGE_MAP()


// CPSImageEditing message handlers

void CPSImageEditing::OnBnClickedButton1()
{
	CFileDialog dlg (TRUE, "*.exe", "*.exe", OFN_HIDEREADONLY, "Programs (*.exe)", this);
	if (dlg.DoModal () == IDOK)
	{
		m_strAppName = dlg.GetPathName ();
		UpdateData (FALSE);

		AfxGetApp ()->WriteProfileString ("ImageEditing", "app", dlg.GetPathName ());
	}
}

void CPSImageEditing::OnEnChangeEdit1()
{
	UpdateData ();
	AfxGetApp ()->WriteProfileString ("ImageEditing", "app", m_strAppName);
}

BOOL CPSImageEditing::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_strAppName = AfxGetApp ()->GetProfileString ("ImageEditing", "app", "");
	UpdateData (FALSE);

	return TRUE;
}
