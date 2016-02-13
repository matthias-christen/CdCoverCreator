#if !defined(AFX_AUTORICHEDITCTRL_H__C26D1E0E_DD32_11D2_B39F_000092914562__INCLUDED_)
#define AFX_AUTORICHEDITCTRL_H__C26D1E0E_DD32_11D2_B39F_000092914562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _AFX_RICHEDITEX_STATE
{
public:
    _AFX_RICHEDITEX_STATE();
    virtual ~_AFX_RICHEDITEX_STATE();
    HINSTANCE m_hInstRichEdit20 ;
};
BOOL PASCAL AfxInitRichEditEx();

#define AEN_ESCAPE 0x0700

// AutoRichEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoRichEditCtrl window

class CAutoRichEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CAutoRichEditCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoRichEditCtrl)
	virtual BOOL Create (DWORD dwStyle, const RECT& rcRect, CWnd* pParentWnd, UINT nID);	
	//}}AFX_VIRTUAL

// Implementation
public:
	long GetSelectionFontSize();
	CString GetSelectionFontName();
	CStringArray m_saFontList;
	void GetSystemFonts(CStringArray &saFontList);
	void SetFontSize(int nPointSize);
	void SetFontName(CString sFontName);
	void SelectColor();
	bool ParagraphIsBulleted();
	void SetParagraphBulleted();
	
	PARAFORMAT GetParagraphFormat();
	
	bool ParagraphIsRight();
	bool ParagraphIsLeft();
	bool ParagraphIsCentered();
	
	void SetParagraphRight();
	void SetParagraphLeft();
	void SetParagraphCenter();
	
	CHARFORMAT GetCharFormat(DWORD dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE);
	
	bool SelectionIsBold();
	bool SelectionIsItalic();
	bool SelectionIsUnderlined();

	void SetSelectionBold();
	void SetSelectionItalic();
	void SetSelectionUnderlined();

	void SetRTF(CString sRTF);
	CString GetRTF();
	virtual ~CAutoRichEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAutoRichEditCtrl)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
		void SetCharStyle(int MASK, int STYLE, int nStart, int nEnd);
		static DWORD CALLBACK CBStreamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
		static DWORD CALLBACK CBStreamOut(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);
		static BOOL CALLBACK CBEnumFonts(LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTORICHEDITCTRL_H__C26D1E0E_DD32_11D2_B39F_000092914562__INCLUDED_)
