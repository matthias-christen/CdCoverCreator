// StyleTemplate.cpp: implementation of the CStyleTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "CdCoverCreator2Doc.h"

#include "StyleTemplate.h"
#include "StyleTemplates.h"

#include "FormattedTextDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL (CStyleTemplate, CObject, VERSIONABLE_SCHEMA | 1)

CStyleTemplate::CStyleTemplate()
{
	Init ();
}

CStyleTemplate::CStyleTemplate(CStyleTemplate& style)
{
	*this = style;
}

CStyleTemplate::CStyleTemplate(CString strName)
{
	Init ();
	((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ().LoadTemplate (strName, this);
}

CStyleTemplate::~CStyleTemplate()
{
}

// serialization
void CStyleTemplate::Serialize(CArchive &ar)
{
	if (ar.IsStoring ())
	{
		ar << m_strTemplateName;

		ar.Write (&m_lfTrackNum, sizeof (LOGFONT));
		ar.Write (&m_lfTrackLength, sizeof (LOGFONT));
		ar.Write (&m_lfTrackTitle, sizeof (LOGFONT));
		ar.Write (&m_lfTrackRemarks, sizeof (LOGFONT));
		ar.Write (&m_lfTrackGroupingTitle, sizeof (LOGFONT));
		ar.Write (&m_lfTrackGroupingRemarks, sizeof (LOGFONT));
		ar.Write (&m_lfTitle[Inlay], sizeof (LOGFONT));
		ar.Write (&m_lfInlaySide, sizeof (LOGFONT));
		//## begin VERSION 2
		ar.Write (&m_lfTitle[Cover], sizeof (LOGFONT));
		ar.Write (&m_lfTitle[Label], sizeof (LOGFONT));
		//## end

		//## begin VERSION 4
		ar.Write (&m_lfBookletPageNumFont, sizeof (LOGFONT));
		//## end

		ar << m_crTrackNum << m_crTrackLength << m_crTrackTitle << m_crTrackRemarks;
		ar << m_crTrackGroupingTitle << m_crTrackGroupingRemarks;
		ar << m_crTitle[Inlay] << m_crInlaySide;
		//## begin VERSION 2

		/*
		ar << m_crBkStart[Inlay] << m_crBkEnd[Inlay];
		ar << m_crTitle[Cover] << m_crBkStart[Cover] << m_crBkEnd[Cover];
		ar << m_crBkStart[Back] << m_crBkEnd[Back];
		ar << m_crTitle[Label] << m_crBkStart[Label] << m_crBkEnd[Label];
		ar << m_crBookletPageNumColor;	//## VERSION 4

		ar << m_nBkAngle[Cover] << m_nBkAngle[Back] << m_nBkAngle[Label] << m_nBkAngle[Inlay];
		*/

		//## begin VERSION 5
		for (int k = 0; k < NUM_RENDER_TYPES; k++)
		{
			ar << m_crTitle[k];
			ar << m_lgdBackground[k].dAngle;
			ar << m_lgdBackground[k].nNumColors;
			ar << (int) m_lgdBackground[k].type;	//## VERSION 8
			for (int l = 0; l < m_lgdBackground[k].nNumColors; l++)
				ar << m_lgdBackground[k].colors[l] << m_lgdBackground[k].positions[l];
		}
		ar << m_crBookletPageNumColor;
		//## end VERSION 5

		// COVER
		ar << m_nTitleAlign[Cover] << m_dTitleMargin[Cover];
		ar << m_dTitleTop[Cover] << m_dTitleHeight[Cover];
		ar << (BOOL) (m_bDisplayTrackgroups[Cover] ? 1 : 0);
		ar << m_nTrackgroupHAlign[Cover] << m_nTrackgroupVAlign[Cover] << m_dTrackgroupHMargin[Cover] << m_dTrackgroupVMargin[Cover];
		ar << (BOOL) (m_bDisplayTitles[Cover] ? 1 : 0);	//## VERSION 3

		// BACK //## VERSION 4
		ar << m_nTitleAlign[Back] << m_dTitleMargin[Back];
		ar << m_dTitleTop[Back] << m_dTitleHeight[Back];
		ar << (BOOL) (m_bDisplayTrackgroups[Back] ? 1 : 0);
		ar << m_nTrackgroupHAlign[Back] << m_nTrackgroupVAlign[Back] << m_dTrackgroupHMargin[Back] << m_dTrackgroupVMargin[Back];
		ar << (BOOL) (m_bDisplayTitles[Back] ? 1 : 0);
		// ## end

		// LABEL
		ar << m_nTitleAlign[Label] << m_dTitleMargin[Label];
		ar << m_nLabelTitlePosition << (BOOL) (m_bLabelTitleRound ? 1 : 0);
		ar << (BOOL) (m_bDisplayTrackgroups[Label] ? 1 : 0);
		ar << (BOOL) (m_bLabelDisplayTracks ? 1 : 0);
		ar << m_nLabelTrackVAlign << m_dLabelTrackVMargin;
		ar << m_nLabelTrackPosition << (BOOL) (m_bLabelTrackRound ? 1 : 0);
		ar << (BOOL) (m_bDisplayTitles[Label] ? 1 : 0);	//## VERSION 3

		//## end

		//## begin VERSION 4
		ar << m_dBookletMarginLeft << m_dBookletMarginTop << m_dBookletMarginRight << m_dBookletMarginBottom;
		ar << (BOOL) (m_bBookletShowPageNums ? 1 : 0) << m_nBookletPageNumAlign;
		//## end

		// INLAY
		// title
		ar << m_nTitleAlign[Inlay] << m_dTitleMargin[Inlay] << m_dTitleTop[Inlay] << m_dTitleHeight[Inlay];
		ar << (BOOL) (m_bDisplayTitles[Inlay] ? 1 : 0);	//## VERSION 3
		// sides
		ar << m_nInlaySideVAlign << m_dInlaySideVMargin;
		ar << (BOOL) (m_bInlayDisplaySidebarTitles ? 1 : 0);		//## VERSION 3
		ar << (BOOL) (m_bInlaySideTextDirection ? 1 : 0);			//## VERSION 6
		ar << (BOOL) (m_bInlaySideAntiparallel ? 1 : 0);			//## VERSION 9

		// tracks
		ar << (BOOL) (m_bTrackFloating ? 1 : 0) << (BOOL) (m_bTrackTotalTime ? 1 : 0);

		for (int i = 0; i < 4; i++)
			ar << m_htaTrack[i].nPosition << (BOOL) (m_htaTrack[i].bDisplay ? 1 : 0) <<
				m_htaTrack[i].nHAlign << m_htaTrack[i].strFormatting;

		ar << m_dTrackMarginLeft << m_dTrackMarginRight << m_dTrackInterItemSpacing;
		ar << m_dTrackGrpSpacingAbove << m_dTrackGrpSpacingBelow;
		ar << (BOOL) (m_bTrackGrpNewLine ? 1 : 0);
	}
	else
	{
		Init ();

		ar >> m_strTemplateName;

		ar.Read (&m_lfTrackNum, sizeof (LOGFONT));
		ar.Read (&m_lfTrackLength, sizeof (LOGFONT));
		ar.Read (&m_lfTrackTitle, sizeof (LOGFONT));
		ar.Read (&m_lfTrackRemarks, sizeof (LOGFONT));
		ar.Read (&m_lfTrackGroupingTitle, sizeof (LOGFONT));
		ar.Read (&m_lfTrackGroupingRemarks, sizeof (LOGFONT));
		ar.Read (&m_lfTitle[Inlay], sizeof (LOGFONT));
		ar.Read (&m_lfInlaySide, sizeof (LOGFONT));
		// ## VERSION 2
		if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 2)
		{
			ar.Read (&m_lfTitle[Cover], sizeof (LOGFONT));
			memcpy (&m_lfTitle[Back], &m_lfTitle[Cover], sizeof (LOGFONT));
			ar.Read (&m_lfTitle[Label], sizeof (LOGFONT));
		}
		else
		{
			memcpy (&m_lfTitle[Cover], &m_lfTitle[Inlay], sizeof (LOGFONT));
			memcpy (&m_lfTitle[Back], &m_lfTitle[Inlay], sizeof (LOGFONT));
			memcpy (&m_lfTitle[Label], &m_lfTitle[Inlay], sizeof (LOGFONT));
		}

		// ## VERSION 4
		if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 4)
			ar.Read (&m_lfBookletPageNumFont, sizeof (LOGFONT));
		else
		{
			ZeroMemory (&m_lfBookletPageNumFont, sizeof (LOGFONT));
			m_lfBookletPageNumFont.lfHeight = -8;
			strcpy (m_lfBookletPageNumFont.lfFaceName, "Tahoma");
		}

		ar >> m_crTrackNum >> m_crTrackLength >> m_crTrackTitle >> m_crTrackRemarks;
		ar >> m_crTrackGroupingTitle >> m_crTrackGroupingRemarks;
		ar >> m_crTitle[Inlay] >> m_crInlaySide;
		if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 2)
		{
			if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 5)
			{
				for (int k = 0; k < NUM_RENDER_TYPES; k++)
				{
					ar >> m_crTitle[k];
					ar >> m_lgdBackground[k].dAngle;
					ar >> m_lgdBackground[k].nNumColors;

					//## VERSION 8
					if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 8)
					{
						int n;
						ar >> n;
						m_lgdBackground[k].type = (GradientType) n;
					}
					else
					{
						m_lgdBackground[k].type = Linear;
					}
					//## end

					for (int l = 0; l < m_lgdBackground[k].nNumColors; l++)
						ar >> m_lgdBackground[k].colors[l] >> m_lgdBackground[k].positions[l];
				}
				ar >> m_crBookletPageNumColor;
			}
			else
			{
				for (int k = 0; k < NUM_RENDER_TYPES; k++)
				{
					m_lgdBackground[k].nNumColors = 2;
					m_lgdBackground[k].positions[0] = 0.0;
					m_lgdBackground[k].positions[1] = 1.0;
				}

				ar >> m_lgdBackground[Inlay].colors[0] >> m_lgdBackground[Inlay].colors[1];
				ar >> m_crTitle[Cover] >> m_lgdBackground[Cover].colors[0] >> m_lgdBackground[Cover].colors[1];
				m_crTitle[Back] = m_crTitle[Cover];
				ar >> m_lgdBackground[Back].colors[0] >> m_lgdBackground[Back].colors[1];
				ar >> m_crTitle[Label] >> m_lgdBackground[Label].colors[0] >> m_lgdBackground[Label].colors[1];

				//## begin VERSION 4
				if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 4)
					ar >> m_crBookletPageNumColor;
				else
					m_crBookletPageNumColor = RGB (0x00, 0x00, 0x00);
				//## end VERSION 4

				int nAngle;
				for (int k = 0; k < 4; k++)
				{
                    ar >> nAngle;
					m_lgdBackground[k].dAngle = deg2rad (nAngle);
				}
			}

			// COVER
			ar >> m_nTitleAlign[Cover] >> m_dTitleMargin[Cover];
			ar >> m_dTitleTop[Cover] >> m_dTitleHeight[Cover];
			BOOL b;
			ar >> b;
			m_bDisplayTrackgroups[Cover] = b == TRUE;
			ar >> m_nTrackgroupHAlign[Cover] >> m_nTrackgroupVAlign[Cover] >> m_dTrackgroupHMargin[Cover] >> m_dTrackgroupVMargin[Cover];

			//## begin VERSION 3
			if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 3)
			{
				ar >> b;
				m_bDisplayTitles[Cover] = b == TRUE;
			}
			else
			{
				m_bDisplayTitles[Cover] = true;
			}
			//## end

			//## begin VERSION 4
			if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 4)
			{
				// BACK
				ar >> m_nTitleAlign[Back] >> m_dTitleMargin[Back];
				ar >> m_dTitleTop[Back] >> m_dTitleHeight[Back];
				ar >> b;
				m_bDisplayTrackgroups[Back] = b == TRUE;
				ar >> m_nTrackgroupHAlign[Back] >> m_nTrackgroupVAlign[Back] >> m_dTrackgroupHMargin[Back] >> m_dTrackgroupVMargin[Back];
				ar >> b;
				m_bDisplayTitles[Back] = b == TRUE;
			}
			else
			{
				m_nTitleAlign[Back] = m_nTitleAlign[Cover];
				m_dTitleMargin[Back] = m_dTitleMargin[Cover];
				m_dTitleTop[Back] = m_dTitleTop[Cover];
				m_dTitleHeight[Back] = m_dTitleHeight[Cover];
				m_bDisplayTrackgroups[Back] = m_bDisplayTrackgroups[Cover];
				m_nTrackgroupHAlign[Back] = m_nTrackgroupHAlign[Cover];
				m_nTrackgroupVAlign[Back] = m_nTrackgroupVAlign[Cover];
				m_dTrackgroupHMargin[Back] = m_dTrackgroupHMargin[Cover];
				m_dTrackgroupVMargin[Back] = m_dTrackgroupVMargin[Cover];
				m_bDisplayTitles[Back] = m_bDisplayTitles[Cover];
			}
			// ## end

			// LABEL
			ar >> m_nTitleAlign[Label] >> m_dTitleMargin[Label];
			ar >> m_nLabelTitlePosition >> b;
			m_bLabelTitleRound = b == TRUE;
			ar >> b;
			m_bDisplayTrackgroups[Label] = b == TRUE;
			ar >> b;
			m_bLabelDisplayTracks = b == TRUE;
			ar >> m_nLabelTrackVAlign >> m_dLabelTrackVMargin;
			ar >> m_nLabelTrackPosition >> b;
			m_bLabelTrackRound = b == TRUE;

			//## begin VERSION 3
			if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 3)
			{
				ar >> b;
				m_bDisplayTitles[Label] = b == TRUE;
			}
			else
			{
				m_bDisplayTitles[Label] = true;
			}
			//## end

			// BOOKLET	// VERSION 4
			if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 4)
			{
				ar >> m_dBookletMarginLeft >> m_dBookletMarginTop >> m_dBookletMarginRight >> m_dBookletMarginBottom;
				ar >> b >> m_nBookletPageNumAlign;
				m_bBookletShowPageNums = b == TRUE;
			}
			else
			{
				m_dBookletMarginLeft = m_dBookletMarginTop = m_dBookletMarginRight = m_dBookletMarginBottom = 1.0;
				m_bBookletShowPageNums = true;
				m_nBookletPageNumAlign = 0;
			}
		}
		else
		{
			m_crTitle[Cover] = m_crTitle[Label] = m_crTitle[Inlay];

//			m_crBkStart[Cover] = m_crBkStart[Back] = m_crBkStart[Label] = m_crBkStart[Inlay] = RGB (0xFF, 0xFF, 0xFF);
//			m_crBkEnd[Cover] = m_crBkEnd[Back] = m_crBkEnd[Label] = m_crBkEnd[Inlay] = RGB (0xFF, 0xFF, 0xFF);
			for (int k = 0; k < NUM_RENDER_TYPES; k++)
			{
				m_lgdBackground[k].nNumColors = 2;
				m_lgdBackground[k].dAngle = 0.0;
				m_lgdBackground[k].colors[0] = m_lgdBackground[k].colors[1] = RGB (0xff, 0xff, 0xff);
				m_lgdBackground[k].positions[0] = 0.0;
				m_lgdBackground[k].positions[1] = 1.0;
			}

			// default COVER and LABEL values
			// COVER
			for (int j = Cover; j <= Label; j++)
			{
				m_nTitleAlign[j] = 2;
				m_dTitleMargin[j] = 1;
				m_dTitleTop[j] = 1.5;
				m_dTitleHeight[j] = 1.2;
				m_bDisplayTrackgroups[j] = true;
				m_nTrackgroupHAlign[j] = 2;
				m_nTrackgroupVAlign[j] = 0;
				m_dTrackgroupHMargin[j] = 0;
				m_dTrackgroupVMargin[j] = 1.5;
				m_bDisplayTitles[j] = true;	// ver 3
			}

			// LABEL
			m_nLabelTitlePosition = 0;
			m_bLabelTitleRound = true;
			m_bLabelDisplayTracks = true;
			m_bDisplayTrackgroups[Label] = true;
//			m_nLabelTrackVAlign = 2;
//			m_dLabelTitleVMargin = 0;
			m_nLabelTrackPosition = 1;
			m_bLabelTrackRound = true;
			m_bDisplayTitles[Label] = true;	// ver 3

			// BOOKLET	// ver 4
			m_crBookletPageNumColor = RGB (0x00, 0x00, 0x00);
			m_dBookletMarginLeft = m_dBookletMarginTop = m_dBookletMarginRight = m_dBookletMarginBottom = 1.0;
			m_bBookletShowPageNums = true;
			m_nBookletPageNumAlign = 0;
		}

		// INLAY

		// title
		////////////////////
		ar >> m_nTitleAlign[Inlay] >> m_dTitleMargin[Inlay] >> m_dTitleTop[Inlay] >> m_dTitleHeight[Inlay];
		//## begin VERSION 3
		BOOL b;
		if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 3)
		{
			ar >> b;
			m_bDisplayTitles[Inlay] = b == TRUE;
		}
		else
			m_bDisplayTitles[Inlay] = true;
		//## end

		// sides
		////////////////////
		ar >> m_nInlaySideVAlign >> m_dInlaySideVMargin;
		//## begin VERSION 3
		if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 3)
		{
			ar >> b;
			m_bInlayDisplaySidebarTitles = b == TRUE;
		}
		else
		{
			m_bInlayDisplaySidebarTitles = true;
		}
		//## end

		//## begin VERSION 6
		if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 6)
		{
			ar >> b;
			m_bInlaySideTextDirection = b == TRUE;
		}
		else
		{
			m_bInlaySideTextDirection = true;
		}
		//## end

		//## begin VERSION 9
		if ((ar.m_pDocument == NULL) || ((CCdCoverCreator2Doc*) ar.m_pDocument)->m_nObjectSchema >= 9)
		{
			ar >> b;
			m_bInlaySideAntiparallel = b == TRUE;
		}
		else
		{
			m_bInlaySideAntiparallel = false;
		}
		//## end		

		// tracks
		int n;
		ar >> n;
		m_bTrackFloating = n ? 1 : 0;
		ar >> n;
		m_bTrackTotalTime = n ? 1 : 0;

		for (int i = 0; i < 4; i++)
		{
			ar >> m_htaTrack[i].nPosition >> n >> m_htaTrack[i].nHAlign >> m_htaTrack[i].strFormatting;
			m_htaTrack[i].bDisplay = n ? 1 : 0;
		}

		ar >> m_dTrackMarginLeft >> m_dTrackMarginRight >> m_dTrackInterItemSpacing;
		ar >> m_dTrackGrpSpacingAbove >> m_dTrackGrpSpacingBelow;
		ar >> n;
		m_bTrackGrpNewLine = n ? 1 : 0;

		if (AfxGetMainWnd () != NULL)
		{
			CStyleTemplates& tmpl = ((CCdCoverCreator2App*) AfxGetApp ())->GetTemplates ();
			if (tmpl.Exists (m_strTemplateName))
			{
				/*
				// template already exists
				CStyleTemplate t;
				tmpl.LoadTemplate (m_strTemplateName, &t);
				if (t != *this)
				{
					// existing template is different from the loaded one.
					// ask user whether to create new template under another name,
					// overwrite existing template or not to undertake any action
					switch (AfxMessageBox (IDS_TEMPLATE_ALREADYEXISTS, MB_ICONQUESTION | MB_YESNOCANCEL))
					{
					case IDYES:	// overwrite existing
						tmpl.StoreTemplate (*this);
						break;
					case IDNO:	// create new template
						{
							CString s;
							// find first free template name and store template
							for (int i = 1; ; i++)
							{
								s.Format ("%d", i);
								s = m_strTemplateName + s;
								if (!tmpl.Exists (s))
								{
									m_strTemplateName = s;
									tmpl.StoreTemplate (*this);
									break;
								}
							}

							m_bTemplateNameChanged = true;
						}
						break;
					case IDCANCEL:
						*this = t;
						break;
					}
				}
				*/
			}
			else
			{
				// template does not exist. create new one
				tmpl.StoreTemplate (*this);
			}
		}
	}
}

// operators
CStyleTemplate& CStyleTemplate::operator= (CStyleTemplate& s)
{
	Init ();

	m_strTemplateName = s.GetName ();
	SetMeasures (s.GetMediumType ());

	memcpy (&m_lfTrackNum, s.GetTrackNumFont (), sizeof (LOGFONT));
	memcpy (&m_lfTrackLength, s.GetTrackLengthFont (), sizeof (LOGFONT));
	memcpy (&m_lfTrackTitle, s.GetTrackTitleFont (), sizeof (LOGFONT));
	memcpy (&m_lfTrackRemarks, s.GetTrackRemarksFont (), sizeof (LOGFONT));
	memcpy (&m_lfTrackGroupingTitle, s.GetTrackGroupingTitleFont (), sizeof (LOGFONT));
	memcpy (&m_lfTrackGroupingRemarks, s.GetTrackGroupingRemarksFont (), sizeof (LOGFONT));
	memcpy (&m_lfTitle[Cover], s.GetTitleFont (Cover), sizeof (LOGFONT));
	memcpy (&m_lfTitle[Back], &m_lfTitle[Cover], sizeof (LOGFONT));
	memcpy (&m_lfTitle[Label], s.GetTitleFont (Label), sizeof (LOGFONT));
	memcpy (&m_lfTitle[Inlay], s.GetTitleFont (Inlay), sizeof (LOGFONT));
	memcpy (&m_lfInlaySide, s.GetInlaySideFont (), sizeof (LOGFONT));
	memcpy (&m_lfBookletPageNumFont, s.GetBookletPageNumFont (), sizeof (LOGFONT));	// ver 4
	m_crTrackNum = s.GetTrackNumColor ();
	m_crTrackLength = s.GetTrackLengthColor ();
	m_crTrackTitle = s.GetTrackTitleColor ();
	m_crTrackRemarks = s.GetTrackRemarksColor ();
	m_crTrackGroupingTitle = s.GetTrackGroupingTitleColor ();
	m_crTrackGroupingRemarks = s.GetTrackGroupingRemarksColor ();
	m_crBookletPageNumColor = s.GetBookletPageNumColor ();	// ver 4

	for (int j = Cover; j <= Label; j++)
	{
		// colors
		m_crTitle[j] = s.GetTitleColor ((RenderType) j);

		/*
		m_crBkStart[j] = s.GetBkColor ((RenderType) j);
		m_crBkEnd[j] = s.GetBkColor2 ((RenderType) j);
		m_nBkAngle[j] = s.GetBkAngle ((RenderType) j);*/
		memcpy (&m_lgdBackground[j], s.GetGradientData ((RenderType) j), sizeof (LINEARGRADIENTDATA));

		// title
		m_nTitleAlign[j] = s.GetTitleAlign ((RenderType) j);
		m_dTitleMargin[j] = s.GetTitleMargin ((RenderType) j);
		m_dTitleTop[j] = s.GetTitleTop ((RenderType) j);
		m_dTitleHeight[j] = s.GetTitleHeight ((RenderType) j);
		m_bDisplayTitles[j] = s.GetDisplayTitles ((RenderType) j);	// ver 3

		// track groupings
		m_bDisplayTrackgroups[j] = s.GetDisplayTrackGroups ((RenderType) j);
		m_nTrackgroupHAlign[j] = s.GetTrackgroupHAlign ((RenderType) j);
		m_nTrackgroupVAlign[j] = s.GetTrackgroupVAlign ((RenderType) j);
		m_dTrackgroupHMargin[j] = s.GetTrackgroupHMargin ((RenderType) j);
		m_dTrackgroupVMargin[j] = s.GetTrackgroupVMargin ((RenderType) j);
	}

	m_crInlaySide = s.GetInlaySideColor ();

	// cover
	/////////////////////////////////////////////////
	// title
	/*
	m_nTitleHAlign[Cover] = s.GetCoverTitleHAlign ();
	m_dCoverTitleHMargin = s.GetCoverTitleHMargin ();
	m_dTitleTop[Cover] = s.GetTitleTop (Cover);
	m_dTitleHeight[Cover] = s.GetTitleHeight (Cover);
	m_bDisplayTitles[Cover] = s.GetDisplayTitles (Cover);		// ver 3
	*/
	// track groupings
	/*
	m_bCoverDisplayTrackgroups = s.IsCoverTrackgroupDisplayed ();
	m_nCoverTrackgroupHAlign = s.GetCoverTrackgroupHAlign ();
	m_nCoverTrackgroupVAlign = s.GetCoverTrackgroupVAlign ();
	m_dCoverTrackgroupHMargin = s.GetCoverTrackgroupHMargin ();
	m_dCoverTrackgroupVMargin = s.GetCoverTrackgroupVMargin ();
	*/
	
	// label
	/////////////////////////////////////////////////
	// title
//	m_nLabelTitleVAlign = s.GetLabelTitleVAlign ();
//	m_dLabelTitleVMargin = s.GetLabelTitleVMargin ();
	m_nLabelTitlePosition = s.GetLabelTitlePosition ();
	m_bLabelTitleRound = s.IsLabelTitleRound ();
	m_bDisplayTitles[Label] = s.GetDisplayTitles (Label);		// ver 3
	// tracks
	m_bLabelDisplayTracks = s.IsLabelTrackDisplayed ();
	m_bDisplayTrackgroups[Label] = s.GetDisplayTrackGroups (Label);
	m_nLabelTrackVAlign = s.GetLabelTrackVAlign ();
	m_dLabelTrackVMargin = s.GetLabelTrackVMargin ();
	m_nLabelTrackPosition = s.GetLabelTrackPosition ();
	m_bLabelTrackRound = s.IsLabelTrackRound ();
	
	// inlay
	/////////////////////////////////////////////////
	// title
	/*
	m_nInlayTitleHAlign = s.GetInlayTitleHAlign ();
	m_dInlayTitleHMargin = s.GetInlayTitleHMargin ();
	m_dTitleTop[Inlay] = s.GetTitleTop (Inlay);
	m_dTitleHeight[Inlay] = s.GetTitleHeight (Inlay);
	m_bDisplayTitles[Inlay] = s.GetDisplayTitles (Inlay);		// ver 3
	*/
	// sides
	m_nInlaySideVAlign = s.GetInlaySideVAlign ();
	m_dInlaySideVMargin = s.GetInlaySideVMargin ();
	m_bInlayDisplaySidebarTitles = s.GetInlayDisplaySidebarTitles ();	// ver 3
	m_bInlaySideTextDirection = s.GetInlaySideTextDirection ();		// ver 6
	m_bInlaySideAntiparallel = s.IsInlaySideAntiparallel ();	// ver 9

	// booklet	// ver 4
	/////////////////////////////////////////////////
	m_dBookletMarginLeft = s.GetBookletMarginLeft ();
	m_dBookletMarginTop = s.GetBookletMarginTop ();
	m_dBookletMarginRight = s.GetBookletMarginRight ();
	m_dBookletMarginBottom = s.GetBookletMarginBottom ();
	m_bBookletShowPageNums = s.ShowBookletPageNums ();
	m_nBookletPageNumAlign = s.GetBookletPageNumAlign ();

	// tracks
	m_bTrackFloating = s.GetTrackFloating ();
	m_bTrackTotalTime = s.GetTrackTotalTime ();
	m_htaTrack[TrackNum].nPosition = s.GetTrackNumPosition ();
	m_htaTrack[TrackNum].bDisplay = s.GetTrackNumDisplay ();
	m_htaTrack[TrackNum].nHAlign = s.GetTrackNumHAlign ();
	m_htaTrack[TrackNum].strFormatting = s.GetTrackNumFormatting ();
	m_htaTrack[TrackLen].nPosition = s.GetTrackLenPosition ();
	m_htaTrack[TrackLen].bDisplay = s.GetTrackLenDisplay ();
	m_htaTrack[TrackLen].nHAlign = s.GetTrackLenHAlign ();
	m_htaTrack[TrackLen].strFormatting = s.GetTrackLenFormatting ();
	m_htaTrack[TrackTitle].nPosition = s.GetTrackTitlePosition ();
	m_htaTrack[TrackTitle].bDisplay = s.GetTrackTitleDisplay ();
	m_htaTrack[TrackTitle].nHAlign = s.GetTrackTitleHAlign ();
	m_htaTrack[TrackRemark].nPosition = s.GetTrackRemarkPosition ();
	m_htaTrack[TrackRemark].bDisplay = s.GetTrackRemarkDisplay ();
	m_htaTrack[TrackRemark].nHAlign = s.GetTrackRemarkHAlign ();

	m_dTrackMarginLeft = s.GetTrackMarginLeft ();
	m_dTrackMarginRight = s.GetTrackMarginRight ();
	m_dTrackInterItemSpacing = s.GetTrackInterItemSpacing ();

	m_dTrackGrpSpacingAbove = s.GetTrackGrpSpacingAbove ();
	m_dTrackGrpSpacingBelow = s.GetTrackGrpSpacingBelow ();
	m_bTrackGrpNewLine = s.GetTrackGrpNewLine ();

	return *this;
}

bool CStyleTemplate::operator ==(CStyleTemplate &s)
{
	if (m_strTemplateName != s.GetName ()) return false;

	if (memcmp (&m_lfTrackNum, s.GetTrackNumFont (), sizeof (LOGFONT))) return false;
	if (memcmp (&m_lfTrackLength, s.GetTrackLengthFont (), sizeof (LOGFONT))) return false;
	if (memcmp (&m_lfTrackTitle, s.GetTrackTitleFont (), sizeof (LOGFONT))) return false;
	if (memcmp (&m_lfTrackRemarks, s.GetTrackRemarksFont (), sizeof (LOGFONT))) return false;
	if (memcmp (&m_lfTrackGroupingTitle, s.GetTrackGroupingTitleFont (), sizeof (LOGFONT))) return false;
	if (memcmp (&m_lfTrackGroupingRemarks, s.GetTrackGroupingRemarksFont (), sizeof (LOGFONT))) return false;

	for (int j = Cover; j <= Label; j++)
	{
		if (memcmp (&m_lfTitle[j], s.GetTitleFont ((RenderType) j), sizeof (LOGFONT))) return false;

		// colors
		if (m_crTitle[j] != s.GetTitleColor ((RenderType) j)) return false;

		/*
		if (m_crBkStart[j] != s.GetBkColor ((RenderType) j)) return false;
		if (m_crBkEnd[j] != s.GetBkColor2 ((RenderType) j)) return false;
		if (m_nBkAngle[j] != s.GetBkAngle ((RenderType) j)) return false;*/
		if (!compare_lineargradientdata (&m_lgdBackground[j], s.GetGradientData ((RenderType) j)))
			return false;

		// title
		if (m_nTitleAlign[j] != s.GetTitleAlign ((RenderType) j)) return false;
		if (m_dTitleMargin[j] != s.GetTitleMargin ((RenderType) j)) return false;
		if (m_dTitleTop[j] != s.GetTitleTop ((RenderType) j)) return false;
		if (m_dTitleHeight[j] != s.GetTitleHeight ((RenderType) j)) return false;
		if (m_bDisplayTitles[j] != s.GetDisplayTitles ((RenderType) j)) return false;

		// track groupings
		if (m_bDisplayTrackgroups[j] != s.GetDisplayTrackGroups ((RenderType) j)) return false;
		if (m_nTrackgroupHAlign[j] != s.GetTrackgroupHAlign ((RenderType) j)) return false;
		if (m_nTrackgroupVAlign[j] != s.GetTrackgroupVAlign ((RenderType) j)) return false;
		if (m_dTrackgroupHMargin[j] != s.GetTrackgroupHMargin ((RenderType) j)) return false;
		if (m_dTrackgroupVMargin[j] != s.GetTrackgroupVMargin ((RenderType) j)) return false;
	}

/*	if (memcmp (&m_lfTitle[Cover], s.GetTitleFont (Cover), sizeof (LOGFONT))) return false;
	if (memcmp (&m_lfTitle[Label], s.GetTitleFont (Label), sizeof (LOGFONT))) return false;
	if (memcmp (&m_lfTitle[Inlay], s.GetTitleFont (Inlay), sizeof (LOGFONT))) return false;
*/
	if (memcmp (&m_lfInlaySide, s.GetInlaySideFont (), sizeof (LOGFONT))) return false;
	if (memcmp (&m_lfBookletPageNumFont, s.GetBookletPageNumFont (), sizeof (LOGFONT))) return false;	// ver 4
	if (m_crTrackNum != s.GetTrackNumColor ()) return false;
	if (m_crTrackLength != s.GetTrackLengthColor ()) return false;
	if (m_crTrackTitle != s.GetTrackTitleColor ()) return false;
	if (m_crTrackRemarks != s.GetTrackRemarksColor ()) return false;
	if (m_crTrackGroupingTitle != s.GetTrackGroupingTitleColor ()) return false;
	if (m_crTrackGroupingRemarks != s.GetTrackGroupingRemarksColor ()) return false;
/*	if (m_crTitle[Cover] != s.GetTitleColor (Cover)) return false;
	if (m_crBkStart[Cover] != s.GetBkColor (Cover)) return false;
	if (m_crBkEnd[Cover] != s.GetBkColor2 (Cover)) return false;
	if (m_crBkStart[Back] != s.GetBkColor (Back)) return false;
	if (m_crBkEnd[Back] != s.GetBkColor2 (Back)) return false;
	if (m_crTitle[Label] != s.GetTitleColor (Label)) return false;
	if (m_crBkStart[Label] != s.GetBkColor (Label)) return false;
	if (m_crBkEnd[Label] != s.GetBkColor2 (Label)) return false;
	if (m_crTitle[Inlay] != s.GetTitleColor (Inlay)) return false;
	if (m_crBkStart[Inlay] != s.GetBkColor (Inlay)) return false;
	if (m_crBkEnd[Inlay] != s.GetBkColor2 (Inlay)) return false;
*/
	if (m_crInlaySide != s.GetInlaySideColor ()) return false;
	if (m_crBookletPageNumColor != s.GetBookletPageNumColor ()) return false;	// ver 4
/*
	if (m_nBkAngle[Cover] != s.GetBkAngle (Cover)) return false;
	if (m_nBkAngle[Back] != s.GetBkAngle (Back)) return false;
	if (m_nBkAngle[Label] != s.GetBkAngle (Label)) return false;
	if (m_nBkAngle[Inlay] != s.GetBkAngle (Inlay)) return false;
*/
	// cover
	/////////////////////////////////////////////////
	// title
/*	if (m_nCoverTitleHAlign != s.GetCoverTitleHAlign ()) return false;
	if (m_dCoverTitleHMargin != s.GetCoverTitleHMargin ()) return false;
	if (m_dTitleTop[Cover] != s.GetTitleTop (Cover)) return false;
	if (m_dTitleHeight[Cover] != s.GetTitleHeight (Cover)) return false;
	if (m_bDisplayTitles[Cover] != s.GetDisplayTitles (Cover)) return false;		// ver 3
*/
	// track groupings
/*
	if (m_bCoverDisplayTrackgroups != s.IsCoverTrackgroupDisplayed ()) return false;
	if (m_nCoverTrackgroupHAlign != s.GetCoverTrackgroupHAlign ()) return false;
	if (m_nCoverTrackgroupVAlign != s.GetCoverTrackgroupVAlign ()) return false;
	if (m_dCoverTrackgroupHMargin != s.GetCoverTrackgroupHMargin ()) return false;
	if (m_dCoverTrackgroupVMargin != s.GetCoverTrackgroupVMargin ()) return false;
*/
	
	// label
	/////////////////////////////////////////////////
	// title
//	if (m_nLabelTitleVAlign != s.GetLabelTitleVAlign ()) return false;
//	if (m_dLabelTitleVMargin != s.GetLabelTitleVMargin ()) return false;
	if (m_nLabelTitlePosition != s.GetLabelTitlePosition ()) return false;
	if (m_bLabelTitleRound != s.IsLabelTitleRound ()) return false;
	if (m_bDisplayTitles[Label] != s.GetDisplayTitles (Label)) return false;		// ver 3
	// tracks
	if (m_bLabelDisplayTracks != s.IsLabelTrackDisplayed ()) return false;
	if (m_bDisplayTrackgroups[Label] != s.GetDisplayTrackGroups (Label)) return false;
	if (m_nLabelTrackVAlign != s.GetLabelTrackVAlign ()) return false;
	if (m_dLabelTrackVMargin != s.GetLabelTrackVMargin ()) return false;
	if (m_nLabelTrackPosition != s.GetLabelTrackPosition ()) return false;
	if (m_bLabelTrackRound != s.IsLabelTrackRound ()) return false;
	
	// inlay
	/////////////////////////////////////////////////
	// title
/*	if (m_nInlayTitleHAlign != s.GetInlayTitleHAlign ()) return false;
	if (m_dInlayTitleHMargin != s.GetInlayTitleHMargin ()) return false;
	if (m_dTitleTop[Inlay] != s.GetTitleTop (Inlay)) return false;
	if (m_dTitleHeight[Inlay] != s.GetTitleHeight (Inlay)) return false;
	if (m_bDisplayTitles[Inlay] != s.GetDisplayTitles (Inlay)) return false;		// ver 3
*/
	// sides
	if (m_nInlaySideVAlign != s.GetInlaySideVAlign ()) return false;
	if (m_dInlaySideVMargin != s.GetInlaySideVMargin ()) return false;
	if (m_bInlayDisplaySidebarTitles != s.GetInlayDisplaySidebarTitles ()) return false;	// ver 3
	if (m_bInlaySideTextDirection != s.GetInlaySideTextDirection ()) return false;	// ver 6
	if (m_bInlaySideAntiparallel != s.IsInlaySideAntiparallel ()) return false;	// ver 9

	// booklet		// ver 4
	/////////////////////////////////////////////////
	if (m_dBookletMarginLeft != s.GetBookletMarginLeft ()) return false;
	if (m_dBookletMarginTop != s.GetBookletMarginTop ()) return false;
	if (m_dBookletMarginRight != s.GetBookletMarginRight ()) return false;
	if (m_dBookletMarginBottom != s.GetBookletMarginBottom ()) return false;
	if (m_bBookletShowPageNums != s.ShowBookletPageNums ()) return false;
	if (m_nBookletPageNumAlign != s.GetBookletPageNumAlign ()) return false;

	// tracks
	if (m_bTrackFloating != s.GetTrackFloating ()) return false;
	if (m_bTrackTotalTime != s.GetTrackTotalTime ()) return false;
	if (m_htaTrack[TrackNum].nPosition != s.GetTrackNumPosition ()) return false;
	if (m_htaTrack[TrackNum].bDisplay != s.GetTrackNumDisplay ()) return false;
	if (m_htaTrack[TrackNum].nHAlign != s.GetTrackNumHAlign ()) return false;
	if (m_htaTrack[TrackNum].strFormatting != s.GetTrackNumFormatting ()) return false;
	if (m_htaTrack[TrackLen].nPosition != s.GetTrackLenPosition ()) return false;
	if (m_htaTrack[TrackLen].bDisplay != s.GetTrackLenDisplay ()) return false;
	if (m_htaTrack[TrackLen].nHAlign != s.GetTrackLenHAlign ()) return false;
	if (m_htaTrack[TrackLen].strFormatting != s.GetTrackLenFormatting ()) return false;
	if (m_htaTrack[TrackTitle].nPosition != s.GetTrackTitlePosition ()) return false;
	if (m_htaTrack[TrackTitle].bDisplay != s.GetTrackTitleDisplay ()) return false;
	if (m_htaTrack[TrackTitle].nHAlign != s.GetTrackTitleHAlign ()) return false;
	if (m_htaTrack[TrackRemark].nPosition != s.GetTrackRemarkPosition ()) return false;
	if (m_htaTrack[TrackRemark].bDisplay != s.GetTrackRemarkDisplay ()) return false;
	if (m_htaTrack[TrackRemark].nHAlign != s.GetTrackRemarkHAlign ()) return false;

	if (m_dTrackMarginLeft != s.GetTrackMarginLeft ()) return false;
	if (m_dTrackMarginRight != s.GetTrackMarginRight ()) return false;
	if (m_dTrackInterItemSpacing != s.GetTrackInterItemSpacing ()) return false;

	if (m_dTrackGrpSpacingAbove != s.GetTrackGrpSpacingAbove ()) return false;
	if (m_dTrackGrpSpacingBelow != s.GetTrackGrpSpacingBelow ()) return false;
	if (m_bTrackGrpNewLine != s.GetTrackGrpNewLine ()) return false;

	return true;
}

bool CStyleTemplate::operator !=(CStyleTemplate &s)
{
	return !(*this == s);
}

// general functions
void CStyleTemplate::Init()
{
	SetMeasures (MediumJewelCase);

	ZeroMemory (m_nTitleAlign, NUM_RENDER_TYPES * sizeof (int));
	ZeroMemory (m_dTitleMargin, NUM_RENDER_TYPES * sizeof (int));
	ZeroMemory (m_sizeTitleRect, NUM_RENDER_TYPES * sizeof (CSize));
	ZeroMemory (m_dTitleHeight, NUM_RENDER_TYPES * sizeof (double));
	ZeroMemory (m_dTitleTop, NUM_RENDER_TYPES * sizeof (double));
	ZeroMemory (m_bDisplayTitles, NUM_RENDER_TYPES * sizeof (bool));
	ZeroMemory (m_bDisplayTrackgroups, NUM_RENDER_TYPES * sizeof (bool));
	ZeroMemory (m_nTrackgroupHAlign, NUM_RENDER_TYPES * sizeof (int));
	ZeroMemory (m_nTrackgroupVAlign, NUM_RENDER_TYPES * sizeof (int));
	ZeroMemory (m_dTrackgroupHMargin, NUM_RENDER_TYPES * sizeof (double));
	ZeroMemory (m_dTrackgroupVMargin, NUM_RENDER_TYPES * sizeof (double));
	ZeroMemory (m_lgdBackground, NUM_RENDER_TYPES * sizeof (LINEARGRADIENTDATA));

	m_bDisplayTrackgroups[Inlay] = true;

	m_bTemplateNameChanged = false;
}

void CStyleTemplate::SetMeasures (MediumType type)
{
	m_medium = type;

	for (int i = 0; i < NUM_RENDER_TYPES; i++)
		m_bHasRenderType[i] = true;

	switch (type)
	{
	case MediumJewelCase:
		m_rcInlayTotal = CRect (0, 0, 15000, 11780);
	//	m_rcCoverTotal = CRect (0, 0, 12200, 12200);
		m_rcCoverTotal = CRect (0, 0, 12000, 12000);
		m_rcBookletTotal = CRect (0, 0, 24000, 12000);

		m_rcInlaySideLeft = CRect (0, 0, 650, 11780);
		m_rcInlaySideRight = CRect (14350, 0, 15000, 11780);
		break;

	case MediumSlimCase:
		m_rcInlayTotal = CRect (0, 0, 1800, 11600);
		m_rcCoverTotal = CRect (0, 0, 13800, 12000);
		m_rcBookletTotal = CRect (0, 0, 24000, 12000);

		m_rcInlaySideLeft = CRect (0, 0, 0, 11600);
		m_rcInlaySideRight = CRect (1500, 0, 1800, 11600);

		break;

	case MediumPaperSleeve:
		m_rcInlayTotal = CRect (0, 0, 15000, 12000);
		m_rcCoverTotal = CRect (0, 0, 13000, 13000);
		m_rcBookletTotal = CRect (0, 0, 24000, 12000);

		m_rcInlaySideLeft = CRect (0, 0, 650, 12000);
		m_rcInlaySideRight = CRect (14350, 0, 15000, 12000);

		m_bHasRenderType[Inlay] = false;
		break;

	case MediumDVDBox:
		m_rcInlayTotal = CRect (0, 0, 14300, 18700);
		m_rcCoverTotal = CRect (0, 0, 13000, 18700);
		m_rcBookletTotal = CRect (0, 0, 26000, 18700);

		m_rcInlaySideLeft = CRect (0, 0, 0, 0);
		m_rcInlaySideRight = CRect (13000, 0, 14300, 18700);
		break;
	}

	m_rcLabelTotal = CRect (0, 0, 12000, 12000);
//	m_rcLabelInner = CRect (4400, 4400, 7600, 7600);
	m_rcLabelInner = CRect (4400, 4400, 7600, 7600);
	m_rcLabelInner2 = CRect (5300, 5300, 6800, 6800);
}

int CStyleTemplate::GetTrackPositionById(int nId)
{
	if ((0 <= nId) && (nId < 4))
		return m_htaTrack[nId].nPosition;

	return -1;
}

UINT CStyleTemplate::GetDrawRectAlignment(int nAlign)
{
	switch (nAlign)
	{
	case 0:	// left
		return DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_LEFT;
	case 1:	// right
		return DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_RIGHT;
	case 2:	// centered
		return DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX | DT_CENTER;
	}

	return 0;
}

UINT CStyleTemplate::GetDrawRectVAlignment(int nAlign)
{
	switch (nAlign)
	{
	case 0:	// top
		return DT_TOP;
	case 1:	// bottom
		return DT_BOTTOM;
	case 2:	// centered
		return DT_VCENTER;
	}

	return 0;
}

// fonts
CFont* CStyleTemplate::SelectTrackNumFont (CRenderDC& rdc, double dFactor)
{
	return CreateFont (&m_lfTrackNum, m_crTrackNum, rdc, dFactor);
}

CFont* CStyleTemplate::SelectTrackLengthFont (CRenderDC& rdc, double dFactor)
{
	return CreateFont (&m_lfTrackLength, m_crTrackLength, rdc, dFactor);
}

CFont* CStyleTemplate::SelectTrackTitleFont (CRenderDC& rdc, double dFactor)
{
	return CreateFont (&m_lfTrackTitle, m_crTrackTitle, rdc, dFactor);
}

CFont* CStyleTemplate::SelectTrackRemarksFont (CRenderDC& rdc, double dFactor)
{
	return CreateFont (&m_lfTrackRemarks, m_crTrackRemarks, rdc, dFactor);
}

CFont* CStyleTemplate::SelectTrackGroupingTitleFont (CRenderDC& rdc, double dFactor)
{
	return CreateFont (&m_lfTrackGroupingTitle, m_crTrackGroupingTitle, rdc, dFactor);
}

CFont* CStyleTemplate::SelectTrackGroupingRemarksFont (CRenderDC& rdc, double dFactor)
{
	return CreateFont (&m_lfTrackGroupingRemarks, m_crTrackGroupingRemarks, rdc, dFactor);
}

CFont* CStyleTemplate::SelectTitleFont (CRenderDC& rdc, RenderType rt, double dFactor)
{
	return CreateFont (&m_lfTitle[rt], m_crTitle[rt], rdc, dFactor);
}

CFont* CStyleTemplate::SelectInlaySideFont (CRenderDC& rdc, bool bIsLeftSide, double dFactor)
{
	m_lfInlaySide.lfEscapement = m_bInlaySideTextDirection ? 900 : -900;
	if (!bIsLeftSide && m_bInlaySideAntiparallel)
		m_lfInlaySide.lfEscapement = -m_lfInlaySide.lfEscapement;

	CFont* pFont = CreateFont (&m_lfInlaySide, m_crInlaySide, rdc, dFactor);
	m_lfInlaySide.lfEscapement = 0;

	return pFont;
}

CFont* CStyleTemplate::CreateFont(LOGFONT *pLf, COLORREF col, CRenderDC &rdc, double dFactor)
{
	LOGFONT lf;
	memcpy (&lf, pLf, sizeof (LOGFONT));
	lf.lfHeight = (int) ((lf.lfHeight /* rdc.GetWidth ())*/ * dFactor /* m_rcInlayTotal.Width (*/));
	if (lf.lfHeight == 0)
		lf.lfHeight = 1;

	rdc.m_pDC->SetTextColor (col);

	CFont f;
	f.CreateFontIndirect (&lf);
	return rdc.m_pDC->SelectObject (&f);
}

// mapping functions:
// cm -> pixels
int CStyleTemplate::MapX(double x, CRenderDC &rdc, RenderType rt)
{
	switch (rt)
	{
	case Cover:
	case Back:
		return (int) ((x * rdc.GetWidth (rt)) / (double) m_rcCoverTotal.Width ());

	case Label:
		return (int) ((x * rdc.GetWidth (rt)) / (double) m_rcLabelTotal.Width ());

	case Inlay:
		return (int) ((x * rdc.GetWidth (rt)) / (double) m_rcInlayTotal.Width ());
	}

	return 0;
}

int CStyleTemplate::MapY(double y, CRenderDC &rdc, RenderType rt)
{
	switch (rt)
	{
	case Cover:
	case Back:
		return (int) ((y * rdc.GetHeight (rt)) / (double) m_rcCoverTotal.Height ());

	case Label:
		return (int) ((y * rdc.GetHeight (rt)) / (double) m_rcLabelTotal.Height ());

	case Inlay:
		return (int) ((y * rdc.GetHeight (rt)) / (double) m_rcInlayTotal.Height ());
	}

	return 0;
}

// pixels -> cm
double CStyleTemplate::MapXr(double x, CRenderDC &rdc, RenderType rt)
{
	double dResult = 0;

	switch (rt)
	{
	case Cover:
	case Back:
		dResult = x * m_rcCoverTotal.Width () / (double) rdc.GetWidth (rt) /*/ 1000.0*/;
		break;

	case Label:
		dResult = x * m_rcLabelTotal.Width () / (double) rdc.GetWidth (rt) /*/ 1000.0*/;
		break;

	case Inlay:
		dResult = x * m_rcInlayTotal.Width () / (double) rdc.GetWidth (rt) /*/ 1000.0*/;
		break;
	}

	// only 3 digits after decimal point
	return (int) dResult / 1000.0;
}

double CStyleTemplate::MapYr(double y, CRenderDC &rdc, RenderType rt)
{
	double dResult = 0;

	switch (rt)
	{
	case Cover:
	case Back:
		dResult = y * m_rcCoverTotal.Height () / (double) rdc.GetHeight (rt) /*/ 1000.0*/;
		break;

	case Label:
		dResult = y * m_rcLabelTotal.Height () / (double) rdc.GetHeight (rt) /*/ 1000.0*/;
		break;

	case Inlay:
		dResult = y * m_rcInlayTotal.Height () / (double) rdc.GetHeight (rt) /*/ 1000.0*/;
		break;
	}

	return (int) dResult / 1000.0;
}

// size functions
CRect* CStyleTemplate::GetTotalInlayRect(CRenderDC &rdc)
{
	m_rect.left = m_rect.top = 0;
	m_rect.right = MapX (m_rcInlayTotal.right, rdc, Inlay);
	m_rect.bottom = MapY (m_rcInlayTotal.bottom, rdc, Inlay);

	return &m_rect;
}

CRect* CStyleTemplate::GetInlayTitleRect(CRenderDC& rdc)
{
	switch (m_nTitleAlign[Inlay])
	{
	case 0:
		m_rect.left = MapX ((int) (m_dTitleMargin[Inlay] * 1000), rdc, Inlay);
		m_rect.right = MapX (m_rcInlaySideRight.left, rdc, Inlay);
		break;
	case 1:
		m_rect.left = MapX (m_rcInlaySideLeft.right, rdc, Inlay);
		m_rect.right = MapX (m_rcInlaySideRight.left - (int) (m_dTitleMargin[Inlay] * 1000), rdc, Inlay);
		break;
	case 2:
		m_rect.left = MapX (m_rcInlaySideLeft.right, rdc, Inlay);
		m_rect.right = MapX (m_rcInlaySideRight.left, rdc, Inlay);
		break;
	}

	m_rect.top = MapY ((int) (m_dTitleTop[Inlay] * 1000), rdc, Inlay);
	m_rect.bottom = MapY ((int) ((m_dTitleTop[Inlay] + m_dTitleHeight[Inlay]) * 1000), rdc, Inlay);

	CheckBoundaries (m_rcInlayTotal, rdc, Inlay);

	return &m_rect;
}

CRect* CStyleTemplate::GetInlayLeftSideRect(CRenderDC &rdc)
{
	m_rect.left = MapX (m_rcInlaySideLeft.left, rdc, Inlay);
	m_rect.top = MapY (m_rcInlaySideLeft.top, rdc, Inlay);
	m_rect.right = MapX (m_rcInlaySideLeft.right, rdc, Inlay);
	m_rect.bottom = MapY (m_rcInlaySideLeft.bottom, rdc, Inlay);

	return &m_rect;
}

CRect* CStyleTemplate::GetInlayRightSideRect(CRenderDC &rdc)
{
	m_rect.left = MapX (m_rcInlaySideRight.left, rdc, Inlay);
	m_rect.top = MapY (m_rcInlaySideRight.top, rdc, Inlay);
	m_rect.right = MapX (m_rcInlaySideRight.right, rdc, Inlay);
	m_rect.bottom = MapY (m_rcInlaySideRight.bottom, rdc, Inlay);

	return &m_rect;
}

CRect* CStyleTemplate::GetInlayTrackRect(CRenderDC &rdc)
{
	GetInlayTitleRect (rdc);
	m_rect.top = m_rect.bottom;
	m_rect.bottom = MapY (m_rcInlayTotal.bottom, rdc, Inlay);

	m_rect.left = MapX (m_rcInlaySideLeft.right + (int) (m_dTrackMarginLeft * 1000), rdc, Inlay);
	m_rect.right = MapX (m_rcInlaySideRight.left - (int) (m_dTrackMarginRight * 1000), rdc, Inlay);

	CheckBoundaries (m_rcInlayTotal, rdc, Inlay);

	return &m_rect;
}

// rt can be Cover (default) or Back
CRect* CStyleTemplate::GetCoverTitleRect(CRenderDC& rdc, RenderType rt)
{
/*	switch (m_nCoverTitleHAlign)
	{
	case 0:
		m_rect.left = MapX ((int) (m_dCoverTitleHMargin * 1000), rdc, Cover);
		m_rect.right = m_rect.left + m_sizeTitleRect[Cover].cx;
		break;

	case 1:
		m_rect.left = (MapX (m_rcCoverTotal.left, rdc, Cover) + MapX (m_rcCoverTotal.right, rdc, Cover) - m_sizeTitleRect[Cover].cx) / 2;
		m_rect.right = m_rect.left + m_sizeTitleRect[Cover].cx;
		break;

	case 2:
		m_rect.right = MapX (m_rcCoverTotal.right, rdc, Cover);
		m_rect.left = m_rect.right - m_sizeTitleRect[Cover].cx;
		break;
	}

	m_rect.top = MapY ((int) (m_dTitleTop[Cover] * 1000), rdc, Cover);
	m_rect.bottom = MapY ((int) ((m_dTitleTop[Cover] + m_dTitleHeight[Cover]) * 1000), rdc, Cover);
//	m_rect.bottom = m_rect.top + m_sizeTitleRect[Cover].cy;
*/

	switch (m_nTitleAlign[rt])
	{
	case 0:
		m_rect.left = MapX ((int) (m_dTitleMargin[rt] * 1000), rdc, rt);
		m_rect.right = MapX (m_rcCoverTotal.right, rdc, rt);
		break;
	case 1:
		m_rect.left = MapX (m_rcCoverTotal.left, rdc, rt);
		m_rect.right = MapX (m_rcCoverTotal.right - (int) (m_dTitleMargin[rt] * 1000), rdc, rt);
		break;
	case 2:
		m_rect.left = MapX (m_rcCoverTotal.left, rdc, rt);
		m_rect.right = MapX (m_rcCoverTotal.right, rdc, rt);
		break;
	}

	m_rect.top = MapY ((int) (m_dTitleTop[rt] * 1000), rdc, rt);
	m_rect.bottom = MapY ((int) ((m_dTitleTop[rt] + m_dTitleHeight[rt]) * 1000), rdc, rt);

	CheckBoundaries (m_rcCoverTotal, rdc, rt);

	return &m_rect;
}

CRect* CStyleTemplate::GetCoverTrackRect(CRenderDC &rdc)
{
	GetCoverTitleRect (rdc);

	switch (m_nTrackgroupHAlign[Cover])
	{
	case 0:	// left
		m_rect.left = MapX ((int) (m_dTrackgroupHMargin[Cover] * 1000), rdc, Cover);
		m_rect.right = MapX (m_rcCoverTotal.right, rdc, Cover);
		break;
	case 1:	// right
		m_rect.left = MapX (m_rcCoverTotal.left, rdc, Cover);
		m_rect.right = MapX (m_rcCoverTotal.right - (int) (m_dTrackgroupHMargin[Cover] * 1000), rdc, Cover);
		break;
	case 2:	// center
		m_rect.left = MapX (m_rcCoverTotal.left, rdc, Cover);
		m_rect.right = MapX (m_rcCoverTotal.right, rdc, Cover);
		break;
	}

	switch (m_nTrackgroupVAlign[Cover])
	{
	case 0:	// top
		m_rect.top = m_rect.bottom + MapY ((int) (m_dTrackgroupVMargin[Cover] * 1000), rdc, Cover);
		m_rect.bottom = MapY (m_rcCoverTotal.bottom, rdc, Cover);
		break;
	case 2:	// center
		m_rect.top = m_rect.bottom;
		m_rect.bottom = MapY (m_rcCoverTotal.bottom, rdc, Cover);
		break;
	case 1:	// bottom
		m_rect.top = m_rect.bottom;
		m_rect.bottom = MapY (m_rcCoverTotal.bottom - (int) (m_dTrackgroupVMargin[Cover] * 1000), rdc, Cover);
		break;
	}

	CheckBoundaries (m_rcCoverTotal, rdc, Cover);

	return &m_rect;
}

CRect* CStyleTemplate::GetBackTrackRect(CRenderDC &rdc)
{
	GetCoverTitleRect (rdc, Back);
	m_rect.top = m_rect.bottom;
	m_rect.bottom = MapY (m_rcCoverTotal.bottom, rdc, Back);

	m_rect.left = MapX ((int) (m_dTrackMarginLeft * 1000), rdc, Back);
	m_rect.right = MapX (m_rcCoverTotal.right - (int) (m_dTrackMarginRight * 1000), rdc, Back);

	CheckBoundaries (m_rcCoverTotal, rdc, Back);

	return &m_rect;
}

CRect* CStyleTemplate::GetLabelTitleRect(CRenderDC& rdc)
{
	CRect rcBounds = m_rcLabelTotal;

	if (m_bLabelTitleRound)
	{
		GetTotalLabelRect (rdc);
		if (m_nTitleAlign[Label] < 2)	// top, bottom
		{
			m_rect.DeflateRect (
				MapX ((int) (m_dTitleMargin[Label] * 1000), rdc, Label),
				MapY ((int) (m_dTitleMargin[Label] * 1000), rdc, Label));
		}
		else	// centered
		{
			CRect r = m_rect;	// total rect
			GetInnerLabelRect (rdc);
			m_rect.left = (r.left + m_rect.left) / 2;
			m_rect.top = (r.top + m_rect.top) / 2;
			m_rect.right = (r.right + m_rect.right) / 2;
			m_rect.bottom = (r.bottom + m_rect.bottom) / 2;
		}
	}
	else
	{
		switch (m_nTitleAlign[Label])
		{
		case 0:	// top
			m_rect.top = MapY ((int) (m_dTitleMargin[Label] * 1000), rdc, Label);
			m_rect.bottom = MapY (m_rcLabelInner.top, rdc, Label);

			// set the bounds that must not be exceeded
			rcBounds.bottom = m_rcLabelInner.top;
			break;

		case 1:	// bottom
			m_rect.top = MapY (m_rcLabelInner.bottom, rdc, Label);
			m_rect.bottom = MapY (m_rcLabelTotal.bottom - (int) (m_dTitleMargin[Label] * 1000), rdc, Label);

			rcBounds.top = m_rcLabelInner.bottom;
			break;

		case 2:	// centered
			{
				if (m_nLabelTitlePosition == 0)	// above hole
				{
					m_rect.top = MapY (m_rcLabelTotal.top, rdc, Label);
					m_rect.bottom = MapY (m_rcLabelInner.top, rdc, Label);

					rcBounds.bottom = m_rcLabelInner.top;
				}
				else	// below hole
				{
					m_rect.top = MapY (m_rcLabelInner.bottom, rdc, Label);
					m_rect.bottom = MapY (m_rcLabelTotal.bottom, rdc, Label);

					rcBounds.top = m_rcLabelInner.bottom;
				}
			}
			break;
		}

		m_rect.left = MapX (m_rcLabelTotal.left, rdc, Label);
		m_rect.right = MapX (m_rcLabelTotal.right, rdc, Label);
	}

	CheckBoundaries (rcBounds, rdc, Label);

	return &m_rect;
}

CRect* CStyleTemplate::GetLabelTrackRectRound(CRenderDC &rdc)
{
	GetLabelTitleRect (rdc);

	CRect rcBounds = m_rcLabelTotal;

	GetTotalLabelRect (rdc);
	if (m_nLabelTrackVAlign < 2)	// top, bottom
	{
		m_rect.DeflateRect (
			MapX ((int) (m_dLabelTrackVMargin * 1000), rdc, Label),
			MapY ((int) (m_dLabelTrackVMargin * 1000), rdc, Label));
	}
	else	// centered
	{/*
		CRect r = m_rect;	// total rect
		GetInnerLabelRect (rdc);
		m_rect.left = (r.left + m_rect.left) / 2;
		m_rect.top = (r.top + m_rect.top) / 2;
		m_rect.right = (r.right + m_rect.right) / 2;
		m_rect.bottom = (r.bottom + m_rect.bottom) / 2;*/
	}

	CheckBoundaries (rcBounds, rdc, Label);

	return &m_rect;
}

CRect* CStyleTemplate::GetLabelTrackRectStraight(CRenderDC &rdc)
{
	GetLabelTitleRect (rdc);

	CRect rcBounds = m_rcLabelTotal;

	switch (m_nLabelTrackVAlign)
	{
	case 0:	// top
		m_rect.top = MapY ((int) (m_dLabelTrackVMargin * 1000), rdc, Label);
		m_rect.bottom = MapY (m_rcLabelInner.top, rdc, Label);

		rcBounds.bottom = m_rcLabelInner.top;
		break;

	case 1:	// bottom
		m_rect.top = MapY (m_rcLabelInner.bottom, rdc, Label);
		m_rect.bottom = MapY (m_rcLabelTotal.bottom - (int) (m_dLabelTrackVMargin * 1000), rdc, Label);

		rcBounds.top = m_rcLabelInner.bottom;
		break;

	case 2:	// centered
		if (m_nLabelTrackPosition == 0)	// above hole
		{
			m_rect.top = MapY (m_rcLabelTotal.top, rdc, Label);
			m_rect.bottom = MapY (m_rcLabelInner.top, rdc, Label);

			rcBounds.bottom = m_rcLabelInner.top;
		}
		else	// below hole
		{
			m_rect.top = MapY (m_rcLabelInner.bottom, rdc, Label);
			m_rect.bottom = MapY (m_rcLabelTotal.bottom, rdc, Label);

			rcBounds.top = m_rcLabelInner.bottom;
		}
		break;
	}

	CheckBoundaries (rcBounds, rdc, Label);

	return &m_rect;
}

CRect* CStyleTemplate::GetLabelTrackRect(CRenderDC &rdc)
{
	if (m_bLabelTrackRound)
		return GetLabelTrackRectRound (rdc);
	else
		return GetLabelTrackRectStraight (rdc);
}

void CStyleTemplate::SetTitleRect(CRect r, RenderType rt, CRenderDC &rdc)
{
	// if centered and the left edge of the rect is != 0, switch to
	// left align
	if ((m_nTitleAlign[rt] == 2) && (r.left != 0))
		m_nTitleAlign[rt] = 0;

	// set margin
	m_dTitleMargin[rt] = MapXr (m_nTitleAlign[rt] == 1 ? r.right : r.left, rdc, rt);

	switch (rt)
	{
	case Cover:
	case Back:
		// if alignment is 'right', margin is measured from the right edge
		if (m_nTitleAlign[rt] == 1)
			m_dTitleMargin[rt] = m_rcCoverTotal.Width () / 1000.0 - m_dTitleMargin[rt];
		break;

	case Label:
		if (m_bLabelTitleRound)
		{
			// get the side that has change the most
			CRect rcOld = *GetLabelTitleRect (rdc);
			int nDeltaLeft = abs (rcOld.left - r.left);
			int nDeltaTop = abs (rcOld.top - r.top);
			int nDeltaRight = abs (rcOld.right - r.right);
			int nDeltaBottom = abs (rcOld.bottom - r.bottom);

			//if ((nDeltaLeft >= nDeltaTop) && (nDeltaLeft >= nDeltaRight) & (nDeltaLeft >= nDeltaBottom))
			if ((nDeltaLeft < nDeltaTop) && (nDeltaLeft < nDeltaRight) && (nDeltaLeft < nDeltaBottom) && (nDeltaTop == nDeltaBottom) && (nDeltaTop == nDeltaRight) && (nDeltaBottom == nDeltaRight))
				m_dTitleMargin[rt] = MapXr (r.left, rdc, rt);
			else if ((nDeltaTop >= nDeltaLeft) && (nDeltaTop >= nDeltaRight) && (nDeltaTop >= nDeltaBottom))
				m_dTitleMargin[rt] = MapYr (r.top, rdc, rt);
			else if ((nDeltaRight >= nDeltaLeft) && (nDeltaRight >= nDeltaTop) && (nDeltaRight >= nDeltaBottom))
				m_dTitleMargin[rt] = m_rcLabelTotal.Width () / 1000 - MapXr (r.right, rdc, rt);
			else
				m_dTitleMargin[rt] = m_rcLabelTotal.Height () / 1000 - MapYr (r.bottom, rdc, rt);
		}
		break;

	case Inlay:
		// if alignment is 'right', margin is measured from the right edge
		if (m_nTitleAlign[Inlay] == 1)
			m_dTitleMargin[Inlay] = (m_rcInlayTotal.Width () - m_rcInlaySideRight.Width ()) / 1000.0 - m_dTitleMargin[Inlay];

		break;
	}

	m_dTitleTop[rt] = MapYr (r.top, rdc, rt);
	m_dTitleHeight[rt] = MapYr (r.bottom, rdc, rt) - m_dTitleTop[rt];
}

void CStyleTemplate::SetTrackRect(CRect r, RenderType rt, CRenderDC &rdc)
{
	switch (rt)
	{
	case Cover:
		// if centered and the left edge of the rect is != 0, switch to
		// left align
		if ((m_nTrackgroupHAlign[Cover] == 2) && (r.left != 0))
			m_nTrackgroupHAlign[Cover] = 0;

		// set margins
		m_dTrackgroupHMargin[Cover] = MapXr (m_nTrackgroupHAlign[Cover] == 1 ? r.right : r.left, rdc, rt);

		m_dTrackgroupVMargin[Cover] = MapYr ((m_nTrackgroupVAlign[Cover] == 1 ? r.bottom : r.top) - GetCoverTitleRect (rdc)->bottom, rdc, rt);
		if (m_nTrackgroupVAlign[Cover] == 1)
			m_dTrackgroupVMargin[Cover] = m_rcCoverTotal.Height () / 1000.0 - m_dTrackgroupVMargin[Cover];
		break;

	case Label:
		break;

	case Inlay:
		// set margin
		m_dTrackMarginLeft = MapXr (r.left, rdc, rt) - m_rcInlaySideLeft.Width () / 1000.0;
		m_dTrackMarginRight = (m_rcInlayTotal.Width () - m_rcInlaySideRight.Width ()) / 1000.0 - MapXr (r.right, rdc, Inlay);
		m_dTitleHeight[Inlay] = MapYr (r.top, rdc, rt) - m_dTitleTop[Inlay];
		break;

	case Back:
		// set margin
		m_dTrackMarginLeft = MapXr (r.left, rdc, rt);
		m_dTrackMarginRight = m_rcCoverTotal.Width () / 1000.0 - MapXr (r.right, rdc, rt);
		m_dTitleHeight[Back] = MapYr (r.top, rdc, rt) - m_dTitleTop[Back];
		break;
	}
}

int CStyleTemplate::GetTrackInterItemSpacing(CRenderDC &rdc, RenderType rt)
{
	return MapX ((int) (m_dTrackInterItemSpacing * 1000), rdc, rt);
}

int CStyleTemplate::GetInlaySideVAlignTop(CRenderDC &rdc, int nTxtExtent)
{
	// retrieve side rect (result in m_rect)
	GetInlayLeftSideRect (rdc);
	if (m_rect.top == m_rect.bottom)
		GetInlayRightSideRect (rdc);
	int nMargin = MapY ((int) (m_dInlaySideVMargin * 1000), rdc, Inlay);

	switch (m_nInlaySideVAlign)
	{
	case 0:	// top
		return nMargin;
	case 1:	// bottom
		return m_rect.bottom - nMargin - nTxtExtent;
	case 2:	// center
		return (m_rect.bottom - nTxtExtent) / 2;
	}

	return 0;
}

///////////////

CRect* CStyleTemplate::GetTotalLabelRect (CRenderDC &rdc)
{
	m_rect.left = m_rect.top = 0;
	m_rect.right = MapX (m_rcLabelTotal.right, rdc, Label);
	m_rect.bottom = MapY (m_rcLabelTotal.bottom, rdc, Label);

	return &m_rect;
}

CRect* CStyleTemplate::GetInnerLabelRect (CRenderDC &rdc)
{
	m_rect.left = MapX (m_rcLabelInner.left, rdc, Label);
	m_rect.top = MapY (m_rcLabelInner.top, rdc, Label);
	m_rect.right = MapX (m_rcLabelInner.right, rdc, Label);
	m_rect.bottom = MapY (m_rcLabelInner.bottom, rdc, Label);

	return &m_rect;
}

CRect* CStyleTemplate::GetInner2LabelRect (CRenderDC &rdc)
{
	m_rect.left = MapX (m_rcLabelInner2.left, rdc, Label);
	m_rect.top = MapY (m_rcLabelInner2.top, rdc, Label);
	m_rect.right = MapX (m_rcLabelInner2.right, rdc, Label);
	m_rect.bottom = MapY (m_rcLabelInner2.bottom, rdc, Label);

	return &m_rect;
}

CRect* CStyleTemplate::GetTotalCoverRect (CRenderDC &rdc)
{
	m_rect.left = m_rect.top = 0;
	m_rect.right = MapX (m_rcCoverTotal.right, rdc, Cover);
	m_rect.bottom = MapY (m_rcCoverTotal.bottom, rdc, Cover);

	return &m_rect;
}

void CStyleTemplate::CheckBoundaries(CRect rcRef, CRenderDC& rdc, RenderType rt)
{
	m_rect.NormalizeRect ();

	int n = MapX (rcRef.left, rdc, rt);
	if (m_rect.left < n)
		m_rect.left = n;
	if (m_rect.right < n)
		m_rect.right = n;

	n = MapX (rcRef.right, rdc, rt);
	if (m_rect.right > n)
		m_rect.right = n;
	if (m_rect.left > n)
		m_rect.left = n;

	n = MapY (rcRef.top, rdc, rt);
	if (m_rect.top < n)
		m_rect.top = n;
	if (m_rect.bottom < n)
		m_rect.bottom = n;

	n = MapY (rcRef.bottom, rdc, rt);
	if (m_rect.bottom > n)
		m_rect.bottom = n;
	if (m_rect.top > n)
		m_rect.top = n;
}

CRect* CStyleTemplate::GetTotalRect(CRenderDC &rdc, RenderType rt)
{
	switch (rt)
	{
	case Cover:
	case Back:
		return GetTotalCoverRect (rdc);
	case Inlay:
		return GetTotalInlayRect (rdc);
	case Label:
		return GetTotalLabelRect (rdc);
	}

	return NULL;
}

CRect* CStyleTemplate::GetTotalRect (RenderType rt)
{
	switch (rt)
	{
	case Cover:
	case Back:
		return GetTotalCoverRect ();
	case Inlay:
		return GetTotalInlayRect ();
	case Label:
		return GetTotalLabelRect ();
	}

	return NULL;
}
