// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "CdCoverCreator2.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_COMMAND(ID_BUTTON_MINIMIZE, OnWindowMinimize)
	ON_COMMAND(ID_BUTTON_RESTORE, OnWindowRestore)
	ON_COMMAND(ID_BUTTON_CLOSE, OnWindowClose)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_MINIMIZE, OnUpdateWindowMinimize)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RESTORE, OnUpdateWindowRestore)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CLOSE, OnUpdateWindowClose)
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers

void CChildFrame::OnWindowMinimize ()
{
	ShowWindow (SW_MINIMIZE);
}

void CChildFrame::OnWindowRestore ()
{
	if (IsZoomed ())
		MDIRestore ();
	else
		MDIMaximize ();
}

void CChildFrame::OnWindowClose ()
{
	MDIDestroy ();
}

void CChildFrame::OnUpdateWindowMinimize (CCmdUI* pCmdUI)
{
}

void CChildFrame::OnUpdateWindowRestore (CCmdUI* pCmdUI)
{
}

void CChildFrame::OnUpdateWindowClose (CCmdUI* pCmdUI)
{
}