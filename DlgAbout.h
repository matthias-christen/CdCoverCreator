#pragma once


// CDlgAbout dialog

class CDlgAbout : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CDlgAbout)

public:
	CDlgAbout(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAbout();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_DIALOG_ABOUT, IDH = IDR_HTML_DLGABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
