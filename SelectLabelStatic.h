#pragma once

#define SLN_STATECHANGED (WM_USER+2403)
#define ON_SLN_STATECHANGED(id, memberFxn) \
	ON_CONTROL(SLN_STATECHANGED, id, memberFxn)

// CSelectLabelStatic

class CSelectLabelStatic : public CStatic
{
	DECLARE_DYNAMIC(CSelectLabelStatic)

public:
	CSelectLabelStatic();
	virtual ~CSelectLabelStatic();

	void SetState (int nState);
	inline void InternalSetState (int nState) { m_nState = nState; }
	inline int GetState () { return m_nState; }

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	afx_msg void OnPaint();

protected:
	void DrawLabel (CDC* pDC, CPoint ptCenter, int nDiameter, bool bSelected);
	int m_nState;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual void PreSubclassWindow();
};


