// StyleTemplates.cpp: implementation of the CStyleTemplates class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "StyleTemplates.h"

#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStyleTemplates::CStyleTemplates()
{
	CString strPath = ((CCdCoverCreator2App*) AfxGetApp ())->GetTemplateDir () + "*.cct";

	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile (strPath, &fd);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		fd.cFileName[strlen (fd.cFileName) - 4] = 0;	// cut off the extension
		m_strarrTemplates.Add (fd.cFileName);

		if (!FindNextFile (hFind, &fd))
			break;
	}
}

CStyleTemplates::~CStyleTemplates()
{

}

bool CStyleTemplates::LoadTemplate (CString strName, CStyleTemplate* pTemplate)
{
	try
	{
		CFile file (((CCdCoverCreator2App*) AfxGetApp ())->GetTemplateDir () + strName + ".cct", CFile::modeRead);

		CArchive ar (&file, CArchive::load);
		pTemplate->Serialize (ar);
		ar.Close ();

		file.Close ();

		return true;
	}
	catch (...)
	{
		return false;
	}
}

void CStyleTemplates::StoreTemplate (CStyleTemplate& Template)
{
	if (!Exists (Template.GetName ()))
		m_strarrTemplates.Add (Template.GetName ());

	CFile file (((CCdCoverCreator2App*) AfxGetApp ())->GetTemplateDir () + Template.GetName () + ".cct", CFile::modeCreate | CFile::modeWrite);

	CArchive ar (&file, CArchive::store);
	Template.Serialize (ar);
	ar.Close ();

	file.Close ();
}

bool CStyleTemplates::Exists (CString strName)
{
	for (int i = 0; i < m_strarrTemplates.GetSize (); i++)
		if (m_strarrTemplates.GetAt (i) == strName)
			return true;

	return false;
}

void CStyleTemplates::ExportTemplates ()
{
	CStyleTemplate t;
	CString strId;

	int nCount = AfxGetApp ()->GetProfileInt ("Templates", "Count", 0);
	for (int i = 1; i <= nCount; i++)
	{
		strId.Format ("%d", i);
		LoadTemplateOld (AfxGetApp ()->GetProfileString ("Templates", strId), &t);
		StoreTemplate (t);
	}
}

/*
CString CStyleTemplates::GetTemplateName(int nId)
{
	CString strId;
	strId.Format ("%d", nId);
	return AfxGetApp ()->GetProfileString ("Templates", strId);
}

int CStyleTemplates::GetTemplatesCount()
{
	return AfxGetApp ()->GetProfileInt ("Templates", "Count", 0);
}

void CStyleTemplates::AddTemplate(CString strName)
{
	CString strId;
	strId.Format ("%d", GetTemplatesCount () + 1);
	AfxGetApp ()->WriteProfileString ("Templates", strId, strName);
	AfxGetApp ()->WriteProfileInt ("Templates", "Count", GetTemplatesCount () + 1);
}*/

bool CStyleTemplates::LoadTemplateOld (CString strName, CStyleTemplate* pTemplate)
{
	// load template
	CString strTmplSection = "Templates\\" + strName;

	pTemplate->m_strTemplateName = strName;

	CWinApp* pApp = AfxGetApp ();

	LOGFONT* pLf;
	UINT uiSize = sizeof (LOGFONT);

	pApp->GetProfileBinary (strTmplSection, "lfTrackNum", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfTrackNum, pLf, sizeof (LOGFONT));
	pApp->GetProfileBinary (strTmplSection, "lfTrackLength", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfTrackLength, pLf, sizeof (LOGFONT));
	pApp->GetProfileBinary (strTmplSection, "lfTrackTitle", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfTrackTitle, pLf, sizeof (LOGFONT));
	pApp->GetProfileBinary (strTmplSection, "lfTrackRemark", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfTrackRemarks, pLf, sizeof (LOGFONT));
	pApp->GetProfileBinary (strTmplSection, "lfTrackGroupingTitle", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfTrackGroupingTitle, pLf, sizeof (LOGFONT));
	pApp->GetProfileBinary (strTmplSection, "lfTrackGroupingRemarks", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfTrackGroupingRemarks, pLf, sizeof (LOGFONT));

	double* pD;
	float* pF;

	CString strType[] = {"Cover", "Back", "Inlay", "Label", "Booklet"};
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
	{
		if (pApp->GetProfileBinary (strTmplSection, "lf" + strType[i] + "Title", (LPBYTE*) &pLf, &uiSize) != 0)
			memcpy (&pTemplate->m_lfTitle[i], pLf, sizeof (LOGFONT));

		pTemplate->m_crTitle[i] = pApp->GetProfileInt (strTmplSection, "cr" + strType[i] + "Title", 0);

		pTemplate->m_lgdBackground[i].nNumColors = pApp->GetProfileInt (strTmplSection, "cr" + strType[i] + "BkNumColors", 2);
		for (int j = 0; j < pTemplate->m_lgdBackground[i].nNumColors; j++)
		{
			CString strId = "cr" + strType[i] + "Bk";
			if (j > 0)
				strId.AppendFormat ("%d", j + 1);

			pTemplate->m_lgdBackground[i].colors[j] = pApp->GetProfileInt (strTmplSection, strId, RGB (0xFF, 0xFF, 0xFF));

			uiSize = sizeof (float);
			pApp->GetProfileBinary (strTmplSection, strId + "Pos", (LPBYTE*) &pF, &uiSize);
			pTemplate->m_lgdBackground[i].positions[j] = pF ? *pF : (j == 0 ? 0 : 1);
		}
		pTemplate->m_lgdBackground[i].dAngle = deg2rad (pApp->GetProfileInt (strTmplSection, strType[i] + "BkAngle", 0));

		uiSize = sizeof (double);

		pTemplate->m_nTitleAlign[i] = pApp->GetProfileInt (strTmplSection, strType[i] + "TitleHAlign", 0);
		pApp->GetProfileBinary (strTmplSection, strType[i] + "TitleHMargin", (LPBYTE*) &pD, &uiSize);
		pTemplate->m_dTitleMargin[i] = pD ? *pD : 0;
		pApp->GetProfileBinary (strTmplSection, strType[i] + "TitleTop", (LPBYTE*) &pD, &uiSize);
		pTemplate->m_dTitleTop[i] = pD ? *pD : 0;
		pApp->GetProfileBinary (strTmplSection, strType[i] + "TitleHeight", (LPBYTE*) &pD, &uiSize);
		pTemplate->m_dTitleHeight[i] = pD ? *pD : 0;

		pTemplate->m_bDisplayTrackgroups[i] = pApp->GetProfileInt (strTmplSection, strType[i] + "DisplayTrackgroups", 1) ? true : false;
		pTemplate->m_nTrackgroupHAlign[i] = pApp->GetProfileInt (strTmplSection, strType[i] + "TrackgroupHAlign", 0);
		pTemplate->m_nTrackgroupVAlign[i] = pApp->GetProfileInt (strTmplSection, strType[i] + "TrackgroupVAlign", 0);
		pApp->GetProfileBinary (strTmplSection, strType[i] + "TrackgroupHMargin", (LPBYTE*) &pD, &uiSize);
		pTemplate->m_dTrackgroupHMargin[i] = pD ? *pD : 0;
		pApp->GetProfileBinary (strTmplSection, strType[i] + "TrackgroupVMargin", (LPBYTE*) &pD, &uiSize);
		pTemplate->m_dTrackgroupVMargin[i] = pD ? *pD : 0;
		pTemplate->m_bDisplayTitles[i] = pApp->GetProfileInt (strTmplSection, strType[i] + "DisplayTitles", 1) ? true : false;
	}
	
	//## begin VERSION 2
	/*~~~
	if (pApp->GetProfileBinary (strTmplSection, "lfTitle", (LPBYTE*) &pLf, &uiSize) != 0)
		memcpy (&pTemplate->m_lfTitle[Inlay], pLf, sizeof (LOGFONT));
	if (pApp->GetProfileBinary (strTmplSection, "lfInlayTitle", (LPBYTE*) &pLf, &uiSize) != 0)
		memcpy (&pTemplate->m_lfTitle[Inlay], pLf, sizeof (LOGFONT));
	pApp->GetProfileBinary (strTmplSection, "lfCoverTitle", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfTitle[Cover], pLf == NULL ? &pTemplate->m_lfTitle[Inlay] : pLf, sizeof (LOGFONT));
	pApp->GetProfileBinary (strTmplSection, "lfLabelTitle", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfTitle[Label], pLf == NULL ? &pTemplate->m_lfTitle[Label] : pLf, sizeof (LOGFONT));
	~~~*///## end

	//## begin VERSION 4
	if (pApp->GetProfileBinary (strTmplSection, "lfBookletPageNum", (LPBYTE*) &pLf, &uiSize) != 0)
		memcpy (&pTemplate->m_lfBookletPageNumFont, pLf, sizeof (LOGFONT));
	else
	{
		ZeroMemory (&pTemplate->m_lfBookletPageNumFont, sizeof (LOGFONT));
		pTemplate->m_lfBookletPageNumFont.lfHeight = -8;
		strcpy (pTemplate->m_lfBookletPageNumFont.lfFaceName, "Tahoma");
	}
	//## end

	pApp->GetProfileBinary (strTmplSection, "lfSide", (LPBYTE*) &pLf, &uiSize);
	memcpy (&pTemplate->m_lfInlaySide, pLf, sizeof (LOGFONT));
	pTemplate->m_crTrackNum = pApp->GetProfileInt (strTmplSection, "crTrackNum", 0);
	pTemplate->m_crTrackLength = pApp->GetProfileInt (strTmplSection, "crTrackLength", 0);
	pTemplate->m_crTrackTitle = pApp->GetProfileInt (strTmplSection, "crTrackTitle", 0);
	pTemplate->m_crTrackRemarks = pApp->GetProfileInt (strTmplSection, "crTrackRemark", 0);
	pTemplate->m_crTrackGroupingTitle = pApp->GetProfileInt (strTmplSection, "crTrackGroupingTitle", 0);
	pTemplate->m_crTrackGroupingRemarks = pApp->GetProfileInt (strTmplSection, "crTrackGroupingRemark", 0);
	
	//## begin VERSION 2
	/*~~~
	pTemplate->m_crTitle[Cover] = pApp->GetProfileInt (strTmplSection, "crCoverTitle", 0);
	pTemplate->m_crBkStart[Cover] = pApp->GetProfileInt (strTmplSection, "crCoverBk", RGB (0xFF, 0xFF, 0xFF));
	pTemplate->m_crBkEnd[Cover] = pApp->GetProfileInt (strTmplSection, "crCoverBk2", RGB (0xFF, 0xFF, 0xFF));
	pTemplate->m_crBkStart[Back] = pApp->GetProfileInt (strTmplSection, "crBackBk", RGB (0xFF, 0xFF, 0xFF));
	pTemplate->m_crBkEnd[Back] = pApp->GetProfileInt (strTmplSection, "crBackBk2", RGB (0xFF, 0xFF, 0xFF));
	pTemplate->m_crTitle[Label] = pApp->GetProfileInt (strTmplSection, "crLabelTitle", 0);
	pTemplate->m_crBkStart[Label] = pApp->GetProfileInt (strTmplSection, "crLabelBk", RGB (0xFF, 0xFF, 0xFF));
	pTemplate->m_crBkEnd[Label] = pApp->GetProfileInt (strTmplSection, "crLabelBk2", RGB (0xFF, 0xFF, 0xFF));
	pTemplate->m_crBkStart[Inlay] = pApp->GetProfileInt (strTmplSection, "crInlayBk", RGB (0xFF, 0xFF, 0xFF));
	pTemplate->m_crBkEnd[Inlay] = pApp->GetProfileInt (strTmplSection, "crInlayBk2", RGB (0xFF, 0xFF, 0xFF));
	pTemplate->m_nBkAngle[Cover] = pApp->GetProfileInt (strTmplSection, "CoverBkAngle", 0);
	pTemplate->m_nBkAngle[Back] = pApp->GetProfileInt (strTmplSection, "BackBkAngle", 0);
	pTemplate->m_nBkAngle[Label] = pApp->GetProfileInt (strTmplSection, "LabelBkAngle", 0);
	pTemplate->m_nBkAngle[Inlay] = pApp->GetProfileInt (strTmplSection, "InlayBkAngle", 0);
	~~~*/
	//## end

	//## begin VERSION 4
	pTemplate->m_crBookletPageNumColor = pApp->GetProfileInt (strTmplSection, "crBookletPageNum", RGB (0x00, 0x00, 0x00));
	//## end
	
	pTemplate->m_crTitle[Inlay] = pApp->GetProfileInt (strTmplSection, "crTitle", 0);
	pTemplate->m_crInlaySide = pApp->GetProfileInt (strTmplSection, "crSide", 0);

	//## begin VERSION 2
	// cover
	/*~~~
	pTemplate->m_nTitleHAlign[Cover] = pApp->GetProfileInt (strTmplSection, "CoverTitleHAlign", 0);
	pApp->GetProfileBinary (strTmplSection, "CoverTitleHMargin", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dCoverTitleHMargin = pD ? *pD : 0;
	pApp->GetProfileBinary (strTmplSection, "CoverTitleTop", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTitleTop[Cover] = pD ? *pD : 0;
	pApp->GetProfileBinary (strTmplSection, "CoverTitleHeight", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTitleHeight[Cover] = pD ? *pD : 0;
	pTemplate->m_bCoverDisplayTrackgroups = pApp->GetProfileInt (strTmplSection, "CoverDisplayTrackgroups", 1) ? true : false;
	pTemplate->m_nCoverTrackgroupHAlign = pApp->GetProfileInt (strTmplSection, "CoverTrackgroupHAlign", 0);
	pTemplate->m_nCoverTrackgroupVAlign = pApp->GetProfileInt (strTmplSection, "CoverTrackgroupVAlign", 0);
	pApp->GetProfileBinary (strTmplSection, "CoverTrackgroupHMargin", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dCoverTrackgroupHMargin = pD ? *pD : 0;
	pApp->GetProfileBinary (strTmplSection, "CoverTrackgroupVMargin", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dCoverTrackgroupVMargin = pD ? *pD : 0;
	//## begin VERSION 3
	pTemplate->m_bDisplayTitles[Cover] = pApp->GetProfileInt (strTmplSection, "CoverDisplayTitles", 1) ? true : false;
	//## end VERSION 3
	~~~*/

	// label
	//////////////////////////////
	// title
//	pTemplate->m_nLabelTitleVAlign = pApp->GetProfileInt (strTmplSection, "LabelTitleVAlign", 0);
//	pApp->GetProfileBinary (strTmplSection, "LabelTitleVMargin", (LPBYTE*) &pD, &uiSize);
//	pTemplate->m_dLabelTitleVMargin = pD ? *pD : 0;
	pTemplate->m_nLabelTitlePosition = pApp->GetProfileInt (strTmplSection, "LabelTitlePosition", 0);
	pTemplate->m_bLabelTitleRound = pApp->GetProfileInt (strTmplSection, "LabelTitleRound", 1) ? true : false;
	//## begin VERSION 3
	//pTemplate->m_bDisplayTitles[Label] = pApp->GetProfileInt (strTmplSection, "LabelDisplayTitles", 1) ? true : false;
	//## end VERSION 3

	// tracks
	pTemplate->m_bLabelDisplayTracks = pApp->GetProfileInt (strTmplSection, "LabelDisplayTracks", 0) ? true : false;
	pTemplate->m_bDisplayTrackgroups[Label] = pApp->GetProfileInt (strTmplSection, "LabelDisplayTrackgroups", 0) ? true : false;
	pTemplate->m_nLabelTrackVAlign = pApp->GetProfileInt (strTmplSection, "LabelTrackVAlign", 0);
	pApp->GetProfileBinary (strTmplSection, "LabelTrackVMargin", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dLabelTrackVMargin = pD ? *pD : 0;
	pTemplate->m_nLabelTrackPosition = pApp->GetProfileInt (strTmplSection, "LabelTrackPosition", 0);
	pTemplate->m_bLabelTrackRound = pApp->GetProfileInt (strTmplSection, "LabelTrackRound", 0) ? true : false;
	//## end VERSION 2

	//## begin VERSION 4
	pApp->GetProfileBinary (strTmplSection, "BookletMarginLeft", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dBookletMarginLeft = pD ? *pD : 1.0;
	pApp->GetProfileBinary (strTmplSection, "BookletMarginTop", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dBookletMarginTop = pD ? *pD : 1.0;
	pApp->GetProfileBinary (strTmplSection, "BookletMarginRight", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dBookletMarginRight = pD ? *pD : 1.0;
	pApp->GetProfileBinary (strTmplSection, "BookletMarginBottom", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dBookletMarginBottom = pD ? *pD : 1.0;
	pTemplate->m_bBookletShowPageNums = pApp->GetProfileInt (strTmplSection, "BookletShowPageNums", 1) ? true : false;
	pTemplate->m_nBookletPageNumAlign = pApp->GetProfileInt (strTmplSection, "BookletPageNumAlign", 0);
	//## end

	// inlay
	/*~~~
	pTemplate->m_nInlayTitleHAlign = pApp->GetProfileInt (strTmplSection, "HTitleAlign", 0);
	pApp->GetProfileBinary (strTmplSection, "HTitleMargin", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dInlayTitleHMargin = *pD;
	pApp->GetProfileBinary (strTmplSection, "TitleTop", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTitleTop[Inlay] = *pD;
	pApp->GetProfileBinary (strTmplSection, "TitleHeight", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTitleHeight[Inlay] = *pD;
	//## begin VERSION 3
	pTemplate->m_bDisplayTitles[Inlay] = pApp->GetProfileInt (strTmplSection, "InlayDisplayTitles", 1) ? true : false;
	//## end VERSION 3
	~~~*/

	// sides
	pTemplate->m_nInlaySideVAlign = pApp->GetProfileInt (strTmplSection, "VSideAlign", 0);
	pApp->GetProfileBinary (strTmplSection, "VSideMargin", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dInlaySideVMargin = *pD;
	//## begin VERSION 3
	pTemplate->m_bInlayDisplaySidebarTitles = pApp->GetProfileInt (strTmplSection, "InlayDisplaySidebarTitles", 1) ? true : false;
	//## end VERSION 3

	//## begin VERSION 6
	pTemplate->m_bInlaySideTextDirection = pApp->GetProfileInt (strTmplSection, "InlaySideTextDirection", 1) ? true : false;
	//## end VERSION 6

	// tracks
	pTemplate->m_bTrackFloating = pApp->GetProfileInt (strTmplSection, "TrackFloating", 0) == 0 ? false : true;
	pTemplate->m_bTrackTotalTime = pApp->GetProfileInt (strTmplSection, "TrackTotalTime", 0) == 0 ? false : true;

	pTemplate->m_htaTrack[TrackNum].nPosition = pApp->GetProfileInt (strTmplSection, "TrackNumPosition", 0);
	pTemplate->m_htaTrack[TrackNum].bDisplay = pApp->GetProfileInt (strTmplSection, "TrackNumDisplay", 0) == 0 ? false : true;
	pTemplate->m_htaTrack[TrackNum].nHAlign = pApp->GetProfileInt (strTmplSection, "TrackNumAlign", 0);
	pTemplate->m_htaTrack[TrackNum].strFormatting = pApp->GetProfileString (strTmplSection, "TrackNumFormatting", "#.");
	pTemplate->m_htaTrack[TrackLen].nPosition = pApp->GetProfileInt (strTmplSection, "TrackLenPosition", 0);
	pTemplate->m_htaTrack[TrackLen].bDisplay = pApp->GetProfileInt (strTmplSection, "TrackLenDisplay", 0) == 0 ? false : true;
	pTemplate->m_htaTrack[TrackLen].nHAlign = pApp->GetProfileInt (strTmplSection, "TrackLenAlign", 0);
	pTemplate->m_htaTrack[TrackLen].strFormatting = pApp->GetProfileString (strTmplSection, "TrackLenFormatting", "#:0##");
	pTemplate->m_htaTrack[TrackTitle].nPosition = pApp->GetProfileInt (strTmplSection, "TrackTitlePosition", 0);
	pTemplate->m_htaTrack[TrackTitle].bDisplay = pApp->GetProfileInt (strTmplSection, "TrackTitleDisplay", 0) == 0 ? false : true;
	pTemplate->m_htaTrack[TrackTitle].nHAlign = pApp->GetProfileInt (strTmplSection, "TrackTitleAlign", 0);
	pTemplate->m_htaTrack[TrackRemark].nPosition = pApp->GetProfileInt (strTmplSection, "TrackRemarkPosition", 0);
	pTemplate->m_htaTrack[TrackRemark].bDisplay = pApp->GetProfileInt (strTmplSection, "TrackRemarkDisplay", 0) == 0 ? false : true;
	pTemplate->m_htaTrack[TrackRemark].nHAlign = pApp->GetProfileInt (strTmplSection, "TrackRemarkAlign", 0);

	pApp->GetProfileBinary (strTmplSection, "TrackMarginLeft", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTrackMarginLeft = *pD;
	pApp->GetProfileBinary (strTmplSection, "TrackMarginRight", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTrackMarginRight = *pD;
	pApp->GetProfileBinary (strTmplSection, "TrackInterItemSpacing", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTrackInterItemSpacing = *pD;

	pApp->GetProfileBinary (strTmplSection, "TrackGrpSpacingAbove", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTrackGrpSpacingAbove = *pD;
	pApp->GetProfileBinary (strTmplSection, "TrackGrpSpacingBelow", (LPBYTE*) &pD, &uiSize);
	pTemplate->m_dTrackGrpSpacingBelow = *pD;
	pTemplate->m_bTrackGrpNewLine = pApp->GetProfileInt (strTmplSection, "TrackGrpNewLine", 0) == 0 ? false : true;

	return true;
}

/*
void CStyleTemplates::StoreTemplate(CStyleTemplate& Template)
{
	if (!Exists (Template.GetName ()))
		AddTemplate (Template.GetName ());

	CString strTmplSection = "Templates\\" + Template.GetName ();

	CWinApp* pApp = AfxGetApp ();

	pApp->WriteProfileString (strTmplSection, "Name", Template.m_strTemplateName);

	pApp->WriteProfileBinary (strTmplSection, "lfTrackNum", (LPBYTE) &Template.m_lfTrackNum, sizeof (LOGFONT));
	pApp->WriteProfileBinary (strTmplSection, "lfTrackLength", (LPBYTE) &Template.m_lfTrackLength, sizeof (LOGFONT));
	pApp->WriteProfileBinary (strTmplSection, "lfTrackTitle", (LPBYTE) &Template.m_lfTrackTitle, sizeof (LOGFONT));
	pApp->WriteProfileBinary (strTmplSection, "lfTrackRemark", (LPBYTE) &Template.m_lfTrackRemarks, sizeof (LOGFONT));
	pApp->WriteProfileBinary (strTmplSection, "lfTrackGroupingTitle", (LPBYTE) &Template.m_lfTrackGroupingTitle, sizeof (LOGFONT));
	pApp->WriteProfileBinary (strTmplSection, "lfTrackGroupingRemarks", (LPBYTE) &Template.m_lfTrackGroupingRemarks, sizeof (LOGFONT));

	CString strType[] = {"Cover", "Back", "Inlay", "Label", "Booklet"};
	for (int i = 0; i < NUM_RENDER_TYPES; i++)
	{
		pApp->WriteProfileBinary (strTmplSection, "lf" + strType[i] + "Title", (LPBYTE) &Template.m_lfTitle[i], sizeof (LOGFONT));

		pApp->WriteProfileInt (strTmplSection, "cr" + strType[i] + "Title", Template.m_crTitle[i]);

		pApp->WriteProfileInt (strTmplSection, "cr" + strType[i] + "BkNumColors", Template.m_lgdBackground[i].nNumColors);
		for (int j = 0; j < Template.m_lgdBackground[i].nNumColors; j++)
		{
			CString strId = "cr" + strType[i] + "Bk";
			if (j > 0)
				strId.AppendFormat ("%d", j + 1);

			pApp->WriteProfileInt (strTmplSection, strId, Template.m_lgdBackground[i].colors[j]);
			pApp->WriteProfileBinary (strTmplSection, strId + "Pos", (LPBYTE) &Template.m_lgdBackground[i].positions[j], sizeof (double));
		}
		pApp->WriteProfileInt (strTmplSection, strType[i] + "BkAngle", rad2deg (Template.m_lgdBackground[i].dAngle));

		pApp->WriteProfileInt (strTmplSection, strType[i] + "TitleHAlign", Template.m_nTitleAlign[i]);
		pApp->WriteProfileBinary (strTmplSection, strType[i] + "TitleHMargin", (LPBYTE) &Template.m_dTitleMargin[i], sizeof (double));
		pApp->WriteProfileBinary (strTmplSection, strType[i] + "TitleTop", (LPBYTE) &Template.m_dTitleTop[i], sizeof (double));
		pApp->WriteProfileBinary (strTmplSection, strType[i] + "TitleHeight", (LPBYTE) &Template.m_dTitleHeight[i], sizeof (double));
		pApp->WriteProfileInt (strTmplSection, strType[i] + "DisplayTitles", Template.m_bDisplayTitles[i] ? 1 : 0);

		pApp->WriteProfileInt (strTmplSection, strType[i] + "DisplayTrackgroups", Template.m_bDisplayTrackgroups[i] ? 1 : 0);
		pApp->WriteProfileInt (strTmplSection, strType[i] + "TrackgroupHAlign", Template.m_nTrackgroupHAlign[i]);
		pApp->WriteProfileInt (strTmplSection, strType[i] + "TrackgroupVAlign", Template.m_nTrackgroupVAlign[i]);
		pApp->WriteProfileBinary (strTmplSection, strType[i] + "TrackgroupHMargin", (LPBYTE) &Template.m_dTrackgroupHMargin[i], sizeof (double));
		pApp->WriteProfileBinary (strTmplSection, strType[i] + "TrackgroupVMargin", (LPBYTE) &Template.m_dTrackgroupVMargin[i], sizeof (double));

	}
		
	//## begin VERSION 2
	/°
	pApp->WriteProfileBinary (strTmplSection, "lfCoverTitle", (LPBYTE) &Template.m_lfTitle[Cover], sizeof (LOGFONT));
	pApp->WriteProfileBinary (strTmplSection, "lfLabelTitle", (LPBYTE) &Template.m_lfTitle[Label], sizeof (LOGFONT));
	°/
	//## end
	
	//## begin VERSION 4
	pApp->WriteProfileBinary (strTmplSection, "lfBookletPageNum", (LPBYTE) &Template.m_lfBookletPageNumFont, sizeof (LOGFONT));
	//## end

//	pApp->WriteProfileBinary (strTmplSection, "lfInlayTitle", (LPBYTE) &Template.m_lfTitle[Inlay], sizeof (LOGFONT));
	pApp->WriteProfileBinary (strTmplSection, "lfSide", (LPBYTE) &Template.m_lfInlaySide, sizeof (LOGFONT));
	pApp->WriteProfileInt (strTmplSection, "crTrackNum", Template.m_crTrackNum);
	pApp->WriteProfileInt (strTmplSection, "crTrackLength", Template.m_crTrackLength);
	pApp->WriteProfileInt (strTmplSection, "crTrackTitle", Template.m_crTrackTitle);
	pApp->WriteProfileInt (strTmplSection, "crTrackRemark", Template.m_crTrackRemarks);
	pApp->WriteProfileInt (strTmplSection, "crTrackGroupingTitle", Template.m_crTrackGroupingTitle);
	pApp->WriteProfileInt (strTmplSection, "crTrackGroupingRemark", Template.m_crTrackGroupingRemarks);

	//## begin VERSION 2
	/°
	pApp->WriteProfileInt (strTmplSection, "crCoverTitle", Template.m_crTitle[Cover]);
	pApp->WriteProfileInt (strTmplSection, "crCoverBk", Template.m_crBkStart[Cover]);
	pApp->WriteProfileInt (strTmplSection, "crCoverBk2", Template.m_crBkEnd[Cover]);
	pApp->WriteProfileInt (strTmplSection, "crBackBk", Template.m_crBkStart[Back]);
	pApp->WriteProfileInt (strTmplSection, "crBackBk2", Template.m_crBkEnd[Back]);
	pApp->WriteProfileInt (strTmplSection, "crLabelTitle", Template.m_crTitle[Label]);
	pApp->WriteProfileInt (strTmplSection, "crLabelBk", Template.m_crBkStart[Label]);
	pApp->WriteProfileInt (strTmplSection, "crLabelBk2", Template.m_crBkEnd[Label]);
	pApp->WriteProfileInt (strTmplSection, "crInlayBk", Template.m_crBkStart[Inlay]);
	pApp->WriteProfileInt (strTmplSection, "crInlayBk2", Template.m_crBkEnd[Inlay]);

	pApp->WriteProfileInt (strTmplSection, "CoverBkAngle", Template.m_nBkAngle[Cover]);
	pApp->WriteProfileInt (strTmplSection, "BackBkAngle", Template.m_nBkAngle[Back]);
	pApp->WriteProfileInt (strTmplSection, "LabelBkAngle", Template.m_nBkAngle[Label]);
	pApp->WriteProfileInt (strTmplSection, "InlayBkAngle", Template.m_nBkAngle[Inlay]);
	°/
	//## end

	//## begin VERSION 4
	pApp->WriteProfileInt (strTmplSection, "crBookletPageNum", Template.m_crBookletPageNumColor);
	//## end
	
	//pApp->WriteProfileInt (strTmplSection, "crTitle", Template.m_crTitle[Inlay]);
	pApp->WriteProfileInt (strTmplSection, "crSide", Template.m_crInlaySide);

	//## begin VERSION 2
	// cover
	/°
	pApp->WriteProfileInt (strTmplSection, "CoverTitleHAlign", Template.m_nCoverTitleHAlign);
	pApp->WriteProfileBinary (strTmplSection, "CoverTitleHMargin", (LPBYTE) &Template.m_dCoverTitleHMargin, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "CoverTitleTop", (LPBYTE) &Template.m_dTitleTop[Cover], sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "CoverTitleHeight", (LPBYTE) &Template.m_dTitleHeight[Cover], sizeof (double));
	//## begin VERSION 3
	pApp->WriteProfileInt (strTmplSection, "CoverDisplayTitles", Template.m_bDisplayTitles[Cover] ? 1 : 0);
	°/
	//## end VERSION 3

	// tracks
	/°
	pApp->WriteProfileInt (strTmplSection, "CoverDisplayTrackgroups", Template.m_bCoverDisplayTrackgroups ? 1 : 0);
	pApp->WriteProfileInt (strTmplSection, "CoverTrackgroupHAlign", Template.m_nCoverTrackgroupHAlign);
	pApp->WriteProfileInt (strTmplSection, "CoverTrackgroupVAlign", Template.m_nCoverTrackgroupVAlign);
	pApp->WriteProfileBinary (strTmplSection, "CoverTrackgroupHMargin", (LPBYTE) &Template.m_dCoverTrackgroupHMargin, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "CoverTrackgroupVMargin", (LPBYTE) &Template.m_dCoverTrackgroupVMargin, sizeof (double));
	°/
	
	// label
	//////////////////////////////
	// title
//	pApp->WriteProfileInt (strTmplSection, "LabelTitleVAlign", Template.m_nLabelTitleVAlign);
//	pApp->WriteProfileBinary (strTmplSection, "LabelTitleVMargin", (LPBYTE) &Template.m_dLabelTitleVMargin, sizeof (double));
	pApp->WriteProfileInt (strTmplSection, "LabelTitlePosition", Template.m_nLabelTitlePosition);
	pApp->WriteProfileInt (strTmplSection, "LabelTitleRound", Template.m_bLabelTitleRound ? 1 : 0);
	//## begin VERSION 3
	//pApp->WriteProfileInt (strTmplSection, "LabelDisplayTitles", Template.m_bDisplayTitles[Label] ? 1 : 0);
	//## end VERSION 3

	// tracks
	pApp->WriteProfileInt (strTmplSection, "LabelDisplayTracks", Template.m_bLabelDisplayTracks ? 1 : 0);
//	pApp->WriteProfileInt (strTmplSection, "LabelDisplayTrackgroups", Template.m_bLabelDisplayTrackgroups ? 1 : 0);
	pApp->WriteProfileInt (strTmplSection, "LabelTrackVAlign", Template.m_nLabelTrackVAlign);
	pApp->WriteProfileBinary (strTmplSection, "LabelTrackVMargin", (LPBYTE) &Template.m_dLabelTrackVMargin, sizeof (double));
	pApp->WriteProfileInt (strTmplSection, "LabelTrackPosition", Template.m_nLabelTrackPosition);
	pApp->WriteProfileInt (strTmplSection, "LabelTrackRound", Template.m_bLabelTrackRound ? 1 : 0);
	//## end VERSION 2

	//## begin VERSION 4
	pApp->WriteProfileBinary (strTmplSection, "BookletMarginLeft", (LPBYTE) &Template.m_dBookletMarginLeft, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "BookletMarginTop", (LPBYTE) &Template.m_dBookletMarginTop, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "BookletMarginRight", (LPBYTE) &Template.m_dBookletMarginRight, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "BookletMarginBottom", (LPBYTE) &Template.m_dBookletMarginBottom, sizeof (double));
	pApp->WriteProfileInt (strTmplSection, "BookletShowPageNums", Template.m_bBookletShowPageNums ? 1 : 0);
	pApp->WriteProfileInt (strTmplSection, "BookletPageNumAlign", Template.m_nBookletPageNumAlign);
	//## end

	// inlay
	// title
	/°
	pApp->WriteProfileInt (strTmplSection, "HTitleAlign", Template.m_nInlayTitleHAlign);
	pApp->WriteProfileBinary (strTmplSection, "HTitleMargin", (LPBYTE) &Template.m_dInlayTitleHMargin, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "TitleTop", (LPBYTE) &Template.m_dTitleTop[Inlay], sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "TitleHeight", (LPBYTE) &Template.m_dTitleHeight[Inlay], sizeof (double));
	//## begin VERSION 3
	pApp->WriteProfileInt (strTmplSection, "InlayDisplayTitles", Template.m_bDisplayTitles[Inlay] ? 1 : 0);
	//## end VERSION 3
	°/

	// sides
	pApp->WriteProfileInt (strTmplSection, "VSideAlign", Template.m_nInlaySideVAlign);
	pApp->WriteProfileBinary (strTmplSection, "VSideMargin", (LPBYTE) &Template.m_dInlaySideVMargin, sizeof (double));
	//## begin VERSION 3
	pApp->WriteProfileInt (strTmplSection, "InlayDisplaySidebarTitles", Template.m_bInlayDisplaySidebarTitles ? 1 : 0);
	//## end VERSION 3

	//## begin VERSION 6
	pApp->WriteProfileInt (strTmplSection, "InlaySideTextDirection", Template.m_bInlaySideTextDirection ? 1 : 0);
	//## end VERSION 6

	// tracks
	pApp->WriteProfileInt (strTmplSection, "TrackFloating", Template.m_bTrackFloating ? 1 : 0);
	pApp->WriteProfileInt (strTmplSection, "TrackTotalTime", Template.m_bTrackTotalTime ? 1 : 0);

	pApp->WriteProfileInt (strTmplSection, "TrackNumPosition", Template.m_htaTrack[TrackNum].nPosition);
	pApp->WriteProfileInt (strTmplSection, "TrackNumDisplay", Template.m_htaTrack[TrackNum].bDisplay ? 1 : 0);
	pApp->WriteProfileInt (strTmplSection, "TrackNumAlign", Template.m_htaTrack[TrackNum].nHAlign);
	pApp->WriteProfileString (strTmplSection, "TrackNumFormatting", Template.m_htaTrack[TrackNum].strFormatting);
	pApp->WriteProfileInt (strTmplSection, "TrackLenPosition", Template.m_htaTrack[TrackLen].nPosition);
	pApp->WriteProfileInt (strTmplSection, "TrackLenDisplay", Template.m_htaTrack[TrackLen].bDisplay ? 1 : 0);
	pApp->WriteProfileInt (strTmplSection, "TrackLenAlign", Template.m_htaTrack[TrackLen].nHAlign);
	pApp->WriteProfileString (strTmplSection, "TrackLenFormatting", Template.m_htaTrack[TrackLen].strFormatting);
	pApp->WriteProfileInt (strTmplSection, "TrackTitlePosition", Template.m_htaTrack[TrackTitle].nPosition);
	pApp->WriteProfileInt (strTmplSection, "TrackTitleDisplay", Template.m_htaTrack[TrackTitle].bDisplay ? 1 : 0);
	pApp->WriteProfileInt (strTmplSection, "TrackTitleAlign", Template.m_htaTrack[TrackTitle].nHAlign);
	pApp->WriteProfileInt (strTmplSection, "TrackRemarkPosition", Template.m_htaTrack[TrackRemark].nPosition);
	pApp->WriteProfileInt (strTmplSection, "TrackRemarkDisplay", Template.m_htaTrack[TrackRemark].bDisplay ? 1 : 0);
	pApp->WriteProfileInt (strTmplSection, "TrackRemarkAlign", Template.m_htaTrack[TrackRemark].nHAlign);

	pApp->WriteProfileBinary (strTmplSection, "TrackMarginLeft", (LPBYTE) &Template.m_dTrackMarginLeft, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "TrackMarginRight", (LPBYTE) &Template.m_dTrackMarginRight, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "TrackInterItemSpacing", (LPBYTE) &Template.m_dTrackInterItemSpacing, sizeof (double));

	pApp->WriteProfileBinary (strTmplSection, "TrackGrpSpacingAbove", (LPBYTE) &Template.m_dTrackGrpSpacingAbove, sizeof (double));
	pApp->WriteProfileBinary (strTmplSection, "TrackGrpSpacingBelow", (LPBYTE) &Template.m_dTrackGrpSpacingBelow, sizeof (double));
	pApp->WriteProfileInt (strTmplSection, "TrackGrpNewLine", Template.m_bTrackGrpNewLine ? 1 : 0);
}

bool CStyleTemplates::Exists(CString strName)
{
	// check whether template exists
	CString strId;
	for (int i = 1; i <= GetTemplatesCount (); i++)
	{
		strId.Format ("%d", i);
		if (AfxGetApp ()->GetProfileString ("Templates", strId) == strName)
			return true;
	}

	return false;
}

void CStyleTemplates::LoadFromResource(UINT nId, CStyleTemplate *pTemplate)
{
	HRSRC hResInfo = ::FindResource (AfxGetResourceHandle (), MAKEINTRESOURCE (nId), "STYLETEMPLATE");
	HGLOBAL hglData = ::LoadResource (AfxGetResourceHandle (), hResInfo);

	CMemFile memfile ((BYTE*) ::LockResource (hglData),
		::SizeofResource (AfxGetResourceHandle (), hResInfo));

	CArchive ar (&memfile, CArchive::load);
	pTemplate->Serialize (ar);

	::FreeResource (hglData);
}
*/