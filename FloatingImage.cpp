#include "StdAfx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"
#include "floatingimage.h"

#include "ImagePreviewFileDialogEx.h"

IMPLEMENT_DYNCREATE(CFloatingImage, CFloatingGraphicsObject)

CFloatingImage::CFloatingImage(void)
{
	m_pPicture = NULL;
	m_nContextMenuID = IDR_MENU_FLOATINGIMAGE;

	m_tracker.KeepAspectRatio (true);
}

CFloatingImage::~CFloatingImage(void)
{
	if (m_pPicture != NULL)
		delete m_pPicture;
}

void CFloatingImage::Render (CRenderDC rdc, RenderType rt, CRect rectRender, bool bIsPrinting)
{
	if (m_pPicture != NULL)
		m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, rectRender.left, rectRender.bottom, rectRender.Width (), rectRender.Height (), bIsPrinting);
}

bool CFloatingImage::Add (CWnd* pWnd, CPoint pt, RenderType rt, CRenderDC rdc, CStyleTemplate* pStyle, CAutoRichEditCtrl* pEditCtrl, CSize sizeOffset)
{
	CFloatingGraphicsObject::Add (pWnd, pt, rt, rdc, pStyle, pEditCtrl, sizeOffset);

	CString strFilename;
	strFilename.LoadString (IDS_OPENBKIMGFILE_FILENAME);
	CString strFilter;
	strFilter.LoadString (IDS_OPENBKIMGFILE_FILTER);

	//CString strInitialDir = ((CCdCoverCreator2App*) AfxGetApp ())->GetTextureDir ();
	CImagePreviewFileDialogEx dlg (TRUE, NULL, /*strInitialDir +*/ strFilename, 0, strFilter, AfxGetMainWnd ());
	//dlg.m_ofn.lpstrInitialDir = strInitialDir.Left (strInitialDir.GetLength () - 1);

	if (dlg.DoModal () == IDOK)
	{
		if (m_pPicture != NULL)
			delete m_pPicture;

		m_pPicture = new CPicture (m_strImgFilename = dlg.GetPathName ());

		CSize size = m_pPicture->GetSize ();
		m_dWidth = size.cx * 0.02;
		m_dHeight = size.cy * 0.02;
		//m_tracker.m_rect.right = m_tracker.m_rect.left + size.cx;
		//m_tracker.m_rect.bottom = m_tracker.m_rect.top + size.cy;

		return true;
	}

	return false;
}

void CFloatingImage::SetTransparentColor (COLORREF crTransparent)
{
	m_crTransparent = crTransparent;
	if (m_pPicture != NULL)
		m_pPicture->SetTransparentColor (crTransparent);
}

void CFloatingImage::Serialize (CArchive& ar)
{
	CFloatingGraphicsObject::Serialize (ar);

	if (ar.IsStoring ())
	{
		ar << m_strImgFilename;
		ar << m_crTransparent;

		//## VER 7 begin
		ar << m_pPicture->GetContrast ();
		ar << m_pPicture->GetBrightness ();
		ar << m_pPicture->GetSaturation ();
		ar << m_pPicture->GetHue ();
		ar << m_pPicture->GetGamma ();
		//## end VER 7

		if (!m_strImgFilename.IsEmpty () && !((CCdCoverCreator2Doc*) ar.m_pDocument)->IsSerializingFromCUndo ())
		{
			CFile f (m_strImgFilename, CFile::modeRead | CFile::shareDenyWrite);
			DWORD dwCount = f.GetLength ();

			BYTE* pBuf = new BYTE[f.GetLength ()];
			f.Read (pBuf, dwCount);

			ar << dwCount;
			ar.Write (pBuf, dwCount);

			delete pBuf;
		}
	}
	else
	{
		ar >> m_strImgFilename;
		ar >> m_crTransparent;

		//## VER 7 begin
		double fContrast = 1.0;
		double fBrightness = 0.0;
		double fSaturation = 1.0;
		double fHue = 0.0;
		double fGamma = 1.0;
		if (((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 7)
		{
			ar >> fContrast >> fBrightness >> fSaturation >> fHue;
			ar >> fGamma;
		}
		//## end VER 7

		if (!m_strImgFilename.IsEmpty ())
		{
			if (!((CCdCoverCreator2Doc*) ar.m_pDocument)->IsSerializingFromCUndo ())
			{
				DWORD dwLen;
				ar >> dwLen;

				BYTE* pBuf = new BYTE[dwLen];
				ar.Read (pBuf, dwLen);

				OFSTRUCT of;
				if (::OpenFile (m_strImgFilename, &of, OF_EXIST) == HFILE_ERROR)
				{
					// image file does not exist.
					// read from file and create image file

					CString strPath = ar.GetFile ()->GetFilePath ();
					m_strImgFilename = strPath.Left (strPath.ReverseFind ('\\')) +
						m_strImgFilename.Mid (m_strImgFilename.ReverseFind ('\\'));

					if (::OpenFile (m_strImgFilename, &of, OF_EXIST) == HFILE_ERROR)
					{
						CFile f (m_strImgFilename, CFile::modeCreate | CFile::modeWrite);
						f.Write (pBuf, dwLen);
						f.Close ();
					}

	//				m_bImgFilenameChanged = true;
				}

				delete pBuf;
			}

			if (m_pPicture != NULL)
				delete m_pPicture;
			
			m_pPicture = new CPicture (m_strImgFilename);
			m_pPicture->AdjustColors (fContrast, fBrightness, fSaturation, fHue);
			m_pPicture->SetGamma (fGamma);
		}
	}
}