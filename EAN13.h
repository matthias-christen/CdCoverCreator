#pragma once
#include "eanbase.h"

class CEAN13 : public CEANBase
{
public:
	CEAN13 ();
	~CEAN13 (void);

	virtual void SetCode (CString strCode, CString strAddon = "");
	virtual CSize GetSize ();
	virtual void Render (Graphics* pGraphics, CRect rect);

private:
	CString m_strBars;
	CString m_strAddBars;
};
