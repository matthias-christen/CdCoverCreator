// Picture.cpp: implementation of the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "Picture.h"
#include "BitmapPrinter.h"
#include "QColorMatrix.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL (CPicture, CObject, VERSIONABLE_SCHEMA | 1)

using namespace Gdiplus;

CPicture::CPicture()
{
	m_pImage = NULL;
	//m_hglBuffer = NULL;
	m_pData = NULL;

	m_strFilename.Empty ();

	m_Attributes.Reset ();
	m_fContrast = 1.0;
	m_fBrightness = 0.0;
	m_fSaturation = 1.0;
	m_fHue = 0.0;
	m_fGamma = 1.0;
}

CPicture::CPicture(CString strFilename)
{
	m_pImage = NULL;
	//m_hglBuffer = NULL;
	m_pData = NULL;

	m_strFilename = strFilename;

	m_Attributes.Reset ();
	m_fContrast = 1.0;
	m_fBrightness = 0.0;
	m_fSaturation = 1.0;
	m_fHue = 0.0;
	m_fGamma = 1.0;

	m_crTransparent = -1;

	/*
	CFile file (strFilename, CFile::modeRead | CFile::shareDenyNone);
	ULONGLONG nLen = file.GetLength ();
	m_hglBuffer = GlobalAlloc (nLen, GMEM_MOVEABLE);
	if (m_hglBuffer)
	{
		void* pBuf = GlobalLock (m_hglBuffer);
		if (pBuf)
		{
			file.Read (pBuf, nLen);
			file.Close ();

			IStream* pInStream = NULL;
			if (::CreateStreamOnHGlobal (m_hglBuffer, FALSE, &pInStream) == S_OK)
			{
				m_pImage = Bitmap::FromStream (pInStream);
				pInStream->Release ();

				if (m_pImage)
				{
					if (m_pImage->GetLastStatus () == Gdiplus::Ok)
						return;

					delete m_pImage;
					m_pImage = NULL;
				}
			}
			m_pImage = NULL;
			::GlobalUnlock (m_hglBuffer);
		}
		::GlobalFree (m_hglBuffer);
		m_hglBuffer = NULL;
		file.Close ();
	}

	file.Close ();*/

	Load ();
}

CPicture::~CPicture()
{
	Destroy ();

	/*
	if (m_hglBuffer != NULL)
	{
		::GlobalUnlock (m_hglBuffer);
		::GlobalFree (m_hglBuffer);
		m_hglBuffer = NULL;
	}*/
}

void CPicture::Load ()
{
	// open the image in non-exclusive mode since the image is not edited
	// ::FromFile does not provide such a method, so copy the bitmap's bits
	// to the actual image

	BSTR bstrFilename = m_strFilename.AllocSysString ();
	Bitmap* pImage = Bitmap::FromFile (bstrFilename);
	::SysFreeString (bstrFilename);

	if (pImage->GetLastStatus () == Gdiplus::Ok)
	{
		Rect r (0, 0, pImage->GetWidth (), pImage->GetHeight ());
		BitmapData bitmapData;
		pImage->LockBits (&r, ImageLockModeRead, PixelFormat32bppARGB, &bitmapData);
		m_pData = new BYTE[bitmapData.Width * bitmapData.Height * 4];
		memcpy (m_pData, bitmapData.Scan0, bitmapData.Width * bitmapData.Height * 4);
		Bitmap bmp (bitmapData.Width, bitmapData.Height, bitmapData.Stride,
			bitmapData.PixelFormat, m_pData);
		m_pImage = bmp.Clone (r, PixelFormat32bppARGB);

		pImage->UnlockBits (&bitmapData);
	}

	delete pImage;
}

void CPicture::Destroy ()
{
	if (m_pData != NULL)
	{
		delete m_pData;
		m_pData = NULL;
	}

	if (m_pImage != NULL)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

void CPicture::Reload ()
{
	Destroy ();
	Load ();
}

void CPicture::Render(CDC* pDC, HANDLE hDevice, int x, int y, int nDstWidth, int nDstHeight, bool bIsPrinting, bool bPreview)
{
/*	if (m_pPicture == NULL)
		return;

	OLE_XSIZE_HIMETRIC width;
	OLE_YSIZE_HIMETRIC height;
	m_pPicture->get_Width (&width);
	m_pPicture->get_Height (&height);

	// smaller scale for printing
	int nPrintFactor = GetPrintFactor (pDC, bPreview);
	int w = bIsPrinting ? nDstWidth / nPrintFactor : nDstWidth;
	int h = bIsPrinting ? abs (nDstHeight) / nPrintFactor : abs (nDstHeight);

	// bitmap printer is relative to the image -
	// images must always be placed at the origin.
	x = bIsPrinting ? x / nPrintFactor : x;
	y = bIsPrinting ? y / nPrintFactor : y;

	if (CPicture::IsTransparentBltAvailable () && (m_crTransparent != -1))
	{
		CDC memDC;
		memDC.CreateCompatibleDC (pDC);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap (pDC, w, h);
		CBitmap *pOldBmp = memDC.SelectObject (&bmp);

		memDC.FillSolidRect (0, 0, w, h, m_crTransparent);
		m_pPicture->Render (memDC.GetSafeHdc (), 0, h, w, -h, 0, 0, width, height, NULL);

		m_pFnxTransparentBlt (pDC->GetSafeHdc (), x, y - h, w, h,
			memDC.GetSafeHdc (), 0, 0, w,h, m_crTransparent);

		memDC.SelectObject (pOldBmp);
		bmp.DeleteObject ();
		memDC.DeleteDC ();
	}
	else
		m_pPicture->Render (pDC->GetSafeHdc (), x, y, w, -h, 0, 0, width, height, NULL);
*/

	if (m_pImage == NULL)
		return;

	int h = abs (nDstHeight);

	if (bIsPrinting)
	{
		Graphics graphics (pDC->GetSafeHdc (), hDevice);
		graphics.SetPageUnit (UnitPixel);
		Rect rcDest (x, y - h, nDstWidth, h);
		graphics.DrawImage (m_pImage, rcDest, 0, 0, m_pImage->GetWidth (), m_pImage->GetHeight (), UnitPixel, &m_Attributes);
	}
	else
	{
		Graphics graphics (pDC->GetSafeHdc ());
		graphics.SetPageUnit (UnitPixel);
		Rect rcDest (x, y - h, nDstWidth, h);
		graphics.DrawImage (m_pImage, rcDest, 0, 0, m_pImage->GetWidth (), m_pImage->GetHeight (), UnitPixel, &m_Attributes);
	}
}

CSize CPicture::GetSize(int nMapMode)
{
	if (m_pImage == NULL)
		return CSize (0, 0);

	return CSize (m_pImage->GetWidth (), m_pImage->GetHeight ());
}

bool CPicture::IsValid()
{
	return m_pImage != NULL;
}

void CPicture::SetTransparentColor (COLORREF crTransparent)
{
	if (m_crTransparent == crTransparent)
		return;

	m_crTransparent = crTransparent;

	Color pixel;
	int maxY = m_pImage->GetHeight ();
	int maxX = m_pImage->GetWidth ();

	Rect rect (0, 0, maxX, maxY);
	BitmapData bitmapData;
	if (m_pImage->LockBits (&rect, ImageLockModeRead | ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData) == Ok)
	{
		UINT* pPixels = (UINT*) bitmapData.Scan0;
		UINT uColor = ((crTransparent & 0x0000ff) << 16) | (crTransparent & 0x00ff00) | ((crTransparent & 0xff0000) >> 16);

		for (int y = 0; y < maxY; y++)
		{
			for (int x = 0; x < maxX; x++)
			{
				if ((pPixels[y * bitmapData.Stride / 4 + x] & 0x00ffffff) == uColor)
					pPixels[y * bitmapData.Stride / 4 + x] &= 0x00ffffff;
				else
					pPixels[y * bitmapData.Stride / 4 + x] |= 0xff000000;
			}
		}

		m_pImage->UnlockBits (&bitmapData);
	}
	else
		AfxMessageBox (IDS_TRANSPARENCYNOTSUPPORTED, MB_ICONEXCLAMATION);
}

void CPicture::SetGamma (double fGamma)
{
	m_fGamma = fGamma;
	m_Attributes.SetGamma (fGamma);
}

void CPicture::SetContrast (double fContrast)
{
	m_fContrast = fContrast;

	QColorMatrix mat;
	mat.ScaleColors (m_fContrast);
	mat.TranslateColors (m_fBrightness, MatrixOrderAppend);
	mat.SetSaturation (m_fSaturation, MatrixOrderAppend);
	mat.RotateHue (m_fHue);
	m_Attributes.SetColorMatrix (&mat);
}

void CPicture::SetBrightness (double fBrightness)
{
	m_fBrightness = fBrightness;

	QColorMatrix mat;
	mat.ScaleColors (m_fContrast);
	mat.TranslateColors (m_fBrightness, MatrixOrderAppend);
	mat.SetSaturation (m_fSaturation, MatrixOrderAppend);
	mat.RotateHue (m_fHue);
	m_Attributes.SetColorMatrix (&mat);
}

void CPicture::SetSaturation (double fSaturation)
{
	m_fSaturation = fSaturation;

	QColorMatrix mat;
	mat.ScaleColors (m_fContrast);
	mat.TranslateColors (m_fBrightness, MatrixOrderAppend);
	mat.SetSaturation (m_fSaturation, MatrixOrderAppend);
	mat.RotateHue (m_fHue);
	m_Attributes.SetColorMatrix (&mat);
}

void CPicture::SetHue (double fHue)
{
	m_fHue = fHue;

	QColorMatrix mat;
	mat.ScaleColors (m_fContrast);
	mat.TranslateColors (m_fBrightness, MatrixOrderAppend);
	mat.SetSaturation (m_fSaturation, MatrixOrderAppend);
	mat.RotateHue (m_fHue);
	m_Attributes.SetColorMatrix (&mat);
}

void CPicture::AdjustColors (double fContrast, double fBrightness, double fSaturation, double fHue)
{
	m_fContrast = fContrast;
	m_fBrightness = fBrightness;
	m_fSaturation = fSaturation;
	m_fHue = fHue;

	QColorMatrix mat;
	mat.ScaleColors (m_fContrast);
	mat.TranslateColors (m_fBrightness, MatrixOrderAppend);
	mat.SetSaturation (m_fSaturation, MatrixOrderAppend);
	mat.RotateHue (m_fHue);
	m_Attributes.SetColorMatrix (&mat);
}