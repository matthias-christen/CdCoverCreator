// StyleTemplateWizardPages.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "resource.h"
#include "STWSelectTmplPage.h"

#include "StyleTemplateWizard.h"
#include "StyleTemplates.h"

#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CSTWSelectTmplPage, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CSTWSelectTmplPage property page

CSTWSelectTmplPage::CSTWSelectTmplPage() : CPropertyPage(CSTWSelectTmplPage::IDD)
{
	//{{AFX_DATA_INIT(CSTWSelectTmplPage)
	m_bCreateNewTemplate = FALSE;
	m_strNewTemplateName = _T("");
	m_strTemplateName = _T("");
	//}}AFX_DATA_INIT
}

CSTWSelectTmplPage::~CSTWSelectTmplPage()
{
}

void CSTWSelectTmplPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSTWSelectTmplPage)
	DDX_Control(pDX, IDC_COMBO_TEMPLATE, m_Combo);
	DDX_Check(pDX, IDC_CHECK_CREATENEWTEMPLATE, m_bCreateNewTemplate);
	DDX_Text(pDX, IDC_EDIT_TEMPLATENAME, m_strNewTemplateName);
	DDX_CBString(pDX, IDC_COMBO_TEMPLATE, m_strTemplateName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSTWSelectTmplPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSTWSelectTmplPage)
	ON_BN_CLICKED(IDC_CHECK_CREATENEWTEMPLATE, OnCheckCreatenewtemplate)
	ON_CBN_SELCHANGE(IDC_COMBO_TEMPLATE, OnSelchangeComboTemplate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSTWSelectTmplPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	CStringArray& arr = ((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ().GetTemplates ();
	for (int i = 0; i < arr.GetSize (); i++)
		((CComboBox*) GetDlgItem (IDC_COMBO_TEMPLATE))->AddString (arr.GetAt (i));

	CString strCur = ((CStyleTemplateWizard*) GetParent ())->m_strCurrentStyleName;
	if (strCur.IsEmpty ())
		((CComboBox*) GetDlgItem (IDC_COMBO_TEMPLATE))->SetCurSel (0);
	else
		((CComboBox*) GetDlgItem (IDC_COMBO_TEMPLATE))->SelectString (-1, strCur);
	OnSelchangeComboTemplate ();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSTWSelectTmplPage::OnCheckCreatenewtemplate() 
{
	GetDlgItem (IDC_EDIT_TEMPLATENAME)->EnableWindow (
		((CButton*) GetDlgItem (IDC_CHECK_CREATENEWTEMPLATE))->GetCheck ());
	GetDlgItem (IDC_EDIT_TEMPLATENAME)->SetFocus ();
}

LRESULT CSTWSelectTmplPage::OnWizardNext() 
{
	if (!CreateTemplate ())
		return -1;
	return CPropertyPage::OnWizardNext();
}

BOOL CSTWSelectTmplPage::OnWizardFinish() 
{
	CreateTemplate ();	
	return CPropertyPage::OnWizardFinish();
}

BOOL CSTWSelectTmplPage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT);
	
	if (::IsWindow (((CStyleTemplateWizard*) GetParent ())->m_wndPreview.GetSafeHwnd ()))
		((CStyleTemplateWizard*) GetParent ())->m_wndPreview.InvalidateRect (NULL);

	return CPropertyPage::OnSetActive();
}

CString CSTWSelectTmplPage::GetTemplateName()
{
	if (::IsWindow (GetSafeHwnd ()))
		UpdateData ();

	return m_bCreateNewTemplate ? m_strNewTemplateName : m_strTemplateName;
}

void CSTWSelectTmplPage::OnSelchangeComboTemplate() 
{
	CString s;
	CComboBox* pCmb = (CComboBox*) GetDlgItem (IDC_COMBO_TEMPLATE);
	pCmb->GetLBText (pCmb->GetCurSel (), s);

	// Check whether template has been modified
	// and if so, ask user whether to safe template
	CStyleTemplateWizard* pWiz = (CStyleTemplateWizard*) GetParent ();
	CStyleTemplates& tmpl = ((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ();

	if (pWiz->m_pTemplate != NULL)
	{
		CStyleTemplate t;

		if (tmpl.LoadTemplate (pWiz->m_pTemplate->GetName (), &t))
			if (*pWiz->m_pTemplate != t)
				if (AfxMessageBox (IDS_SAVETEMPLATE, MB_ICONQUESTION | MB_YESNO) == IDYES)
				{
					tmpl.StoreTemplate (*pWiz->m_pTemplate);

					if (t.GetName ().IsEmpty ())
					{
						// name is empty: a new template has been created
						// insert name into combo box
						pCmb->AddString (pWiz->m_pTemplate->GetName ());
						pCmb->SelectString (-1, s);
					}
				}
	}

	// switch to newly selected template
	if (!s.IsEmpty ())
	{
		if (pWiz->m_pTemplate == NULL)
			pWiz->m_pTemplate = new CStyleTemplate ();

		tmpl.LoadTemplate (s, pWiz->m_pTemplate);

		if (::IsWindow (pWiz->m_wndPreview.GetSafeHwnd ()))
			pWiz->m_wndPreview.InvalidateRect (NULL);
	}
}

void CSTWSelectTmplPage::OnButtonDelete() 
{
	if (AfxMessageBox (IDS_CONFIRM_DELETETEMPLATE, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		CString strTemplate;
		m_Combo.GetWindowText (strTemplate);
		if (strTemplate.IsEmpty ())
			return;

		// get the number of templates
		CWinApp* pApp = AfxGetApp ();
		int nCount = pApp->GetProfileInt ("Templates", "Count", 0);
		CString s, strName;
		bool bMove = false;

		// delete from the list
		for (int i = 1; i <= nCount; i++)
		{
			if (bMove)
			{
				s.Format ("%d", i);
				strName = pApp->GetProfileString ("Templates", s);
				s.Format ("%d", i - 1);
				pApp->WriteProfileString ("Templates", s, strName);
			}

			s.Format ("%d", i);
			if (pApp->GetProfileString ("Templates", s) == strTemplate)
				bMove = true;
		}

		pApp->WriteProfileInt ("Templates", "Count", nCount - 1);

		// delete the template's settings
		DWORD dwRet = SHDeleteKey (HKEY_CURRENT_USER, "Software\\CdCoverCreator\\Templates\\" + strTemplate);

		// remove from the combo box
		m_Combo.DeleteString (m_Combo.FindStringExact (-1, strTemplate));
	}
}

bool CSTWSelectTmplPage::CreateTemplate()
{
	UpdateData ();

	// if a new template is created, new name must no be empty
	if (m_bCreateNewTemplate)
		if (m_strNewTemplateName.IsEmpty ())
		{
			AfxMessageBox (IDS_NEWTEMPLATENAMEEMPTY, MB_ICONEXCLAMATION);
			GetDlgItem (IDC_EDIT_TEMPLATENAME)->SetFocus ();

			return false;
		}

	CStyleTemplate* pTemplate = ((CStyleTemplateWizard*) GetParent ())->m_pTemplate;
	if (pTemplate == NULL)
		pTemplate = new CStyleTemplate ();

	((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ().LoadTemplate (m_strTemplateName, pTemplate);
	if (m_bCreateNewTemplate)
		pTemplate->m_strTemplateName = m_strNewTemplateName;

	return true;
}