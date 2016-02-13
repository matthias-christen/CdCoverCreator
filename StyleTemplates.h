// StyleTemplates.h: interface for the CStyleTemplates class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STYLETEMPLATES_H__3EC8A16E_F220_4BFA_A362_EF9D00FA8030__INCLUDED_)
#define AFX_STYLETEMPLATES_H__3EC8A16E_F220_4BFA_A362_EF9D00FA8030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StyleTemplate.h"

class CStyleTemplates
{
public:
	bool LoadTemplate (CString strName, CStyleTemplate* pTemplate);
	void StoreTemplate (CStyleTemplate& Template);
	bool Exists (CString strName);
	CStringArray& GetTemplates () { return m_strarrTemplates; }

	// exports the templates from the registry to the file system (new mode)
	void ExportTemplates ();

	//void LoadFromResource (UINT nId, CStyleTemplate* pTemplate);
	//void AddTemplate (CString strName);
	//int GetTemplatesCount();
	//CString GetTemplateName (int nId);

	CStyleTemplates();
	virtual ~CStyleTemplates();

protected:
	CStringArray m_strarrTemplates;

	bool LoadTemplateOld (CString strName, CStyleTemplate* pTemplate);
};

#endif // !defined(AFX_STYLETEMPLATES_H__3EC8A16E_F220_4BFA_A362_EF9D00FA8030__INCLUDED_)
