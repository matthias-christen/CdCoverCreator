// RenderDC.h: interface for the CRenderDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENDERDC_H__C1C2C179_5AFC_4FA5_8712_33EC7ED7BA3C__INCLUDED_)
#define AFX_RENDERDC_H__C1C2C179_5AFC_4FA5_8712_33EC7ED7BA3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRenderDC  
{
public:
	CRenderDC (CDC* pDC, int nHeight, CObject* pStyle);
	CRenderDC (CDC* pDC, HANDLE hndPrinter, int nHeight, CObject* pStyle);

	virtual ~CRenderDC();

	CDC* m_pDC;
	HANDLE m_hndPrinter;

	int GetWidth (RenderType rt);
	int GetHeight (RenderType rt);

protected:
	void SetMeasures (int nHeight, CObject* pStyle);

	int m_nWidthInlay, m_nWidthCover, m_nWidthLabel;
	int m_nHeightInlay, m_nHeightCover, m_nHeightLabel;
};

#endif // !defined(AFX_RENDERDC_H__C1C2C179_5AFC_4FA5_8712_33EC7ED7BA3C__INCLUDED_)
