// CdCoverCreator2Doc.h : interface of the CCdCoverCreator2Doc class
//


#pragma once

#include "TracksInfo.h"
#include "BackgroundInfo.h"
#include "StyleTemplate.h"
#include "FloatingGraphicsObject.h"
#include "DataCDInfo.h"

#include "WebCoversDialog.h"

#include "undo.h"

#include <list>

class CCdCoverCreator2Doc : public CDocument, public CUndo
{
protected: // create from serialization only
	CCdCoverCreator2Doc();
	DECLARE_DYNCREATE(CCdCoverCreator2Doc)

// Attributes
public:
	CStyleTemplate* m_pStyle;
	CBackgroundInfo *m_pBackground[NUM_RENDER_TYPES];
	CTracksInfo* m_pTracks;
	CDataCDInfo* m_pDataCDInfo;
	std::list<CFloatingGraphicsObject*> m_listFloatingObjects[NUM_RENDER_TYPES];

// Operations
public:
	bool SetRTFTitle (CString strRTFTitle, RenderType rt);
	CString GetRTFTitle (RenderType rt);
	void ApplyStyleToRTFTitles (CStyleTemplate* pStyle, RenderType rt = (RenderType) -1);

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();

// Implementation
public:
	virtual ~CCdCoverCreator2Doc();

	CString m_strBookletRTFText;
	void SetBkAlphaFilename(CString strFilename);
	bool SetBkImageFilename (CString strFilename);
	UINT m_nObjectSchema;

	void DisplayStyle();
	void UpdateStyle();
	void SetStyle (CString strStyle);
	
	void DisplayMediumType ();
	void SetMediumType (MediumType type);

	inline void CheckPoint (bool bSetModifiedFlag = true)
	{
		CheckPt ();
		if (bSetModifiedFlag)
			SetModifiedFlag ();
	}

	bool Properties (NewDocMode mode);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	bool m_bCreating;
	afx_msg void OnProperties();
	afx_msg void OnBkimgImage();
	afx_msg void OnBkimgScan();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileLightscribe();
};


