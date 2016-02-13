// PageSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "PageSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

double round (int n)
{
	return ((n / 50) * 50) / 1000.0;
}

/////////////////////////////////////////////////////////////////////////////
// CPageSetupDlg dialog


CPageSetupDlg::CPageSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPageSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSetupDlg)
	m_dCoverBackMarginLeft = 0.0;
	m_dCoverBackMarginTop = 0.0;
	m_dCoverMarginLeft = 0.0;
	m_dCoverMarginTop = 0.0;
	m_dInlayMarginLeft = 0.0;
	m_dInlayMarginTop = 0.0;
	m_dLabelMarginLeft = 0.0;
	m_dLabelMarginTop = 0.0;
	m_nCoverPO = -1;
	m_nCoverbackPO = -1;
	m_nInlayPO = -1;
	m_nLabelPO = -1;
	m_b2Labels = FALSE;
	m_dBookletMarginLeft = 0.0;
	m_dBookletMarginTop = 0.0;
	m_nBookletPO = -1;
	m_nIncludeCutLines = 0;
	//}}AFX_DATA_INIT
}

CPageSetupDlg::~CPageSetupDlg ()
{
	::DestroyIcon (m_hIconPrintPower);
	::DestroyIcon (m_hIconFunPrint);
}

void CPageSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSetupDlg)

#ifndef PEROUTKA_FEDORISIN
	DDX_Control(pDX, IDC_BUTTON_FUNPRINT, m_btnFunPrint);
	DDX_Control(pDX, IDC_BUTTON_PRINTPOWER, m_btnPrintPower);
#endif

	DDX_Control(pDX, IDC_COMBO_COVERBACKLABELTYPE, m_ComboCoverbackLabelType);
	DDX_Control(pDX, IDC_COMBO_COVERLABELTYPE, m_ComboCoverLabelType);
	DDX_Control(pDX, IDC_COMBO_INLAYLABELTYPE, m_ComboInlayLabelType);
	DDX_Control(pDX, IDC_COMBO_LABELLABELTYPE, m_ComboLabelLabelType);
	DDX_Control(pDX, IDC_COMBO_PRINTINLAYTOGETHER, m_ComboPrintInlay);
	DDX_Control(pDX, IDC_COMBO_PRINTCOVERBACKTOGETHER, m_ComboPrintCoverback);
	DDX_Text(pDX, IDC_EDIT_COVERBACKMARGINLEFT, m_dCoverBackMarginLeft);
	DDX_Text(pDX, IDC_EDIT_COVERBACKMARGINTOP, m_dCoverBackMarginTop);
	DDX_Text(pDX, IDC_EDIT_COVERMARGINLEFT, m_dCoverMarginLeft);
	DDX_Text(pDX, IDC_EDIT_COVERMARGINTOP, m_dCoverMarginTop);
	DDX_Text(pDX, IDC_EDIT_INLAYMARGINLEFT, m_dInlayMarginLeft);
	DDX_Text(pDX, IDC_EDIT_INLAYMARGINTOP, m_dInlayMarginTop);
	DDX_Text(pDX, IDC_EDIT_LABELMARGINLEFT, m_dLabelMarginLeft);
	DDX_Text(pDX, IDC_EDIT_LABELMARGINTOP, m_dLabelMarginTop);
	DDX_Radio(pDX, IDC_RADIO_COVER_PORTRAIT, m_nCoverPO);
	DDX_Radio(pDX, IDC_RADIO_COVERBACK_PORTRAIT, m_nCoverbackPO);
	DDX_Radio(pDX, IDC_RADIO_INLAY_PORTRAIT, m_nInlayPO);
	DDX_Radio(pDX, IDC_RADIO_LABEL_PORTRAIT, m_nLabelPO);
	DDX_Check(pDX, IDC_CHECK_2LABELS, m_b2Labels);
	DDX_Text(pDX, IDC_EDIT_BOOKLETMARGINLEFT, m_dBookletMarginLeft);
	DDX_Text(pDX, IDC_EDIT_BOOKLETMARGINTOP, m_dBookletMarginTop);
	DDX_Radio(pDX, IDC_RADIO_BOOKLET_PORTRAIT, m_nBookletPO);
	DDX_Check(pDX, IDC_CHECK_INCLUDECUTLINESWHENPRINGING, m_nIncludeCutLines);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CPageSetupDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_COVERLABELTYPE, OnSelchangeComboCoverlabeltype)
	ON_CBN_SELCHANGE(IDC_COMBO_COVERBACKLABELTYPE, OnSelchangeComboCoverbacklabeltype)
	ON_CBN_SELCHANGE(IDC_COMBO_LABELLABELTYPE, OnSelchangeComboLabellabeltype)
	ON_CBN_SELCHANGE(IDC_COMBO_INLAYLABELTYPE, OnSelchangeComboInlaylabeltype)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINTCOVERBACKTOGETHER, OnSelchangeComboPrintcoverbacktogether)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINTINLAYTOGETHER, OnSelchangeComboPrintinlaytogether)
	ON_BN_CLICKED(IDC_BUTTON_PRINTPOWER, OnButtonPrintpower)
	ON_BN_CLICKED(IDC_BUTTON_FUNPRINT, OnButtonFunprint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSetupDlg message handlers

/**
 * @deprecated
 */
void CPageSetupDlg::LoadMeasures(CComboBox *pCombo, UINT nId)
{
	HRSRC hResInfo = ::FindResource (AfxGetResourceHandle (), MAKEINTRESOURCE (nId), "LABELMEASURES");
	HGLOBAL hglData = ::LoadResource (AfxGetResourceHandle (), hResInfo);

	CMemFile memfile ((BYTE*) ::LockResource (hglData),
		::SizeofResource (AfxGetResourceHandle (), hResInfo));

	CArchive ar (&memfile, CArchive::load);
	
	CString s;
	DWORD dwX, dwY, dwW, dwH;
	int i;

	TRY
	{
		for ( ; ;)
		{
			ar >> s >> dwX >> dwY >> dwW >> dwH;
			i = pCombo->AddString (s);
			pCombo->SetItemDataPtr (i, new CRect (dwX, dwY, dwX + dwW, dwY + dwH));

			CString s1;
			s1.Format ("%s: x=%d, y=%d, w=%d, h=%d", s.GetBuffer (0), dwX, dwY, dwW, dwH);
			AfxMessageBox (s1);
		}
	}
	CATCH (CArchiveException, pEx)
	{
	}
	CATCH (CFileException, pEx)
	{
	}
	END_CATCH

	::FreeResource (hglData);
}

void CPageSetupDlg::LoadMeasures (CComboBox* pCombo, pug::xml_node& nodeType)
{
	// list media nodes
	int i;
	int nLeft, nTop, nWidth, nHeight;

	for (pug::xml_node::child_iterator it = nodeType.children_begin (); it != nodeType.children_end (); ++it)
		if (strcmp (it->name (), "measure") == 0)
		{
			i = pCombo->AddString (it->attribute ("name").value ());

			nLeft = (int) (atof (it->attribute ("left")) * 1000);
			nTop = (int) (atof (it->attribute ("top")) * 1000);
			nWidth = (int) (atof (it->attribute ("width")) * 1000);
			nHeight = (int) (atof (it->attribute ("height")) * 1000);

			pCombo->SetItemDataPtr (i, new CRect (nLeft, nTop, nLeft + nWidth, nTop + nHeight));
		}

}

BOOL CPageSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

/*
	LoadMeasures (&m_ComboCoverLabelType, IDR_LABELMEASURES_COV);
	LoadMeasures (&m_ComboCoverbackLabelType, IDR_LABELMEASURES_CBK);
	LoadMeasures (&m_ComboLabelLabelType, IDR_LABELMEASURES_LAB);
	LoadMeasures (&m_ComboInlayLabelType, IDR_LABELMEASURES_INL);
*/	

	// create an XML parser
	pug::xml_parser parser;
	CString strFilename = ((CCdCoverCreator2App*) AfxGetApp ())->GetApplicationDirectory () + "measures.xml";

	parser.parse_file (strFilename.GetBuffer (0),
		//parse_dtd_only |			//Parse only '<!DOCTYPE [*]>'
		pug::parse_doctype |		//Parse '<!DOCTYPE ...>' section, with '[...]' as data member.
		pug::parse_dtd |			//Parse whatever is in DOCTYPE data member ('[...]').
		pug::parse_pi |				//Parse '<?...?>'
		pug::parse_cdata |			//Parse '<![CDATA[...]]>', and '<![INCLUDE[...]]>'
		pug::parse_comments |		//Parse <!--...-->'
		pug::parse_wnorm |			//Normalize all entities that are flagged to be trimmed.
		pug::parse_trim_entity |	//Trim DTD entities.
		pug::parse_trim_attribute |	//Trim 'foo="..."'.
		pug::parse_trim_pcdata |	//Trim '>...<'
		pug::parse_trim_cdata |		//Trim '<![CDATA[...]]>'
		pug::parse_trim_comment		//Trim <!--...-->'
	);

	pug::xml_node document = parser.document ();
	pug::xml_node root = document.first_node (pug::node_element);

	for (pug::xml_node::child_iterator it = root.children_begin (); it != root.children_end (); ++it)
	{
		if (strcmp (it->name (), "type") != 0)
			continue;

		const char* szTypeName = it->attribute ("name").value ();
		if (strcmp (szTypeName, "Cover") == 0)
			LoadMeasures (&m_ComboCoverLabelType, *it);
		else if (strcmp (szTypeName, "Back") == 0)
			LoadMeasures (&m_ComboCoverbackLabelType, *it);
		else if (strcmp (szTypeName, "Label") == 0)
			LoadMeasures (&m_ComboLabelLabelType, *it);
		else if (strcmp (szTypeName, "Inlay") == 0)
			LoadMeasures (&m_ComboInlayLabelType, *it);
	}


	if (!m_strCoverLabelType.IsEmpty ())
		m_ComboCoverLabelType.SelectString (-1, m_strCoverLabelType);
	if (!m_strCoverbackLabelType.IsEmpty ())
		m_ComboCoverbackLabelType.SelectString (-1, m_strCoverbackLabelType);
	if (!m_strLabelLabelType.IsEmpty ())
		m_ComboLabelLabelType.SelectString (-1, m_strLabelLabelType);
	if (!m_strInlayLabelType.IsEmpty ())
		m_ComboInlayLabelType.SelectString (-1, m_strInlayLabelType);

	switch (m_rtRenderCoverbackWith)
	{
	case (RenderType) -1:
		m_ComboPrintCoverback.SetCurSel (0);
		break;
	case Cover:
		m_ComboPrintCoverback.SetCurSel (1);
		break;
	case Inlay:
		m_ComboPrintCoverback.SetCurSel (2);
		break;
	}
	OnSelchangeComboPrintcoverbacktogether ();

	switch (m_rtRenderInlayWith)
	{
	case (RenderType) -1:
		m_ComboPrintInlay.SetCurSel (0);
		break;
	case Cover:
		m_ComboPrintInlay.SetCurSel (1);
		break;
	case Back:
		m_ComboPrintInlay.SetCurSel (2);
		break;
	}
	OnSelchangeComboPrintinlaytogether ();

#ifndef PEROUTKA_FEDORISIN
	m_hIconPrintPower = (HICON) ::LoadImage (AfxGetResourceHandle (), MAKEINTRESOURCE (IDI_ICON_PRINTPOWER), IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR);
	m_hIconFunPrint = (HICON) ::LoadImage (AfxGetResourceHandle (), MAKEINTRESOURCE (IDI_ICON_FUNPRINT), IMAGE_ICON, 64, 64, LR_DEFAULTCOLOR);
	m_btnPrintPower.SetIcon (m_hIconPrintPower);
	m_btnFunPrint.SetIcon (m_hIconFunPrint);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageSetupDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// delete data
	for (int i = 0; i < m_ComboCoverLabelType.GetCount (); i++)
		delete m_ComboCoverLabelType.GetItemDataPtr (i);
	for (int i = 0; i < m_ComboCoverbackLabelType.GetCount (); i++)
		delete m_ComboCoverbackLabelType.GetItemDataPtr (i);
	for (int i = 0; i < m_ComboLabelLabelType.GetCount (); i++)
		delete m_ComboLabelLabelType.GetItemDataPtr (i);
	for (int i = 0; i < m_ComboInlayLabelType.GetCount (); i++)
		delete m_ComboInlayLabelType.GetItemDataPtr (i);
}

void CPageSetupDlg::OnSelchangeComboCoverlabeltype() 
{
	int i = m_ComboCoverLabelType.GetCurSel ();
	if (i == -1)
		return;
	CRect* pR = (CRect*) m_ComboCoverLabelType.GetItemDataPtr (i);

	m_dCoverMarginLeft = round (pR->left);
	m_dCoverMarginTop = round (pR->top);

	UpdateData (false);
}

void CPageSetupDlg::OnSelchangeComboCoverbacklabeltype() 
{
	int i = m_ComboCoverbackLabelType.GetCurSel ();
	if (i == -1)
		return;
	CRect* pR = (CRect*) m_ComboCoverbackLabelType.GetItemDataPtr (i);

	m_dCoverBackMarginLeft = round (pR->left);
	m_dCoverBackMarginTop = round (pR->top);

	UpdateData (false);
}

void CPageSetupDlg::OnSelchangeComboLabellabeltype() 
{
	int i = m_ComboLabelLabelType.GetCurSel ();
	if (i == -1)
		return;
	CRect* pR = (CRect*) m_ComboLabelLabelType.GetItemDataPtr (i);

	m_dLabelMarginLeft = round (pR->left);
	m_dLabelMarginTop = round (pR->top);

	UpdateData (false);
}

void CPageSetupDlg::OnSelchangeComboInlaylabeltype() 
{
	int i = m_ComboInlayLabelType.GetCurSel ();
	if (i == -1)
		return;
	CRect* pR = (CRect*) m_ComboInlayLabelType.GetItemDataPtr (i);

	m_dInlayMarginLeft = round (pR->left);
	m_dInlayMarginTop = round (pR->top);

	UpdateData (false);
}

void CPageSetupDlg::OnOK() 
{
	m_ComboCoverLabelType.GetWindowText (m_strCoverLabelType);
	m_ComboCoverbackLabelType.GetWindowText (m_strCoverbackLabelType);
	m_ComboLabelLabelType.GetWindowText (m_strLabelLabelType);
	m_ComboInlayLabelType.GetWindowText (m_strInlayLabelType);

	RenderType ar1[] = {(RenderType) -1, Cover, Inlay};
	RenderType ar2[] = {(RenderType) -1, Cover, Back};
	m_rtRenderCoverbackWith = ar1[m_ComboPrintCoverback.GetCurSel ()];
	m_rtRenderInlayWith = ar2[m_ComboPrintInlay.GetCurSel ()];

	CDialog::OnOK();
}

void CPageSetupDlg::OnSelchangeComboPrintcoverbacktogether() 
{
	bool bEnable = m_ComboPrintCoverback.GetCurSel () == 0;
	GetDlgItem (IDC_RADIO_COVERBACK_PORTRAIT)->EnableWindow (bEnable);
	GetDlgItem (IDC_RADIO_COVERBACK_LANDSCAPE)->EnableWindow (bEnable);
}

void CPageSetupDlg::OnSelchangeComboPrintinlaytogether() 
{
	bool bEnable = m_ComboPrintInlay.GetCurSel () == 0;
	GetDlgItem (IDC_RADIO_INLAY_PORTRAIT)->EnableWindow (bEnable);
	GetDlgItem (IDC_RADIO_INLAY_LANDSCAPE)->EnableWindow (bEnable);
}

HINSTANCE CPageSetupDlg::GotoURL (LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];	

    // First try ShellExecute()
    HINSTANCE result = ShellExecute (NULL, _T ("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR)
	{		
        if (GetRegKey (HKEY_CLASSES_ROOT, _T (".htm"), key) == ERROR_SUCCESS)
		{
            lstrcat (key, _T ("\\shell\\open\\command"));

            if (GetRegKey (HKEY_CLASSES_ROOT, key, key) == ERROR_SUCCESS)
			{
                TCHAR *pos;
                pos = _tcsstr (key, _T ("\"%1\""));
                if (pos == NULL)
				{										// No quotes found
                    pos = strstr(key, _T ("%1"));		// Check for %1, without quotes
                    if (pos == NULL)					// No parameter at all...
                        pos = key + lstrlen (key) - 1;
                    else
                        *pos = '\0';					// Remove the parameter
                }
                else
                    *pos = '\0';						// Remove the parameter

                lstrcat (pos, _T (" "));
                lstrcat (pos, url);
                result = (HINSTANCE) WinExec (key,showcmd);
            }
        }
	}
	  
    return result;
}

LONG CPageSetupDlg::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
    }

    return retval;
}

void CPageSetupDlg::OnButtonPrintpower() 
{
	GotoURL ("http://partners.webmasterplan.com/click.asp?ref=156987&site=1317&type=b26&bnb=26", SW_SHOW);
}

void CPageSetupDlg::OnButtonFunprint() 
{
	GotoURL ("http://partners.webmasterplan.com/click.asp?ref=156987&site=2141&type=b7&bnb=7", SW_SHOW);
}
