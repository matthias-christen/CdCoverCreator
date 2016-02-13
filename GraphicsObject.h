// GraphicsObject.h: interface for the CGraphicsObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHICSOBJECT_H__8C2A151E_703C_4B1C_A4A7_F69792175A18__INCLUDED_)
#define AFX_GRAPHICSOBJECT_H__8C2A151E_703C_4B1C_A4A7_F69792175A18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphicsObject : public CObject
{
public:
	CGraphicsObject();
	virtual ~CGraphicsObject();

	virtual void Render (CDC* pDC, HANDLE hndPrinter, CRect r, bool bIsPrinting) = 0;
};

#endif // !defined(AFX_GRAPHICSOBJECT_H__8C2A151E_703C_4B1C_A4A7_F69792175A18__INCLUDED_)
