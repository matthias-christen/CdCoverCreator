#include "stdafx.h"
#include "resource.h"
#include "Splasher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define MOUSE_MOVE 0xF012

BEGIN_MESSAGE_MAP(CSplashThread, CWinThread)
	//{{AFX_MSG_MAP(CSplashThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CSplashThread, CWinThread)

CSplashThread::CSplashThread()
{
}

CSplashThread::~CSplashThread()
{
}

BOOL CSplashThread::InitInstance()
{
	//Attach this threads UI state to the main one, This will ensure that 
	//the activation state is managed consistenly across the two threads
	ASSERT (AfxGetApp ());
	BOOL bSuccess = AttachThreadInput (m_nThreadID, AfxGetApp ()->m_nThreadID, TRUE);
	if (!bSuccess)
		TRACE(_T("Failed in call to AttachThredInput, GetLastError:%d\n"), ::GetLastError());

	//Create the Splash Screen UI
	BOOL bCreated = m_SplashScreen.Create ();
	VERIFY (bCreated);
	m_pMainWnd = &m_SplashScreen;
	return bCreated;
}

void CSplashThread::HideSplash()
{
	m_SplashScreen.m_nTimerId = m_SplashScreen.SetTimer (1, 2000, NULL);
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	//{{AFX_MSG_MAP(CSplashWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

CSplashWnd::CSplashWnd()
{
	m_bOKToClose = FALSE;
	m_nHeight = 0;
	m_nWidth = 0;

	m_pBitmap = NULL;
}

CSplashWnd::~CSplashWnd()
{
	//destroy our invisible owner when we're done
	if (m_wndOwner.m_hWnd != NULL)
		m_wndOwner.DestroyWindow();

	if (m_pBitmap != NULL)
		delete m_pBitmap;
}

using namespace Gdiplus;

bool CSplashWnd::LoadBitmap()
{
	m_pBitmap = new CGdiPlusBitmapResource (
#ifdef PEROUTKA_FEDORISIN
		IDR_JPEG_SPLASHSCREEN_PF,
#else
		IDR_JPEG_SPLASHSCREEN,
#endif
		"JPEG");

	m_nWidth = ((Bitmap*) *m_pBitmap)->GetWidth ();
	m_nHeight = ((Bitmap*) *m_pBitmap)->GetHeight ();

	return true;
}

BOOL CSplashWnd::Create()
{
	//Load up the bitmap from file or from resource
	VERIFY (LoadBitmap ());

	//Modify the owner window of the splash screen to be an invisible WS_POPUP 
	//window so that the splash screen does not appear in the task bar
	LPCTSTR pszWndClass = AfxRegisterWndClass(0);
	VERIFY(m_wndOwner.CreateEx(0, pszWndClass, _T(""), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0));

	//Create this window
	pszWndClass = AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	VERIFY(CreateEx(0, pszWndClass, _T(""), WS_POPUP | WS_VISIBLE, 0, 0, m_nWidth, m_nHeight, m_wndOwner.GetSafeHwnd(), NULL));
#ifndef _DEBUG
	SetWindowPos (&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#endif

	return TRUE;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Center the splash window on the screen
	CenterWindow();

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc (this);

	Graphics g (dc.GetSafeHdc ());
	g.DrawImage ((Bitmap*) *m_pBitmap, 0, 0, m_nWidth, m_nHeight);
}

//This message is an optional extra, If you do not want the splash screen
//to be not be dragable then remove this function and its message map entry
void CSplashWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//Fake a Window drag
	SendMessage(WM_LBUTTONUP);
	SendMessage(WM_SYSCOMMAND, MOUSE_MOVE);
}

void CSplashWnd::OnClose() 
{
	if (m_bOKToClose) 
		CWnd::OnClose();
}

void CSplashWnd::OnTimer(UINT nIDEvent)
{
	KillTimer (m_nTimerId);

	//Wait until the splash screen has been created
	//before trying to close it
	while (!GetSafeHwnd())
		;

	SetOKToClose ();
	SendMessage (WM_CLOSE);

	CWnd::OnTimer(nIDEvent);
}
