// Picture.h: interface for the CPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__91A21481_FE40_4991_9D8A_B08FF7FC0BA9__INCLUDED_)
#define AFX_PICTURE_H__91A21481_FE40_4991_9D8A_B08FF7FC0BA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <atlimage.h>
#include <Gdiplus.h>

class CPicture : public CObject
{
	DECLARE_SERIAL (CPicture)

public:
	CPicture();
	bool IsValid();
	CPicture(CString strFilename);
	virtual ~CPicture();

	void Reload ();

	void Render(CDC* pDC, HANDLE hDevice, int x, int y, int nDstWidth, int nDstHeight, bool bIsPrinting, bool bPreview = false);
	CSize GetSize(int nMapMode = MM_TEXT);

	inline COLORREF GetTransparentColor () { return m_crTransparent; }
	void SetTransparentColor (COLORREF crTransparent);

	void SetGamma (double fGamma);
	void SetContrast (double fContrast);
	void SetBrightness (double fBrightness);
	void SetSaturation (double fSaturation);
	void SetHue (double fHue);
	void AdjustColors (double fContrast, double fBrightness, double fSaturation, double fHue);

	inline CString GetFilename () { return m_strFilename; }

	inline double GetGamma () { return m_fGamma; }
	inline double GetContrast () { return m_fContrast; }
	inline double GetBrightness () { return m_fBrightness; }
	inline double GetSaturation () { return m_fSaturation; }
	inline double GetHue () { return m_fHue; }

protected:
	void Load ();
	void Destroy ();

	COLORREF m_crTransparent;

	//HGLOBAL m_hglBuffer;
	BYTE* m_pData;
	Gdiplus::Bitmap* m_pImage;
	Gdiplus::ImageAttributes m_Attributes;

	CString m_strFilename;

	double m_fGamma;
	double m_fContrast;
	double m_fBrightness;
	double m_fSaturation;
	double m_fHue;
};

#endif // !defined(AFX_PICTURE_H__91A21481_FE40_4991_9D8A_B08FF7FC0BA9__INCLUDED_)
