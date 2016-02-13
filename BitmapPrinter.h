// BitmapPrinter.h: interface for the CBitmapPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPPRINTER_H__6034B137_6219_4914_99DB_6A7DDF94896D__INCLUDED_)
#define AFX_BITMAPPRINTER_H__6034B137_6219_4914_99DB_6A7DDF94896D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PRINT_FACTOR1 1	// factor for 1 Bit color depth printers (black&white)
#define PRINT_FACTOR2 2 // factor for <8 Bit color depth printers
#define PRINT_FACTOR3 4 // other

int GetPrintFactor (CDC* pDC, bool bPreview);

class CBitmapPrinter  
{
public:
	CBitmapPrinter(int x, int y, int nWidth, int nHeight);
	virtual ~CBitmapPrinter();

	CDC* Attach (CDC* pDC, bool bPreview);
	void Release();

private:
	void PrintBitmap ();
	HANDLE Bitmap2DIB ();
	HANDLE DDBToDIB (CBitmap& bitmap, DWORD dwCompression, CPalette* pPal);

	CDC* m_pDCRender;
	CDC* m_pDC;
	CBitmap m_bmpPrint;
	CBitmap* m_pBmpOld;

	int m_x;
	int m_y;
	int m_nWidth;
	int m_nHeight;
protected:
	CPalette m_Palette;
};

#endif // !defined(AFX_BITMAPPRINTER_H__6034B137_6219_4914_99DB_6A7DDF94896D__INCLUDED_)
