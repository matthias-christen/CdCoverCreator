// BitmapPrinter.cpp: implementation of the CBitmapPrinter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "BitmapPrinter.h"

//#include <windowsx.h>
#include "GBitmapPrinter.h"
using namespace printer;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int GetPrintFactor (CDC* pDC, bool bPreview)
{
//	return 1;

	// preview
	if (bPreview)
		return PRINT_FACTOR3;

	// printing
//return 1;
	if (pDC->GetDeviceCaps (BITSPIXEL) == 1)
		return PRINT_FACTOR1;
	if (pDC->GetDeviceCaps (BITSPIXEL) < 8)
		return PRINT_FACTOR2;

	return PRINT_FACTOR3;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBitmapPrinter::CBitmapPrinter(int x, int y, int nWidth, int nHeight)
{
	m_x = x;
	m_y = y;
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

CBitmapPrinter::~CBitmapPrinter()
{
}

CDC* CBitmapPrinter::Attach(CDC *pDC, bool bPreview)
{
	m_pDC = pDC;

	m_pDCRender = new CDC ();
	m_pDCRender->CreateCompatibleDC (m_pDC);
	int nPrintFactor = GetPrintFactor (m_pDC, bPreview);
	m_bmpPrint.CreateCompatibleBitmap (m_pDC, m_nWidth / nPrintFactor, abs (m_nHeight) / nPrintFactor);
	m_pBmpOld = m_pDCRender->SelectObject (&m_bmpPrint);
	m_pDCRender->PatBlt (0, 0, m_nWidth / nPrintFactor, abs (m_nHeight) / nPrintFactor, WHITENESS);

	return m_pDCRender;
}

void CBitmapPrinter::Release()
{
	// Create logical palette if device support a palette
	if (m_pDC->GetDeviceCaps (RASTERCAPS) & RC_PALETTE)
	{
		UINT nSize = sizeof (LOGPALETTE) + (sizeof (PALETTEENTRY) * 256);
		LOGPALETTE* pLP = (LOGPALETTE*) new BYTE[nSize];
		pLP->palVersion = 0x300;
		pLP->palNumEntries = (unsigned short) GetSystemPaletteEntries (m_pDC->GetSafeHdc (), 0, 255, pLP->palPalEntry);
		
		// Create the palette
		m_Palette.CreatePalette (pLP);
		
		// Free memory
		delete[] pLP;
	}

	m_pDCRender->SelectObject (m_pBmpOld);
	m_pDCRender->DeleteDC ();
	delete m_pDCRender;

	PrintBitmap ();

	m_bmpPrint.DeleteObject ();
}

void CBitmapPrinter::PrintBitmap ()
{
	// create a GBitmapPrinter object
	GBitmapPrinter BitmapPrinter;

	// NOTE: Be sure to deselect the DDB or DIB
	// section from the memory DC before calling
	// the SetBitmap() member function.

	// disassociate the bitmap from the memory DC
	//hBmp = static_cast<HBITMAP> (SelectObject (hMemDC, hOldBmp));

	// specify the target bitmap
	BitmapPrinter.SetBitmap ((HBITMAP) m_bmpPrint, NULL);

	// restore the association
	//hOldBmp = static_cast<HBITMAP> (SelectObject (hMemDC, hBmp));

	// specify the target rectangle
	//BitmapPrinter.SetPrintScale (GBP_SCALE_PAGE);
	RECT rect;
	rect.left = m_x;
	rect.top = m_y;
	rect.right = rect.left + m_nWidth;
	rect.bottom = rect.top + m_nHeight;
	BitmapPrinter.SetPrintRect (rect);

	// set the document name
	BitmapPrinter.SetDocName ("Bitmap Printing Demo");

	// print the bitmap
	//BitmapPrinter.PrintBitmapDoc(pd.hDC);
	BitmapPrinter.PrintBitmap (m_pDC->GetSafeHdc ());

	// clean up
	//DeleteDC(pd.hDC);
	//if (pd.hDevMode) GlobalFree(pd.hDevMode);
	//if (pd.hDevNames) GlobalFree(pd.hDevNames);
}