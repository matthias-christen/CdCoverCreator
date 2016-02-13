#if !defined(AFX_DEGREESSTATIC_H__D34F4CC8_78AA_4A34_96DB_C1CAB5D7B6AA__INCLUDED_)
#define AFX_DEGREESSTATIC_H__D34F4CC8_78AA_4A34_96DB_C1CAB5D7B6AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DegreesStatic.h : header file
//

#define DSN_UPDATE		0x1000
#define DSN_BUTTONUP	0x1001

#define ON_DSN_UPDATE(id, memberFxn) \
	ON_CONTROL(DSN_UPDATE, id, memberFxn)
#define ON_DSN_BUTTONUP(id, memberFxn) \
	ON_CONTROL(DSN_BUTTONUP, id, memberFxn)

/////////////////////////////////////////////////////////////////////////////
// CDegreesStatic window

class CDegreesStatic : public CStatic
{
// Construction
public:
	CDegreesStatic();

// Attributes
public:

// Operations
public:
	inline double GetAngle () { return m_dAngle; }
	inline double GetNeedleWidth () { return m_dNeedleWidth; }
	inline void SetNeedleWidth (double dWidth) { m_dNeedleWidth = dWidth; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDegreesStatic)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetDegAngle();
	void SetDegAngle (int nAngle);
	void SetAngle (double dAngle);
	virtual ~CDegreesStatic();

	// Generated message map functions
protected:
	double m_dAngle;
	double m_dNeedleWidth;

	void UpdateDisplay (CPoint pt);
	//{{AFX_MSG(CDegreesStatic)
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEGREESSTATIC_H__D34F4CC8_78AA_4A34_96DB_C1CAB5D7B6AA__INCLUDED_)
