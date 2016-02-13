// SelectLabelsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "SelectLabelsDlg.h"

#ifdef PEROUTKA_FEDORISIN

// CSelectLabelsDlg dialog

IMPLEMENT_DYNAMIC(CSelectLabelsDlg, CDialog)

CSelectLabelsDlg::CSelectLabelsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectLabelsDlg::IDD, pParent)
{
	m_pPreviewWnd = NULL;
}

CSelectLabelsDlg::~CSelectLabelsDlg()
{
}

void CSelectLabelsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABELSELECTOR, m_LabelSelector);
}

void CSelectLabelsDlg::SetPreview (CWnd* pPreviewWnd)
{
	m_pPreviewWnd = pPreviewWnd;
}

bool CSelectLabelsDlg::PrintLabel (int nIdx)
{
	return m_LabelSelector.GetState () & (1 << nIdx) ? true : false;
}

BEGIN_MESSAGE_MAP(CSelectLabelsDlg, CDialog)
	ON_SLN_STATECHANGED(IDC_LABELSELECTOR, OnLabelSelected)
END_MESSAGE_MAP()


// CSelectLabelsDlg message handlers

void CSelectLabelsDlg::OnLabelSelected ()
{
	if (m_pPreviewWnd != NULL)
		m_pPreviewWnd->InvalidateRect (NULL);
}

#endif