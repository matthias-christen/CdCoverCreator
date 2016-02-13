#include "StdAfx.h"
#include "CdCoverCreator2.h"
#include ".\lightscribe.h"
#include "LightscribeDlg.h"

#include "MainFrm.h"

CLightscribe::CLightscribe (HBITMAP hBmp, BITMAPINFOHEADER* pBi, BYTE* pBits)
{
	m_nRefsCount = 0;
	m_bPrintingFinished = false;

	m_hBmp = hBmp;
	m_pBi = pBi;
	m_pBits = pBits;

	m_pPrinter = NULL;

	AfxGetApp ()->DoWaitCursor (1);

	// create grayscale image (must be grayscale for lightscribe)
	for (long i = 0; i < pBi->biWidth * pBi->biHeight; i++)
	{
		// grayscale formula: gray = 0.3 red + 0.59 green + 0.11 blue
		BYTE byGray = (pBits[3 * i + 0] * 11 + pBits[3 * i + 1] * 59 + pBits[3 * i + 2] * 30) / 100;
		pBits[3 * i + 0] = pBits[3 * i + 1] = pBits[3 * i + 2] = byGray;
	}

	AfxGetApp ()->DoWaitCursor (-1);

	LSCAPILib::ILSEnumDiscPrinters* pEnum = NULL;
	LSCAPILib::ILSDiscPrinter* pPrinter = NULL;
	LSCAPILib::ILSDiscPrintSession* pSession = NULL;

	try
	{
		//CoInitializeEx (NULL, 0);

		LSCAPILib::ILSDiscPrintMgrPtr* pManager = new LSCAPILib::ILSDiscPrintMgrPtr ("{5197646C-00EA-4307-A067-61319EBBE499}");
		(*pManager)->EnumDiscPrinters (&pEnum);

		if (pEnum->Count () == 0)
		{
			AfxMessageBox ("No Lightscribe device found", MB_ICONERROR);
			pEnum->Release ();
			pManager->Release ();
			delete pManager;
			return;
		}

		// show dialog
		AfxGetApp ()->DoWaitCursor (1);
		CStringList listDevices;
		bool bPreviewGenerated = false;
		for (int i = 0; i < pEnum->Count (); i++)
		{
			pEnum->Item (i, &pPrinter);
			if (pPrinter->Validate () != S_OK)
			{
				pPrinter->Release ();
				continue;
			}

			listDevices.AddTail ((LPCTSTR) pPrinter->GetPrinterDisplayName ());

			// generate preview
			if (!bPreviewGenerated)
			{
				CString strPreviewPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetImageCacheDir () + "lspreview.bmp";
				BSTR bstrPreviewPath = strPreviewPath.AllocSysString ();

				LSCAPILib::Size size;
				size.x = 200;
				size.y = 200;

				pPrinter->OpenPrintSession (&pSession);
				pSession->PrintPreview (LSCAPILib::windows_bitmap, LSCAPILib::label_mode_full,
					LSCAPILib::draw_default, LSCAPILib::quality_draft,
					LSCAPILib::media_generic,
					(BYTE*) m_pBi, sizeof (BITMAPINFOHEADER), m_pBits, m_pBi->biSizeImage, bstrPreviewPath, LSCAPILib::windows_bitmap, &size, VARIANT_TRUE);

				::SysFreeString (bstrPreviewPath);
				pSession->Close ();
				pSession->Release ();

				bPreviewGenerated = true;
			}

			pPrinter->Release ();
		}
		AfxGetApp ()->DoWaitCursor (-1);

		// cleanup
		pEnum->Release ();
		pManager->Release ();
		delete pManager;

		CLightscribeDlg dlg (AfxGetMainWnd (), &listDevices);
		switch (dlg.DoModal ())
		{
		case IDOK:
			{
				switch (dlg.m_nPrintQuality)
				{
				case 0:
					m_printQuality = LSCAPILib::quality_draft;
					break;
				case 1:
					m_printQuality = LSCAPILib::quality_normal;
					break;
				case 2:
					m_printQuality = LSCAPILib::quality_best;
					break;
				}

				m_hwndStatus = ((CMainFrame*) AfxGetMainWnd ())->GetStatusBar ()->GetSafeHwnd ();
				m_hwndProgress = ((CMainFrame*) AfxGetMainWnd ())->GetProgressBar ()->GetSafeHwnd ();
				ShowWindow (m_hwndProgress, SW_SHOW);

				// re-create the manager and the enum
				LSCAPILib::ILSDiscPrintMgrPtr* pManager = new LSCAPILib::ILSDiscPrintMgrPtr ("{5197646C-00EA-4307-A067-61319EBBE499}");
				(*pManager)->EnumDiscPrinters (&pEnum);
				m_pPrinter = NULL;

				// find the printer that has been selected
				for (int i = 0; i < pEnum->Count (); i++)
				{
					pEnum->Item (i, &pPrinter);
					if (pPrinter->Validate () != S_OK)
					{
						pPrinter->Release ();
						continue;
					}

					if (strcmp ((const char*) pPrinter->GetPrinterDisplayName (), dlg.m_strLightscribeDevice.GetBuffer (-1)) == 0)
					{
						m_pPrinter = pPrinter;
						break;
					}
					else
						pPrinter->Release ();
				}
				pEnum->Release ();
				pManager->Release ();
				delete pManager;

				// print the disc if there is any printer
				if (m_pPrinter != NULL)
				{
					// identify the media

					// start printing
					AfxBeginThread (PrintThread, this);
				}
				else
					CleanUp ();
			}
			break;

		case IDCANCEL:
			// clean up
			CleanUp ();
			break;
		}
	}
	catch (_com_error &err)
	{
		AfxGetApp ()->DoWaitCursor (-1);
		ShowErrorMessage (err.Error () & 0x0000ffff);

		if (pSession != NULL)
		{
			pSession->Close ();
			pSession->Release ();
		}
		if (pPrinter != NULL)
			pPrinter->Release ();
		if (pEnum != NULL)
			pEnum->Release ();

		CleanUp ();
	}
}

UINT CLightscribe::PrintThread (LPVOID lpParam)
{
	CLightscribe* _THIS = (CLightscribe*) lpParam;
	//_THIS->m_nTimeEstimatesCount = 0;

	LSCAPILib::ILSDiscPrintSession* pSession = NULL;

	int nStep = 0;

	try
	{
		// step 0:
		// add exclusive use
		_THIS->m_pPrinter->AddExclusiveUse ();
		nStep++;

		// step 1:
		// lock drive
		_THIS->m_pPrinter->LockDriveTray ();
		nStep++;

		// step 2:
		// get a session
		_THIS->m_pPrinter->OpenPrintSession (&pSession);
		nStep++;

		// step 3:
		// set the callback class
		pSession->SetProgressCallback (_THIS);
		nStep++;

		// step 4:
		// print the disc
		pSession->PrintDisc (LSCAPILib::windows_bitmap, LSCAPILib::label_mode_full,
			LSCAPILib::draw_fit_smallest_to_label, _THIS->m_printQuality,
			LSCAPILib::media_recognized,
			(BYTE*) (_THIS->m_pBi), _THIS->m_pBi->biSize, _THIS->m_pBits, _THIS->m_pBi->biSizeImage);
		nStep++;

		// terminate the session
		pSession->ReleaseProgressCallback ();
		pSession->Close ();
		pSession->Release ();

		// unlock drive
		_THIS->m_pPrinter->UnlockDriveTray ();
		_THIS->m_pPrinter->OpenDriveTray ();
		_THIS->m_pPrinter->ReleaseExclusiveUse ();

		// clean up
		ShowWindow (_THIS->m_hwndProgress, SW_HIDE);
		PostMessage (_THIS->m_hwndStatus, SB_SETTEXT, 1, (LPARAM) "");
		//PostMessage (_THIS->m_hwndStatus, SB_SETTEXT, 3, (LPARAM) "");

		_THIS->CleanUp ();
	}
	catch (_com_error &err)
	{
		_THIS->ShowErrorMessage (err.Error () & 0x0000ffff);

		// cleanup LS stuff that has been done in this method
		switch (nStep)
		{
		case 5:
		case 4:
		case 3:
			if (pSession != NULL)
				pSession->ReleaseProgressCallback ();
		case 2:
			if (pSession != NULL)
			{
				pSession->Close ();
				pSession->Release ();
			}
		case 1:
			_THIS->m_pPrinter->UnlockDriveTray ();
		case 0:
			_THIS->m_pPrinter->ReleaseExclusiveUse ();
		}

		// clean up general/GUI stuff
		ShowWindow (_THIS->m_hwndProgress, SW_HIDE);
		PostMessage (_THIS->m_hwndStatus, SB_SETTEXT, 1, (LPARAM) "");
		//PostMessage (_THIS->m_hwndStatus, SB_SETTEXT, 3, (LPARAM) "");

		_THIS->CleanUp ();
	}

	return 0;
}

void CLightscribe::CleanUp ()
{
	if (m_pPrinter != NULL)
		m_pPrinter->Release ();

	//CoUninitialize ();

	DeleteObject (m_hBmp);
	if (m_pBi != NULL)
		delete m_pBi;

	delete this;
}

//void CLightscribe::UpdateTime ()
//{
//	if (m_nSecondsSinceLastAdjust >= 60 || m_lCurrRemainingSeconds < 0)
//	{
//		m_lCurrRemainingSeconds = m_lEstimatedRemainingSeconds;
//		m_nSecondsSinceLastAdjust = 0;
//	}
//
//	sprintf (m_szMsgTime, "Time left: %d:%02d", m_lCurrRemainingSeconds / 60, m_lCurrRemainingSeconds % 60);
//	::PostMessage (m_hwndStatus, SB_SETTEXT, 3, (LPARAM) m_szMsgTime);
//}

void CLightscribe::ShowErrorMessage (WORD wError)
{
	CString strErrorMsg = "";
	bool bUpdate = false;

	switch (wError)
	{
	case 0x205:		// LS_E_InvalidMediaOrientation
		strErrorMsg = "To label your LightScribe disc, please insert the disc with the label side down.";
		break;

	case 0x208:		// LS_E_UnableToReadMediaParams
	case 0x23D:		// LS_E_NonLightScribeMedia
		strErrorMsg = "The disc in the LightScribe drive is not recognized.\nPlease ensure that the disc in the drive is a LightScribe disc\nand is inserted into the drive label side down.\n\nIf your LightScribe disc is still not recognized,\ndirt or smudges may be on the disc.\nPlease wipe both disc surfaces with a soft cloth and try again.";
		break;

	case 0x209:		// LS_E_InvalidMediaParams
	case 0x220:		// LS_E_MediaNotSupportedByDrive
	case 0x23C:		// LS_E_IncompatibleMedia
		strErrorMsg = "This disc is incompatible with the selected LightScribe drive and can not be labeled.";
		break;

	case 0x211:		// LS_E_UnsupportedSource
	case 0x212:		// LS_E_InvalidSource
	case 0x21B:		// LS_E_CallbackException
	case 0x21D:		// LS_E_UnsupportedPreview
	case 0x223:		// LS_E_ReentrantCall
	case 0x226:		// LS_E_BadArgument
	case 0x22D:		// LS_E_UnsupportedPrintQuality

	case 0x22F:		// LS_E_InternalError
	case 0x1080:	// LS_E_CommunicationsError
	case 0x1081:	// LS_E_HardwareError

		strErrorMsg = "Your labeling application has caused an internal LightScribe System Software (LSS) error.\nPlease restart your labeling application and try your label again.\n\nIf this error occurs again,\nplease contact your labeling application support.";
		break;

	case 0x219:		// LS_E_Aborted
		strErrorMsg = "LightScribe labeling has been canceled";
		break;

	case 0x21A:		// LS_E_MemoryError
		strErrorMsg = "";
		break;

	case 0x21E:		// LS_E_DriveInUse
	case 0x22A:		// LS_E_DriveTrayLocked
		strErrorMsg = "The LightScribe drive is currently in use";
		break;

	case 0x229:		// LS_E_PrintPreviewFileError
		strErrorMsg = "";
		break;
	
	case 0x22B:		// LS_E_UnsupportedDrive
	case 0x22E:		// LS_E_CorruptResourceFile
	case 0x234:		// LS_E_UpdateRequired
	case 0x237:		// LS_E_UnsupportedMediaId
	case 0x239:		// LS_E_UnsupportedDriveCommunications
		strErrorMsg = "An update to the LightScribe System Software (LSS) is required to use your selected LightScribe drive.";
		bUpdate = true;
		break;

	case 0x22C:		// LS_E_UnsupportedMedia
	case 0x23B:		// LS_E_UnsupportedMediaNoUpdate
		strErrorMsg = "“The LightScribe System Software (LSS) does not contain information about the LightScribe disc in the drive.\nFor optimal LightScribe labeling, an update to the LSS is recommended before labeling to this disc.\nOr you may choose to use generic labeling.";
		bUpdate = true;
		break;

	case 0x233:		// LS_E_DriveChanged
		strErrorMsg = "The LightScribe drive has changed.\nThis may have been caused by a hardware change or a Plug and Play event.\nPlease re-select the LightScribe drive that you wish to use.";
		break;

	case 0x23E:		// LS_E_InstallationError
		strErrorMsg = "A critical internal LightScribe System Software (LSS) error has occurred.\nPlease reinstall or update the LSS and try your label again.";
		bUpdate = true;
		break;

	case 0x1012:	// LS_E_NoMedia
		strErrorMsg = "A LightScribe disc was not detected in the drive.\nPlease insert a LightScribe disc with the label side down.";
		break;

	case 340:
		strErrorMsg = "The Lightscribe System Software isn't installed on your computer.\nPlease download it from http://www.lightscribe.com and install it.";

	default:
		strErrorMsg.Format ("Lightscribe failure.\nUnknown error: %d", wError);
	}

	if (bUpdate)
		strErrorMsg += "\n\nFor updates of the LSS please visit http://www.lightscribe.com";

	AfxMessageBox (strErrorMsg, MB_ICONERROR);
}

CLightscribe::~CLightscribe (void)
{
}

HRESULT CLightscribe::QueryInterface (const GUID& riid, void** ppObj)
{
	HRESULT hr = E_NOINTERFACE;

	if (riid == IID_IUnknown)
	{
		*ppObj = static_cast<LSCAPILib::ILSDiscPrintProgressEvents*> (this);
		hr = S_OK;
	}
	else if (riid == __uuidof (LSCAPILib::ILSDiscPrintProgressEvents))
	{
		*ppObj = static_cast<LSCAPILib::ILSDiscPrintProgressEvents*> (this);
		hr = S_OK;
	}

	if (SUCCEEDED (hr))
		AddRef ();

	return hr;
}

ULONG CLightscribe::AddRef ()
{
	return ++m_nRefsCount;
}

ULONG CLightscribe::Release ()
{
	if (--m_nRefsCount)
	{
		delete this;
		return 0;
	}

	return m_nRefsCount;
}

HRESULT CLightscribe::raw_NotifyPreparePrintProgress (int nCurrent, int nFinal)
{
	::PostMessage (m_hwndStatus, SB_SETTEXT, 1, (LPARAM) "Lightscribe: Prepare Printing");
	::PostMessage (m_hwndProgress, PBM_SETRANGE32, 0, nFinal);
	::PostMessage (m_hwndProgress, PBM_SETPOS, nCurrent, 0);

	return S_OK;
}

HRESULT CLightscribe::raw_NotifyPrintProgress (int areaPrinted, int totalArea)
{
	sprintf (m_szMsg, "Lightscribe: Printing (%d%%)", (areaPrinted * 100) / totalArea);
	::PostMessage (m_hwndStatus, SB_SETTEXT, 1, (LPARAM) m_szMsg);
	::PostMessage (m_hwndProgress, PBM_SETRANGE32, 0, totalArea);
	::PostMessage (m_hwndProgress, PBM_SETPOS, areaPrinted, 0);

	// calculate remaining time
	//int nElapsedTime = (GetTickCount () - m_dwStartTime) / 1000;
	//DWORD dwTimeLeft = (nElapsedTime * totalArea) / areaPrinted - nElapsedTime;
	//m_lEstimatedRemainingSeconds = (m_lEstimatedRemainingSeconds * m_nTimeEstimatesCount + dwTimeLeft) / (m_nTimeEstimatesCount + 1);
	//m_nTimeEstimatesCount++;
	//UpdateTime ();

	return S_OK;
}

HRESULT CLightscribe::raw_NotifyPrintComplete (HRESULT status)
{
	::PostMessage (m_hwndStatus, SB_SETTEXT, 1, (LPARAM) "Lightscribe Complete");

	m_bPrintingFinished = true;
	return S_OK;
}

HRESULT CLightscribe::raw_QueryCancel (VARIANT_BOOL* pbCancel)
{
	*pbCancel = VARIANT_FALSE;
	return S_OK;
}

HRESULT CLightscribe::raw_ReportLabelTimeEstimate (long lSeconds, VARIANT_BOOL* pbCancel)
{
	//m_lEstimatedRemainingSeconds = lSeconds;
	//m_lCurrRemainingSeconds = lSeconds;
	//m_dwStartTime = GetTickCount ();

	//UpdateTime ();

	*pbCancel = VARIANT_FALSE;
	return S_OK;
}
