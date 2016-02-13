// covertarget_com.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "covertarget_com.h"

#include <stdio.h>
#include <stdlib.h>

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
// Cover host: www.covertarget.com

LPSTR GetHost ()
{
	return "www.covertarget.com";
}

int GetLibVersion ()
{
	return 3;
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
	sprintf (lpszUrl, "http://covertarget.com/ct/1/%c.html", tolower (lpszLetter[0]));
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Referer: http://www.covertarget.com\r\n");
	*lpszData = 0;
}

#define CHECKPARSE(lpsz) if (lpsz == NULL) break

LPSTR _strrchr (LPSTR lpszStartBackwardFromHere, char chToFind)
{
	LPSTR lpszResult = lpszStartBackwardFromHere;
	while (*lpszResult != chToFind)
		lpszResult--;
	return lpszResult;
}

LPSTR find_item (LPCTSTR lpszItem, LPSTR lpszData, LPSTR lpszEndName, LPSTR lpszCurrentTitle, LPSTR lpszName, int& nPtr)
{
	LPSTR lpszEnd = NULL;

	LPSTR lpszTemp = strstr (lpszCurrentTitle, lpszItem);
	if (lpszTemp != NULL)
	{
		// find the end of the <a> tag
		lpszEnd = _strrchr (lpszTemp, '>');
		LPSTR lpszEndA = strstr (lpszEnd, "</a>");

		// find the start of "href"
		LPSTR lpszStartHref = _strrchr (lpszTemp, '<');
		if (lpszStartHref == NULL)
		{
			lpszData[nPtr++] = 1;
			return NULL;
		}

		LPSTR lpszTempName = strstr (lpszStartHref, lpszName);
		if ((lpszTempName == NULL) || (lpszTempName >= lpszEndA))
		{
			lpszData[nPtr++] = 1;
			return NULL;
		}

		if (*(lpszStartHref + 8) == '"')
			lpszEnd--;

		lpszStartHref = strstr (lpszStartHref, "&id=");

		int nLen = lpszEnd - lpszStartHref - 4;

		strncpy (lpszData + nPtr, lpszStartHref + 4, nLen);
		nPtr += nLen;
	}

	lpszData[nPtr++] = 1;
	return lpszEnd;
}

bool find_covers (LPSTR lpszStartTitles, LPSTR lpszLetter, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	*lpszUrl = 0;
	*pnMethod = 1;
	*lpszHeaders = 0;
	*lpszHttpData = 0;
	*pnPages = 0;

	int nPtr = 0;

	if (lpszStartTitles == NULL)
		return false;

	LPSTR lpszCurrentTitle = lpszStartTitles;
	LPSTR lpszEnd;
	int nLen;
	char szName[200];
	*szName = 0;

	for ( ; ; )
	{
		// find next title
		lpszCurrentTitle = strstr (lpszCurrentTitle, "<a href=");
		CHECKPARSE (lpszCurrentTitle);

		// if the old title appers again, continue
		if ((*szName != 0) && (strstr (lpszCurrentTitle, szName) != NULL))
		{
			lpszCurrentTitle += 8;
			continue;
		}

		lpszEnd = strstr (lpszCurrentTitle, ">");

		// get the name
		LPSTR lpszEndName = strstr (lpszEnd, "</a><br>");
		CHECKPARSE (lpszEndName);
		lpszEndName = _strrchr (lpszEndName, '-');
		CHECKPARSE (lpszEndName);

		nLen = lpszEndName - lpszEnd - 2;
		if (nLen < 0)
			break;	// end reached
		strncpy (szName, lpszEnd + 1, nLen);
		szName[nLen] = 0;
		strncpy (lpszData + nPtr, lpszEnd + 1, nLen);
		nPtr += nLen;
		lpszData[nPtr++] = 1;

		// cover
		LPSTR lpszNextItem = find_item (" - Front", lpszData, lpszEndName, lpszCurrentTitle, szName, nPtr);

		// back
		LPSTR lpszTemp = find_item (" - Inside", lpszData, lpszEndName, lpszCurrentTitle, szName, nPtr);
		lpszNextItem = max (lpszNextItem, lpszTemp);

		// inlay
		lpszTemp = find_item (" - Back", lpszData, lpszEndName, lpszCurrentTitle, szName, nPtr);
		lpszNextItem = max (lpszNextItem, lpszTemp);

		// label
		lpszTemp = find_item (" - Cd", lpszData, lpszEndName, lpszCurrentTitle, szName, nPtr);
		if (lpszTemp == NULL)
		{
			nPtr--;
			lpszTemp = find_item (" - Cd1", lpszData, lpszEndName, lpszCurrentTitle, szName, nPtr);
		}
		lpszNextItem = max (lpszNextItem, lpszTemp); 

		// if lpszNextItem is null, no item has been found any more
		if (lpszNextItem == NULL)
			break;
		lpszCurrentTitle = lpszNextItem;
	}

	lpszData[nPtr] = 0;
	return true;
}

bool ListCovers (LPSTR lpszResponse, LPSTR lpszLetter, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	return find_covers (strstr (lpszResponse, "</font><br><br>"),
		lpszLetter, lpszData, lpszUrl, pnMethod, lpszHeaders, lpszHttpData, pnPages);
}

void GetSearchCoversRequest (LPSTR lpszKeywords, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://www.covertarget.com/search.php?search=%s&cat=1", lpszKeywords);
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Referer: http://covertarget.com/\r\n");
	*lpszData = 0;
}

bool SearchCovers (LPSTR lpszResponse, LPSTR lpszKeywords, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	return find_covers (strstr (lpszResponse, "<input type=\"hidden\" name=\"cat\" value=\"1\">"),
		lpszKeywords, lpszData, lpszUrl, pnMethod, lpszHeaders, lpszHttpData, pnPages);
}

char gl_lpszURL[512];

void GetImageRequest (LPSTR lpszName, LPSTR lpszLetter, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://www.covertarget.com/si.php?cat=1&id=%s", lpszName);
	strcpy (gl_lpszURL, lpszUrl);

	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszData = 0;
}

bool GetImageURL (LPSTR lpszResponse, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	*pnMethod = 1;	// GET
	sprintf (lpszHeaders, "Referer: %s\r\n", gl_lpszURL);

	// check the version of CdCoverCreator
	double dVer = atof (lpszData);
	if (dVer < 2.4)
	{
		// send the error image if the version of CCC is not >= 2.4
		strcpy (lpszUrl, "http://www.our-class.net/mmchristen/data/imgs/err_req_ccc24.jpg");
		return true;
	}

	LPSTR lpsz1 = strstr (lpszResponse, "<img src=\"");
	if (lpsz1 == NULL)
	{
		*lpszUrl = 0;
		return false;
	}

	lpsz1 += 10;

	LPSTR lpsz2 = strchr (lpsz1, '\"');
	if (lpsz2 == NULL)
	{
		*lpszUrl = 0;
		return false;
	}

	int n = lpsz2 - lpsz1;
	strncpy (lpszUrl, lpsz1, n);
	lpszUrl[n] = 0;

	return true;
}
