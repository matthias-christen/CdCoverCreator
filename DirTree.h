// DirTree.h: interface for the CDirTree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRTREE_H__7D26FA53_2342_4A37_994C_67174577D58D__INCLUDED_)
#define AFX_DIRTREE_H__7D26FA53_2342_4A37_994C_67174577D58D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "GraphicsObject.h"
#include <list>
#include <map>

#include "RenderDC.h"
#include "StyleTemplate.h"

#include <Gdiplus.h>

typedef struct tagDirItem
{
	CString strPath, strName;
	int nLevel;
	bool bIsFile;
	int nIcon;
} DirItem;

typedef struct tagSysImage
{
	HICON hIcon;
	Gdiplus::Bitmap* pBitmap;
} SysImage;

enum DrawIconType
{
	IconFile,
	IconFolder
};

class CDirTree //: public CGraphicsObject  
{
public:
	CDirTree ();
	CDirTree (CString strRootDir, int nMaxNesting, bool bDisplayFiles, int nColumnsCount, bool bUseSystemIcons);
	virtual ~CDirTree ();

	virtual void Render (CRenderDC& rdc, CRect r, CStyleTemplate& style, bool bIsPrinting);

	void Serialize (CArchive& ar);

	inline CString GetRootDir () { return m_strRootDir; }
	inline int GetMaxNesting () { return m_nMaxNesting; }
	inline bool GetDisplayFiles () { return m_bDisplayFiles; }
	inline int GetColumnsCount () { return m_nColumnsCount; }
	inline bool UseSystemIcons () { return m_bUseSystemIcons; }

	CDirTree& operator = (CDirTree& dirTree);

	bool operator == (CDirTree& dirTree);
	bool operator != (CDirTree& dirTree) { return !(*this == dirTree); }

	bool equals (CString strRootDir, int nMaxNesting, bool bDisplayFiles, int nColumnsCount, bool bUseSystemIcons);

	static void EmptyImageStore ();	// call this method at app termination. Deletes all the image in the store

private:
	double ComputeFontSize (CDC* pDC, CRect r, LOGFONT* pLogfont);
	void AddSubDirs (CString strDir, int nLevel);

	void DrawIcon (CDC* pDC, DrawIconType type, CPoint pt, int nWidth, int nHeight);

	Gdiplus::Bitmap* GetBitmapFromStore (int nIndex);

	std::list<DirItem*> m_list;

	CString m_strRootDir;
	int m_nMaxNesting;
	bool m_bDisplayFiles;
	int m_nColumnsCount;
	bool m_bUseSystemIcons;
};

#endif // !defined(AFX_DIRTREE_H__7D26FA53_2342_4A37_994C_67174577D58D__INCLUDED_)
