#ifndef __SPLASHER_H__
#define __SPLASHER_H__

#include "CGdiplusBitmap.h"

///////////////// Classes //////////////////////////
class CSplashWnd : public CWnd
{
public:
	CSplashWnd();
	~CSplashWnd();

// Operations
	BOOL Create();
	void SetOKToClose() { m_bOKToClose = true; };	
	
	UINT m_nTimerId;

protected:
	//{{AFX_VIRTUAL(CSplashWnd)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSplashWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	
	bool LoadBitmap();
	void CreatePaletteFromBitmap();

	CGdiPlusBitmapResource* m_pBitmap;
	bool m_bOKToClose;
	int m_nHeight;
	int m_nWidth;
	CWnd m_wndOwner;
};


class CSplashThread : public CWinThread
{
public:
	void HideSplash();

protected:
	CSplashThread();
	virtual ~CSplashThread();

	DECLARE_DYNCREATE(CSplashThread)

	//{{AFX_VIRTUAL(CSplashThread)
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSplashThread)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	CSplashWnd m_SplashScreen;
};


#endif //__SPLASHER_H__


