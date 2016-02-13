// BackgroundAlphaTexture.h: interface for the CBackgroundAlphaTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDALPHATEXTURE_H__E546AFC9_7256_4401_B627_F28CBBD07DCC__INCLUDED_)
#define AFX_BACKGROUNDALPHATEXTURE_H__E546AFC9_7256_4401_B627_F28CBBD07DCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphicsObject.h"
#include "RenderDC.h"
#include "StyleTemplate.h"
#include <Gdiplus.h>

class CBackgroundAlphaTexture : public CGraphicsObject
{	
	DECLARE_SERIAL (CBackgroundAlphaTexture)

public:
	CBackgroundAlphaTexture();
	CBackgroundAlphaTexture(CString strImgFilename, int nAlphaFormat, int nSourceConstantAlpha);
	virtual ~CBackgroundAlphaTexture();

	bool operator== (CBackgroundAlphaTexture& bi);
	bool operator!= (CBackgroundAlphaTexture& bi);

	virtual void Render (CDC* pDC, HANDLE hndPrinter, CRect r, bool bIsPrinting);
	virtual void Serialize (CArchive& ar);

	inline CString GetImageFilename () { return m_strImgFilename; }
	inline bool IsImgFilenameChanged () { return m_bImgFilenameChanged; }

	int m_nSourceConstantAlpha;
	int m_nAlphaFormat;

protected:
	CString m_strImgFilename;
	Gdiplus::Bitmap* m_pBitmap;

	bool m_bImgFilenameChanged;
};

#endif // !defined(AFX_BACKGROUNDALPHATEXTURE_H__E546AFC9_7256_4401_B627_F28CBBD07DCC__INCLUDED_)
