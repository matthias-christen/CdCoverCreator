// wcc_punkunitedcom.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "wcc_punkunitedcom.h"
#include <stdio.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

//////////////////////////////////////////////////////////
// Cover host: www.coversite.de

LPSTR GetHost ()
{
	return "www.punkunited.com";
}

int GetLibVersion ()
{
	return 1;
}

void Login (LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	// no authorization required
	*lpszUrl = 0;
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszData = 0;
}

void Logout (LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	*lpszUrl = 0;
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszData = 0;
}

void GetListCoversRequest (LPSTR lpszLetter, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://www.punkunited.terriblesin.com/alphalist.php?alpha=%c", toupper (lpszLetter[0]));
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Referer: http://www.punkunited.terriblesin.com/alphabet.php");
	*lpszData = 0;
}

#define CHECKPARSE(lpsz) if (lpsz == NULL) break

#define STRING_BEFORE_ARTIST "<td width=\"20%\" height=\"18\" nowrap><font size=\"1\" face=\"Verdana, Arial, Helvetica, sans-serif\">"
#define STRING_BEFORE_ALBUM "<td height=\"18\" nowrap><font size=\"1\" face=\"Verdana, Arial, Helvetica, sans-serif\">"
#define STRING_BEFORE_URL "<div align=\"center\"><font size=\"1\" face=\"Verdana, Arial, Helvetica, sans-serif\">"

bool ListCovers (LPSTR lpszResponse, LPSTR lpszLetter, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	*lpszUrl = 0;
	*pnMethod = 1;
	*lpszHeaders = 0;
	*lpszHttpData = 0;
	*pnPages = 0;

	LPSTR lpszStart, lpszEnd;
	LPSTR lpszTemp, lpszTemp1;

	int nPtr = 0;
	int n, nStrLen;

	LPSTR lpszItem[5];

	// covers
	for (lpszStart = lpszResponse; ; )
	{
		lpszStart = strstr (lpszStart + 1, "<tr valign=\"middle\">");
		if (lpszStart == NULL)
			break;

		// copy the name
		/////////////////////////////

		// artist
		lpszTemp = strstr (lpszStart, STRING_BEFORE_ARTIST);
		if (lpszTemp == NULL)
			break;
		lpszStart = lpszTemp;

		lpszEnd = strstr (lpszStart, "</font></td>");
		if (lpszEnd == NULL)
			break;
		if (*(lpszEnd - 1) == ' ')	// skip space character
			lpszEnd--;

		nStrLen = strlen (STRING_BEFORE_ARTIST);
		n = lpszEnd - lpszStart - nStrLen;
		strncpy (lpszData + nPtr, lpszStart + nStrLen, n);
		nPtr += n;
		lpszData[nPtr++] = ':';
		lpszData[nPtr++] = ' ';

		// album
		lpszTemp = strstr (lpszStart, STRING_BEFORE_ALBUM);
		if (lpszTemp == NULL)
			break;
		lpszStart = lpszTemp;

		lpszEnd = strstr (lpszStart, "</font></td>");
		if (lpszEnd == NULL)
			break;
		if (*(lpszEnd - 1) == ' ')	// skip space character
			lpszEnd--;

		nStrLen = strlen (STRING_BEFORE_ARTIST);
		n = lpszEnd - lpszStart - nStrLen;
		strncpy (lpszData + nPtr, lpszStart + nStrLen, n);
		nPtr += n;
		lpszData[nPtr++] = 1;

		// get the url of the pictures
		for (int j = 0; j < 5; j++)
		{
			lpszTemp = strstr (lpszStart, STRING_BEFORE_URL);
			if (lpszTemp == NULL)
				break;
			lpszStart = lpszTemp;

			lpszEnd = strstr (lpszStart, "</font></div></td>");
			if (lpszEnd == NULL)
				break;
			
			lpszTemp = strstr (lpszStart, "<a href=");
			if (lpszTemp < lpszEnd)
				lpszItem[j] = lpszTemp + 62;
			else
				lpszItem[j] = NULL;
		}

		if (lpszEnd == NULL)
			break;

		// copy the urls to the buffer

		// front
		if (lpszItem[0] != NULL)
		{
			lpszTemp1 = strchr (lpszItem[0], '\"');
			strncpy (lpszData + nPtr, lpszItem[0], lpszTemp1 - lpszItem[0]);
			nPtr += lpszTemp1 - lpszItem[0];
		}
		lpszData[nPtr++] = 1;

		// back
		if (lpszItem[1] != NULL)
		{
			lpszTemp1 = strchr (lpszItem[1], '\"');
			strncpy (lpszData + nPtr, lpszItem[1], lpszTemp1 - lpszItem[1]);
			nPtr += lpszTemp1 - lpszItem[1];
		}
		lpszData[nPtr++] = 1;

		// inside
		if (lpszItem[4] != NULL)
		{
			lpszTemp1 = strchr (lpszItem[4], '\"');
			strncpy (lpszData + nPtr, lpszItem[4], lpszTemp1 - lpszItem[4]);
			nPtr += lpszTemp1 - lpszItem[4];
		}
		lpszData[nPtr++] = 1;

		// CD1
		if (lpszItem[2] != NULL)
		{
			lpszTemp1 = strchr (lpszItem[2], '\"');
			strncpy (lpszData + nPtr, lpszItem[2], lpszTemp1 - lpszItem[2]);
			nPtr += lpszTemp1 - lpszItem[2];
		}
		lpszData[nPtr++] = 1;
	}

	lpszData[nPtr] = 0;
	return true;
}

void GetSearchCoversRequest (LPSTR lpszKeywords, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://www.darktown.com//search.php?action=search&what=%s&category=audio", lpszKeywords);
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Referer: http://www.darktown.com/index_ie.php");
	*lpszData = 0;
}

bool SearchCovers (LPSTR lpszResponse, LPSTR lpszKeywords, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	return ListCovers (lpszResponse, lpszKeywords, lpszData, lpszUrl, pnMethod, lpszHeaders, lpszHttpData, pnPages);
}

void GetImageRequest (LPSTR lpszName, LPSTR lpszLetter, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	*lpszUrl = 0;
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszData = 0;
}

bool GetImageURL (LPSTR lpszResponse, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszData = 0;

	sprintf (lpszUrl, "http://www.punkunited.terriblesin.com/covers/%s.jpg", lpszResponse);

	return true;
}
