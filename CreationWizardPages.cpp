// CreationWizardPages.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MainFrm.h"
#include "CreationWizardPages.h"
#include "CdCoverCreator2Doc.h"
#include "BCMenu.h"
#include "ImagePreviewFileDialogEx.h"

#include "CreationWizard.h"

#include "Picture.h"
#include "AudioCDTracks.h"
#include "Track.h"

#include "RenderDC.h"
#include "LabelRenderer.h"

#include "StyleTemplates.h"
#include "StyleTemplateWizard.h"
#include "PickMergeDlg.h"

//#include "ListCDCoversDlg.h"
//#include "SearchCDCoversDlg.h"

#include "cccTwain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define STR_SEPARATOR " * * * "

IMPLEMENT_DYNCREATE(CCWArrangePage, CPropertyPage)
IMPLEMENT_DYNCREATE(CCWBkImgPage, CPropertyPage)
IMPLEMENT_DYNCREATE(CCWFormatPage, CPropertyPage)

/////////////////////////////////////////////////////////////////////////////
// CCWArrangePage property page

CCWArrangePage::CCWArrangePage() : CPropertyPage(CCWArrangePage::IDD)
{
	//{{AFX_DATA_INIT(CCWArrangePage)
	//}}AFX_DATA_INIT

	m_psp.dwFlags |= PSP_DEFAULT | PSP_USEHEADERTITLE | PSP_USEHEADERSUBTITLE;
	m_psp.pszHeaderTitle = _T("Title");
	m_psp.pszHeaderSubTitle = _T("And subtitle");

	m_pTracks = NULL;
}

CCWArrangePage::~CCWArrangePage()
{
	if (m_pTracks != NULL)
		delete m_pTracks;

	m_ImageList.DeleteImageList ();
	::DestroyIcon (m_hIconSep);
	::DestroyIcon (m_hIconTrack);
}

void CCWArrangePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCWArrangePage)
	DDX_Control(pDX, IDC_RICHEDIT_TITLE, m_EditTitle);
	DDX_Control(pDX, IDC_LIST_TRACKS, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCWArrangePage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_ADDGRP, OnButtonAddgrp)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TRACKS, OnRclickListTracks)
	ON_COMMAND(ID_ADDGROUPING, OnAddgrouping)
	ON_COMMAND(ID_DELETEGROUPING, OnDeletegrouping)
	ON_COMMAND(ID__REMOVEREMARKS, OnRemoveRemarks)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEGRP, OnButtonRemovegrp)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TRACKS, OnItemchangedListTracks)
	ON_BN_CLICKED(IDC_BUTTON_CDDB, OnButtonCddb)
	ON_BN_CLICKED(IDC_BUTTON_CDTEXT, OnButtonCdText)
	ON_BN_CLICKED(IDC_BUTTON_SPECIALFUNCTIONS, OnButtonSpecialFunctions)
	ON_COMMAND(ID__MATCHMERGE, OnMatchMerge)
	ON_COMMAND(ID__COPYTRACKLISTINGTOCLIPBOARD, OnCopyToClipboard)
	ON_COMMAND(ID__EXPORTTRACKLISTINGASHTML, OnExportHTML)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_TRACKS, OnBegindragListTracks)
	ON_COMMAND(ID_DELETETRACK, OnDeletetrack)
	ON_MESSAGE(LVN_ENDDRAG, OnEnddragListTracks)
END_MESSAGE_MAP()


BOOL CCWArrangePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// insert columns into list
	CString s;
	s.LoadString (IDS_COL_TRACKS);
	m_List.InsertColumn (0, s, LVCFMT_LEFT, 80);
	s.LoadString (IDS_COL_LENGTH);
	m_List.InsertColumn (1, s, LVCFMT_RIGHT, 50, 1);
	s.LoadString (IDS_COL_TITLE);
	m_List.InsertColumn (2, s, LVCFMT_LEFT, 220, 2);
	s.LoadString (IDS_COL_REMARKS);
	m_List.InsertColumn (3, s, LVCFMT_LEFT, 140, 3);

	// image list
	m_ImageList.Create (16, 16, ILC_COLOR32 | ILC_MASK, 2, 1);
	m_ImageList.Add (m_hIconSep = AfxGetApp ()->LoadIcon (IDI_ICON_SEP));
	m_ImageList.Add (m_hIconTrack = AfxGetApp ()->LoadIcon (IDI_ICON_TRACK));
	m_List.SetImageList (&m_ImageList, LVSIL_SMALL);
	m_List.SetDefaultIcon (1);

	// populate list with tracks
	if (m_pTracks == NULL)
	{
		m_pTracks = new CTracksInfo (DontRetrieve);
		if (m_pTracks->Aborted ())
		{
			((CPropertySheet*) GetParent ())->EndDialog (IDCANCEL);
			return TRUE;
		}

		if (m_pTracks->GetNumTracks () == 0)
		{
			// ignore has been pressed
			// (not aborted, but no tracks either)
			// allow manual editing
			m_List.SetFixedRows (false);
			m_pTracks->SetManuallyEditable (true);
			m_List.SetEditText ("");
		}
		else
		{
			// do not allow track number and length being edited
			m_List.SetFixedRows (true);
			m_List.SetFixedCol (0);
			m_List.SetFixedCol (1);

			m_List.SetEditText ("");
			m_List.SetEditPos (1, 0);

			m_pTracks->SetManuallyEditable (false);

			// there are tracks...
			int n;
			for (int i = 1; i <= m_pTracks->GetNumTracks (); i++)
			{
				s.Format (IDS_FORMAT_TRACK, i);
				n = m_List.InsertItem (i - 1, s, 1);
				m_List.SetItemData (n, i);
				m_List.SetItemText (n, 1, m_pTracks->GetTrackLength (i));
			}

			// Mysteriously, there always is one item too much at the end of the list
			// Delete this ghost...
			m_List.DeleteItem (m_pTracks->GetNumTracks ());
		}
	}
	else
	{
		if (m_pTracks->IsManuallyEditable ())
			m_List.SetFixedRows (false);
		else
		{
			// do not allow track number and length being edited
			m_List.SetFixedRows (true);
			m_List.SetFixedCol (0);
			m_List.SetFixedCol (1);
		}

		m_List.SetEditPos (0, 0);
		SetTracks ();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCWArrangePage::SetTracks()
{
	CString s;

//	SetDlgItemText (IDC_RICHEDIT_TITLE, m_pTracks->GetTitle ());
//	SetDlgItemText (IDC_EDIT_TITLE2, m_pTracks->GetSubtitle ());
	m_EditTitle.SetRTF (m_pTracks->GetRTFTitle (Inlay));
	m_List.DeleteAllItems ();
	CPoint pt = m_List.GetEditPos ();

	int n;
	for (int i = 0; i < m_pTracks->GetNumTracks (); i++)
	{
		CTrack t (m_pTracks->GetTrack (i));
		if (t.IsTrackGrouping ())
			n = m_List.InsertItem (i, STR_SEPARATOR, 0);
		else
		{
			s.Format (IDS_FORMAT_TRACK, t.GetTrackNum ());
			n = m_List.InsertItem (i, s, 1);
		}

		m_List.SetItemData (n, t.GetTrackNum ());
		m_List.SetItemText (n, 1, t.GetLength ());
		m_List.SetItemText (n, 2, t.GetTitle ());
		m_List.SetItemText (n, 3, t.GetRemarks ());
	}

	m_List.SetEditText (m_List.GetItemText (pt.y, pt.x));
}

BOOL CCWArrangePage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT);
	
	return CPropertyPage::OnSetActive();
}

BOOL CCWArrangePage::OnKillActive() 
{
	m_List.SaveEdit ();

	GetTracksInfo ();
	((CCreationWizard*) GetParent ())->m_pTracks = m_pTracks;

	return CPropertyPage::OnKillActive();
}

BOOL CCWArrangePage::OnWizardFinish() 
{
	m_List.SaveEdit ();

	GetTracksInfo ();
	((CCreationWizard*) GetParent ())->m_pTracks = m_pTracks;
	
	return CPropertyPage::OnWizardFinish();
}

CTracksInfo& CCWArrangePage::GetTracksInfo()
{
	if (!::IsWindow (GetSafeHwnd ()))
		return *m_pTracks;

	// create new tracks info
	if (m_pTracks == NULL)
		m_pTracks = new CTracksInfo ();

	// Build tracks info
	CString s;
	GetDlgItemText (IDC_RICHEDIT_TITLE, s);

	/*
	 * TODO: RTF should be merged [ mmc, 11.03.04 ]
	 *
	CString strRTFTitle;
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
	{
		strRTFTitle = m_pTracks->GetRTFTitle ((RenderType) i);
		if (!strRTFTitle.IsEmpty ())
		{
			strRTFTitle.Replace (m_pTracks->GetTitle (), s);
			m_pTracks->SetRTFTitle (strRTFTitle, (RenderType) i);
		}
	}*/

	s.Replace ("\r\n", " · ");
	m_pTracks->SetTitle (s);

//	for (int i = 0; i < NUM_RENDER_TYPES; i++)
//		m_pTracks->SetRTFTitle ("" /*m_EditTitle.GetRTF ()*/, (RenderType) i);

	//m_pTracks->ApplyStyleToRTFTitles (?);

//	GetDlgItemText (IDC_EDIT_TITLE2, s);
//	m_pTracks->SetSubtitle (s);

	if (m_pTracks->IsManuallyEditable ())
	{
		// remove existing tracks
		int n = m_pTracks->GetNumTracks ();
		for (int i = 0; i < n; i++)
			m_pTracks->Remove (0);

		// add tracks from list control
		LVITEM lvItem;
		int m, nIdx = 1;
		for (int i = 0; i < m_List.GetItemCount (); i++)
		{
			lvItem.iItem = i;
			lvItem.iSubItem = 0;
			lvItem.mask = LVIF_IMAGE;
			m_List.GetItem (&lvItem);

			if (lvItem.iImage == 0)
				m = -1;
			else
				m = nIdx++;

			m_pTracks->Add (new CTrack (m,
				m_List.GetItemText (i, 1),
				m_List.GetItemText (i, 2),
				m_List.GetItemText (i, 3)));
		}
	}
	else
		for (int i = 0; i < m_pTracks->GetNumTracks (); i++)
			m_pTracks->SetTrackTexts (i + 1/*m_List.GetItemData (i)*/,
				m_List.GetItemText (i, 2),
				m_List.GetItemText (i, 3));

	return *m_pTracks;
}

void CCWArrangePage::SetTracksInfo(CTracksInfo &tracks)
{
	if (m_pTracks != NULL)
		delete m_pTracks;
	m_pTracks = new CTracksInfo (tracks);
}

void CCWArrangePage::AddGrouping()
{
	POSITION pos = m_List.GetFirstSelectedItemPosition ();
	int idx = m_List.GetNextSelectedItem (pos);
	if (idx == -1)
		idx = m_List.GetItemCount ();

	int n = m_List.InsertItem (idx, STR_SEPARATOR, 0);
	m_List.SetItemData (n, -1);
	m_pTracks->Insert (new CTrack (-1, "", "", ""), n);

	GetDlgItem (IDC_BUTTON_REMOVEGRP)->EnableWindow (false);
}


void CCWArrangePage::DeleteGrouping()
{
	POSITION pos = m_List.GetFirstSelectedItemPosition ();
	int idx = m_List.GetNextSelectedItem (pos);

	if (m_List.GetItemText (idx, 0) == STR_SEPARATOR)
	{
		// prevent the edit keeping the old text
		if (m_List.GetEditPos ().y == idx)
			m_List.MoveEdit (-1, 0);

		// do removal
		m_List.DeleteItem (idx);
		m_pTracks->Remove (idx);
	}
	else	// cannot remove!
		MessageBeep (0xFFFFFFFF);

	GetDlgItem (IDC_BUTTON_REMOVEGRP)->EnableWindow (false);
}

void CCWArrangePage::OnDeletetrack() 
{
	POSITION pos = m_List.GetFirstSelectedItemPosition ();
	int idx = m_List.GetNextSelectedItem (pos);

	if (m_List.GetEditPos ().y == idx)
		m_List.MoveEdit (-1, 0);

	// do removal
	m_List.DeleteItem (idx);
}

void CCWArrangePage::OnRclickListTracks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_List.GetFirstSelectedItemPosition ();
	if (pos == NULL)
		return;

	int idx = m_List.GetNextSelectedItem (pos);

	BCMenu menu;

	if (m_List.GetItemText (idx, 0) == STR_SEPARATOR)
		menu.LoadMenu (IDR_MENU_SEP);
	else
	{
		menu.LoadMenu (IDR_MENU_TRACK);
		if (!m_pTracks->IsManuallyEditable ())
			((BCMenu*) menu.GetSubMenu (0))->RemoveMenu (ID_DELETETRACK, MF_BYCOMMAND);
	}
	menu.LoadToolbar (IDR_TOOLBAR_GRP);

	CPoint pt;
	GetCursorPos (&pt);
	menu.GetSubMenu (0)->TrackPopupMenu (TPM_RIGHTALIGN, pt.x, pt.y, this);

	menu.DestroyMenu ();

	*pResult = 0;
}

void CCWArrangePage::OnRemoveRemarks ()
{
	for (int i = 0; i < m_List.GetItemCount (); i++)
		m_List.SetItemText (i, 3, "");
}

void CCWArrangePage::OnAddgrouping() 
{
	AddGrouping ();	
}

void CCWArrangePage::OnButtonAddgrp() 
{
	AddGrouping ();	
}

void CCWArrangePage::OnDeletegrouping() 
{
	DeleteGrouping ();
}

void CCWArrangePage::OnButtonRemovegrp() 
{
	DeleteGrouping ();
}

void CCWArrangePage::OnButtonSpecialFunctions ()
{
	CRect r;
	GetDlgItem (IDC_BUTTON_SPECIALFUNCTIONS)->GetWindowRect (&r);

	CMenu menu;
	menu.LoadMenu (IDR_MENU_CWSPECIALFUNCTIONS);
	menu.GetSubMenu (0)->TrackPopupMenu (TPM_LEFTALIGN | TPM_RIGHTBUTTON, r.left, r.bottom, this);
}

void CCWArrangePage::OnMatchMerge () 
{
	CPickMergeDlg dlg;

	// add all open documents to the list
	POSITION pos = AfxGetApp ()->GetFirstDocTemplatePosition ();
	CDocTemplate* pDocTmpl = AfxGetApp ()->GetNextDocTemplate (pos);
	pos = pDocTmpl->GetFirstDocPosition ();
	while (pos)
		dlg.m_arrTrackInfo.Add (((CCdCoverCreator2Doc*) pDocTmpl->GetNextDoc (pos))->m_pTracks);

	if (dlg.DoModal () == IDOK)
	{
		// build merged trackinfo
		CTracksInfo ti, *pT;
		for (int i = 0; i < dlg.m_arrTrackInfo.GetSize (); i++)
		{
			pT = (CTracksInfo*) dlg.m_arrTrackInfo.GetAt (i);
			for (int j = 0; j  < pT->GetNumTracks (); j++)
				ti.Add (new CTrack (pT->GetTrack (j)));
		}

		// clear tracks
		if (!dlg.m_DontDeleteExisting)
			for (int i = 0; i < m_pTracks->GetNumTracks (); i++)
				m_pTracks->GetTrack (i).SetText ("", "");

		// match tracks
		GetTracksInfo ();
		m_pTracks->Match (&ti, dlg.m_DontDeleteExisting ? false : true);

		// display
		SetTracks ();
	}
}

void CCWArrangePage::OnCopyToClipboard ()
{
	m_List.SaveEdit ();
	GetTracksInfo ();

	if (m_pTracks == NULL)
		return;

	OpenClipboard ();
	EmptyClipboard ();

	// prepare data
	CString strText = "";
	CString strRTF = "";

	strRTF = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1031{\\fonttbl{\\f0\\fswiss\\fcharset0 Arial;}}\\viewkind4\\uc1\\pard\\ql\\f0\\cf1";
	if (!m_pTracks->GetArtist ().IsEmpty ())
	{
		strRTF += "\\fs20" + m_pTracks->GetArtist () + "\\par";
		strText += m_pTracks->GetArtist () + ": ";
	}
	
	strRTF += "\\fs32 " + m_pTracks->GetTitle () + "\\par\\fs20 ";
	strText += m_pTracks->GetTitle () + "\r\n\r\n";

	for (int i = 0; i < m_pTracks->GetNumTracks (); i++)
	{
		CTrack t = m_pTracks->GetTrack (i);

		strRTF.AppendFormat ("% 2d.\\tab ", i + 1);
		strText.AppendFormat ("%s%d.\t", i >= 9 ? "" : " ", i + 1);

		strRTF += t.GetNiceLength () + "\\tab\\b " + t.GetTitle () + "\\b0\\tab\\i " + t.GetRemarks () + "\\i0\\par ";
		strText += t.GetNiceLength () + "\t" + t.GetTitle () + "\t" + t.GetRemarks () + "\r\n";
	}

	strRTF += "}";

	// set data to clipboard
	HGLOBAL hglText = GlobalAlloc (GMEM_MOVEABLE, strText.GetLength () + 1);
	strcpy ((char*) GlobalLock (hglText), strText.GetBuffer (-1));
	GlobalUnlock (hglText);

	HGLOBAL hglRTF = GlobalAlloc (GMEM_MOVEABLE, strRTF.GetLength () + 1);
	strcpy ((char*) GlobalLock (hglRTF), strRTF.GetBuffer (-1));
	GlobalUnlock (hglText);

	SetClipboardData (CF_TEXT, hglText);
	SetClipboardData (RegisterClipboardFormat (CF_RTF), hglRTF);

	CloseClipboard ();
}

void CCWArrangePage::OnExportHTML ()
{
	m_List.SaveEdit ();
	GetTracksInfo ();

	if (m_pTracks == NULL)
		return;

	CFileDialog dlg (FALSE, "html");
	if (dlg.DoModal () == IDOK)
	{
		CStdioFile file (dlg.GetPathName (), CFile::modeCreate | CFile::modeWrite);

		file.WriteString ("<html>\r\n\t<head>\r\n\t\t<title>Exported Track Listing: " +
			m_pTracks->GetTitle () + "</title>\r\n\t</head>\r\n\r\n\t<body>\r\n");

		if (m_pTracks->GetArtist ().IsEmpty ())
			file.WriteString ("\t\t<h1>" + m_pTracks->GetTitle () + "</h1>\r\n");
		else
			file.WriteString ("\t\t<h1>" + m_pTracks->GetArtist () + "</h1>\r\n\t\t<h2>" +
				m_pTracks->GetTitle () + "</h2>\r\n");

		file.WriteString ("\t\t<table border=\"1\">\r\n\t\t\t<tr>\r\n");
		file.WriteString ("\t\t\t\t<th>Track</th>\r\n");
		file.WriteString ("\t\t\t\t<th>Track title</th>\r\n");
		file.WriteString ("\t\t\t\t<th>Track remarks</th>\r\n");
		file.WriteString ("\t\t\t\t<th>Track length</th>\r\n");
		file.WriteString ("\t\t\t</tr>\r\n\r\n");

		for (int i = 0; i < m_pTracks->GetNumTracks (); i++)
		{
			CTrack t = m_pTracks->GetTrack (i);

			file.WriteString ("\t\t\t<tr>\r\n");
			CString s;
			s.Format ("%d.", i + 1);

			// s
			file.WriteString ("\t\t\t\t<td>");
			if (s.IsEmpty ())
				file.WriteString ("&nbsp;");
			else
				file.WriteString (s);
			file.WriteString ("</td>\r\n");

			// title
			file.WriteString ("\t\t\t\t<td>");
			s = t.GetTitle ();
			if (s.IsEmpty ())
				file.WriteString ("&nbsp;");
			else
				file.WriteString (s);
			file.WriteString ("</td>\r\n");
			
			// remarks
			file.WriteString ("\t\t\t\t<td>");
			s = t.GetRemarks ();
			if (s.IsEmpty ())
				file.WriteString ("&nbsp;");
			else
				file.WriteString (s);
			file.WriteString ("</td>\r\n");
			
			// length
			file.WriteString ("\t\t\t\t<td>");
			s = t.GetNiceLength ();
			if (s.IsEmpty ())
				file.WriteString ("&nbsp;");
			else
				file.WriteString (s);
			file.WriteString ("</td>\r\n");
			
			file.WriteString ("\t\t\t</tr>\r\n");
		}

		file.WriteString ("\t\t</table>\r\n\t</body>\r\n</html>");

		file.Close ();
	}
}

void CCWArrangePage::OnButtonCddb() 
{
	if (m_pTracks != NULL)
		if (m_pTracks->GetCDDBInfo ())
			SetTracks ();
}

void CCWArrangePage::OnButtonCdText ()
{
	if (m_pTracks != NULL)
		if (m_pTracks->GetCDText ())
			SetTracks ();
}

void CCWArrangePage::OnItemchangedListTracks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	POSITION pos = m_List.GetFirstSelectedItemPosition ();
	int idx = m_List.GetNextSelectedItem (pos);

	if (idx < 0)
		GetDlgItem (IDC_BUTTON_REMOVEGRP)->EnableWindow (false);
	else
	{
		if (m_List.GetItemText (idx, 0) == STR_SEPARATOR)
			GetDlgItem (IDC_BUTTON_REMOVEGRP)->EnableWindow (true);
		else
			GetDlgItem (IDC_BUTTON_REMOVEGRP)->EnableWindow (false);
	}

	*pResult = 0;
}

void CCWArrangePage::OnBegindragListTracks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_List.BeginDrag ((NM_LISTVIEW*) pNMHDR);
	*pResult = 0;
}

LRESULT CCWArrangePage::OnEnddragListTracks(WPARAM wParam, LPARAM lParam) 
{
	GetTracksInfo ();
	CTrack t = m_pTracks->GetTrack (LOWORD (wParam));
	m_pTracks->GetTrack (HIWORD (wParam)).SetText (t.GetTitle (), t.GetRemarks ());
	SetTracks ();

	m_List.SetEditPos (m_List.GetEditPos ().x - 1, HIWORD (wParam) - 1);

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CCWBkImgPage property page

CCWBkImgPage::CCWBkImgPage() : CPropertyPage(CCWBkImgPage::IDD)
{
	//{{AFX_DATA_INIT(CCWBkImgPage)
	m_strImgFilename = _T("");
	m_enumMode = -1;
	//}}AFX_DATA_INIT

	m_pPic = NULL;
	m_pBackgroundInfo = NULL;
	m_bSettingTransparentColor = false;
	m_crTransparent = -1;

	CWinApp* pApp = AfxGetApp ();

	m_hIconBrowse = pApp->LoadIcon (IDI_ICON_BROWSE);
	m_hIconScan = pApp->LoadIcon (IDI_ICON_SCAN);
	m_hIconSetTransparent = pApp->LoadIcon (IDI_SETTRANSPARENT);
	m_hIconRemoveTransparent = pApp->LoadIcon (IDI_REMOVETRANSPARENT);

	m_hPickCursor = pApp->LoadCursor (IDC_CURSOR_PICKER);
}

CCWBkImgPage::~CCWBkImgPage()
{
	if (m_pBackgroundInfo != NULL)
		delete m_pBackgroundInfo;
	if (m_pPic != NULL)
		delete m_pPic;

	::DestroyIcon (m_hIconBrowse);
	::DestroyIcon (m_hIconScan);
	::DestroyIcon (m_hIconSetTransparent);
	::DestroyIcon (m_hIconRemoveTransparent);

	::DestroyIcon (m_hIconTL);
	::DestroyIcon (m_hIconTC);
	::DestroyIcon (m_hIconTR);
	::DestroyIcon (m_hIconCL);
	::DestroyIcon (m_hIconCC);
	::DestroyIcon (m_hIconCR);
	::DestroyIcon (m_hIconBL);
	::DestroyIcon (m_hIconBC);
	::DestroyIcon (m_hIconBR);

	::DestroyCursor (m_hPickCursor);
}

void CCWBkImgPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCWBkImgPage)
	DDX_Control(pDX, IDC_SCAN, m_btnScan);
	DDX_Control(pDX, IDC_BUTTON_REMOVETRANSPARENT, m_btnRemoveTransparent);
	DDX_Control(pDX, IDC_BUTTON_TRANSPARENTCOLOR, m_btnSetTransparent);
	DDX_Control(pDX, IDC_STATIC_TRANSPARENT, m_StaticTransparentCol);
	DDX_Control(pDX, IDC_BROWSEBKIMG, m_BrowseBkImg);
	DDX_Text(pDX, IDC_EDIT_BKIMGFILENAME, m_strImgFilename);
	DDX_Radio(pDX, IDC_RADIO1, m_enumMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCWBkImgPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCWBkImgPage)
	ON_BN_CLICKED(IDC_BROWSEBKIMG, OnBrowseBkImg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, OnButtonPreview)
	ON_BN_CLICKED(IDC_BUTTON_TRANSPARENTCOLOR, OnButtonTransparentcolor)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_REMOVETRANSPARENT, OnButtonRemovetransparent)
	ON_BN_CLICKED(IDC_SCAN, OnScan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCWBkImgPage::OnBrowseBkImg() 
{
	CString strFilename;
	strFilename.LoadString (IDS_OPENBKIMGFILE_FILENAME);
	CString strFilter;
	strFilter.LoadString (IDS_OPENBKIMGFILE_FILTER);

	CImagePreviewFileDialogEx dlg (TRUE, NULL, strFilename, 0, strFilter, this);
	if (dlg.DoModal () == IDOK)
	{
		SetDlgItemText (IDC_EDIT_BKIMGFILENAME, dlg.GetPathName ());
		OnButtonPreview ();
	}
}

void CCWBkImgPage::OnScan() 
{
	CcccTwain* pTwain = &((CMainFrame*) AfxGetApp ()->GetMainWnd ())->m_twain;
	pTwain->m_pBkImgPage = this;
	pTwain->SelectSource ();
	if (pTwain->SourceSelected ())
		pTwain->Acquire ();
}

void CCWBkImgPage::OnPaint() 
{
	CPaintDC dc (this); // device context for painting

	if (m_pPic != NULL)
	{
		CRect r;
		GetDlgItem (IDC_PREVIEW)->GetWindowRect (&r);
		ScreenToClient (&r);
		r.DeflateRect (5, 5, 5, 5);

		CSize sz = m_pPic->GetSize ();
		if (sz.cx > r.Width ())
		{
			sz.cy = (sz.cy * r.Width ()) / sz.cx;
			sz.cx = r.Width ();
		}
		if (sz.cy > r.Height ())
		{
			sz.cx = (sz.cx * r.Height ()) / sz.cy;
			sz.cy = r.Height ();
		}

		m_pPic->Render (&dc, NULL,
			(r.left + r.right - sz.cx) / 2,
			(r.top + r.bottom + sz.cy) / 2,
			sz.cx, -sz.cy, false);
	}
}

BOOL CCWBkImgPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// take over bk img from Cover (only the first time!
	// that's why this code stands here in OnInitDialog!)
	CBackgroundInfo* pBkInfoCover = &((CCreationWizard*) GetParent ())->GetBackgroundInfo (Cover);
	if (pBkInfoCover != NULL)
	{
		if ((m_nBkImageType != Cover) && (m_pBackgroundInfo == NULL))
		{
			SetDlgItemText (IDC_EDIT_BKIMGFILENAME, pBkInfoCover->GetImageFilename ());
			if (!pBkInfoCover->GetImageFilename ().IsEmpty ())
				OnButtonPreview ();
		}
	}

	m_BrowseBkImg.SetIcon (m_hIconBrowse);
	m_btnScan.SetIcon (m_hIconScan);

	CWinApp* pApp = AfxGetApp ();

	GetDlgItem (IDC_RADIO1)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconTL = pApp->LoadIcon (IDI_ICON_TL)));
	GetDlgItem (IDC_RADIO2)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconTC = pApp->LoadIcon (IDI_ICON_TC)));
	GetDlgItem (IDC_RADIO3)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconTR = pApp->LoadIcon (IDI_ICON_TR)));
	GetDlgItem (IDC_RADIO4)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconCL = pApp->LoadIcon (IDI_ICON_CL)));
	GetDlgItem (IDC_RADIO5)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconCC = pApp->LoadIcon (IDI_ICON_CC)));
	GetDlgItem (IDC_RADIO6)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconCR = pApp->LoadIcon (IDI_ICON_CR)));
	GetDlgItem (IDC_RADIO7)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconBL = pApp->LoadIcon (IDI_ICON_BL)));
	GetDlgItem (IDC_RADIO8)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconBC = pApp->LoadIcon (IDI_ICON_BC)));
	GetDlgItem (IDC_RADIO9)->SendMessage (BM_SETIMAGE, IMAGE_ICON, (LPARAM) (m_hIconBR = pApp->LoadIcon (IDI_ICON_BR)));

	GetDlgItem (IDC_RADIO5)->SendMessage (BM_SETCHECK, BST_CHECKED, 0);

	if (m_pBackgroundInfo != NULL)
	{
		m_strImgFilename = m_pBackgroundInfo->GetImageFilename ();

		if (m_pPic != NULL)
			delete m_pPic;
		m_pPic = new CPicture (m_strImgFilename);

		m_enumMode = m_pBackgroundInfo->GetAlignmentMode ();

		UpdateData (false);
	}

	m_btnSetTransparent.SetIcon (m_hIconSetTransparent);
	m_btnRemoveTransparent.SetIcon (m_hIconRemoveTransparent);
	m_StaticTransparentCol.SetAngle (0);
	
	if (m_pPic != NULL)
		m_pPic->SetTransparentColor (m_crTransparent);

	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool (&m_BrowseBkImg, IDS_TT_BROWSE);
		m_tooltip.AddTool (&m_btnScan, IDS_TT_SCAN);
		m_tooltip.AddTool (&m_btnSetTransparent, IDS_TT_TRANSPARENT);
		m_tooltip.AddTool (&m_btnRemoveTransparent, IDS_TT_REMOVETRANSPARENT);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCWBkImgPage::OnSetActive() 
{
	CString s;
	switch (m_nBkImageType)
	{
	case Cover:
		s.LoadString (IDS_COVERBKIMAGE);
		break;
	case Back:
		s.LoadString (IDS_BACKBKIMAGE);
		break;
	case Label:
		s.LoadString (IDS_LABELBKIMAGE);
		break;
	case Inlay:
		s.LoadString (IDS_INLAYBKIMAGE);
		break;
	}
	GetParent ()->SetWindowText (s);

	if (m_pBackgroundInfo != NULL)
		if (!m_pBackgroundInfo->GetImageFilename ().IsEmpty ())
			OnButtonPreview ();

	m_StaticTransparentCol.ShowWindow ((m_crTransparent == -1) ? SW_HIDE : SW_SHOW);
	m_StaticTransparentCol.SetStartColor (m_crTransparent);
	m_StaticTransparentCol.SetEndColor (m_crTransparent);

	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_NEXT | PSWIZB_BACK);
	
	return CPropertyPage::OnSetActive();
}

BOOL CCWBkImgPage::OnKillActive() 
{
	GetBackgroundInfo ();
	((CCreationWizard*) GetParent ())->m_pBackground[m_nBkImageType] = m_pBackgroundInfo;

	return CPropertyPage::OnKillActive();
}

BOOL CCWBkImgPage::OnWizardFinish() 
{
	GetBackgroundInfo ();
	((CCreationWizard*) GetParent ())->m_pBackground[m_nBkImageType] = m_pBackgroundInfo;
	
	return CPropertyPage::OnWizardFinish();
}

void CCWBkImgPage::OnButtonPreview() 
{
	if (m_pPic != NULL)
		delete m_pPic;

	CWaitCursor wait;
	CString strFilename;
	GetDlgItemText (IDC_EDIT_BKIMGFILENAME, strFilename);

	m_pPic = new CPicture (strFilename);

	if (!m_pPic->IsValid ())
	{
		AfxMessageBox (IDS_INVALIDPICTURE, MB_ICONEXCLAMATION, 0);
		delete m_pPic;
		m_pPic = NULL;
	}

	InvalidateRect (NULL);

	m_btnSetTransparent.EnableWindow (m_pPic != NULL);
	m_btnRemoveTransparent.EnableWindow (m_pPic != NULL);
}

void CCWBkImgPage::SetBackgroundInfo(CBackgroundInfo &bkgrnd)
{
	if (m_pBackgroundInfo != NULL)
		delete m_pBackgroundInfo;
	m_pBackgroundInfo = new CBackgroundInfo (bkgrnd);

	m_crTransparent = m_pBackgroundInfo->GetTransparentColor ();
}

CBackgroundInfo& CCWBkImgPage::GetBackgroundInfo()
{
	if (!::IsWindow (GetSafeHwnd ()))
		return *m_pBackgroundInfo;

	if (m_pBackgroundInfo != NULL)
		delete m_pBackgroundInfo;

	UpdateData ();
	m_pBackgroundInfo = new CBackgroundInfo (m_nBkImageType, m_strImgFilename, NULL, (BkImageAlignmentMode) m_enumMode, m_crTransparent);

	return *m_pBackgroundInfo;
}

void CCWBkImgPage::OnButtonTransparentcolor() 
{
	m_bSettingTransparentColor = true;
}

void CCWBkImgPage::OnButtonRemovetransparent() 
{
	m_crTransparent = -1;
	m_pPic->SetTransparentColor (-1);
	InvalidateRect (NULL);
	m_StaticTransparentCol.ShowWindow (SW_HIDE);
}

BOOL CCWBkImgPage::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bSettingTransparentColor)
	{
		::SetCursor (m_hPickCursor);
		return TRUE;
	}
	else
		return CPropertyPage::OnSetCursor(pWnd, nHitTest, message);
}

void CCWBkImgPage::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bSettingTransparentColor)
	{
		POINT pt;
		::GetCursorPos (&pt);

		HDC hdc = ::GetDC (NULL);
		m_crTransparent = ::GetPixel (hdc, pt.x, pt.y);
		::ReleaseDC (NULL, hdc);

		m_pPic->SetTransparentColor (m_crTransparent);
		InvalidateRect (NULL);

		m_StaticTransparentCol.SetStartColor (m_crTransparent);
		m_StaticTransparentCol.SetEndColor (m_crTransparent);
		m_StaticTransparentCol.ShowWindow (SW_SHOW);

		m_bSettingTransparentColor = false;
	}

	CPropertyPage::OnLButtonDown(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////
// CCWFormatPage property page

CCWFormatPage::CCWFormatPage() : CPropertyPage(CCWFormatPage::IDD)
{
	//{{AFX_DATA_INIT(CCWFormatPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pStyle = NULL;
}

CCWFormatPage::~CCWFormatPage()
{
	::DestroyIcon (m_hIconStyle);

	if (m_pStyle != NULL)
		delete m_pStyle;
}

void CCWFormatPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCWFormatPage)
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_COMBO_STYLE, m_Combo);
	DDX_Control(pDX, IDC_BUTTON_STYLE, m_ButtonStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCWFormatPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCWFormatPage)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnSelchangeComboStyle)
	ON_BN_CLICKED(IDC_BUTTON_STYLE, OnButtonStyle)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCWFormatPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	CStyleTemplates& tmpl = ((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ();
	CStringArray& arrTmpl = tmpl.GetTemplates ();
	for (int i = 0; i < arrTmpl.GetSize (); i++)
		((CComboBox*) GetDlgItem (IDC_COMBO_STYLE))->AddString (arrTmpl.GetAt (i));

	m_ButtonStyle.SetIcon (m_hIconStyle = AfxGetApp ()->LoadIcon (IDI_ICON_STYLE));

	CString s;
	for (int i = 0; i < 4; i++)
	{
		s.LoadString (IDS_COVER + i);
		m_Tab.InsertItem (i, s);
	}
	m_Tab.SetCurSel (IDS_INLAY - IDS_COVER);
	m_RenderType = Inlay;

	if (m_pStyle != NULL)
		((CComboBox*) GetDlgItem (IDC_COMBO_STYLE))->SelectString (-1, m_pStyle->GetName ());
	else
	{
		m_pStyle = new CStyleTemplate ();
		tmpl.LoadTemplate (arrTmpl.GetAt (0), m_pStyle);
		((CComboBox*) GetDlgItem (IDC_COMBO_STYLE))->SetCurSel (0);
	}

	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
		m_tooltip.AddTool (&m_ButtonStyle, IDS_TT_STYLEED);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCWFormatPage::OnSetActive() 
{
	CPropertySheet* pSheet = (CPropertySheet*) GetParent ();
	pSheet->SetWizardButtons (PSWIZB_FINISH | PSWIZB_BACK);
	
	return CPropertyPage::OnSetActive();
}

BOOL CCWFormatPage::OnKillActive() 
{
	GetStyleTemplate ();
	((CCreationWizard*) GetParent ())->m_pStyle = m_pStyle;

	return CPropertyPage::OnKillActive();
}

BOOL CCWFormatPage::OnWizardFinish() 
{
	GetStyleTemplate ();
	((CCreationWizard*) GetParent ())->m_pStyle = m_pStyle;
	
	return CPropertyPage::OnWizardFinish();
}

void CCWFormatPage::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (m_pStyle != NULL)
	{
		CRect r;
		GetDlgItem (IDC_PREVIEW_LABEL)->GetWindowRect (&r);
		ScreenToClient (&r);
		int w = r.Height () - 70;
		if (m_RenderType != Inlay)
			w = (w * 4) / 5;

		CCreationWizard* pCw = (CCreationWizard*) GetParent ();
		CRenderDC rdc (&dc, r.Height () - 70, m_pStyle);
		dc.SetViewportOrg ((r.Width () - w) / 2, r.top + (r.Height () - rdc.GetHeight (m_RenderType)) / 2 - 10);
		CLabelRenderer renderer (&pCw->GetTracksInfo (), &pCw->GetDataCDInfo (), pCw->GetBackgroundInfo (), NULL, *m_pStyle);
		renderer.Render (rdc, m_RenderType);
	}
}

CStyleTemplate& CCWFormatPage::GetStyleTemplate()
{
	if (!::IsWindow (GetSafeHwnd ()))
		return *m_pStyle;

	if (m_pStyle == NULL)
		m_pStyle = new CStyleTemplate ();

	CString s;
	GetDlgItemText (IDC_COMBO_STYLE, s);
	
	((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ().LoadTemplate (s, m_pStyle);

	return *m_pStyle;
}

void CCWFormatPage::SetStyleTemplate(CStyleTemplate &style)
{
	if (m_pStyle != NULL)
		delete m_pStyle;
	m_pStyle = new CStyleTemplate (style);
}

void CCWFormatPage::OnSelchangeComboStyle() 
{
	CString s;
	GetDlgItemText (IDC_COMBO_STYLE, s);

	if (m_pStyle == NULL)
		m_pStyle = new CStyleTemplate ();
	((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ().LoadTemplate (s, m_pStyle);

	InvalidateRect (NULL);
}

void CCWFormatPage::OnButtonStyle() 
{
	CStyleTemplateWizard propSheet;
	GetDlgItemText (IDC_COMBO_STYLE, propSheet.m_strCurrentStyleName);
	propSheet.DoModal();
}

void CCWFormatPage::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_RenderType = (RenderType) m_Tab.GetCurSel ();

	CRect r;
	GetDlgItem (IDC_PREVIEW_LABEL)->GetWindowRect (&r);
	ScreenToClient (&r);
	r.DeflateRect (20, 20);
	InvalidateRect (&r);

	*pResult = 0;
}

BOOL CCWBkImgPage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

BOOL CCWFormatPage::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CPropertyPage::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}