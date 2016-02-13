// cccTwain.h: interface for the CcccTwain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCTWAIN_H__79008DB7_EA0F_4393_9FA0_792C3505F932__INCLUDED_)
#define AFX_CCCTWAIN_H__79008DB7_EA0F_4393_9FA0_792C3505F932__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Twain.h"
#include "CreationWizardPages.h"

class CcccTwain : public CTwain
{
public:
	CcccTwain();
	virtual ~CcccTwain();

	void CopyImage (HANDLE hBitmap, TW_IMAGEINFO& info);

	CCWBkImgPage* m_pBkImgPage;

protected:
	int GetEncoderClsid (const WCHAR* format, CLSID* pClsid);
};

#endif // !defined(AFX_CCCTWAIN_H__79008DB7_EA0F_4393_9FA0_792C3505F932__INCLUDED_)
