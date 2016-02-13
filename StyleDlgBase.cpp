// StyleDlgBase.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"
#include "StyleDlgBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgBase dialog


CStyleDlgBase::CStyleDlgBase(UINT uid, CWnd* pParent /*=NULL*/)
	: CDialog(uid, pParent)
{
	//{{AFX_DATA_INIT(CStyleDlgBase)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CStyleDlgBase, CDialog)
	//{{AFX_MSG_MAP(CStyleDlgBase)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStyleDlgBase message handlers

void CStyleDlgBase::UpdateDoc (UINT nId)
{
	// update (but only if this control has the focus
	if (nId != 0)
		if (GetFocus ()->GetSafeHwnd () != GetDlgItem (nId)->GetSafeHwnd ())
			return;

	CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
	if (pChild != NULL)
	{
		CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
		if (pDoc != NULL)
		{
			pDoc->UpdateAllViews (NULL);
			pDoc->CheckPoint ();
		}
	}
}