// wcc_coverlandcouk.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "wcc_coverlandcouk.h"

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
// Cover host: www.coverland.co.uk

LPSTR GetHost ()
{
	return "www.coverland.co.uk";
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
	sprintf (lpszUrl, "http://www.coverland.co.uk/search1.php?section=audio&abc=%c", toupper (lpszLetter[0]));
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Referer: http://www.coverland.co.uk/menuatoz.php?sec=audio");
	*lpszData = 0;
}

#define CHECKPARSE(lpsz) if (lpsz == NULL) break

bool ListCovers (LPSTR lpszResponse, LPSTR lpszLetter, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	*lpszUrl = 0;
	*pnMethod = 1;
	*lpszHeaders = 0;
	*lpszHttpData = 0;
	*pnPages = 0;

	int nPtr = 0;

	// find the starting position of the titles
	LPSTR lpszStartTitles = strstr (lpszResponse, "<SELECT name=\"Title\"");
	if (lpszStartTitles == NULL)
		return false;

	// find the starting position of the groups
	LPSTR lpszStartGroups = strstr (lpszStartTitles, "group[i]=new Array()");
	if (lpszStartGroups == NULL)
		return false;

	LPSTR lpszCurrentTitle = lpszStartTitles;
	LPSTR lpszCurrentGroup = lpszStartGroups;
	char szGroup[100];

	LPSTR lpszEnd;
	int nLen;

	bool bFront, bBack, bInside, bCD, bCD1;

	for (int i = 0; ; i++)
	{
		// find next title
		lpszCurrentTitle = strstr (lpszCurrentTitle + 15, "<OPTION VALUE=");
		CHECKPARSE (lpszCurrentTitle);

		lpszEnd = strstr (lpszCurrentTitle, "\">");

		// find the corresponding group
		sprintf (szGroup, "group[%d]", i);
		lpszCurrentGroup = strstr (lpszCurrentGroup, szGroup);
		CHECKPARSE (lpszCurrentGroup);

		// get the name
		LPSTR lpszEndName = strstr (lpszEnd, "</OPTION>");
		CHECKPARSE (lpszEndName);

		nLen = lpszEndName - lpszEnd - 2;
		strncpy (lpszData + nPtr, lpszEnd + 2, nLen);
		nPtr += nLen;
		lpszData[nPtr++] = 1;

		bFront = false;
		bBack = false;
		bInside = false;
		bCD = false;
		bCD1 = false;

		for (int j = 0; ; j++)
		{
			sprintf (szGroup, "group[%d][%d]", i, j);
			LPSTR lpszCurrentSubGroup = strstr (lpszCurrentGroup, szGroup);
			CHECKPARSE (lpszCurrentSubGroup);

			LPSTR lpszStart = strstr (lpszCurrentSubGroup, "(\"");
			CHECKPARSE (lpszStart);
			lpszStart += 2;

			if (!strncmp (lpszStart, "Front", 5))
				bFront = true;
			else if (!strncmp (lpszStart, "Back", 4))
				bBack = true;
			else if (!strncmp (lpszStart, "Inside", 6))
				bInside = true;
			else if (!strncmp (lpszStart, "Cd\"", 3))
				bCD = true;
			else if (!strncmp (lpszStart, "Cd1", 3))
				bCD1 = true;

			lpszCurrentGroup = lpszCurrentSubGroup;
		}

		nLen = lpszEnd - lpszCurrentTitle - 15;

		// cover
		if (bFront)
		{
			strncpy (lpszData + nPtr, lpszCurrentTitle + 15, nLen);
			nPtr += nLen;
			strcpy (lpszData + nPtr, "&Format=%28Front");
			nPtr += 16;
		}
		lpszData[nPtr++] = 1;

		// back
		if (bInside)
		{
			strncpy (lpszData + nPtr, lpszCurrentTitle + 15, nLen);
			nPtr += nLen;
			strcpy (lpszData + nPtr, "&Format=%28Inside");
			nPtr += 17;
		}
		lpszData[nPtr++] = 1;

		// inlay
		if (bBack)
		{
			strncpy (lpszData + nPtr, lpszCurrentTitle + 15, nLen);
			nPtr += nLen;
			strcpy (lpszData + nPtr, "&Format=%28Back");
			nPtr += 15;
		}
		lpszData[nPtr++] = 1;

		// label
		if (bCD)
		{
			strncpy (lpszData + nPtr, lpszCurrentTitle + 15, nLen);
			nPtr += nLen;
			strcpy (lpszData + nPtr, "&Format=%28Cd");
			nPtr += 13;
		}
		else if (bCD1)
		{
			strncpy (lpszData + nPtr, lpszCurrentTitle + 15, nLen);
			nPtr += nLen;
			strcpy (lpszData + nPtr, "&Format=%28Cd1");
			nPtr += 14;
		}
		lpszData[nPtr++] = 1;
	}

	lpszData[nPtr] = 0;
	return true;
}

void GetSearchCoversRequest (LPSTR lpszKeywords, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://www.coverland.co.uk/search_covers.php?Search=%s&Area=covers_audio&Image1.x=15&Image1.y=11", lpszKeywords);
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Referer: http://www.coverland.co.uk/");
	*lpszData = 0;
}

bool SearchCovers (LPSTR lpszResponse, LPSTR lpszKeywords, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	return ListCovers (lpszResponse, lpszKeywords, lpszData, lpszUrl, pnMethod, lpszHeaders, lpszHttpData, pnPages);
}

void GetImageRequest (LPSTR lpszName, LPSTR lpszLetter, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://www.coverland.co.uk/image.php?popup=0&area=covers_audio&Title=%s%%29.jpg&get_cover.x=36&get_cover.y=19", lpszName);
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszData = 0;
}

bool GetImageURL (LPSTR lpszResponse, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszData = 0;

	LPSTR lpsz1 = strstr (lpszResponse, "SRC=\"http://files.coverland.co.uk/");
	if (lpsz1 == NULL)
	{
		*lpszUrl = 0;
		return false;
	}

	lpsz1 += 5;

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
