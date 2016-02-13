#pragma once
#include "afxext.h"

class CFloatObjRectTracker : public CRectTracker
{
public:
	CFloatObjRectTracker(void);
	~CFloatObjRectTracker(void);

	virtual void AdjustRect (int nHandle, LPRECT lpRect);

	void SetRect (CRect r);
	inline void KeepAspectRatio (bool bKeep) { m_bKeepAspectRatio = bKeep; }
	inline void CanResize (bool bCanResize) { m_bCanResize = bCanResize; }

protected:
	CRect m_rectReference;
	bool m_bKeepAspectRatio;
	bool m_bCanResize;
};
