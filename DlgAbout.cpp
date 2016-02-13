// DlgAbout.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "DlgAbout.h"


// CDlgAbout dialog

IMPLEMENT_DYNCREATE(CDlgAbout, CDHtmlDialog)

CDlgAbout::CDlgAbout(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CDlgAbout::IDD, CDlgAbout::IDH, pParent)
{
}

CDlgAbout::~CDlgAbout()
{
}

void CDlgAbout::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CDlgAbout::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CDlgAbout, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CDlgAbout)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CDlgAbout message handlers

HRESULT CDlgAbout::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // return TRUE  unless you set the focus to a control
}

HRESULT CDlgAbout::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // return TRUE  unless you set the focus to a control
}
