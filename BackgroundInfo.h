// BackgroundInfo.h: interface for the CBackgroundInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKGROUNDINFO_H__A874E47D_AE65_4DC2_A92F_53F9F2A45C26__INCLUDED_)
#define AFX_BACKGROUNDINFO_H__A874E47D_AE65_4DC2_A92F_53F9F2A45C26__INCLUDED_

#include "Picture.h"
#include "RenderDC.h"
#include "StyleTemplate.h"
#include "StdAfx.h"	// Added by ClassView
#include "BackgroundAlphaTexture.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum BkImageAlignmentMode
{
	BIAM_TOPLEFT = 0,
	BIAM_TOPCENTER,
	BIAM_TOPRIGHT,
	BIAM_CENTERLEFT,
	BIAM_CENTERCENTER,
	BIAM_CENTERRIGHT,
	BIAM_BOTTOMLEFT,
	BIAM_BOTTOMCENTER,
	BIAM_BOTTOMRIGHT,
	BIAM_STRETCH,
	BIAM_TILE
};

class CBackgroundInfo : public CObject
{
	DECLARE_SERIAL (CBackgroundInfo)

	friend class CStyleDlgBkImage;

public:
	bool SetImage (CString strImgFilename, COLORREF colTransparent = -1);
	CBackgroundAlphaTexture* m_pAlphaTexture;
	CBackgroundInfo (CBackgroundInfo& bkgrnd);
	CBackgroundInfo();
	CBackgroundInfo (RenderType rt, CString strImg, CPicture* pPic, BkImageAlignmentMode biam, COLORREF crTransparent);
	CBackgroundInfo (RenderType rt);
	virtual ~CBackgroundInfo();

	CBackgroundInfo& operator=(CBackgroundInfo& bi);
	bool operator== (CBackgroundInfo& bi);
	bool operator!= (CBackgroundInfo& bi);

	void Render (CRenderDC &rdc, CStyleTemplate &style, bool bIsPrinting);
	virtual void Serialize (CArchive& ar);

	inline CPicture* GetPicture () { return m_pPicture; }
	inline CString GetImageFilename () { return m_strImgFilename; }
	inline BkImageAlignmentMode GetAlignmentMode () { return m_enumMode; }
	inline void SetAlignmentMode (BkImageAlignmentMode mode) { m_enumMode = mode; }
	inline RenderType GetRenderType () { return m_RenderType; }
	inline void SetRenderType (RenderType rt) { m_RenderType = rt; }
	inline COLORREF GetTransparentColor () { return m_crTransparent; }
	inline void SetTransparentColor (COLORREF col) { m_crTransparent = col; }

	inline bool IsImgFilenameChanged () { return m_bImgFilenameChanged; }

protected:
	COLORREF m_crTransparent;
	RenderType m_RenderType;
	bool m_bImgFilenameChanged;
	CPicture* m_pPicture;
	BkImageAlignmentMode m_enumMode;
	CString m_strImgFilename;
};

#endif // !defined(AFX_BACKGROUNDINFO_H__A874E47D_AE65_4DC2_A92F_53F9F2A45C26__INCLUDED_)
