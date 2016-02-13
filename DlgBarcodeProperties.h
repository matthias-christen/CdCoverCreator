#pragma once
#include "colourpicker.h"
#include "afxwin.h"
#include "fontselectcombobox.h"

#include "FloatingBarcode.h"

// CDlgBarcodeProperties dialog

class CDlgBarcodeProperties : public CDialog
{
	DECLARE_DYNAMIC(CDlgBarcodeProperties)

public:
	CDlgBarcodeProperties(CFloatingBarCode* pBarcode, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBarcodeProperties();

// Dialog Data
	enum { IDD = IDD_DIALOG_BARCODEPROPS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CColourPicker m_cpForeground;
	CColourPicker m_cpBackground;
	CComboBox m_cmbType;
	CFontSelectComboBox m_cmbFont;

protected:
	CFloatingBarCode* m_pBarcode;
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	BOOL m_bTransparentBackground;
};
