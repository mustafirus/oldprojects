// Edit.cpp: implementation of the Edit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Edit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_COMMAND_MAP(Edit, Wnd)
END_COMMAND_MAP

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Edit::Edit(DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, Wnd* pWndParent, UINT nID)
{
	Create(0, "", dwStyle, x, y, nWidth, nHeight, pWndParent,
				 (HMENU) nID);
}

Edit::Edit(DWORD dwExStyle, DWORD dwStyle, RECT* pr, Wnd* pWndParent, UINT nID)
{
	Create(0, "", dwStyle, pr->left, pr->top, pr->right - pr->left,
		pr->bottom - pr->top, pWndParent,(HMENU) nID);
}

void Edit::SetFont(HFONT hFont, bool fRedraw)
{
	DefWindowProc(WM_SETFONT, (WPARAM) hFont, MAKELPARAM(fRedraw, 0));
}

