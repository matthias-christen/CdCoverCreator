// EditableListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CdCoverCreator2.h"
#include "EditableListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CELCEdit

CELCEdit::CELCEdit()
{
}

CELCEdit::~CELCEdit()
{
}


BEGIN_MESSAGE_MAP(CELCEdit, CEdit)
	//{{AFX_MSG_MAP(CELCEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CELCEdit message handlers

void CELCEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEditableListCtrl* pCtrl = (CEditableListCtrl*) GetParent ();

	switch (nChar)
	{
	case '\t':
		if (::GetKeyState (VK_SHIFT) & 0x8000)
			pCtrl->MoveEdit (0, -1);
		else
			pCtrl->MoveEdit (0, 1);
		return;

	case '\r':
	case VK_DOWN:
		pCtrl->MoveEdit (1, 0);
		return;

	case VK_UP:
		pCtrl->MoveEdit (-1, 0);
		return;

	case VK_LEFT:
		{
			int nStart, nEnd;
			GetSel (nStart, nEnd);

			if ((nStart == 0) && (nEnd == 0))
			{
				pCtrl->MoveEdit (0, -1);
				return;
			}
		}
		break;
	case VK_RIGHT:
		{
			int nStart, nEnd;
			GetSel (nStart, nEnd);

			CString s;
			GetWindowText (s);

			if ((nStart == s.GetLength ()) && (nEnd == nStart))
			{
				pCtrl->MoveEdit (0, 1);
				return;
			}
		}
		break;

	case VK_NEXT:
	case VK_PRIOR:
	case VK_HOME:
	case VK_END:
	case VK_INSERT:
	case VK_SHIFT:
	case VK_CONTROL:
	case VK_MENU:
		break;

	default:
		break;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CELCEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// eat tabs
	if ((nChar == '\t') || (nChar == '\r'))
		return;

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CELCEdit::OnEditCopy() 
{
	Copy ();
}

void CELCEdit::OnEditCut() 
{
	Cut ();
}

void CELCEdit::OnEditPaste() 
{
	Paste ();
}

void CELCEdit::OnEditUndo() 
{
	Undo ();
}

void CELCEdit::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	int nStart, nEnd;
	GetSel (nStart, nEnd);
	pCmdUI->Enable (nStart != nEnd);
}

void CELCEdit::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	int nStart, nEnd;
	GetSel (nStart, nEnd);
	pCmdUI->Enable (nStart != nEnd);
}

void CELCEdit::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (::IsClipboardFormatAvailable (CF_TEXT));
}

void CELCEdit::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable (CanUndo ());	
}

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl

CEditableListCtrl::CEditableListCtrl()
{
	m_bIsFixedRows = false;
	m_bIsDragging = false;
}

CEditableListCtrl::~CEditableListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditableListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEditableListCtrl)
	ON_WM_TIMER()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditableListCtrl message handlers

void CEditableListCtrl::SaveEdit()
{
	if ((m_nCurEditY >= 0) && (m_nCurEditX >= 0))
	{
		CString strText;
		GetEdit ().GetWindowText (strText);
		SetItemText (m_nCurEditY, m_nCurEditX, strText);
	}
}

void CEditableListCtrl::MoveEdit(int nUp, int nRight)
{
	// save edit content
	SaveEdit ();

	// move
	if (nRight != 0)
		m_nCurEditX = GetNextEditableCol (m_nCurEditX, nRight == 1);
	if (nUp != 0)
		m_nCurEditY = GetNextRow (m_nCurEditY, nUp == 1);

	// ensure that item is visible
	EnsureVisible (m_nCurEditY, false);
	CRect r, rc;
	GetSubItemRect (m_nCurEditY, m_nCurEditX, LVIR_LABEL, r);
	GetClientRect (&rc);
	if (r.right > rc.right)
		Scroll (CSize (r.right - rc.right + 5, 0));
	if (r.left < rc.left)
		Scroll (CSize (r.left - rc.left - 5, 0));

	// place edit and set text
	GetSubItemRect (m_nCurEditY, m_nCurEditX, LVIR_LABEL, r);

	GetEdit ().ShowWindow (SW_SHOW);
	GetEdit ().SetWindowPos (NULL, r.left, r.top, r.Width (), r.Height (), SWP_NOZORDER);
	GetEdit ().SetWindowText (GetItemText (m_nCurEditY, m_nCurEditX));
	GetEdit ().SetSel (0, -1);

	if (GetFocus () == this)
		GetEdit ().SetFocus ();
}

void CEditableListCtrl::OnTimer(UINT nIDEvent) 
{
	KillTimer (1);
	SaveEdit ();
	MoveEdit (0, 0);	

	CListCtrl::OnTimer(nIDEvent);
}

BOOL CEditableListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= LVS_REPORT;
	return CListCtrl::PreCreateWindow(cs);
}

void CEditableListCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CListCtrl::OnSetFocus(pOldWnd);
	SetTimer (0, 200, NULL);
}


CELCEdit& CEditableListCtrl::GetEdit()
{
	if (!::IsWindow (m_Edit.GetSafeHwnd ()))
	{
		CRect r (0, 0, 10, 10);
		m_Edit.Create (WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL | ES_LEFT | ES_NOHIDESEL, r, this, 100);
		m_Edit.SetFont (GetFont ());
		m_Edit.SetParent (this);
		m_Edit.SetMargins (6, 0);

		m_nCurEditX = GetNextEditableCol (-1);
		m_nCurEditY = GetNextRow (-1);
	}

	return m_Edit;
}

void CEditableListCtrl::SetFixedRows(bool bIsFixedRows)
{
	m_bIsFixedRows = bIsFixedRows;
}

void CEditableListCtrl::SetFixedCol(int nCol)
{
	m_mapFixedCol.SetAt (nCol, NULL);
}

int CEditableListCtrl::GetNextEditableCol(int nCol, bool bForeward)
{
	int nCnt = GetHeaderCtrl ()->GetItemCount ();
	if (nCol >= nCnt)
		return nCnt - 1;

	int* pAny = new int;
	int i = nCol;
	do
	{
		i += bForeward ? 1 : -1;
	} while (m_mapFixedCol.Lookup (i, (void*&) pAny));
	delete pAny;

	if (i >= nCnt)
		return nCnt - 1;// maximum columns
	if (i < 0)
		return nCol;	// old column: was already first editable column

	return i;
}

int CEditableListCtrl::GetNextRow(int nRow, bool bForeward)
{
	int nNewRow;

	int i = nRow;
	i += bForeward ? 1 : -1;

	int nCnt = GetItemCount ();

	if (i >= nCnt)
	{
		if (m_bIsFixedRows)
			nNewRow = max (nCnt - 1, 0);
		else
			nNewRow = InsertItem (nCnt, "", m_nDefaultIcon);
	}
	else if (i < 0)
		nNewRow = 0;
	else
		nNewRow = i;

	SetItemState (nNewRow, LVIS_SELECTED, -1);
	return nNewRow;
}

void CEditableListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SaveEdit ();

	LVHITTESTINFO htinfo;
	htinfo.pt = point;

	SetEditPos (htinfo.iSubItem - 1, SubItemHitTest (&htinfo) - 1);
	
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CEditableListCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CListCtrl::OnKillFocus(pNewWnd);
	SaveEdit ();	
}

void CEditableListCtrl::SetEditPos(int x, int y)
{
	m_nCurEditY = GetNextRow (y);
	m_nCurEditX = GetNextEditableCol (x);

	CRect r;
	GetSubItemRect (m_nCurEditY, m_nCurEditX, LVIR_LABEL, r);

	GetEdit ().ShowWindow (SW_SHOW);
	GetEdit ().SetWindowPos (NULL, r.left, r.top, r.Width (), r.Height (), SWP_NOZORDER);
	GetEdit ().SetWindowText (GetItemText (m_nCurEditY, m_nCurEditX));
	GetEdit ().SetSel (0, -1);
	GetEdit ().SetFocus ();
}

CPoint CEditableListCtrl::GetEditPos()
{
	return CPoint (m_nCurEditX, m_nCurEditY);
}

void CEditableListCtrl::SetEditText(CString str)
{
	GetEdit ().SetWindowText (str);
}

void CEditableListCtrl::BeginDrag(NMLISTVIEW* pNmlv)
{
	m_nDragItem = pNmlv->iItem;
	// Tell the tree view control to create an image to use for dragging.
	m_pDragImageList = CreateDragImage (m_nDragItem, &pNmlv->ptAction);

	// Start the drag operation.
	m_pDragImageList->BeginDrag (0, CPoint (0, 0));
	m_pDragImageList->DragEnter (this, pNmlv->ptAction);

	m_bIsDragging = true;
}

void CEditableListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bIsDragging)
	{
		// Drag the item to the current position of the mouse pointer.
		m_pDragImageList->DragMove (point);

		// Find out if the pointer is on the item. If it is, 
		// highlight the item as a drop target. 
		
		/*
		LVHITTESTINFO ht;
		ht.pt = point;
		int nTarget;

		if ((nTarget = HitTest (&ht)) != NULL)
			SetItemState (nTarget, LVIS_SELECTED, -1);
		*/
	}

	CListCtrl::OnMouseMove(nFlags, point);
}

void CEditableListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bIsDragging)
	{
		m_pDragImageList->DragLeave (this);
		m_pDragImageList->EndDrag ();
		delete m_pDragImageList;

		m_bIsDragging = false;

		LVHITTESTINFO ht;
		ht.pt = point;
		int nTarget = HitTest (&ht);
		if (nTarget >= 0)
			GetParent ()->SendMessage (LVN_ENDDRAG, MAKELONG (m_nDragItem, nTarget), 0);
	}
	
	CListCtrl::OnLButtonUp(nFlags, point);
}

void CEditableListCtrl::SetDefaultIcon(int nDefaultIcon)
{
	m_nDefaultIcon = nDefaultIcon;
}
