#pragma once

#include "ColourPicker.h"
#include "Gradient.h"

// CLinearGradientPicker

// CLinearGradientPicker messages
#define LGPN_UPDATE        WM_USER + 1101        // LinearGradient Picker update
#define ON_LGPN_UPDATE(id, memberFxn) \
	ON_CONTROL(LGPN_UPDATE, id, memberFxn)

class CLinearGradientPicker : public CStatic
{
	DECLARE_DYNAMIC(CLinearGradientPicker)

public:
	CLinearGradientPicker ();
	virtual ~CLinearGradientPicker ();

	void SetData (LINEARGRADIENTDATA data);
	void GetData (LINEARGRADIENTDATA* pData);

	void SetAngle (double dAngle);	// angle in radians
	void SetAngle (int nAngle);		// angle in 10*deg

	void SetTicks (COLORREF* colors, float* positions, int nNumTicks);
	void SetTick (COLORREF color, float position);

	void SetStartColor (COLORREF color) { SetTick (color, 0.0); }
	void SetEndColor (COLORREF color) { SetTick (color, 1.0); }

	void SetType (GradientType type) { m_type = type; InvalidateRect (NULL, FALSE); }

	void Reset ();

	inline COLORREF GetColor (int nIndex) { return RGB (m_colors[nIndex].GetRed (), m_colors[nIndex].GetGreen (), m_colors[nIndex].GetBlue ()); }
	inline double GetPosition (int nIndex) { return m_positions[nIndex]; }
	inline int GetTicksCount () { return m_nNumColors; }
	inline GradientType GetType () { return m_type; }

	void EndSelection (UINT nMessage);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnGradientMenu (UINT nId);

	afx_msg LRESULT OnColorChanged (WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()

	void SetSize ();

	void InsertTick (COLORREF color, float fPosition);
	void DeleteTick (int nIdx);
	void DrawTick (Gdiplus::Graphics* pGraphics, int nTickIdx, CRect& rectPicker);
	bool HitTestTick (CPoint point, int nTickIdx, CRect& rectPicker);
	COLORREF GetTickColor (int nTickIdx);
	CPoint GetTickPosition (int nTickIdx, CRect& rectPicker);

	CWnd* m_pParent;

	double m_dAngle;
	Gdiplus::Color m_colors[MAX_TICKS];
	Gdiplus::REAL m_positions[MAX_TICKS];
	GradientType m_type;
	int m_nCurrentTick;
	int m_nNumColors;
	bool m_bMovedTick;
};