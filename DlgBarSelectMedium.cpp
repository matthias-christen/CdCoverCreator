// DlgBarSelectMedium.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"
#include "DlgBarSelectMedium.h"


// CDlgBarSelectMedium dialog

IMPLEMENT_DYNAMIC(CDlgBarSelectMedium, CDialogBar)
CDlgBarSelectMedium::CDlgBarSelectMedium(CWnd* pParent /*=NULL*/)
	: CDialogBar ()
{
}

CDlgBarSelectMedium::~CDlgBarSelectMedium()
{
}

void CDlgBarSelectMedium::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}

void CDlgBarSelectMedium::SetMediumType (MediumType type)
{
	switch (type)
	{
	case MediumJewelCase:
		((CComboBox*) GetDlgItem (IDC_COMBO_MEDIUM))->SelectString (-1, "Jewel Case");
		break;
	case MediumSlimCase:
		((CComboBox*) GetDlgItem (IDC_COMBO_MEDIUM))->SelectString (-1, "Slim Case");
		break;
	case MediumPaperSleeve:
		((CComboBox*) GetDlgItem (IDC_COMBO_MEDIUM))->SelectString (-1, "Paper Sleeve");
		break;
	case MediumDVDBox:
		((CComboBox*) GetDlgItem (IDC_COMBO_MEDIUM))->SelectString (-1, "DVD Box");
		break;
	}
}

MediumType CDlgBarSelectMedium::GetMediumType ()
{
	CString str;
	((CComboBox*) GetDlgItem (IDC_COMBO_MEDIUM))->GetLBText	(((CComboBox*) GetDlgItem (IDC_COMBO_MEDIUM))->GetCurSel (), str);

	if (str == "Jewel Case")
		return MediumJewelCase;
	if (str == "Slim Case")
		return MediumSlimCase;
	if (str == "Paper Sleeve")
		return MediumPaperSleeve;
	if (str == "DVD Box")
		return MediumDVDBox;

	return (MediumType) -1;
}

BEGIN_MESSAGE_MAP(CDlgBarSelectMedium, CDialogBar)
	ON_CBN_SELCHANGE(IDC_COMBO_MEDIUM, OnCbnSelchangeComboMedium)
END_MESSAGE_MAP()


// CDlgBarSelectMedium message handlers

void CDlgBarSelectMedium::OnCbnSelchangeComboMedium()
{
	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
	if (pChild != NULL)
	{
		CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
		if (pDoc != NULL)
			pDoc->SetMediumType (GetMediumType ());
	}
}
