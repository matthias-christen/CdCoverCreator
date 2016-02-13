#pragma once

#include "EANBase.h"

class CEAN8 : public CEANBase
{
public:
	CEAN8 ();
	~CEAN8 (void);

	virtual void SetCode (CString strCode, CString strAddon = "");
	virtual CSize GetSize ();
	virtual void Render (Gdiplus::Graphics* pGraphics, CRect rect);

private:
	CString m_strBars;
};
