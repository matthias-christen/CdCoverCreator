// BackgroundAlphaTexture.cpp: implementation of the CBackgroundAlphaTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"
#include "BackgroundAlphaTexture.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL (CBackgroundAlphaTexture, CObject, VERSIONABLE_SCHEMA | 1)

CBackgroundAlphaTexture::CBackgroundAlphaTexture()
{
	m_nAlphaFormat = 1;
	m_nSourceConstantAlpha = 0x7F;

	m_bImgFilenameChanged = false;
}

using namespace Gdiplus;

CBackgroundAlphaTexture::CBackgroundAlphaTexture (CString strImgFilename, int nAlphaFormat, int nSourceConstantAlpha)
{
	m_strImgFilename = strImgFilename;
	m_nAlphaFormat = nAlphaFormat;
	m_nSourceConstantAlpha = nSourceConstantAlpha;

	m_bImgFilenameChanged = false;

	// load the image
	BSTR bstrFilename = strImgFilename.AllocSysString ();
	m_pBitmap = Bitmap::FromFile (bstrFilename);
	::SysFreeString (bstrFilename);
}

CBackgroundAlphaTexture::~CBackgroundAlphaTexture()
{
}

bool CBackgroundAlphaTexture::operator== (CBackgroundAlphaTexture& bi)
{
	if (m_strImgFilename != bi.GetImageFilename ())
		return false;
	if (m_nAlphaFormat != bi.m_nAlphaFormat)
		return false;
	if (m_nSourceConstantAlpha != bi.m_nSourceConstantAlpha)
		return false;

	return true;
}

bool CBackgroundAlphaTexture::operator!= (CBackgroundAlphaTexture& bi)
{
	return !(*this == bi);
}

void CBackgroundAlphaTexture::Render (CDC* pDC, HANDLE hndPrinter, CRect r, bool bIsPrinting)
{
	if (m_strImgFilename.IsEmpty ())
		return;
	if (m_pBitmap == NULL)
		return;

	Graphics graphics (pDC->GetSafeHdc (), hndPrinter);
	graphics.SetPageUnit (UnitPixel);

	/*	TEXTUREBRUSH DOES NOT PRINT CORRECTLY :-( */
	// create the color matrix
	ColorMatrix cm;
	
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			cm.m[i][j] = i == j ? 1.0 : 0.0;	// identity matrix
	cm.m[3][0] = cm.m[3][1] = cm.m[3][2] = (double) m_nAlphaFormat / 3;
	cm.m[3][3] = (double) m_nSourceConstantAlpha / 0xff;

	// create a texture brush
	ImageAttributes atts;
	atts.SetColorMatrix (&cm);
	Rect rctDest (0, 0, m_pBitmap->GetWidth (), m_pBitmap->GetHeight ());
	TextureBrush brush (m_pBitmap, rctDest, &atts);
	brush.SetWrapMode (WrapModeTile);
	//TextureBrush brush (m_pBitmap, WrapModeTile, rctDest);

	graphics.FillRectangle (&brush, r.left, r.top, r.Width (), r.Height ());
	
/*
	// set the alpha values of the image
	int nMaxX = m_pBitmap->GetWidth ();
	int nMaxY = m_pBitmap->GetHeight ();

	Color pixel;
	
	for (int x = 0; x < nMaxX; x++)
		for (int y = 0; y < nMaxY; y++)
		{
			m_pBitmap->GetPixel (x, y, &pixel);
			m_pBitmap->SetPixel (x, y, Color (m_nSourceConstantAlpha, pixel.GetRed (), pixel.GetGreen (), pixel.GetBlue ()));
		}

	graphics.DrawImage (m_pBitmap, r.left, r.top, 0, 0, nMaxX, nMaxY, UnitPixel);
*/
	// etc. etc.
	// Doesn't work with printing either: no transparency, the texture will be opaque
	// Forget it...
}

void CBackgroundAlphaTexture::Serialize (CArchive& ar)
{
	if (ar.IsStoring ())
	{
		ar << m_strImgFilename;
		if (!m_strImgFilename.IsEmpty () && !((CCdCoverCreator2Doc*) ar.m_pDocument)->IsSerializingFromCUndo ())
		{
			ar << m_nAlphaFormat << m_nSourceConstantAlpha;

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
		if (((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 4)
		{
			// Alpha texture
			ar >> m_strImgFilename;

			if (!m_strImgFilename.IsEmpty () && !((CCdCoverCreator2Doc*) ar.m_pDocument)->IsSerializingFromCUndo ())
			{
				ar >> m_nAlphaFormat >> m_nSourceConstantAlpha;

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
					CFile f (m_strImgFilename, CFile::modeCreate | CFile::modeWrite);
					f.Write (pBuf, dwLen);
					f.Close ();

					m_bImgFilenameChanged = true;
				}

				delete pBuf;

				// load the image
				BSTR bstrFilename = m_strImgFilename.AllocSysString ();
				m_pBitmap = Bitmap::FromFile (bstrFilename);
				::SysFreeString (bstrFilename);
			}
		}
	}
}