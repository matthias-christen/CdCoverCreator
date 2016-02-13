//makejpeg.h - Header file for makejpeg namespace
#define XMD_H
#define HAVE_BOOLEAN
#include "..\jpeglib2\jpeglib.h"

#ifndef MAKEJPEGH
#define MAKEJPEGH


bool JpegFromDib(HANDLE     hDib,		// Handle to DIB
                 int        nQuality,	// JPEG quality (0-100)
                 CString    strJpeg);	// Pathname to target jpeg file

bool DibToSamps(LPBITMAPINFOHEADER pbBmHdr, int nSampsPerRow,
                struct jpeg_compress_struct cinfo, JSAMPARRAY jsmpPixels);

RGBQUAD QuadFromWord(WORD b16);

#endif