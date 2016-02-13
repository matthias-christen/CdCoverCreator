#pragma once


// CDlgSetImageEditor dialog

class CDlgSetImageEditor : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetImageEditor)

public:
	CDlgSetImageEditor(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSetImageEditor();

// Dialog Data
	enum { IDD = IDD_DIALOG_SETIMAGEEDITOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
protected:
	virtual void OnOK();
public:
	CString m_strAppName;
};
