// HttpReqSocket.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "HttpReqSocket.h"

#include <ras.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHttpReqSocket

CHttpReqSocket::CHttpReqSocket()
{
}

CHttpReqSocket::~CHttpReqSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CHttpReqSocket, CSocket)
	//{{AFX_MSG_MAP(CHttpReqSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CHttpReqSocket member functions

int CHttpReqSocket::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	if (m_pbBlocking != NULL)
	{
		WSASetLastError(WSAEINPROGRESS);
		return  FALSE;
	}

	int nResult, i = 0;

	while ((nResult = CAsyncSocket::Receive(lpBuf, nBufLen, nFlags)) == SOCKET_ERROR)
	{
		if (GetLastError() == WSAEWOULDBLOCK)
		{
			if (!PumpMessages(FD_READ))
				return SOCKET_ERROR;
		}
		else
			return SOCKET_ERROR;

		i++;
		if (i > 5)
			return SOCKET_ERROR;
	}
	return nResult;
}

CString CHttpReqSocket::GetHTTPContent(CString strHost, CString strRequest,
									   bool* pbCancelDownload, int& nError,
									   int nApproxContentLength,									   
									   CProgressCtrl *pProgress, CWnd* pStatus,
									   CString strFilename)
{
	// init sockets
	AfxSocketInit ();
	CHttpReqSocket socket;
	nError = 0;

	// try to create the socket
	if (!socket.Create ())
	{
		nError = WSAGetLastError ();
		return "";
	}

	// try to connect to server
	if (!socket.Connect (strHost, 80))
	{
		nError = socket.GetLastError ();
		socket.Close ();
		return "";
	}

	// wait for response
	socket.Send (strRequest, strRequest.GetLength ());

	CString strRecv;
	char szBuf[4096 + 1];
	int nPos, nContLen = -1;
	int nLen = 0;

	// create and open file, if stream is to be written to a file
	// (i.e. strFilename is not empty)
	CFile file;
	bool bUseFile = !strFilename.IsEmpty ();
	bool bHeaderRead = false;
	if (bUseFile)
		file.Open (strFilename, CFile::modeCreate | CFile::modeWrite);

	// init progress control
	if (pProgress != NULL)
		pProgress->SetRange32 (0, nApproxContentLength);

	MSG msg;

	CString strStatus;

	// read http stream
	for (int nRead = 1; nRead > 0; )
	{
		if (*pbCancelDownload)
			break;

		// read from the socket
		nRead = socket.Receive (szBuf, 4096);

		switch (nRead)
		{
		case 0:
			continue;
		case SOCKET_ERROR:
			nError = socket.GetLastError ();
			break;
		default:
			if (bUseFile)
			{
				// write stream to file
				if (!bHeaderRead)
				{
					// try to get the Content-Length header
					char* pBuf = strstr (szBuf, "Content-Length:");
					if (pBuf)
						nContLen = atoi (pBuf + 15);
					if (pProgress != NULL)
						pProgress->SetRange32 (0, nContLen);

					// cut off the header (should not be saved to the file)
					pBuf = strstr (szBuf, "\r\n\r\n");
					int nHeaderLen = (int) (pBuf - szBuf);
					file.Write (pBuf + 4, nRead - nHeaderLen - 4);
//					file.Write (szBuf, nRead);

					// return the header
					LPSTR pRecvBuf = strRecv.GetBuffer (nHeaderLen);
					strncpy (pRecvBuf, szBuf, nHeaderLen);
					pRecvBuf[nHeaderLen] = '\0';
					strRecv.ReleaseBuffer ();

					bHeaderRead = true;
				}
				else
					file.Write (szBuf, nRead);
			}
			else
			{
				// save stream as string to return
				szBuf[nRead] = '\0';
				strRecv += szBuf;

				// try to get the Content-Length header
				// and set the progress control's range appropriately
				if (!bHeaderRead)
				{
					if ((nPos = strRecv.Find ("Content-Length:")) >= 0)
					{
						nContLen = atoi (strRecv.Mid (nPos + 15));
						if (pProgress != NULL)
							pProgress->SetRange32 (0, nContLen);
					}
					bHeaderRead = true;
				}
			}
		}

		nLen += nRead;

		if (nContLen >= 0)
			if (nLen >= nContLen)
				nRead = 0;

		if (pProgress != NULL)
			pProgress->SetPos (nLen);
		if (pStatus != NULL)
		{
			strStatus.Format (IDS_DOWNLOADSTATUS, (double) nLen / 1024.0, (double) nContLen / 1024.0);
			pStatus->SetWindowText (strStatus);
		}

		while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
			DispatchMessage (&msg);
	}

	socket.Close ();

	if (bUseFile)
		file.Close ();

	return strRecv;
}

bool CHttpReqSocket::IsInternetConnected ()
{
	return true;	// MCH 17.09.04 (what the heck...)

	//////////////////////////////////////////////////////////////

	DWORD dwState;
	InternetGetConnectedState (&dwState, 0);

	if (dwState & INTERNET_CONNECTION_LAN)
		return true;
	else
	{
		bool bConnected = false;
		RASCONN rasConn;
		rasConn.dwSize = sizeof (RASCONN);
		DWORD dwBufSize = sizeof (RASCONN);
		DWORD dwNumConns;
		if (RasEnumConnections (&rasConn, &dwBufSize, &dwNumConns) == 0)
		{
			// RasEnumConnections succeeded
			RASCONNSTATUS rasConnStatus;
			for (UINT i = 0; i < dwNumConns; i++)
			{
				rasConnStatus.dwSize = sizeof (RASCONNSTATUS);
				RasGetConnectStatus (rasConn.hrasconn, &rasConnStatus);
				if (rasConnStatus.rasconnstate == RASCS_Connected)
				{
					bConnected = true;
					break;
				}
			}
		}

		return bConnected;
	}
}

CString CHttpReqSocket::URLEncode(CString str)
{
	CString strEnc;
	CString s;
	char c;

	for (int i = 0; i < str.GetLength (); i++)
	{
		c = str.GetAt (i);
		if (c == ' ')
			strEnc += "+";
		else if (c <= 41 || c == 47 || (c >= 58 && c <= 64) || (c >= 91 && c <= 94) || c == 96 || c >= 123)
		{
			s.Format ("%%%02X", (unsigned char) c);
			strEnc += s;
		}
		else
			strEnc += c;
	}

	return strEnc;
}
