// wcc_darktown_com.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "wcc_darktown_com.h"

#include <stdio.h>
#include <stdlib.h>

BOOL APIENTRY DllMain (HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
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
// Cover host: www.darktown.to

LPSTR GetHost ()
{
	return "www.darktown.com";
}

int GetLibVersion ()
{
	return 8;
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
	sprintf (lpszUrl, "http://www.darktown.to/list.php?category=audio&pattern=%c", toupper (lpszLetter[0]));
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Referer: http://www.darktown.to/main.php?category=audio");
	*lpszData = 0;
}

#define CHECKPARSE(lpsz) if (lpsz == NULL) break
#define CHECKPARSE1(lpsz) if (lpsz == NULL) continue

bool get_data (LPSTR lpszType, LPSTR lpszStartSearch, LPSTR lpszEndSearch, LPSTR lpszData, int& nPtr)
{
	bool bFoundSomething = false;
	char szSearch[100];
	sprintf (szSearch, "<strong>%s</strong>", lpszType);
	LPSTR lpszItem = strstr (lpszStartSearch, szSearch);
	
	if (lpszItem != NULL && lpszItem < lpszEndSearch)
	{
		LPSTR lpszItemStart = strstr (lpszItem - 60, "uid");
		LPSTR lpszItemEnd = strchr (lpszItemStart, '\'');
	
		int nLen = lpszItemEnd - (lpszItemStart + 4);
		strncpy (lpszData + nPtr, lpszItemStart + 4, nLen);

		nPtr += nLen;
		bFoundSomething = true;
	}

	lpszData[nPtr++] = 1;
	return bFoundSomething;
}

bool ListCovers (LPSTR lpszResponse, LPSTR lpszLetter, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	*pnMethod = 1;
	*lpszHeaders = 0;
	*lpszHttpData = 0;
	*pnPages = 0;

	int nLen;

	// are there any further pages?
	/*
	LPSTR lpszPages = strstr (lpszResponse, "\">Weiter ></a>");
	if (lpszPages != NULL)
	{
		LPSTR lpszStartNextPage = strstr (lpszPages - 60, "<a href=\"?");
		nLen = lpszPages - (lpszStartNextPage + 9);
		strcpy (lpszUrl, "http://www.darktown.to/search.php");	// /list.php
		strncat (lpszUrl, lpszStartNextPage + 9, nLen);
		//lpszUrl[31 + nLen] = 0;
		lpszUrl[33 + nLen] = 0;

		LPSTR lpszLastPage = strstr (lpszStartNextPage - 70, "\">&nbsp;");
		if (lpszLastPage != NULL)
			*pnPages = atoi (lpszLastPage + 8);
	}*/
	// the "Weiter" and page links are buggy
	// look for the URL just passed. Then look for the next one, which leads to the next page
	// If the URL isn't found, we are on the first page; in this case take the URL of the "Weiter" link
	/*
	LPSTR lpszPages = strstr (lpszResponse, lpszUrl + strlen ("http://www.darktown.to"));
	if (lpszPages != NULL)
	{
		// URL found
		LPSTR lpszStartNextPage = strstr (lpszPages + strlen (lpszUrl), "<a href=\"/");
		if (lpszStartNextPage != NULL)
		{
			LPSTR lpszEndNextPage = strstr (lpszStartNextPage, "\">");
			strcpy (lpszUrl, "http://www.darktown.to");
			nLen = lpszEndNextPage - lpszStartNextPage - 9;
			strncat (lpszUrl, lpszStartNextPage + 9, nLen);
			lpszUrl[strlen ("http://www.darktown.to") + nLen] = 0;
		}
		else
			*lpszUrl = 0;	// no further pages

		// get number of pages
		lpszStartNextPage = strstr (lpszResponse, "\">Weiter ></a>");
		if (lpszStartNextPage != NULL)
		{
			lpszStartNextPage = strstr (lpszStartNextPage - 150, "\">&nbsp;");
			if (lpszStartNextPage != NULL)
				*pnPages = atoi (lpszStartNextPage + 8);
		}
	}
	else
	{
		// URL not found
		LPSTR lpszPages = strstr (lpszResponse, "\">Weiter ></a>");
		if (lpszPages != NULL)
		{
			LPSTR lpszStartNextPage = strstr (lpszPages - 60, "<a href=\"?");
			nLen = lpszPages - (lpszStartNextPage + 9);
			strcpy (lpszUrl, "http://www.darktown.to/search.php");	// /list.php
			strncat (lpszUrl, lpszStartNextPage + 9, nLen);
			//lpszUrl[31 + nLen] = 0;
			lpszUrl[33 + nLen] = 0;

			LPSTR lpszLastPage = strstr (lpszStartNextPage - 70, "\">&nbsp;");
			if (lpszLastPage != NULL)
				*pnPages = atoi (lpszLastPage + 8);
		}
		else
			*lpszUrl = 0;
	}
	*/

	// The "Weiter" link seems to work again. Do it the old way...
	LPSTR lpszPages = strstr (lpszResponse, "\">Weiter ></a>");
	if (lpszPages != NULL)
	{
		LPSTR lpszStartNextPage = strstr (lpszPages - 100, "<a href=\"/search.php?");
		if (lpszStartNextPage != NULL)
		{
			nLen = lpszPages - (lpszStartNextPage + 9);
			strcpy (lpszUrl, "http://www.darktown.to");
			strncat (lpszUrl, lpszStartNextPage + 9, nLen);
			lpszUrl[32 + nLen] = 0;

			LPSTR lpszLastPage = strstr (lpszStartNextPage - 70, "\">&nbsp;");
			if (lpszLastPage != NULL)
				*pnPages = atoi (lpszLastPage + 8);
		}
		else
			lpszUrl[0] = 0;
	}
	else
		lpszUrl[0] = 0;

	int nPtr = 0;

	// a typical entry:
	/*
	<table width=100%  align=center border=0 cellspacing=0 cellpadding=0 height=44>
		<tr>
			<td width=50% height=22 align=left bgcolor=#502020>
				<font face=verdana size=2>
					<strong>Babydream "Vol. 11 - Baby Beach" </strong>
					<!-- subcategory -->
				</font>
			</td>
			<td width=30% height=22 align=left bgcolor=#502020>
				<font face=arial size=1></font>
				&nbsp;
			</td>
			<td height=22 width=20% align=right bgcolor=#502020>
				<a href="javascript:openCentered('/preview.php?uid=042288&category=audio','Preview','700','190')">
					<font face=tahoma size=2><strong>Vorschau</strong>
				</a>
			</td>
		</tr>
		<tr>
			<td height=20 colspan=2 align=left>
				<font face=verdana size=2>| 
					<a href="/coverdownload.php?category=audio&uid=042288&type=Front">
						<strong>Front</strong>
					</a> | 
					<a href="/coverdownload.php?category=audio&uid=042288&type=Back">
						<strong>Back</strong>
					</a> | 
					<a href="/coverdownload.php?category=audio&uid=042288&type=CD">
						<strong>CD</strong>						
					</a> |
			</td>
			<td width=120 height=23 align=right>
				<a target="_new" href="http://www.amazon.de/exec/obidos/external-search?tag=darktown0d-21&keyword=Babydream%20Vol.%2011%20-%20Baby%20Beach&index=music">
					<img src="http://img.darktown.to/banner/amazon.gif" width="120" height="23" alt="Kaufen bei Amazon" hspace=0 vspace=0 border=0>
				</a>
			</td>
		</tr>
	</table>
	*/

	LPSTR lpszStart = lpszResponse;

	for ( ; ; )
	{
		lpszStart = strstr (lpszStart, "<table width=100%  align=center border=0 cellspacing=0 cellpadding=0 height=44>");//<tr><td width=50% height=22 align=left bgcolor=");
		CHECKPARSE (lpszStart);

		// find the title
		LPSTR lpszTitleStart = strstr (lpszStart, "<strong>");
		CHECKPARSE1 (lpszTitleStart);

		LPSTR lpszTitleEnd = strstr (lpszTitleStart, "</strong>");
		CHECKPARSE1 (lpszTitleEnd);

		nLen = lpszTitleEnd - lpszTitleStart - 8;
		strncpy (lpszData + nPtr, lpszTitleStart + 8, nLen);
		nPtr += nLen;
		lpszData[nPtr++] = 1;

		// where does this item end?
		LPSTR lpszEndItem = strstr (lpszTitleEnd, "<a target=\"_new\" href=\"http://www.amazon.de");
		if (lpszEndItem == NULL)
			lpszEndItem = strstr (lpszTitleEnd, "</td></tr></table>");

		// find cover ("Front")
		if (!get_data ("Front", lpszTitleEnd, lpszEndItem, lpszData, nPtr))
		{
			nPtr--;
			get_data ("Front1", lpszTitleEnd, lpszEndItem, lpszData, nPtr);
		}

		// find back ("Inlay")
		get_data ("Inlay", lpszTitleEnd, lpszEndItem, lpszData, nPtr);

		// find inlay ("Back")
		get_data ("Back", lpszTitleEnd, lpszEndItem, lpszData, nPtr);

		// find label ("CD")
		get_data ("CD", lpszTitleEnd, lpszEndItem, lpszData, nPtr);

		lpszStart = lpszEndItem;
	}

	lpszData[nPtr] = 0;
	return true;
}

void GetSearchCoversRequest (LPSTR lpszKeywords, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://www.darktown.to/search.php?action=search&what=%s&category=audio&submit=++Suche++", lpszKeywords);
	*pnMethod = 1;	// GET
	strcpy (lpszHeaders, "Referer: http://www.darktown.to/nav_rechts_2.php");
	*lpszData = 0;
}

bool SearchCovers (LPSTR lpszResponse, LPSTR lpszKeywords, LPSTR lpszData, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszHttpData, int* pnPages)
{
	return ListCovers (lpszResponse, lpszKeywords, lpszData, lpszUrl, pnMethod, lpszHeaders, lpszHttpData, pnPages);
}

void GetImageRequest (LPSTR lpszName, LPSTR lpszLetter, LPSTR lpszCookie, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	sprintf (lpszUrl, "http://www.darktown.to/coverdownload.php?category=audio&uid=%s", lpszName);
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;
	*lpszData = 0;
}

bool GetImageURL (LPSTR lpszResponse, LPSTR lpszUrl, int* pnMethod, LPSTR lpszHeaders, LPSTR lpszData)
{
	*pnMethod = 1;	// GET
	*lpszHeaders = 0;

	// check the version of CdCoverCreator
	double dVer = atof (lpszData);
	if (dVer < 2.4)
	{
		// send the error image if the version of CCC is not >= 2.4
		strcpy (lpszUrl, "http://www.our-class.net/mmchristen/data/imgs/err_req_ccc24.jpg");
		return true;
	}

	LPSTR lpsz1 = strstr (lpszResponse, "<a target=\"_new\" href=\"http://img.darktown.to");
	if (lpsz1 == NULL)
	{
		*lpszUrl = 0;
		return false;
	}

	lpsz1 += strlen ("<a target=\"_new\" href=\"");

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