#pragma once
#include <gdiplus.h>

#define INCH 25.40

// Definitions of EAN barcode
#define MODULE 0.330
#define EAN8_HEIGHT 18.23
#define EAN13_HEIGHT 22.85

#define LEFT_GUARD "101"		// 111
#define RIGHT_GUARD "101"		// 111
#define CENTER_GUARD "01010"	// 11111
#define ADDON_GUARD "1011"		// 112
#define ADDON_DELIN "01"		// 11 delineator

class CEANSize
{
public:
	CEANSize (float fZoom, float fHeightCut, int nQuality) {
		m_fZoom = fZoom;
		m_fHeightCut = fHeightCut;
		m_nQuality = nQuality;
	}

	float m_fZoom;
	float m_fHeightCut;
	int m_nQuality;
};

using namespace Gdiplus;

class CEANBase
{
public:
	virtual void SetCode (CString strCode, CString strAddon = "");
	CString GetCode () { return m_strCode.Left (m_nType); }

	virtual CSize GetSize () = 0;	// returns the size in 100th of millimeters
	virtual void Render (Graphics* pGraphics, CRect rect) = 0;

	void SetFont (CString strFont) { m_strFont = strFont; }
	CString GetFont () { return m_strFont; }

	void SetForegroundColor (COLORREF colForegroundColor) { m_colForegroundColor = colForegroundColor; }
	COLORREF GetForegroundColor () { return m_colForegroundColor; }

	// sets the background behaviour
	// nBackgroundMode is either OPAQUE or TRANSPARENT. In the first case
	// the background color of the barcode is set to colBackgroundColor.
	void SetBackground (int nBackgroundMode, COLORREF colBackgroundColor) {
		m_nBackgroundMode = nBackgroundMode;
		m_colBackgroundColor = colBackgroundColor;
	}
	int GetBackgroundMode () { return m_nBackgroundMode; }
	COLORREF GetBackgroundColor () { return m_colBackgroundColor; }

protected:
	CEANBase (int nType);
	~CEANBase (void);

	int CalcCheckDigit ();
	CString Explode (int nDigit, char chSet = 'A');

	CString m_strCode;
	int m_nType;	// 8 for EAN8, 13 for EAN13

	CString m_strAddon;
	int m_nAddonType;

	CString m_strEAN[10];
	CString m_strSet[10];
	CString m_strSetAddon2[4];
	CString m_strSetAddon5[10];
	CEANSize* m_eansize[10];

	CString m_strFont;
	COLORREF m_colForegroundColor;
	int m_nBackgroundMode;
	COLORREF m_colBackgroundColor;
};
