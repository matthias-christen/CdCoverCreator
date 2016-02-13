// wsl_lyricsdotru.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "wsl_lyricsdotru.h"
#include <stdlib.h>
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


LPSTR GetHost ()
{
	return "www.lyricsdot.ru";
}

int GetLibVersion ()
{
	return 1;
}

void GetLyricsRequest (LPSTR lpszTitle, LPSTR lpszArtist, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://lyricsdot.com/search.html?band=%s&album=&song=%s&match=soundex",
		lpszArtist, lpszTitle);
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Accept-Encoding: deflate\n");
	*lpszData = 0;
}

bool GetLyrics (LPSTR lpszResponse,	LPSTR lpszArtist, LPSTR lpszAlbum, LPSTR lpszTitle,
				LPSTR lpszText, LPSTR lpszUrl, bool bCopyUrls, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData)
{
	*lpszArtist = 0;
	*lpszAlbum = 0;
	*lpszTitle = 0;
	*lpszText = 0;
	*lpszUrl = 0;
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszHttpData = 0;

	// no match found
	if (strstr (lpszResponse, "<span class=\"redhead\">Nothing found.</span>") != NULL)
		return false;

	// multiple matches found: return list of URLs (seperated by (char) 1)
	if (strstr (lpszResponse, "<span class=\"head\">&nbsp;&nbsp;&nbsp;&nbsp;Page 1, matches 1..") != NULL)
	{
		if (bCopyUrls)
		{
			LPSTR lpszUrlStart = lpszResponse;
			LPSTR lpszUrlEnd;
			int nPtr = 0, n;
			while ((lpszUrlStart = strstr (lpszUrlStart, "<td><a class=\"link\" href=\"search.html?id=")) != NULL)
			{
				lpszUrlEnd = strstr (lpszUrlStart, "\">");
				if (lpszUrlEnd == NULL)
					break;

				strcpy (lpszUrl + nPtr, "http://lyricsdot.com/");
				nPtr += 24;
				n = lpszUrlEnd - lpszUrlStart - 26;
				strncpy (lpszUrl + nPtr, lpszUrlStart + 26, n);
				nPtr += n;
				lpszUrl[nPtr++] = 1;

				lpszUrlStart += 50;
			}

			lpszUrl[nPtr - 1] = 0;
		}

		return false;	// lyrics have not yet been found
	}

	// song has been found
	LPSTR lpszItem;
	LPSTR lpszEnd;
	LPSTR lpszTemp;
	int n, nPtr = 0;

	// get artist
	lpszTemp = lpszResponse;
	if ((lpszItem = strstr (lpszResponse, "<a class=\"head\" href=\"search.html?band=")) != NULL)
	{
		lpszItem = strstr (lpszItem, "\">");
		if (lpszItem != NULL)
		{
			lpszEnd = strstr (lpszItem, "</a>");
			n = lpszEnd - lpszItem - 2;
			strncpy (lpszArtist, lpszItem + 2, n);
			lpszArtist[n] = 0;
		}
		else
			lpszEnd = lpszTemp;
	}
	else
		lpszEnd = lpszTemp;

	// get song
	lpszTemp = lpszEnd;
	if ((lpszItem = strstr (lpszEnd, "</a>, &quot;")) != NULL)
	{
		lpszEnd = strstr (lpszItem, "&quot;</span>");
		if (lpszEnd != NULL)
		{
			n = lpszEnd - lpszItem - 12;
			strncpy (lpszTitle, lpszItem + 12, n);
			lpszTitle[n] = 0;
		}
		else
			lpszEnd = lpszTemp;
	}
	else
		lpszEnd = lpszTemp;

	// get album
	lpszTemp = lpszEnd;
	if ((lpszItem = strstr (lpszEnd, "<a class=\"link\" href=\"search.html?band=")) != NULL)
	{
		lpszItem = strstr (lpszItem, "\">");
		if (lpszItem != NULL)
		{
			lpszEnd = strstr (lpszItem, "</a>");
			if (lpszEnd != NULL)
			{
				n = lpszEnd - lpszItem - 2;
				strncpy (lpszAlbum, lpszItem + 2, n);
				lpszAlbum[n] = 0;
			}
			else
				lpszEnd = lpszTemp;
		}
		else
			lpszEnd = lpszTemp;
	}
	else
		lpszEnd = lpszTemp;

	// get lyrics
	lpszTemp = lpszEnd;
	LPSTR lpszStart = strstr (lpszEnd, "<span class=\"text\"><br>") + 23;
	lpszEnd = strstr (lpszStart, "</span>");
	if (lpszEnd != NULL)
	{
		LPSTR lpszStr;
		for (n = 0; ; )
		{
			lpszStr = strstr (lpszStart, "<br />");
			if (lpszStr == NULL)
				break;
			
			strncpy (lpszText + n, lpszStart, lpszStr - lpszStart);
			n += lpszStr - lpszStart + 5;
			strcpy (lpszText + n - 5, "\\par ");

			lpszStart = lpszStr + 7;
		}

		// last line
		strncpy (lpszText + n, lpszStart, lpszEnd - lpszStart);
		n += lpszEnd - lpszStart;

		*(lpszText + n) = 0;
	}

	return true;
}
