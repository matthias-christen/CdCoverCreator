#pragma once

#include "RTFTitles.h"
#include "DirTree.h"
#include "RenderDC.h"
#include "StyleTemplate.h"

class CDataCDInfo : public CObject, public CRTFTitles
{
	DECLARE_SERIAL (CDataCDInfo)

public:
	CDataCDInfo(void);
	CDataCDInfo (CDataCDInfo& info);

	~CDataCDInfo(void);

	void Render (CRenderDC& rdc, RenderType rt, CStyleTemplate& style);

	void DrawLabelTitle (CRenderDC& rdc, CStyleTemplate& style);
	void CalcLabelTitleRect (CRenderDC& rdc, CStyleTemplate& style);

	void DrawSideBars (CRenderDC& rdc, CStyleTemplate& style);
	void CalcInlaySideBarRect (CRenderDC& rdc, CStyleTemplate& style);

	void Serialize (CArchive& ar);

	CDataCDInfo& operator = (CDataCDInfo& info);
	bool operator == (CDataCDInfo& info);
	bool operator != (CDataCDInfo& info);

	CDirTree* m_pDirTree;
	CString m_strCDTitle;

protected:
	double m_dTitleFontFactor;
	double m_dSideFontFactor;
};
