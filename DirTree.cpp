// DirTree.cpp: implementation of the CDirTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "DirTree.h"

#include "MainFrm.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static std::map<int, SysImage*> IMAGE_STORE;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirTree::CDirTree ()
{
}

CDirTree::CDirTree (CString strRootDir, int nMaxNesting, bool bDisplayFiles, int nColumnsCount, bool bUseSystemIcons)
{
	m_strRootDir = strRootDir;
	m_nMaxNesting = max (nMaxNesting, 1);
	m_bDisplayFiles = bDisplayFiles;
	m_nColumnsCount = max (nColumnsCount, 1);
	m_bUseSystemIcons = bUseSystemIcons;

	// build the directory tree
	AddSubDirs (strRootDir, 0);
}

CDirTree::~CDirTree()
{
	// delete the list entries
	for (std::list<DirItem*>::iterator it = m_list.begin (); it != m_list.end (); it++)
		delete *it;
	m_list.clear ();
}

double CDirTree::ComputeFontSize (CDC* pDC, CRect r, LOGFONT* pLogfont)
{
	// compute font size
	CFont font;
	font.CreateFontIndirect (pLogfont);
	CFont* pFontOld = pDC->SelectObject (&font);

	// vertical scaling factor
	double dScalingFactorHeight = (double) (m_nColumnsCount * r.Height ()) / (double) (pDC->GetTextExtent ("X").cy * m_list.size ());

	// horizontal scaling factor
	int nMaxWidth = 0;
	for (std::list<DirItem*>::iterator it = m_list.begin (); it != m_list.end (); it++)
	{
		CSize sz = pDC->GetTextExtent ((*it)->strName);
		// width + indent
		nMaxWidth = max (nMaxWidth, sz.cx + ((*it)->nLevel + 1) * sz.cy);
	}
	double dScalingFactorWidth = (double) r.Width () / (double) (nMaxWidth * m_nColumnsCount);

	pDC->SelectObject (pFontOld);
	font.DeleteObject ();

	if (pLogfont->lfHeight < 0)
		return min (ceil (pLogfont->lfHeight * min (dScalingFactorWidth, dScalingFactorHeight)), -1);
	else
		return max (ceil (pLogfont->lfHeight * min (dScalingFactorWidth, dScalingFactorHeight)), 1);
}

using namespace Gdiplus;

void CDirTree::Render (CRenderDC& rdc, CRect r, CStyleTemplate& style, bool bIsPrinting)
{
//	CPoint ptViewptOrg = rdc.m_pDC->GetViewportOrg ();

	// select font
	LOGFONT* pLf = style.GetTrackTitleFont ();
	LOGFONT lf;
	memcpy (&lf, pLf, sizeof (LOGFONT));
	lf.lfHeight = (LONG) ComputeFontSize (rdc.m_pDC, r, &lf);

	CPen pen (PS_SOLID, 1, style.GetTrackNumColor ());
	CPen* pOldPen = (CPen*) rdc.m_pDC->SelectObject (&pen);

	rdc.m_pDC->SetBkMode (TRANSPARENT);
	rdc.m_pDC->SetTextColor (style.GetTrackTitleColor ());

	// compute line height
	CFont font;
	font.CreateFontIndirect (&lf);
	CFont* pOldFont = rdc.m_pDC->SelectObject (&font);

	int nHeight = rdc.m_pDC->GetTextExtent ("X").cy;
	int nWidth = nHeight;
	int nIndent = nHeight;

	// initialise points
	CPoint pt (0, r.top);
	PointF ptfOrg;

	// column number and row index
	int nCol = 0;
	int i = 0;
	int nItemsPerCol = m_list.size () / m_nColumnsCount + (m_list.size () % m_nColumnsCount == 0 ? 0 : 1);

	int nPrevLevel = 0;
	std::list<CPoint*> stackLines;

	// process the list
	for (std::list<DirItem*>::iterator it = m_list.begin (); it != m_list.end (); it++)
	{
		pt.x = (*it)->nLevel * nIndent + nCol * r.Width () / m_nColumnsCount + r.left;
		
		if (m_bUseSystemIcons)
		{
			Graphics graphics (rdc.m_pDC->GetSafeHdc (), rdc.m_hndPrinter);
			graphics.SetPageUnit (UnitPixel);
			Rect rcDest (pt.x, pt.y, nWidth, nHeight);
			Bitmap* pBmp = GetBitmapFromStore ((*it)->nIcon);
			graphics.DrawImage (pBmp, rcDest, 0, 0, /*pBmp->GetWidth (), pBmp->GetHeight ()*/ 32, 32, UnitPixel);
		}
		else
			DrawIcon (rdc.m_pDC, (*it)->bIsFile ? IconFile : IconFolder, pt, nWidth, nHeight);

//		rdc.m_pDC->SetViewportOrg (ptViewptOrg);
		rdc.m_pDC->TextOut (pt.x + nWidth, pt.y, (*it)->strName);

		// lines
		if (nPrevLevel < (*it)->nLevel)
		{
			// level increases -- push new line on the stack
			stackLines.push_front (new CPoint (nPrevLevel * nIndent + nCol * r.Width () / m_nColumnsCount + nWidth / 2 + r.left, pt.y));
		}
		else if (nPrevLevel > (*it)->nLevel)
		{
			if (pt.y > r.top)
				for (int j = (*it)->nLevel; j < nPrevLevel; j++)
				{
					// level decreases -- pop line from stack
					CPoint* pPt = stackLines.front ();

					// draw the line
					//graphics.DrawLine (&pen, pPt->x, pPt->y, pPt->x, pt.y - nHeight / 5);
					rdc.m_pDC->MoveTo (*pPt);
					rdc.m_pDC->LineTo (pPt->x, pt.y - nHeight / 5);

					// clean up
					delete pPt;
					stackLines.pop_front ();
				}
		}

		pt.y += nHeight;
		i++;

		// last item of the column has been reached.
		// Prepare stuff for the new column
		if (i == nItemsPerCol)
		{
			// draw the lines
			for (std::list<CPoint*>::iterator it = stackLines.begin (); it != stackLines.end (); it++)
			{
				CPoint* pPt = *it;
				//graphics.DrawLine (&pen, pPt->x, pPt->y, pPt->x, pt.y - 2);
				rdc.m_pDC->MoveTo (*pPt);
				rdc.m_pDC->LineTo (pPt->x, pt.y - 2);

				// offset the x coordinate
				pPt->x += r.Width () / m_nColumnsCount;
				// set y to zero so that the line is draw from the top in the next column
				pPt->y = r.top;
			}

			i = 0;
			nCol++;
			pt.y = r.top;
		}

		// remember the level
		nPrevLevel = (*it)->nLevel;
	}

	// empty the lines stack
	for (std::list<CPoint*>::iterator itStack = stackLines.begin (); itStack != stackLines.end (); itStack++)
	{
		if ((*itStack)->y > r.top)
		{
			//graphics.DrawLine (&pen, (*itStack)->x, (*itStack)->y, (*itStack)->x, pt.y - 2);
			rdc.m_pDC->MoveTo (**itStack);
			rdc.m_pDC->LineTo ((*itStack)->x, pt.y - 2);
		}

		delete *itStack;
	}

	rdc.m_pDC->SelectObject (pOldFont);
	font.DeleteObject ();

	rdc.m_pDC->SelectObject (pOldPen);
	pen.DeleteObject ();
}

void CDirTree::AddSubDirs (CString strDir, int nLevel)
{
	WIN32_FIND_DATA wfd;
	SHFILEINFO sfi;

	if (strDir[strDir.GetLength () - 1] != '\\')
		strDir += "\\";

	HANDLE hFFile = ::FindFirstFile (strDir + "*.*", &wfd);
	if (hFFile != INVALID_HANDLE_VALUE)
		do
		{
			// skip the '.' and '..' folders
			if ((strcmp (wfd.cFileName, ".") == 0) || (strcmp (wfd.cFileName, "..") == 0))
				continue;

			// add the entry to the list
			bool bIsFile = wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? false : true;
			if (m_bDisplayFiles || (!m_bDisplayFiles && !bIsFile))
			{
				// create a new DirItem
				DirItem* pItem = new DirItem;
				pItem->strPath = strDir + wfd.cFileName;
				pItem->strName = wfd.cFileName;
				pItem->nLevel = nLevel;
				pItem->bIsFile = bIsFile;

				// get the icon index (in the system image list)
				if (m_bUseSystemIcons)
				{
					if (pItem->bIsFile)
						SHGetFileInfo (pItem->strPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof (SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
					else
					{
						char szPath[MAX_PATH];
						GetSystemDirectory (szPath, MAX_PATH);
						SHGetFileInfo (szPath, 0, &sfi, sizeof (SHFILEINFO), SHGFI_ICON);
					}

					pItem->nIcon = sfi.iIcon;
				}
				else
					pItem->nIcon = -1;

				m_list.push_back (pItem);

				// recursive call for sub dirs
				if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (nLevel < m_nMaxNesting - 1))
					AddSubDirs (strDir + pItem->strName + "\\", nLevel + 1);
			}
		} while (::FindNextFile (hFFile, &wfd));
	::FindClose (hFFile);
}

void CDirTree::Serialize (CArchive& ar)
{
	if (ar.IsLoading ())
	{
		ar >> m_strRootDir >> m_bDisplayFiles >> m_nMaxNesting >> m_nColumnsCount >> m_bUseSystemIcons;

		SHFILEINFO sfi;

		int n;
		ar >> n;
		for (int i = 0; i < n; i++)
		{
			DirItem* pItem = new DirItem;
			ar >> pItem->strName >> pItem->strPath >> pItem->nLevel >> pItem->bIsFile;

			// get the icon index (in the system image list)
			if (m_bUseSystemIcons)
			{
				if (pItem->bIsFile)
					SHGetFileInfo (pItem->strPath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof (SHFILEINFO), SHGFI_ICON | SHGFI_USEFILEATTRIBUTES);
				else
				{
					char szPath[MAX_PATH];
					GetSystemDirectory (szPath, MAX_PATH);
					SHGetFileInfo (szPath, 0, &sfi, sizeof (SHFILEINFO), SHGFI_ICON);
				}

				pItem->nIcon = sfi.iIcon;
			}

			m_list.push_back (pItem);
		}
	}
	else
	{
		ar << m_strRootDir << m_bDisplayFiles << m_nMaxNesting << m_nColumnsCount << m_bUseSystemIcons;

		int n = m_list.size ();
		ar << n;

		for (std::list<DirItem*>::iterator it = m_list.begin (); it != m_list.end (); it++)
			ar << (*it)->strName << (*it)->strPath << (*it)->nLevel << (*it)->bIsFile;
	}
}

CDirTree& CDirTree::operator = (CDirTree& dirTree)
{
	// copy member vars
	m_strRootDir = dirTree.GetRootDir ();
	m_nMaxNesting = dirTree.GetMaxNesting ();
	m_bDisplayFiles = dirTree.GetDisplayFiles ();
	m_nColumnsCount = dirTree.GetColumnsCount ();
	m_bUseSystemIcons = dirTree.UseSystemIcons ();

	// copy list
	for (std::list<DirItem*>::iterator it = m_list.begin (); it != m_list.end (); it++)
		delete *it;
	m_list.clear ();

	for (std::list<DirItem*>::iterator it = dirTree.m_list.begin (); it != dirTree.m_list.end (); it++)
	{
		// create a new DirItem
		DirItem* pItem = new DirItem;
		pItem->strPath = (*it)->strPath;
		pItem->strName = (*it)->strName;
		pItem->nLevel = (*it)->nLevel;
		pItem->bIsFile = (*it)->bIsFile;
		pItem->nIcon = (*it)->nIcon;

		m_list.push_back (pItem);
	}

	return *this;
}

bool CDirTree::operator == (CDirTree& dirTree)
{
	return equals (dirTree.GetRootDir (), dirTree.GetMaxNesting (), dirTree.GetDisplayFiles (), dirTree.GetColumnsCount (), dirTree.UseSystemIcons ());
}

bool CDirTree::equals (CString strRootDir, int nMaxNesting, bool bDisplayFiles, int nColumnsCount, bool bUseSystemIcons)
{
	if (m_strRootDir != strRootDir)
		return false;
	if (m_nMaxNesting != nMaxNesting)
		return false;
	if (m_bDisplayFiles != bDisplayFiles)
		return false;
	if (m_nColumnsCount != nColumnsCount)
		return false;
	if (m_bUseSystemIcons != bUseSystemIcons)
		return false;

	return true;
}

CPoint pnt (int x, int y, int nWidth, int nHeight, CPoint pt)
{
	return CPoint (x * nWidth / 100 + pt.x, y * nHeight / 100 + pt.y);
}

void CDirTree::DrawIcon (CDC* pDC, DrawIconType type, CPoint pt, int nWidth, int nHeight)
{
	CPoint* points;
	int nCount;

	switch (type)
	{
	case IconFile:
		nCount = 7;
		points = new CPoint[7];
		points[0] = pnt (80, 5, nWidth, nHeight, pt);
		points[1] = pnt (80, 95, nWidth, nHeight, pt);
		points[2] = pnt (20, 95, nWidth, nHeight, pt);
		points[3] = pnt (20, 5, nWidth, nHeight, pt);
		points[4] = pnt (80, 5, nWidth, nHeight, pt);
		points[5] = pnt (80, 25, nWidth, nHeight, pt);
		points[6] = pnt (60, 5, nWidth, nHeight, pt);
		break;

	case IconFolder:
		nCount = 9;
		points = new CPoint[9];
		points[0] = pnt (0, 20, nWidth, nHeight, pt);
		points[1] = pnt (0, 95, nWidth, nHeight, pt);
		points[2] = pnt (100, 95, nWidth, nHeight, pt);
		points[3] = pnt (100, 30, nWidth, nHeight, pt);
		points[4] = pnt (90, 20, nWidth, nHeight, pt);
		points[5] = pnt (40, 20, nWidth, nHeight, pt);
		points[6] = pnt (30, 5, nWidth, nHeight, pt);
		points[7] = pnt (10, 5, nWidth, nHeight, pt);
		points[8] = pnt (0, 20, nWidth, nHeight, pt);
		break;
	}

	pDC->Polyline (points, nCount);
}

Gdiplus::Bitmap* CDirTree::GetBitmapFromStore (int nIndex)
{
	std::map<int, SysImage*>::iterator it = IMAGE_STORE.find (nIndex);
	if (it == IMAGE_STORE.end ())
	{
		// not found in the store. add it.
		SysImage* pSi = new SysImage;
		pSi->hIcon = ((CMainFrame*) AfxGetMainWnd ())->m_ilSystemImageList.ExtractIcon (nIndex);
		pSi->pBitmap = Bitmap::FromHICON (pSi->hIcon);

		IMAGE_STORE.insert (std::map<int, SysImage*>::value_type (nIndex, pSi));

		return pSi->pBitmap;
	}
	else
		return it->second->pBitmap;
}

void CDirTree::EmptyImageStore ()
{
	for (std::map<int, SysImage*>::iterator it = IMAGE_STORE.begin (); it != IMAGE_STORE.end (); it++)
	{
		delete it->second->pBitmap;			// NOTE: GdiShutdown must not have yet been called before this
		::DestroyIcon (it->second->hIcon);
		delete it->second;
	}

	IMAGE_STORE.clear ();
}