// DlgSetImageEditor.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "DlgSetImageEditor.h"


// CDlgSetImageEditor dialog

IMPLEMENT_DYNAMIC(CDlgSetImageEditor, CDialog)
CDlgSetImageEditor::CDlgSetImageEditor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetImageEditor::IDD, pParent)
	, m_strAppName(_T(""))
{
}

CDlgSetImageEditor::~CDlgSetImageEditor()
{
}

void CDlgSetImageEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strAppName);
}


BEGIN_MESSAGE_MAP(CDlgSetImageEditor, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgSetImageEditor message handlers

void CDlgSetImageEditor::OnBnClickedButton1()
{
	CFileDialog dlg (TRUE, "*.exe", "*.exe", OFN_HIDEREADONLY, "Programs (*.exe)", this);
	if (dlg.DoModal () == IDOK)
	{
		m_strAppName = dlg.GetPathName ();
		UpdateData (FALSE);
	}
}

void CDlgSetImageEditor::OnOK()
{
	UpdateData ();
	AfxGetApp ()->WriteProfileString ("ImageEditing", "app", m_strAppName);

	CDialog::OnOK();
}
