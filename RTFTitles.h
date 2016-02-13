#pragma once
#include "StyleTemplate.h"
#include "RenderDC.h"

class CRTFTitles
{
public:
	CRTFTitles(void);
	~CRTFTitles(void);

	inline CString GetRTFTitle (RenderType rt) { return m_strRTFTitle[rt]; }
	bool SetRTFTitle (CString strRTFTitle, RenderType rt) {
		if (m_strRTFTitle[rt] != strRTFTitle) {
			m_strRTFTitle[rt] = strRTFTitle;
			return true;
		}
		return false;
	}

	void ApplyStyleToRTFTitles(CStyleTemplate *pStyle, RenderType rt = (RenderType) -1);
	void DrawTitle2 (CRenderDC &rdc, RenderType rt, CStyleTemplate &style, CPoint& ptTracksOffset);

protected:
	CString m_strRTFTitle[NUM_RENDER_TYPES];
};
