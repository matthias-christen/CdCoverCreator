// HyperlinkStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "HyperlinkStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHyperlinkStatic

CHyperlinkStatic::CHyperlinkStatic()
{
	m_bTrackLeave = false;
	m_pFont = m_pFontUnderline = NULL;

	m_hLinkCursor = AfxGetApp ()->LoadCursor (IDC_CURSOR_HAND);
	m_hBkBrush = ::GetSysColorBrush (COLOR_3DFACE);
}

CHyperlinkStatic::~CHyperlinkStatic()
{
	if (m_pFontUnderline)
	{
		m_pFontUnderline->DeleteObject ();
		delete m_pFontUnderline;
	}

	DestroyCursor (m_hLinkCursor);
}


BEGIN_MESSAGE_MAP(CHyperlinkStatic, CStatic)
	//{{AFX_MSG_MAP(CHyperlinkStatic)
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperlinkStatic message handlers

void CHyperlinkStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTrackLeave)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof (TRACKMOUSEEVENT);
		tme.hwndTrack = GetSafeHwnd ();
		tme.dwFlags = TME_LEAVE;

		_TrackMouseEvent (&tme);

		m_bTrackLeave = true;
		InvalidateRect (NULL);
	}
	
	CStatic::OnMouseMove(nFlags, point);
}

void CHyperlinkStatic::OnClicked() 
{
	CString s;
	GetWindowText (s);
	GotoURL ("mailto:" + s + "?subject=" + m_strSubject + "&cc=" + m_strCC + "&bcc=" + m_strBCC + "&body=" + m_strBody, SW_SHOW);
}

LRESULT CHyperlinkStatic::OnMouseLeave (WPARAM wParam, LPARAM lParam)
{
	m_bTrackLeave = false;
	InvalidateRect (NULL);

	return S_OK;
}

HINSTANCE CHyperlinkStatic::GotoURL (LPCTSTR url, int showcmd)
{
    TCHAR key[MAX_PATH + MAX_PATH];	

    // First try ShellExecute()
    HINSTANCE result = ShellExecute (NULL, _T ("open"), url, NULL,NULL, showcmd);

    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR)
	{		
        if (GetRegKey (HKEY_CLASSES_ROOT, _T (".htm"), key) == ERROR_SUCCESS)
		{
            lstrcat (key, _T ("\\shell\\open\\command"));

            if (GetRegKey (HKEY_CLASSES_ROOT, key, key) == ERROR_SUCCESS)
			{
                TCHAR *pos;
                pos = _tcsstr (key, _T ("\"%1\""));
                if (pos == NULL)
				{										// No quotes found
                    pos = strstr(key, _T ("%1"));		// Check for %1, without quotes
                    if (pos == NULL)					// No parameter at all...
                        pos = key + lstrlen (key) - 1;
                    else
                        *pos = '\0';					// Remove the parameter
                }
                else
                    *pos = '\0';						// Remove the parameter

                lstrcat (pos, _T (" "));
                lstrcat (pos, url);
                result = (HINSTANCE) WinExec (key,showcmd);
            }
        }
	}
	  
    return result;
}

HBRUSH CHyperlinkStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	if (m_pFont == NULL)
	{
		m_pFont = GetFont ();

		LOGFONT lf;
		m_pFont->GetLogFont (&lf);

		m_pFontUnderline = new CFont ();
		lf.lfUnderline = 1;
		m_pFontUnderline->CreateFontIndirect (&lf);
	}

	pDC->SelectObject (m_bTrackLeave ? m_pFontUnderline : m_pFont);
	pDC->SetTextColor (RGB (0x00, 0x00, 0xFF));
	pDC->SetBkMode (TRANSPARENT);
	
	return m_hBkBrush;
}

LONG CHyperlinkStatic::GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];
		RegQueryValue(hkey, NULL, data, &datasize);
		lstrcpy(retdata,data);
		RegCloseKey(hkey);
    }

    return retval;
}

BOOL CHyperlinkStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	::SetCursor (m_hLinkCursor);
	return TRUE;
}

BOOL CHyperlinkStatic::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= SS_NOTIFY;
	return CStatic::PreCreateWindow(cs);
}

void CHyperlinkStatic::PreSubclassWindow() 
{
	LONG lStyle = ::GetWindowLong (GetSafeHwnd (), GWL_STYLE);
	::SetWindowLong (GetSafeHwnd (), GWL_STYLE, lStyle | SS_NOTIFY);

	CStatic::PreSubclassWindow();
}

void CHyperlinkStatic::SetBodyText(CString str)
{
	m_strBody = encode (str);
}

void CHyperlinkStatic::SetSubjectText(CString str)
{
	m_strSubject = encode (str);
}

void CHyperlinkStatic::SetBCC(CString str)
{
	m_strBCC = encode (str);
}

void CHyperlinkStatic::SetCC(CString str)
{
	m_strCC = encode (str);
}

CString CHyperlinkStatic::encode(CString s)
{
	CString str, s1;
	for (int i = 0; i < s.GetLength (); i++)
	{
		if ((s[i] <= 0x20) || (s[i] >= 0x7F))
		{
			s1.Format ("%%%02X", s[i]);
			str += s1;
		}
		else
			str += s[i];
	}

	return str;
}
