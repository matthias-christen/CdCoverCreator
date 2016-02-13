#pragma once

#include "ProgressBar.h"

#import "C:\Programme\Gemeinsame Dateien\LightScribe\LSCAPI.dll"

class CLightscribe : public LSCAPILib::ILSDiscPrintProgressEvents
{
public:
	CLightscribe (HBITMAP hBmp, BITMAPINFOHEADER* pBi, BYTE* pBits);
	~CLightscribe (void);

	static UINT PrintThread (LPVOID lpParam);

	void CleanUp ();

	// IUnknown
	HRESULT __stdcall QueryInterface (const GUID& riid, void** ppObj);
	ULONG __stdcall AddRef ();
	ULONG __stdcall Release ();

	// ILSDiscPrintProgressEvents

    HRESULT NotifyPreparePrintProgress (int current, int final)
	{
		return raw_NotifyPreparePrintProgress (current, final);
	}

    HRESULT NotifyPrintProgress (int areaPrinted, int totalArea)
	{
		return raw_NotifyPrintProgress (areaPrinted, totalArea);
	}

    HRESULT NotifyPrintComplete (HRESULT status)
	{
		return raw_NotifyPrintComplete (status);
	}

    VARIANT_BOOL QueryCancel ()
	{
		VARIANT_BOOL b;
		raw_QueryCancel (&b);
		return b;
	}

    VARIANT_BOOL ReportLabelTimeEstimate (long seconds)
	{
		VARIANT_BOOL b;
		raw_ReportLabelTimeEstimate (seconds, &b);
		return b;
	}

    //
    // Raw methods provided by interface
    //
	
	virtual HRESULT __stdcall raw_NotifyPreparePrintProgress (/*[in]*/ int nCurrent, /*[in]*/ int nFinal);
	virtual HRESULT __stdcall raw_NotifyPrintProgress (/*[in]*/ int nAreaPrinted, /*[in]*/ int nTotalArea);
	virtual HRESULT __stdcall raw_NotifyPrintComplete (/*[in]*/ HRESULT hrStatus);
	virtual HRESULT __stdcall raw_QueryCancel (/*[out,retval]*/ VARIANT_BOOL* pbCancel);
	virtual HRESULT __stdcall raw_ReportLabelTimeEstimate (/*[in]*/ long lSeconds, /*[out,retval]*/ VARIANT_BOOL* pbCancel);

private:
	//void UpdateTime ();
	void ShowErrorMessage (WORD wError);

private:
	//LSCAPILib::ILSDiscPrintMgrPtr* m_pManager;
	LSCAPILib::ILSDiscPrinter* m_pPrinter;

	LSCAPILib::PrintQuality m_printQuality;

	HBITMAP m_hBmp;
	BITMAPINFOHEADER* m_pBi;
	BYTE* m_pBits;

	HWND m_hwndProgress;
	HWND m_hwndStatus;
	char m_szMsg[128], m_szMsgTime[128];

	//DWORD m_dwStartTime;
	//long m_lEstimatedRemainingSeconds;
	//int m_nTimeEstimatesCount;
	//long m_lCurrRemainingSeconds;
	//int m_nSecondsSinceLastAdjust;
	//long m_nPrevAreaPrinted;

	int m_nRefsCount;
	bool m_bPrintingFinished;
};
