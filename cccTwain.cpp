// cccTwain.cpp: implementation of the CcccTwain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"

#include "cccTwain.h"
#include "DIB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CcccTwain::CcccTwain()
{

}

CcccTwain::~CcccTwain()
{

}

/*
static unsigned char masktable[] = { 0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01 };

void Create24Bit (CDIB& dibSource, CDIB& dibDest)
{
	dibDest.Create (dibSource.Width (), dibSource.Height (), 24);
	int nPal = dibSource.GetPaletteSize ();
	BYTE bPalette[768];
	COLORREF col;

	for (int i = 0; i < nPal; i++)
	{
		col = dibSource.PaletteColor (i);
		bPalette[i * 3 + 2] = GetRValue (col);
		bPalette[i * 3 + 1] = GetGValue (col);
		bPalette[i * 3 + 0] = GetBValue (col);
	}

	int j;
	BYTE *pSrc, *pDst, *pPtr;

	for(i = 0; i < dibSource.Height (); i++)
	{
		pSrc = dibSource.GetLinePtr (i);
		pDst = dibDest.GetLinePtr (i);
		pPtr = pDst;
		int nIndex;

		for(j = 0; j < dibSource.Width (); j++, pPtr += 3)
		{
			switch (nPal)
			{
			case 2:
				nIndex = (pSrc[j >> 3] & masktable[j & 7]) ? 1 : 0;
				break;
			case 16:
				nIndex = (j & 1) ? pSrc[j >> 1] & 0x0f : (pSrc[j >> 1] >> 4) & 0x0f;
				break;
			case 256:
				nIndex = pSrc[j];
				break;
			}
			ASSERT (nIndex < nPal);
			memcpy (pPtr, bPalette + nIndex * 3, 3);
		}
		nIndex = (pPtr - pDst) / 3;
		ASSERT (nIndex <= dibSource.Width ());
	}		
}
*/

int CcccTwain::GetEncoderClsid (const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;		// number of image encoders
	UINT  size = 0;		// size of the image encoder array in bytes

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

	Gdiplus::GetImageEncodersSize (&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*) (malloc (size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	Gdiplus::GetImageEncoders (num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
		if (wcscmp (pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free (pImageCodecInfo);
			return j;  // Success
		}

	free (pImageCodecInfo);
	return -1;  // Failure
}

void CcccTwain::CopyImage (HANDLE hBitmap, TW_IMAGEINFO& info)
{
	CDIB* pDib = new CDIB ();
	pDib->CreateFromHandle (hBitmap, info.BitsPerPixel);

	/*
	// save image in temp directory as JPEG
	CString strFilename;
	strFilename.Format ("%sscan%d.jpg", ((CCdCoverCreator2App*) AfxGetApp ())->GetImageCacheDir ().GetBuffer (0), GetTickCount ());
	JpegFromDib (dib.DIBHandle (), 75, strFilename);
	dib.DestroyDIB ();
	*/

	CString strFilename;
	strFilename.Format ("%sscan%d.jpg", ((CCdCoverCreator2App*) AfxGetApp ())->GetImageCacheDir ().GetBuffer (0), GetTickCount ());

	pDib->SaveBMP (strFilename + ".bmp");

	{
		BSTR bstrFilenameJPEG = strFilename.AllocSysString ();
		BSTR bstrFilenameBMP = CString (strFilename + ".bmp").AllocSysString ();
		Gdiplus::Bitmap bitmap (bstrFilenameBMP);

		CLSID clsidJpeg;
		CcccTwain::GetEncoderClsid (L"image/jpeg", &clsidJpeg);
		bitmap.Save (bstrFilenameJPEG, &clsidJpeg);
		
		::SysFreeString (bstrFilenameJPEG);
		::SysFreeString (bstrFilenameBMP);
	}

	pDib->DestroyDIB ();
	delete pDib;

	DeleteFile (strFilename + ".bmp");
	DWORD dwErr = GetLastError ();

	// set image
	if (m_pBkImgPage != NULL)
	{
		m_pBkImgPage->SetDlgItemText (IDC_EDIT_BKIMGFILENAME, strFilename);
		m_pBkImgPage->OnButtonPreview ();
	}
	else
	{
		CMDIChildWnd* pChild = ((CMDIFrameWnd*) AfxGetMainWnd ())->MDIGetActive ();
		if (pChild != NULL)
		{
			CCdCoverCreator2Doc* pDoc = (CCdCoverCreator2Doc*) pChild->GetActiveDocument ();
			if (pDoc != NULL)
				pDoc->SetBkImageFilename (strFilename);
		}
	}
}
