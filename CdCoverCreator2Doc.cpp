// CdCoverCreator2Doc.cpp : implementation of the CCdCoverCreator2Doc class
//

#include "stdafx.h"
#include "CdCoverCreator2.h"

#include "CdCoverCreator2Doc.h"

#include "MainFrm.h"
#include "ImagePreviewFileDialogEx.h"
#include "CdCoverCreator2Doc.h"
#include "CdCoverCreator2View.h"
#include "CreationWizard.h"
#include "StyleTemplateWizard.h"
#include "StyleTemplates.h"
#include "WebCdCoversBrowser1.h"
#include "M3UPlaylist.h"

#include "FloatingGraphicsObject.h"
#include "FloatingText.h"
#include "FloatingImage.h"
#include "FloatingBarcode.h"

#include "cccTwain.h"
#include "Lightscribe.h"
#include ".\cdcovercreator2doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CURRENT_VERSION 9

// CCdCoverCreator2Doc

IMPLEMENT_DYNCREATE(CCdCoverCreator2Doc, CDocument)

BEGIN_MESSAGE_MAP(CCdCoverCreator2Doc, CDocument)
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_BKIMG_IMAGE, OnBkimgImage)
	ON_COMMAND(ID_BKIMG_SCAN, OnBkimgScan)
	ON_COMMAND(ID_FILE_LIGHTSCRIBE, OnFileLightscribe)
END_MESSAGE_MAP()


// CCdCoverCreator2Doc construction/destruction

CCdCoverCreator2Doc::CCdCoverCreator2Doc()
{
	m_pTracks = NULL;
	m_pDataCDInfo = NULL;
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		m_pBackground[i] = NULL;
	m_pStyle = NULL;
}

CCdCoverCreator2Doc::~CCdCoverCreator2Doc()
{
	if (m_pTracks != NULL)
		delete m_pTracks;
	m_pTracks = NULL;

	if (m_pDataCDInfo != NULL)
		delete m_pDataCDInfo;
	m_pDataCDInfo = NULL;

	for (int i = 0; i < NUM_RENDER_TYPES; i++)
	{
		if (m_pBackground[i] != NULL)
			delete m_pBackground[i];
		m_pBackground[i] = NULL;

		for (std::list<CFloatingGraphicsObject*>::iterator it = m_listFloatingObjects[i].begin (); it != m_listFloatingObjects[i].end (); it++)
			delete *it;
		m_listFloatingObjects[i].clear ();
	}

	if (m_pStyle != NULL)
		delete m_pStyle;
	m_pStyle = NULL;
}

BOOL CCdCoverCreator2Doc::OnNewDocument()
{
	m_bCreating = true;

	// do not make new doc on init of the app
	if (((CCdCoverCreator2App*) AfxGetApp ())->m_bFirstDoc)
	{
		((CCdCoverCreator2App*) AfxGetApp ())->m_bFirstDoc = false;
		return FALSE;
	}

	if (m_pTracks != NULL)
		delete m_pTracks;
	m_pTracks = NULL;

	if (m_pDataCDInfo != NULL)
		delete m_pDataCDInfo;
	m_pDataCDInfo = NULL;

	for (int i = 0; i < NUM_RENDER_TYPES; i++)
	{
		if (m_pBackground[i] != NULL)
			delete m_pBackground[i];
		m_pBackground[i] = NULL;
	}

	if (m_pStyle != NULL)
		delete m_pStyle;
	m_pStyle = NULL;

	switch (((CCdCoverCreator2App*) AfxGetApp ())->m_NewDocMode)
	{
	case Empty:
		{
			m_pTracks = new CTracksInfo ();
			for (int i = 0; i < NUM_RENDER_TYPES; i++)
				m_pBackground[i] = new CBackgroundInfo ((RenderType) i, "", NULL, BIAM_CENTERCENTER, -1);
			m_pStyle = new CStyleTemplate ("Empty");
		}

		CheckPoint (false);
		m_bCreating = false;
		return TRUE;

	case AudioCD:
	case DataCD:
		if (Properties (((CCdCoverCreator2App*) AfxGetApp ())->m_NewDocMode))
		{
			if (!CDocument::OnNewDocument())
			{
				m_bCreating = false;
				return FALSE;
			}

			CheckPoint ();
			m_bCreating = false;

			ApplyStyleToRTFTitles (m_pStyle);

			return TRUE;
		}
		break;

	case Web:
		{
			m_pTracks = new CTracksInfo ();
			m_pStyle = new CStyleTemplate ("Empty");
				for (int i = 0; i < NUM_RENDER_TYPES - 1; i++)
					m_pBackground[i] = new CBackgroundInfo ((RenderType) i);

			// show the cover browser
			((CMainFrame*) AfxGetMainWnd ())->m_wndLeftPanel.m_pLastDoc = this;
			((CMainFrame*) AfxGetMainWnd ())->m_bmCoverBrowserMode = BrowserSearch;
			((CMainFrame*) AfxGetMainWnd ())->OnViewCoverbrowser ();
		}

		CheckPoint ();
		m_bCreating = false;
		return TRUE;

	case ImportM3U:
		{
			m_pTracks = new CTracksInfo ();
			m_pStyle = new CStyleTemplate ("Azur");
			for (int i = 0; i < NUM_RENDER_TYPES - 1; i++)
				m_pBackground[i] = new CBackgroundInfo ((RenderType) i);

			CString strFilename;
			strFilename.LoadString (IDS_OPENM3U_FILENAME);
			CString strFilter;
			strFilter.LoadString (IDS_OPENM3U_FILTER);

			CFileDialog dlg (TRUE, NULL, strFilename, 0, strFilter, AfxGetMainWnd ());
			if (dlg.DoModal () == IDOK)
			{
				// read the playlist
				AfxGetApp ()->BeginWaitCursor ();

				CM3UPlaylist pl (dlg.GetPathName ());

				// set titles
				m_pTracks->SetTitle (pl.m_strName);
				SetTitle (pl.m_strName);

				int n = 0;
				CString strRemark, strLength;

				for (std::list<CPlaylistEntry*>::iterator it = pl.m_List.begin (); it != pl.m_List.end (); it++)
				{
					// create the remark consisting of artist and (album, year), if available
					strRemark = (*it)->m_strArtist;
					if (!(*it)->m_strAlbum.IsEmpty () || !(*it)->m_strYear.IsEmpty ())
					{
						strRemark += " (";
						if (!(*it)->m_strAlbum.IsEmpty ())
							strRemark += (*it)->m_strAlbum + ", ";
						strRemark += (*it)->m_strYear + ")";
					}

					// format the length of the track
					strLength.Format ("%02d:%02d", (*it)->m_nLengthSec / 60, (*it)->m_nLengthSec % 60);

					// add to the track's list
					m_pTracks->Add (new CTrack (++n, strLength, (*it)->m_strTitle, strRemark));
				}

				AfxGetApp ()->EndWaitCursor ();

				CheckPoint ();
				m_bCreating = false;
			
				ApplyStyleToRTFTitles (m_pStyle);

				return TRUE;
			}
		}

		m_bCreating = false;
		return FALSE;
	}

	m_bCreating = false;
	return FALSE;
}


// CCdCoverCreator2Doc serialization

void CCdCoverCreator2Doc::Serialize(CArchive& ar)
{
	ar.m_pDocument = this;

	// instantiate objects
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
	{
		if (m_pBackground[i] == NULL)
			m_pBackground[i] = new CBackgroundInfo ((RenderType) i);
	}
	if (m_pStyle == NULL)
		m_pStyle = new CStyleTemplate ();

	// version number
	DWORD dwVer = CURRENT_VERSION;
	if (ar.IsLoading ())
	{
		ar.GetFile ()->Read (&dwVer, sizeof (DWORD));
		if (dwVer >= 16)
		{
			// version 1 had no version number, so
			// - dwVer is >= 16
			// - file must be reset to start position
			ar.GetFile ()->SeekToBegin ();
			//ar.SetObjectSchema (1);
			m_nObjectSchema = 1;
		}
		else
			m_nObjectSchema = dwVer;
	}
	else
	{
		ar << dwVer;
		m_nObjectSchema = CURRENT_VERSION;

		// save the contents via the view
		POSITION pos = GetFirstViewPosition ();
		if ((pos != NULL) && !m_bCreating)
		{
			CCdCoverCreator2View* pView = (CCdCoverCreator2View*) GetNextView (pos);
			if (pView != NULL)
				pView->ShowRenderType (pView->GetRenderType ());
		}
	}

	/////////////////////////////////////////////////////////////////
	// Serialization

	// serialize backgrounds
	// version 1: only inlay background
	// version 2: cover, label and inlay background

	int nNumRenderTypes = m_nObjectSchema <= 3 ? 4 : NUM_RENDER_TYPES;

	if (m_nObjectSchema == 1)
	{
		// serialize tracks
		if (m_pTracks == NULL)
			m_pTracks = new CTracksInfo ();
		m_pTracks->Serialize (ar);

		// serialize backgrounds
		m_pBackground[Inlay]->Serialize (ar);

		// copy inlay background to other backgrounds
		*m_pBackground[Cover] = *m_pBackground[Inlay];
		*m_pBackground[Back] = *m_pBackground[Inlay];
		*m_pBackground[Label] = *m_pBackground[Inlay];

		m_pBackground[Cover]->SetRenderType (Cover);
		m_pBackground[Back]->SetRenderType (Back);
		m_pBackground[Label]->SetRenderType (Label);

		// serialize style
		m_pStyle->Serialize (ar);
	}
	else
	{
		// serialize tracks
		int nMode;
		if (ar.IsLoading ())
		{
			if (m_nObjectSchema >= 6)
				ar >> nMode;
			else
				nMode = (int) AudioCD;
		}
		else
		{
			nMode = m_pTracks == NULL ? DataCD : AudioCD;
			ar << nMode;
		}

		switch (nMode)
		{
		case (int) AudioCD:
			if (m_pTracks == NULL)
				m_pTracks = new CTracksInfo ();
			m_pTracks->Serialize (ar);
			break;

		case (int) DataCD:
			if (m_pDataCDInfo == NULL)
				m_pDataCDInfo = new CDataCDInfo ();
			m_pDataCDInfo->Serialize (ar);
			break;
		}

		// serialize backgrounds
		for (int i = 0; i < nNumRenderTypes; i++)
			m_pBackground[i]->Serialize (ar);

		// serialize style
		m_pStyle->Serialize (ar);
	}

	if (ar.IsStoring ())
	{
		ar << m_strBookletRTFText;

		// serialize floating objects
		int nType;
		for (int i = 0; i < NUM_RENDER_TYPES; i++)
		{
			ar << (int) m_listFloatingObjects[i].size ();
			for (std::list<CFloatingGraphicsObject*>::iterator it = m_listFloatingObjects[i].begin (); it != m_listFloatingObjects[i].end (); it++)
			{

				if ((*it)->IsKindOf (RUNTIME_CLASS (CFloatingText)))
					nType = 0;
				else if ((*it)->IsKindOf (RUNTIME_CLASS (CFloatingImage)))
					nType = 1;
				else if ((*it)->IsKindOf (RUNTIME_CLASS (CFloatingBarCode)))
					nType = 2;
				else
					nType = -1;

				ar << nType;
				(*it)->Serialize (ar);
			}
		}

		// medium type
		nType = (int) m_pStyle->GetMediumType ();
		ar << nType;
	}
	else
	{
		if (m_nObjectSchema >= 4)
			ar >> m_strBookletRTFText;

		// serialize floating objects
		if (m_nObjectSchema >= 6)
		{
			int nObjCount, nType;
			CFloatingGraphicsObject* pObj;

			for (int i = 0; i < NUM_RENDER_TYPES; i++)
			{
				// clear the object list
				for (std::list<CFloatingGraphicsObject*>::iterator it = m_listFloatingObjects[i].begin (); it != m_listFloatingObjects[i].end (); it++)
					delete *it;
				m_listFloatingObjects[i].clear ();

				// load the objects
				ar >> nObjCount;
				for (int j = 0; j < nObjCount; j++)
				{
					ar >> nType;
					switch (nType)
					{
					case 0:
						pObj = new CFloatingText ();
						break;
					case 1:
						pObj = new CFloatingImage ();
						break;
					case 2:
						pObj = new CFloatingBarCode ();
						break;
					}

					pObj->Serialize (ar);
					m_listFloatingObjects[i].push_back (pObj);
				}
			}

			// medium type
			ar >> nType;
			m_pStyle->SetMeasures ((MediumType) nType);
		}
	}

	if (ar.IsLoading ())
	{
		DisplayStyle ();
		DisplayMediumType ();
	}
}


// CCdCoverCreator2Doc diagnostics

#ifdef _DEBUG
void CCdCoverCreator2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCdCoverCreator2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCdCoverCreator2Doc commands

bool CCdCoverCreator2Doc::Properties (NewDocMode mode)
{
#ifdef _DEBUG
    CMemoryState oldMemState, newMemState, diffMemState;
    oldMemState.Checkpoint ();
#endif

	CCreationWizard wizard (mode);
	bool bSetCheckPoint = false;

	// set tracks
	if (m_pTracks != NULL)
		wizard.SetTracksInfo (*m_pTracks);

	// set data CD info
	if (m_pDataCDInfo != NULL)
		wizard.SetDataCDInfo (*m_pDataCDInfo);

	// set backgrounds
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		if (m_pBackground[i] != NULL)
			wizard.SetBackgroundInfo (*m_pBackground[i], (RenderType) i);

	// set style
	if (m_pStyle != NULL)
		wizard.SetStyleTemplate (*m_pStyle);

	bool bRet = wizard.DoModal () == ID_WIZFINISH;
	if (bRet)
	{
		CTracksInfo* pTiNew = mode == AudioCD ? new CTracksInfo (wizard.GetTracksInfo ()) : NULL;
		CDataCDInfo* pDCdiNew = mode == DataCD ? new CDataCDInfo (wizard.GetDataCDInfo ()) : NULL;

		CBackgroundInfo* pBiNew[NUM_RENDER_TYPES];
		for (int i = 0; i < NUM_RENDER_TYPES; i++)
		{
			CBackgroundInfo* pBkInfo = wizard.GetBackgroundInfo ()[i];
			if (pBkInfo != NULL)
				pBiNew[i] = new CBackgroundInfo (*pBkInfo);
			else
				pBiNew[i] = new CBackgroundInfo ((RenderType) i);
		}

		CStyleTemplate* pStNew = new CStyleTemplate (wizard.GetStyleTemplate ());

		// set new data and set modified flag, if data has changed

		// tracks
		if (m_pTracks == NULL)
		{
			bSetCheckPoint = true;
			SetModifiedFlag ();
		}
		else
		{
			if (*pTiNew != *m_pTracks)
			{
				bSetCheckPoint = true;
				SetModifiedFlag ();
			}
			delete m_pTracks;
		}
		m_pTracks = pTiNew;

		// data CD info
		if (m_pDataCDInfo == NULL)
		{
			bSetCheckPoint = true;
			SetModifiedFlag ();
		}
		else
		{
			if (*pDCdiNew != *m_pDataCDInfo)
			{
				bSetCheckPoint = true;
				SetModifiedFlag ();
			}
			delete m_pDataCDInfo;
		}
		m_pDataCDInfo = pDCdiNew;

		for (int i = 0; i < NUM_RENDER_TYPES; i++)
		{
			if (m_pBackground[i] == NULL)
			{
				bSetCheckPoint = true;
				SetModifiedFlag ();
			}
			else
			{
				if (*pBiNew[i] != *m_pBackground[i])
				{
					bSetCheckPoint = true;
					SetModifiedFlag ();
				}
				delete m_pBackground[i];
			}
			m_pBackground[i] = pBiNew[i];
		}

		if (m_pStyle == NULL)
		{
			bSetCheckPoint = true;
			SetModifiedFlag ();
		}
		else
		{
			if (*pStNew != *m_pStyle)
			{
				bSetCheckPoint = true;
				SetModifiedFlag ();
			}
			delete m_pStyle;
		}
		m_pStyle = pStNew;

		// refresh display
		UpdateAllViews (NULL);

		// set default doc title if document has not been safed yet
		if (GetPathName ().IsEmpty ())
		{
			if (m_pTracks != NULL)
				SetTitle (m_pTracks->GetTitle ().SpanExcluding (":"));
			else if (m_pDataCDInfo != NULL)
				SetTitle (m_pDataCDInfo->m_strCDTitle);
		}
	}

#ifdef _DEBUG
    newMemState.Checkpoint ();
//	if (diffMemState.Difference (oldMemState, newMemState))
//		diffMemState.DumpAllObjectsSince ();
#endif

	if (bSetCheckPoint)
		CheckPoint ();

	DisplayStyle ();
	DisplayMediumType ();

	return bRet;
}

void CCdCoverCreator2Doc::OnProperties() 
{
	Properties (m_pTracks == NULL ? DataCD : AudioCD);
}

void CCdCoverCreator2Doc::SetStyle(CString strStyle)
{
	// save old style sheet for checking whether the style has changed
	CStyleTemplate* pStNew = NULL;
	bool bSetCheckPoint = false;

	if (m_pStyle == NULL)
	{
		m_pStyle = new CStyleTemplate ();
		bSetCheckPoint = true;
		SetModifiedFlag ();
	}
	else
		pStNew = new CStyleTemplate (*m_pStyle);

	// load new template
	((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ().LoadTemplate (strStyle, m_pStyle);

	if (pStNew != NULL)
	{
		if (*pStNew != *m_pStyle)
		{
			bSetCheckPoint = true;
			SetModifiedFlag ();

			// RTF title must be re-formatted
			// (if it is empty, it will be done in CTracksInfo::DrawTitle2)
//			for (int i = 0; i < NUM_RENDER_TYPES; i++)
//				m_pTracks->SetRTFTitle ("", (RenderType) i);
			ApplyStyleToRTFTitles (m_pStyle);
		}

		delete pStNew;
	}

	UpdateAllViews (NULL);

	// update the style palette window
	POSITION pos = GetFirstViewPosition ();
	CCdCoverCreator2View* pView = (CCdCoverCreator2View*) GetNextView (pos);
	RenderType rt = pView->GetRenderType ();
	((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (m_pStyle, m_pBackground[rt], rt);

	if (bSetCheckPoint)
		CheckPoint ();
}

void CCdCoverCreator2Doc::SetMediumType (MediumType type)
{
	if (m_pStyle->GetMediumType () == type)
		return;

	m_pStyle->SetMeasures (type);

	// switch to the next available render type
	POSITION pos = GetFirstViewPosition ();
	CCdCoverCreator2View* pView = (CCdCoverCreator2View*) GetNextView (pos);
	RenderType rt = pView->GetRenderType ();
	int i = 1;
	while (!m_pStyle->m_bHasRenderType[rt])
	{
		rt = (RenderType) (((int) rt + 1) % NUM_RENDER_TYPES);

		i++;
		if (i == NUM_RENDER_TYPES)
			break;
	}

	// switch to the render type or update the views, if still the same render type
	if (rt != pView->GetRenderType ())
		pView->ShowRenderType (rt);
	else
		UpdateAllViews (NULL);

	// update the left panel
	((CMainFrame*) AfxGetMainWnd ())->m_wndLeftPanel.Update (true);

	CheckPoint ();
}

void CCdCoverCreator2Doc::UpdateStyle()
{
	if (m_pStyle != NULL)
		SetStyle (m_pStyle->GetName ());
}

BOOL CCdCoverCreator2Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	// serialize...
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// template name may have been changed during loading:
	// if new template is created under new name
	if (m_pStyle != NULL)
		if (m_pStyle->IsTemplateNameChanged ())
			SetModifiedFlag ();

	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		if (m_pBackground[i] != NULL)
			if (m_pBackground[i]->IsImgFilenameChanged () || m_pBackground[i]->m_pAlphaTexture->IsImgFilenameChanged ())
			{
				SetModifiedFlag ();
				break;	// exit loop (it suffices to set the modified flag once...)
			}
	
	CheckPoint (false);
	return TRUE;
}

void CCdCoverCreator2Doc::DisplayStyle()
{
	// update the style display in the dlgbar combo
	((CMainFrame*) AfxGetMainWnd ())->ListStyles (m_pStyle ? m_pStyle->GetName () : "");

	// update the style palette window
	POSITION pos = GetFirstViewPosition ();
	CCdCoverCreator2View* pView = (CCdCoverCreator2View*) GetNextView (pos);
	RenderType rt = pView->GetRenderType ();
	((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (m_pStyle, m_pBackground[rt], rt);
}

void CCdCoverCreator2Doc::DisplayMediumType ()
{
	if (m_pStyle != NULL)
		((CMainFrame*) AfxGetMainWnd ())->m_wndDlgBarSelectMedium.SetMediumType (m_pStyle->GetMediumType ());
}

void CCdCoverCreator2Doc::OnBkimgImage() 
{
	CString strFilename;
	strFilename.LoadString (IDS_OPENBKIMGFILE_FILENAME);
	CString strFilter;
	strFilter.LoadString (IDS_OPENBKIMGFILE_FILTER);

	POSITION pos = GetFirstViewPosition ();
	CCdCoverCreator2View* pView = (CCdCoverCreator2View*) GetNextView (pos);

	CImagePreviewFileDialogEx dlg (TRUE, NULL, strFilename, 0, strFilter, pView);
	if (dlg.DoModal () == IDOK)
		if (pView != NULL)
			SetBkImageFilename (dlg.GetPathName ());
}

void CCdCoverCreator2Doc::OnBkimgScan() 
{
	CcccTwain* pTwain = &((CMainFrame*) AfxGetApp ()->GetMainWnd ())->m_twain;
	pTwain->m_pBkImgPage = NULL;
	pTwain->SelectSource ();
	if (pTwain->SourceSelected ())
		pTwain->Acquire ();
}

bool CCdCoverCreator2Doc::SetBkImageFilename(CString strFilename)
{
	bool bResult = false;

	POSITION pos = GetFirstViewPosition ();
	CCdCoverCreator2View* pView = (CCdCoverCreator2View*) GetNextView (pos);
	if (pView != NULL)
	{
		RenderType rt = pView->GetRenderType ();
		bResult = m_pBackground[rt]->SetImage (strFilename);

		((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (m_pStyle, m_pBackground[rt], rt);
		UpdateAllViews (NULL);

		OFSTRUCT of;
		if ((::OpenFile (strFilename, &of, OF_EXIST) != HFILE_ERROR) || strFilename.IsEmpty ())
			CheckPoint ();

		SetModifiedFlag ();
	}

	return bResult;
}

void CCdCoverCreator2Doc::SetBkAlphaFilename(CString strFilename)
{
	POSITION pos = GetFirstViewPosition ();
	CCdCoverCreator2View* pView = (CCdCoverCreator2View*) GetNextView (pos);
	if (pView != NULL)
	{
		RenderType rt = pView->GetRenderType ();
		CBackgroundAlphaTexture* pBkAlpha = m_pBackground[rt]->m_pAlphaTexture;
		int nAlphaFormat = pBkAlpha->m_nAlphaFormat;
		int nSCnstAlpha = pBkAlpha->m_nSourceConstantAlpha;
		delete pBkAlpha;
		m_pBackground[rt]->m_pAlphaTexture = new CBackgroundAlphaTexture (strFilename, nAlphaFormat, nSCnstAlpha);
		((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (m_pStyle, m_pBackground[rt], rt);

		UpdateAllViews (NULL);

		OFSTRUCT of;
		if ((::OpenFile (strFilename, &of, OF_EXIST) != HFILE_ERROR) || strFilename.IsEmpty ())
			CheckPoint ();

		SetModifiedFlag ();
	}
}

void CCdCoverCreator2Doc::OnCloseDocument() 
{
	// update style dlg, left panel
	((CMainFrame*) AfxGetMainWnd ())->UpdateStyleDlgs (NULL, NULL, (RenderType) -1);
	
	CDocument::OnCloseDocument();
}

bool CCdCoverCreator2Doc::SetRTFTitle (CString strRTFTitle, RenderType rt)
{
	if (m_pTracks != NULL)
		return m_pTracks->SetRTFTitle (strRTFTitle, rt);
	if (m_pDataCDInfo != NULL)
		return m_pDataCDInfo->SetRTFTitle (strRTFTitle, rt);

	return false;
}

CString CCdCoverCreator2Doc::GetRTFTitle (RenderType rt)
{
	if (m_pTracks != NULL)
		return m_pTracks->GetRTFTitle (rt);
	if (m_pDataCDInfo != NULL)
		return m_pDataCDInfo->GetRTFTitle (rt);

	return "";
}

void CCdCoverCreator2Doc::ApplyStyleToRTFTitles (CStyleTemplate* pStyle, RenderType rt)
{
	if (m_pTracks != NULL)
		m_pTracks->ApplyStyleToRTFTitles (pStyle, rt);

	if (m_pDataCDInfo != NULL)
		m_pDataCDInfo->ApplyStyleToRTFTitles (pStyle, rt);
}

void CCdCoverCreator2Doc::OnFileLightscribe()
{
	CSize szUnit (236, 236);	// 2834x2834 Pixel^2 bitmap equals a 12x12 cm^2 square

	// save the contents via the view
	POSITION pos = GetFirstViewPosition ();
	if (pos != NULL)
	{
		CCdCoverCreator2View* pView = (CCdCoverCreator2View*) GetNextView (pos);
		if (pView != NULL)
		{
			CDC* pDC = pView->GetDC ();

			int nHeight = (int) ((m_pStyle->GetTotalCoverRect ()->Height () * szUnit.cy) / 1000);
			BITMAPINFOHEADER* pBi = new BITMAPINFOHEADER;
			BYTE* pBitmapBits;

			pBi->biSize = sizeof (BITMAPINFOHEADER);
			pBi->biBitCount = 24;
			pBi->biPlanes = 1;
			pBi->biCompression = BI_RGB;
			pBi->biWidth = nHeight;
			pBi->biHeight = nHeight;
			pBi->biSizeImage = ((((pBi->biWidth * pBi->biBitCount) + 31) & ~31) >> 3) * pBi->biHeight;
			pBi->biXPelsPerMeter = 24139;
			pBi->biYPelsPerMeter = 24139;
			pBi->biClrUsed = 0;
			pBi->biClrImportant = 0;
			HBITMAP hBmp = CreateDIBSection (pDC->GetSafeHdc (), (CONST BITMAPINFO*) pBi, DIB_RGB_COLORS, (void**) &pBitmapBits, NULL, 0);

			CDC memDC;
			memDC.CreateCompatibleDC (pDC);
			HBITMAP hOldBmp = (HBITMAP) SelectObject (memDC.GetSafeHdc (), hBmp);
			memDC.PatBlt (0, 0, nHeight, nHeight, WHITENESS);

			CRenderDC rdc (&memDC, nHeight, m_pStyle);

			CLabelRenderer renderer (m_pTracks, m_pDataCDInfo, m_pBackground, m_listFloatingObjects, *m_pStyle);
			renderer.Render (rdc, Label, false, true, false, false);

			SelectObject (memDC.GetSafeHdc (), hOldBmp);
			memDC.DeleteDC ();
			pView->ReleaseDC (pDC);

			new CLightscribe (hBmp, pBi, pBitmapBits);
		}
	}
}