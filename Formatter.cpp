// Formatter.cpp: implementation of the CFormatter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "Formatter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFormatter::CFormatter()
{

}

CFormatter::~CFormatter()
{

}

CString CFormatter::Format(CString strFormat, int nParam1, int nParam2)
{
	const CString strOp = "0+-";
	CString strCFmt, s;
	int nPos, nDigCount = 0;
	bool bHasMod;

	int nPosOld = 0;
	for ( ; nPosOld < strFormat.GetLength (); )
	{
		nPos = strFormat.Find ('#', nPosOld);
		if (nPos < 0)
			break;

		// count #
		int i = nPos + 1; 
		for ( ; i < strFormat.GetLength (); i++)
			if (strFormat[i] != '#')
				break;
		nDigCount = i - nPos;
		
		bHasMod = false;
		if (nPos > 0)
			// find modifiers in front of #
			if (strOp.Find (strFormat[nPos - 1]) >= 0)
				bHasMod = true;

		strCFmt += strFormat.Mid (nPosOld, nPos - nPosOld - (bHasMod ? 1 : 0)) + "%";
		if (bHasMod)
			strCFmt += strFormat[nPos - 1];

		if (nDigCount > 1)
		{
			s.Format ("%d", nDigCount);
			strCFmt += s;
		}
		strCFmt += 'd';

		nPosOld = nPos + nDigCount;
	}
	strCFmt += strFormat.Mid (nPosOld);

	s.Format (strCFmt, nParam1, nParam2);
	return s;
}
