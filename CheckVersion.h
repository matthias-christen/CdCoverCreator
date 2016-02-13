// CheckVersion.h: interface for the CCheckVersion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKVERSION_H__C107AA19_524A_4FB6_AE0A_2937FF641C8B__INCLUDED_)
#define AFX_CHECKVERSION_H__C107AA19_524A_4FB6_AE0A_2937FF641C8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCheckVersion  
{
public:
	static CString GetProgramVersion();
	static bool Prompt(bool bIgnoreSkip);
	static bool Check(CString strURL, bool bExamineDoCheckVersion = true);
	CCheckVersion();
	virtual ~CCheckVersion();

	static LONG GetRegKey (HKEY key, LPCTSTR subkey, LPTSTR retdata);
	static HINSTANCE GotoURL (LPCTSTR url, int showcmd);
};

#endif // !defined(AFX_CHECKVERSION_H__C107AA19_524A_4FB6_AE0A_2937FF641C8B__INCLUDED_)
