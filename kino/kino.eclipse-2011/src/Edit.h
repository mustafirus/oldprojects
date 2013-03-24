// Edit.h: interface for the Edit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDIT_H__7D1A6E12_41BE_11D2_A71C_204C4F4F5020__INCLUDED_)
#define AFX_EDIT_H__7D1A6E12_41BE_11D2_A71C_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Wnd.h"

class Edit : public Wnd  
{
public:
	Edit(DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, Wnd* pWndParent, UINT nID);
	Edit(DWORD dwExStyle, DWORD dwStyle, RECT* prect, Wnd* pWndParent, UINT nID);
	virtual ~Edit(){};

	bool IsModified(){return DefWindowProc(EM_GETMODIFY, 0, 0) > 0;};
	void SetFont(HFONT hFont, bool fRedraw = true);

	virtual const char* GetClassName(){return "EDIT";};
	DECLARE_COMMAND_MAP()
};

#endif // !defined(AFX_EDIT_H__7D1A6E12_41BE_11D2_A71C_204C4F4F5020__INCLUDED_)
