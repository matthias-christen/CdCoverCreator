// BackgroundInfo.cpp: implementation of the CBackgroundInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"
#include "BackgroundInfo.h"

#include "GradientRenderer.h"
#include "BitmapPrinter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL (CBackgroundInfo, CObject, VERSIONABLE_SCHEMA | 1)

CBackgroundInfo::CBackgroundInfo()
{
	m_pPicture = NULL;
	m_enumMode = BIAM_CENTERCENTER;
	m_bImgFilenameChanged = false;
	m_crTransparent = -1;
	m_RenderType = (RenderType) 0;
	m_pAlphaTexture = new CBackgroundAlphaTexture ();
}

CBackgroundInfo::CBackgroundInfo(RenderType rt)
{
	m_pPicture = NULL;
	m_enumMode = BIAM_CENTERCENTER;
	m_bImgFilenameChanged = false;
	m_crTransparent = -1;
	m_RenderType = rt;
	m_pAlphaTexture = new CBackgroundAlphaTexture ();
}

CBackgroundInfo::CBackgroundInfo(CBackgroundInfo& bkgrnd)
{
	*this = bkgrnd;	
	m_bImgFilenameChanged = false;
	m_pAlphaTexture = new CBackgroundAlphaTexture ();
}

CBackgroundInfo::CBackgroundInfo (RenderType rt, CString strImg, CPicture* pPic, BkImageAlignmentMode biam, COLORREF crTransparent)
{
	m_RenderType = rt;
	m_strImgFilename = strImg;

	if (pPic == NULL)
	{
		m_pPicture = new CPicture (strImg);
		m_pPicture->SetTransparentColor (crTransparent);
		m_crTransparent = crTransparent;
	}
	else
	{
		m_pPicture = pPic;
		m_crTransparent = pPic->GetTransparentColor ();
	}

	m_enumMode = biam;

	m_bImgFilenameChanged = false;
	m_pAlphaTexture = new CBackgroundAlphaTexture ();
}

CBackgroundInfo::~CBackgroundInfo()
{
	if (m_pPicture != NULL)
		delete m_pPicture;
	if (m_pAlphaTexture != NULL)
		delete m_pAlphaTexture;
}

CBackgroundInfo& CBackgroundInfo::operator =(CBackgroundInfo& bi)
{
	m_strImgFilename = bi.GetImageFilename ();
	m_RenderType = bi.GetRenderType ();
	m_pPicture = new CPicture (m_strImgFilename);
	m_crTransparent = bi.GetTransparentColor ();
	m_pPicture->SetTransparentColor (m_crTransparent);
	m_enumMode = bi.GetAlignmentMode ();

	return *this;
}

void CBackgroundInfo::Render(CRenderDC& rdc, CStyleTemplate& style, bool bIsPrinting)
{
	CCdCoverCreator2App* pApp = (CCdCoverCreator2App*) AfxGetApp ();

	CRect r, rcS1, rcS2;

	switch (m_RenderType)
	{
	case Cover:
		r = *style.GetTotalCoverRect (rdc);
		rcS1 = rcS2 = r;
		rcS1.left = rcS1.right = r.left;
		rcS2.left = rcS2.right = r.right;
		break;

	case Back:
		r = *style.GetTotalCoverRect (rdc);
		rcS1 = rcS2 = r;
		rcS1.left = rcS1.right = r.left;
		rcS2.left = rcS2.right = r.right;
		break;

	case Inlay:
		r = *style.GetTotalInlayRect (rdc);
		rcS1 = *style.GetInlayLeftSideRect (rdc);
		rcS2 = *style.GetInlayRightSideRect (rdc);
		break;

	case Label:
		r = *style.GetTotalLabelRect (rdc);
		rcS1 = rcS2 = r;
		rcS1.left = rcS1.right = r.left;
		rcS2.left = rcS2.right = r.right;
		break;
	}

//	COLORREF crBkColorStart = style.GetBkColor (m_RenderType);
//	COLORREF crBkColorEnd = style.GetBkColor2 (m_RenderType);
//	int nAngle = style.GetBkAngle (m_RenderType);

	bool bPreview = rdc.m_pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC));

	CRect rcGradient = r;

	if (((m_enumMode != BIAM_STRETCH) && (m_enumMode != BIAM_TILE)) ||
		(((m_enumMode == BIAM_STRETCH) || (m_enumMode == BIAM_TILE)) && (m_crTransparent != -1)))
	{
		CGradientRenderer::DrawGradient (rdc.m_pDC, rdc.m_hndPrinter, rcGradient, style.GetGradientData (m_RenderType),
			rdc.m_pDC->IsKindOf (RUNTIME_CLASS (CPreviewDC)) ? (CPreviewDC*) rdc.m_pDC : NULL);
	}

	if (m_pPicture != NULL)
	{
		m_pPicture->SetTransparentColor (m_crTransparent);

		CSize szPic = m_pPicture->GetSize ();
		szPic.cx = style.MapX (szPic.cx * 20, rdc, m_RenderType);
		szPic.cy = style.MapY (szPic.cy * 20, rdc, m_RenderType);
		double dFctX = min (1.0, rdc.GetWidth (m_RenderType) / (double) szPic.cx);
		double dFctY = min (1.0, rdc.GetHeight (m_RenderType) / (double) szPic.cy);
		szPic.cx = (int) (szPic.cx * min (dFctX, dFctY));
		szPic.cy = (int) (szPic.cy * min (dFctX, dFctY));

		switch (m_enumMode)
		{
		case BIAM_TOPLEFT:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, rcS1.right, r.top + szPic.cy, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_TOPCENTER:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, r.left + (r.Width () - szPic.cx) / 2, r.top + szPic.cy, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_TOPRIGHT:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, rcS2.left - szPic.cx, r.top + szPic.cy, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_CENTERLEFT:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, rcS1.right, r.top + (r.Height () - szPic.cy) / 2 + szPic.cy, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_CENTERCENTER:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, r.left + (r.Width () - szPic.cx) / 2, r.top + (r.Height () - szPic.cy) / 2 + szPic.cy, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_CENTERRIGHT:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, rcS2.left - szPic.cx, r.top + (r.Height () - szPic.cy) / 2 + szPic.cy, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_BOTTOMLEFT:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, rcS1.right, r.bottom, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_BOTTOMCENTER:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, r.left + (r.Width () - szPic.cx) / 2, r.bottom, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_BOTTOMRIGHT:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, rcS2.left - szPic.cx, r.bottom, szPic.cx, -szPic.cy, bIsPrinting, bPreview);
			break;
		case BIAM_STRETCH:
			m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, r.left, r.top + r.Height (), r.Width (), -r.Height (), bIsPrinting, bPreview);
			break;
		case BIAM_TILE:
			{
				// Render using integral pictures
				int n = r.Width () / szPic.cx + 1;
				int m = r.Height () / szPic.cy + 1;
				int w = r.Width () / n;
				int h = r.Height () / m;
				for (int x = 0; x < n; x++)
					for (int y = 0; y < m; y++)
						m_pPicture->Render (rdc.m_pDC, rdc.m_hndPrinter, r.left + x * w, r.top + (y + 1) * h, w, -h - 1, bIsPrinting, bPreview);

				/*
				int w, h;
				for (int x = r.left; x < r.right; x += szPic.cx)
				{
					w = x + szPic.cy < r.right ? szPic.cx : r.right - x;
					for (int y = r.top; y < r.bottom; y += szPic.cy)
					{
						h = y + szPic.cy < r.bottom ? szPic.cy : r.bottom - y;
						m_pPicture->Render (pDCRender, x, y + h, w, -h-1, bIsPrinting, bPreview);
					}
				}*/
			}
		}
	}

	m_pAlphaTexture->Render (rdc.m_pDC, rdc.m_hndPrinter, rcGradient, bIsPrinting);
}

void CBackgroundInfo::Serialize(CArchive &ar)
{
	TRACE ("VER: %d\n", ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema);

	if (ar.IsStoring ())
	{
		ar << m_strImgFilename << m_enumMode;

		if (((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 2)
		{
			ar << m_crTransparent;
		}

		//## VER 7 begin
		if (m_pPicture != NULL)
		{
			ar << m_pPicture->GetContrast ();
			ar << m_pPicture->GetBrightness ();
			ar << m_pPicture->GetSaturation ();
			ar << m_pPicture->GetHue ();
			ar << m_pPicture->GetGamma ();
		}
		else
		{
			double f0 = 0.0f;
			double f1 = 1.0f;
			ar << f1 << f0 << f1 << f0 << f1;
		}
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
		int n;
		ar >> m_strImgFilename >> n;
		m_enumMode = (BkImageAlignmentMode) n;

		if (((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 2)
		{
			ar >> m_crTransparent;
		}

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

		if (!m_strImgFilename.IsEmpty () && !((CCdCoverCreator2Doc*) ar.m_pDocument)->IsSerializingFromCUndo ())
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

				m_bImgFilenameChanged = true;
			}

			delete pBuf;

			if (m_pPicture != NULL)
				delete m_pPicture;

			m_pPicture = new CPicture (m_strImgFilename);
			m_pPicture->AdjustColors (fContrast, fBrightness, fSaturation, fHue);
			m_pPicture->SetGamma (fGamma);
		}
	}

	//## begin VERISON 4: Alpha texture
	m_pAlphaTexture->Serialize (ar);
	//## end
}

bool CBackgroundInfo::operator ==(CBackgroundInfo &bi)
{
	if (m_enumMode != bi.GetAlignmentMode ())
		return false;
	if (m_strImgFilename != bi.GetImageFilename ())
		return false;
	if (m_crTransparent != bi.GetTransparentColor ())
		return false;
	if (m_RenderType != bi.GetRenderType ())
		return false;

	return true;
}

bool CBackgroundInfo::operator !=(CBackgroundInfo &bi)
{
	return !(*this == bi);
}

bool CBackgroundInfo::SetImage(CString strImgFilename, COLORREF colTransparent)
{
	if (m_pPicture != NULL)
		delete m_pPicture;

	m_pPicture = new CPicture (strImgFilename);
	m_pPicture->SetTransparentColor (colTransparent);
	m_crTransparent = colTransparent;
	m_strImgFilename = strImgFilename;

	m_bImgFilenameChanged = true;

	return m_pPicture->IsValid ();
}
