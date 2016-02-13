// CdCoverCreator2View.h : interface of the CCdCoverCreator2View class
//


#pragma once

#include "LabelRenderer.h"	// Added by ClassView
#include "StdAfx.h"	// Added by ClassView
#include <afxole.h>
#include "AutoRichEditCtrl.h"	// Added by ClassView
#include "TrackEdit.h"

#include "SelectLabelsDlg.h"
#include "FloatingGraphicsObject.h"

#include <vector>

class CCdCoverCreator2View : public CScrollView
{
	friend class CLeftPanelDlgBar;

protected: // create from serialization only
	CCdCoverCreator2View();
	DECLARE_DYNCREATE(CCdCoverCreator2View)

// Attributes
public:
	CCdCoverCreator2Doc* GetDocument() const;

// Operations
public:

// Overrides
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
public:
	virtual ~CCdCoverCreator2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CRectTracker m_trackerImage;
	CRectTracker m_trackerTracks;
	CRectTracker m_trackerTitle;

	RenderType GetRenderType();
	void ShowRenderType (RenderType rt);
	
	CAutoRichEditCtrl m_wndRichEdit;
	CTrackEdit m_wndTrackEdit;

#ifdef PEROUTKA_FEDORISIN
	CSelectLabelsDlg m_dlgSelectLabels;
#endif

protected:

// Generated message map functions
protected:
	void DoPrint (CDC* pDC, CPrintInfo* pInfo, RenderType rt, CSize szUnit, int nLeft, int nTop, int nParam = 0, bool bRevertPaper = false);
	void DoPrintSlimCase (CDC* pDC, CPrintInfo* pInfo, RenderType rt, CSize szUnit, int nLeft, int nTop);
	void DoPrintPaperSleeve (CDC* pDC, CPrintInfo* pInfo, RenderType rt, CSize szUnit, int nLeft, int nTop);
	void DoPrintDVDBox (CDC* pDC, CPrintInfo* pInfo, RenderType rt, CSize szUnit, int nLeft, int nTop);
	void DoPrintBooklet (CDC* pDC, CPrintInfo* pInfo, CSize szUnit, int nLeft, int nTop, double dMarginLeft, double dMarginTop, int nOrientation);
	void DoPreparePrintBooklet (CDC* pDC, CPrintInfo* pInfo);

	void SetRTF (CString strRTF);

	int GetEncoderClsid (const WCHAR* format, CLSID* pClsid);

	void AddToolObject (CFloatingGraphicsObject* pObj, CPoint pt);
	CFloatingGraphicsObject* m_pCurrentAddingObj;
	CFloatingGraphicsObject* m_pCurrentContextMenuObj;

	std::vector<long> m_vecBookletPages;

	void DrawTrackers (CRenderDC& rdc);
	CRectTracker* m_pPrevTrck;

	void AdjustScrollSizes();
	
	RenderType m_RenderType;
	CBitmap m_Bitmap;
	int m_nHeight;

	bool m_bIsEditingTitle;
	int m_nEditingTrackIdx;
	int m_nEditingTrackNum;
	CFloatingGraphicsObject* m_pEditingFGO;

	bool m_bSettingTransparentColor;
	HCURSOR m_hPickCursor;

	HANDLE m_hndPrinter;

	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnUpdateZoomin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomout(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnShowinlay();
	afx_msg void OnUpdateShowinlay(CCmdUI* pCmdUI);
	afx_msg void OnShowcover();
	afx_msg void OnUpdateShowcover(CCmdUI* pCmdUI);
	afx_msg void OnShowlabel();
	afx_msg void OnUpdateShowlabel(CCmdUI* pCmdUI);
	afx_msg void OnShowback();
	afx_msg void OnUpdateShowback(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowbooklet();
	afx_msg void OnUpdateShowbooklet(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg void OnRichEditEscape ();
	afx_msg void OnTrackEditEscape ();
	afx_msg void OnTrackEditUp ();
	afx_msg void OnTrackEditDown ();
	afx_msg void OnTrackEditNext ();
	afx_msg void OnTrackEditPrev ();

	afx_msg void OnToolAddText ();
	afx_msg void OnToolAddImage ();
	afx_msg void OnToolAddBarcode ();
	afx_msg void OnUpdateToolAddText (CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolAddImage (CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolAddBarcode (CCmdUI* pCmdUI);

	afx_msg void OnFloatingObjectMenuPosition (UINT nId);
	afx_msg void OnFloatingObjectMenuDelete ();
	afx_msg void OnFloatingImageMenuSelectTransparentColor ();
	afx_msg void OnFloatingImageMenuAdjustColors ();
	afx_msg void OnFloatingImageEdit ();
	afx_msg void OnFloatingImageReload ();
	afx_msg void OnFloatingImageAlign (UINT nId);
	afx_msg void OnFloatingImageStretchToFit ();
	afx_msg void OnFloatingBarcodeProperties ();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAlignBottomleft();
	afx_msg void OnUpdateFileLightscribe(CCmdUI *pCmdUI);
	afx_msg void OnFileSaveAsImage();
};

#ifndef _DEBUG  // debug version in CdCoverCreator2View.cpp
inline CCdCoverCreator2Doc* CCdCoverCreator2View::GetDocument() const
   { return reinterpret_cast<CCdCoverCreator2Doc*>(m_pDocument); }
#endif

