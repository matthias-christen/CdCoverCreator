// SelectLabelPrintDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "SelectLabelPrintDialog.h"
#include <Dlgs.h>


// CSelectLabelPrintDialog

IMPLEMENT_DYNAMIC(CSelectLabelPrintDialog, CPrintDialog)

CSelectLabelPrintDialog::CSelectLabelPrintDialog (BOOL bPrintSetupOnly, CSelectLabelStatic* pSelectLabelReference, DWORD dwFlags, CWnd* pParentWnd) :
	CPrintDialog (bPrintSetupOnly, dwFlags, pParentWnd)
{
	m_pd.lpPrintTemplateName = (LPTSTR) MAKEINTRESOURCE (PRINTDLGORD);
	m_pd.Flags |= PD_ENABLEPRINTTEMPLATE;
	m_pd.hInstance = AfxGetInstanceHandle ();
	AfxGetApp ()->GetPrinterDeviceDefaults (&m_pd);

	m_pLabelSelectorReference = pSelectLabelReference;
	m_LabelSelector.InternalSetState (pSelectLabelReference->GetState ());
}

CSelectLabelPrintDialog::~CSelectLabelPrintDialog()
{
}


BEGIN_MESSAGE_MAP(CSelectLabelPrintDialog, CPrintDialog)
	ON_SLN_STATECHANGED(IDC_LABELSELECTOR, OnLabelSelected)
END_MESSAGE_MAP()


void CSelectLabelPrintDialog::DoDataExchange(CDataExchange* pDX)
{
	CPrintDialog::DoDataExchange (pDX);

	DDX_Control(pDX, IDC_LABELSELECTOR, m_LabelSelector);
}

// CSelectLabelPrintDialog message handlers

void CSelectLabelPrintDialog::OnLabelSelected ()
{
	m_pLabelSelectorReference->InternalSetState (m_LabelSelector.GetState ());
}

void CSelectLabelPrintDialog::OnOK()
{
	CPrintDialog::OnOK();
}
