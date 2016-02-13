// undo.h implementation
// Author - Keith Rule (keithr@europa.com)
//
// A description of this code can be found in May 1997 - Windows Tech Journal.
// Back issues can be ordered from either (800) 241-4320 or (918) 831-9557.
//
// An extended version of this class is part of the MFC Class of the week
// (http://www.weeklymfc.com/).

#ifndef _UNDO_H_
#define _UNDO_H_
//------------------------------------------------------------
//  Undo/Redo for MFC By Keith Rule
class CUndo {
private:
	CObList	m_undolist;		// Stores undo states
	CObList	m_redolist;		// Stores redo states
	long	m_growsize;		// Adjust for faster saves
	long	m_undoLevels;	// Requested Undolevels 
	long	m_chkpt;
	bool m_bSerializingFromCUndo;	// MCH 15.01.03: serialization is done for undo/redo purpose

	void AddUndo(CMemFile*);
	void AddRedo(CMemFile *pFile); 
	void Load(CMemFile*);
	void Store(CMemFile*);
	void ClearRedoList();
	
public:

	// Here are the hooks into the CDocument class
	virtual void Serialize(CArchive& ar) = 0;
	virtual void DeleteContents() { }

	// User accessable functions
	CUndo(long undolevels = 32, long = 32768);	// Constructor
	~CUndo();			// Destructor
	bool CanUndo();		// Returns TRUE if can Undo
	bool CanRedo();		// Returns TRUE if can Redo
	void Undo();		// Restore next Undo state
	void Redo();		// Restore next Redo state				
	void CheckPt();		// Save current state 
	void EnableCheckPt();
	void DisableCheckPt();

	inline bool IsSerializingFromCUndo () { return m_bSerializingFromCUndo; }
};

// Constructor
inline CUndo::CUndo (long undolevels, long growsize) : 
	m_growsize(growsize), m_undoLevels(undolevels),
	m_chkpt(0)
{
	m_bSerializingFromCUndo = false;
} 

// Remove contents of the redo list
inline void CUndo::ClearRedoList()
{
	// Clear redo list
	POSITION pos = m_redolist.GetHeadPosition(); 
	CMemFile* nextFile = NULL;
	while (pos)
	{
		nextFile = (CMemFile*) m_redolist.GetNext (pos);
		delete nextFile;
	}
	m_redolist.RemoveAll ();
}

// Destructor
inline CUndo::~CUndo() 
{
	// Clear undo list
	POSITION pos = m_undolist.GetHeadPosition ();
	CMemFile* nextFile = NULL;
	while (pos)
	{
		nextFile = (CMemFile*) m_undolist.GetNext (pos);
		delete nextFile;
	}
	m_undolist.RemoveAll ();	

	// Clear redo list
	ClearRedoList ();
}

// Checks undo availability, may be used to enable menus
inline bool CUndo::CanUndo() 
{
	return (m_undolist.GetCount () > 1);
}

// Checks redo availability, may be used to enable menus
inline bool CUndo::CanRedo() 
{
	return (m_redolist.GetCount () > 0);
}

// Adds state to the beginning of undo list		
inline void CUndo::AddUndo(CMemFile* file) 
{
	// Remove old state if there are more than max allowed
	if (m_undolist.GetCount() > m_undoLevels)
	{
		CMemFile* pFile = (CMemFile*) m_undolist.RemoveTail ();
		delete pFile;
	}
	// Add new state to head of undo list
	m_undolist.AddHead (file);
}

// Saves current object into CMemFile instance
inline void CUndo::Store (CMemFile* file) 
{
	file->SeekToBegin ();
	CArchive ar (file, CArchive::store);
//	ar.m_pDocument = reinterpret_cast<CDocument*> (this);	// MCH 15.01.03
	// done in CCdCoverCreator2Doc::Serialize	(MCH 25.07.04)

	m_bSerializingFromCUndo = true;
	Serialize (ar); 
	m_bSerializingFromCUndo = false;

	ar.Close();
}

// Loads CMemfile instance to current object
inline void CUndo::Load (CMemFile* file) 
{
	DeleteContents ();
	file->SeekToBegin ();
	CArchive ar (file, CArchive::load);
//	ar.m_pDocument = reinterpret_cast<CDocument*> (this);	// MCH 15.01.03

	m_bSerializingFromCUndo = true;
	Serialize (ar); 
	m_bSerializingFromCUndo = false;

	ar.Close ();
}

// Save current object state to Undo list
inline void CUndo::CheckPt ()
{
	if (m_chkpt <= 0)
	{
//Beep (440, 50);
		CMemFile* file = new CMemFile (m_growsize);
		Store (file);
		AddUndo (file);
		ClearRedoList ();
	}
}

inline void CUndo::EnableCheckPt()
{
	if (m_chkpt > 0)
		m_chkpt--;
}

inline void CUndo::DisableCheckPt()
{
	m_chkpt++;
}

// Place CMemFile instnace on Redo list
inline void CUndo::AddRedo(CMemFile *file) 
{
	// Move state to head of redo list
	m_redolist.AddHead (file);
}

// Perform an Undo command
inline void CUndo::Undo() 
{
	if (CanUndo ())
	{
		// Remember that the head of the undo list
		// is the current state. So we just move that
		// to the Redo list and load then previous state.
		CMemFile *pFile = (CMemFile *) m_undolist.GetHead ();
		m_undolist.RemoveHead ();
		AddRedo (pFile);
		pFile = (CMemFile*) m_undolist.GetHead ();
		Load (pFile);
	}
}

//Perform a Redo Command
inline void CUndo::Redo ()
{
	if (CanRedo ())
	{
		CMemFile *pFile = (CMemFile*) m_redolist.GetHead ();
		m_redolist.RemoveHead ();
		AddUndo (pFile);
		Load (pFile);
	}
}

#endif
