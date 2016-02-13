#pragma once
#include "afxwin.h"
#include <Gdiplus.h>

using namespace Gdiplus;

// CLightscribeDlg dialog

class CLightscribeDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightscribeDlg)

public:
	CLightscribeDlg(CWnd* pParent, CStringList* pListDevices);   // standard constructor
	virtual ~CLightscribeDlg();

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

// Dialog Data
	enum { IDD = IDD_DIALOG_LIGHTSCRIBE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	int m_nPrintQuality;
	CString m_strLightscribeDevice;

private:
	Image* m_pImage;
	CStringList* m_pListDevices;	
};
