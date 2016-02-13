//makejpeg.cpp - Source file for makejpeg namespace
#include "stdafx.h"
#include <memory.h>
#include <stdio.h>
#include <math.h>

#pragma pack(8)
extern "C"
{
	#define XMD_H
	#define HAVE_BOOLEAN
	#include "..\jpeglib2\jpeglib.h"
}
#pragma pack(1)
     
#include <setjmp.h>

struct ima_error_mgr
{
	struct  jpeg_error_mgr pub;	// "public" fields
	jmp_buf setjmp_buffer;		// for return to caller
};

#include "makejpeg.h"

////////////////////////////////////////////////////////////////////////////
//This function takes the contents of a DIB
//and turns it into a JPEG file.
//
//The DIB may be monochrome, 16-color, 256-color, or 24-bit color.
//
//Any functions or data items beginning with "jpeg_" belong to jpeg.lib,
//and are not included here.
//
//The function assumes 3 color components per pixel.
/////////////////////////////////////////////////////////////////////////////
bool JpegFromDib(HANDLE     hDib,     // Handle to DIB
                 int        nQuality, // JPEG quality (0-100)
                 CString    strJpeg)  // Pathname to jpeg file
{
    // Basic sanity checks...
	ASSERT ((nQuality >= 0) && (nQuality <= 100));
	ASSERT (hDib != NULL);
	ASSERT (!strJpeg.IsEmpty ());

    LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER) GlobalLock (hDib);

    byte *buf2 = 0;

    // Use libjpeg functions to write scanlines to disk in JPEG format
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    FILE* pOutFile;			// Target file 
    int nSampsPerRow;		// Physical row width in image buffer 
    JSAMPARRAY jsmpArray;	// Pixel RGB buffer for JPEG file

    cinfo.err = jpeg_std_error (&jerr); // Use default error handling (ugly!)

    jpeg_create_compress(&cinfo);

    if ((pOutFile = fopen (strJpeg, "wb")) == NULL)
    {
		TRACE ("Cannot open %s", (LPSTR) (LPCTSTR) strJpeg);
        jpeg_destroy_compress (&cinfo);
		GlobalUnlock (hDib);
        return false;
    }

    jpeg_stdio_dest (&cinfo, pOutFile);

    cinfo.image_width = lpbi->biWidth;		// Image width and height, in pixels 
    cinfo.image_height = lpbi->biHeight;
    cinfo.input_components = 3;				// Color components per pixel
											// (RGB_PIXELSIZE - see jmorecfg.h)
    cinfo.in_color_space = JCS_RGB;			// Colorspace of input image

    jpeg_set_defaults (&cinfo);
    jpeg_set_quality (&cinfo, nQuality /* Quality: 0-100 scale */, TRUE /* Limit to baseline-JPEG values*/);
    jpeg_start_compress (&cinfo, TRUE);

    // JSAMPLEs per row in output buffer
    nSampsPerRow = cinfo.image_width * cinfo.input_components; 

    // Allocate array of pixel RGB values
    jsmpArray = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, nSampsPerRow, cinfo.image_height);

	bool bOk = true;
    if (bOk &= DibToSamps (lpbi, nSampsPerRow, cinfo, jsmpArray))
        // Write the array of scan lines to the JPEG file
        jpeg_write_scanlines (&cinfo, jsmpArray, cinfo.image_height);

    jpeg_finish_compress(&cinfo);	// Always finish
    fclose(pOutFile);
    jpeg_destroy_compress(&cinfo);	// Free resources

	GlobalUnlock (hDib);

	return bOk;
}

////////////////////////////////////////////////////////////////
//This function fills a jsmpArray with the RGB values
//for the CBitmap.
//
//It has been improved to handle all legal bitmap formats.
//
//A jsmpArray is a big array of RGB values, 3 bytes per value.
//
//Note that rows of pixels are processed bottom to top:
//The data in the jsamp array must be arranged top to bottom.
////////////////////////////////////////////////////////////////
bool DibToSamps (LPBITMAPINFOHEADER pbBmHdr, int nSampsPerRow,
                struct jpeg_compress_struct cinfo, JSAMPARRAY jsmpPixels)
{
	// Sanity...
	ASSERT (pbBmHdr != NULL);
	ASSERT (nSampsPerRow > 0);

	int r = 0;
	int p = 0;
	int q = 0;
	int b = 0;
	int n = 0;
	int nUnused = 0;
	int nBytesWide = 0;
	int nUsed = 0;
	int nLastBits = 0;
	int nLastNibs = 0;
	int nCTEntries = 0;
	int nRow = 0;
	int nByte = 0;
	int nPixel = 0;

	BYTE bytCTEnt = 0;

	// The bit count tells you the format of the bitmap:
	// Decide how many entries will be in the color table (if any)
	switch (pbBmHdr->biBitCount)
	{
	case 1:
		nCTEntries = 2;		// Monochrome
		break;
	case 4:
		nCTEntries = 16;	// 16-color
		break;
	case 8:
		nCTEntries = 256;	// 256-color
		break;
	case 16:
	case 24:
	case 32:
		nCTEntries = 0;		// No color table needed
		break;
	default:
		return false;		// unsupported format
	}

	// Point to the color table and pixels
	DWORD dwCTab = (DWORD) pbBmHdr + pbBmHdr->biSize;
	LPRGBQUAD pCTab  = (LPRGBQUAD) (dwCTab);
	LPSTR lpBits = (LPSTR) pbBmHdr + (WORD) pbBmHdr->biSize + (WORD) (nCTEntries * sizeof (RGBQUAD));

	// Different formats for the image bits
	LPBYTE lpPixels = (LPBYTE) lpBits;
	RGBQUAD* pRgbQs = (RGBQUAD*) lpBits;
	WORD* wPixels = (WORD*) lpBits;

	// Set up the jsamps according to the bitmap's format.
	// Note that rows are processed bottom to top, because
	// that's how bitmaps are created.
	switch (pbBmHdr->biBitCount)
	{
	case 1:
		nUsed = (pbBmHdr->biWidth + 7) / 8;
		nUnused = (((nUsed + 3) / 4) * 4) - nUsed;
		nBytesWide = nUsed + nUnused;
		nLastBits  = 8 - ((nUsed * 8) - pbBmHdr->biWidth);

		for (r = 0; r < pbBmHdr->biHeight; r++)
		{
			for (p = 0, q = 0; p < nUsed; p++)
			{
				nRow = (pbBmHdr->biHeight - r - 1) * nBytesWide;
				nByte = nRow + p;

				int nBUsed = (p < (nUsed - 1)) ? 8 : nLastBits;

				for (b = 0;b < nBUsed; b++)
				{
					bytCTEnt = lpPixels[nByte] << b;
					bytCTEnt = bytCTEnt >> 7;

					jsmpPixels[r][q + 0] = pCTab[bytCTEnt].rgbRed;
					jsmpPixels[r][q + 1] = pCTab[bytCTEnt].rgbGreen;
					jsmpPixels[r][q + 2] = pCTab[bytCTEnt].rgbBlue;

					q += 3;
				}
			}
		}
		break;

	case 4:
		nUsed = (pbBmHdr->biWidth + 1) / 2;
		nUnused = (((nUsed + 3) / 4) * 4) - nUsed;
		nBytesWide = nUsed + nUnused;
		nLastNibs = 2 - ((nUsed * 2) - pbBmHdr->biWidth);

		for (r = 0; r < pbBmHdr->biHeight; r++)
		{
			for (p = 0, q = 0; p < nUsed; p++)
			{
				nRow = (pbBmHdr->biHeight - r - 1) * nBytesWide;
				nByte = nRow + p;

				int nNibbles = (p < nUsed - 1) ? 2 : nLastNibs;

				for(n = 0; n < nNibbles; n++)
				{
					bytCTEnt = lpPixels[nByte] << (n * 4);
					bytCTEnt = bytCTEnt >> (4 - (n * 4));

					jsmpPixels[r][q + 0] = pCTab[bytCTEnt].rgbRed;
					jsmpPixels[r][q + 1] = pCTab[bytCTEnt].rgbGreen;
					jsmpPixels[r][q + 2] = pCTab[bytCTEnt].rgbBlue;

					q += 3;
				}
			}
		}
		break;

	default:	// !!!!???
	case 8: // Each byte is a pointer to a pixel color
		nUnused = (((pbBmHdr->biWidth + 3) / 4) * 4) - pbBmHdr->biWidth;
		for (r = 0;r < pbBmHdr->biHeight; r++)
		{
			for (p = 0,q = 0; p < pbBmHdr->biWidth; p++, q += 3)
			{
				nRow = (pbBmHdr->biHeight-r-1) * (pbBmHdr->biWidth + nUnused);
				nPixel = nRow + p;

				jsmpPixels[r][q + 0] = pCTab[lpPixels[nPixel]].rgbRed;
				jsmpPixels[r][q + 1] = pCTab[lpPixels[nPixel]].rgbGreen;
				jsmpPixels[r][q + 2] = pCTab[lpPixels[nPixel]].rgbBlue;
			}
		}
		break;

	case 16: // Hi-color (16 bits per pixel)
		nBytesWide = (pbBmHdr->biWidth);
		nUnused = (((nBytesWide + 1) / 2) * 2) - nBytesWide;
		nBytesWide += nUnused;
		for (r = 0;r < pbBmHdr->biHeight; r++)
		{
			for (p = 0, q = 0; p < (nBytesWide-nUnused); p++, q += 3)
			{
				nRow = (pbBmHdr->biHeight-r-1) * nBytesWide;
				nPixel = nRow + p;

				RGBQUAD quad = QuadFromWord (wPixels[nPixel]);

				jsmpPixels[r][q + 0] = quad.rgbRed;
				jsmpPixels[r][q + 1] = quad.rgbGreen;
				jsmpPixels[r][q + 2] = quad.rgbBlue;
			}
		}
		break;

	case 24:
		nBytesWide = (pbBmHdr->biWidth * 3);
		nUnused = (((nBytesWide + 3) / 4) * 4) - nBytesWide;
		nBytesWide += nUnused;

		for (r = 0;r < pbBmHdr->biHeight; r++)
		{
			for (p = 0, q = 0; p < (nBytesWide-nUnused); p += 3, q += 3)
			{
				nRow = (pbBmHdr->biHeight-r-1) * nBytesWide;
				nPixel = nRow + p;

				jsmpPixels[r][q + 0] = lpPixels[nPixel + 2];	// Red
				jsmpPixels[r][q + 1] = lpPixels[nPixel + 1];	// Green
				jsmpPixels[r][q + 2] = lpPixels[nPixel + 0];	// Blue
			}
		}
		break;

	case 32:
		for (r = 0; r < pbBmHdr->biHeight; r++)
		{
			for (p = 0, q = 0; p < pbBmHdr->biWidth; p++, q += 3)
			{
				nRow = (pbBmHdr->biHeight - r - 1) * pbBmHdr->biWidth;
				nPixel = nRow + p;

				jsmpPixels[r][q + 0] = pRgbQs[nPixel].rgbRed;
				jsmpPixels[r][q + 1] = pRgbQs[nPixel].rgbGreen;
				jsmpPixels[r][q + 2] = pRgbQs[nPixel].rgbBlue;
			}
		}
		break;
	}

	return true;
}
 
////////////////////////////////////////
//This function turns a 16-bit pixel
//into an RGBQUAD value.
////////////////////////////////////////
RGBQUAD QuadFromWord (WORD b16)
{
   BYTE bytVals[] =
   {
     0,  16, 24, 32,  40, 48, 56, 64,
     72, 80, 88, 96, 104,112,120,128,
     136,144,152,160,168,176,184,192,
     200,208,216,224,232,240,248,255
   };

   WORD wR = b16;
   WORD wG = b16;
   WORD wB = b16;

   wR <<=  1;
   wR >>= 11;
   wG <<=  6;
   wG >>= 11;
   wB <<= 11;
   wB >>= 11;

   RGBQUAD rgb;

   rgb.rgbReserved = 0;
   rgb.rgbBlue     = bytVals[wB];
   rgb.rgbGreen    = bytVals[wG];
   rgb.rgbRed      = bytVals[wR];

   return rgb;
}
