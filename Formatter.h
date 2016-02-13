// Formatter.h: interface for the CFormatter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORMATTER_H__C62A17F2_1918_46A4_AF95_359E51FD82C2__INCLUDED_)
#define AFX_FORMATTER_H__C62A17F2_1918_46A4_AF95_359E51FD82C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFormatter  
{
public:
	static CString Format (CString strFormat, int nParam1, int nParam2 = 0);
	CFormatter();
	virtual ~CFormatter();

};

#endif // !defined(AFX_FORMATTER_H__C62A17F2_1918_46A4_AF95_359E51FD82C2__INCLUDED_)
