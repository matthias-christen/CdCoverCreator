#pragma once


// CPSImageEditing dialog

class CPSImageEditing : public CPropertyPage
{
	DECLARE_DYNAMIC(CPSImageEditing)

public:
	CPSImageEditing(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPSImageEditing();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_PS_IMAGEEDITING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_strAppName;
	afx_msg void OnEnChangeEdit1();
	virtual BOOL OnInitDialog();
};
