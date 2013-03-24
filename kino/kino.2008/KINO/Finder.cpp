// Finder.cpp : implementation file
//

#include "stdx.h"
#include "dblib.h"
#include "wndlib.h"
#include "Resource.h"
#include "KinoRes.h"
#include "MainWnd.h"
#include "Finder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef BASECLASS
#undef BASECLASS
#endif
#define BASECLASS Edit
/////////////////////////////////////////////////////////////////////////////
// Finder
Finder::Finder(Wnd* pParentWnd, RECT* r, RSWhere* prsw, Finder** ppf) :
Edit(0, ES_AUTOHSCROLL|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_BORDER, r, pParentWnd, 0),
pRSWhere(prsw), ppFinder(ppf)
{
	DefWindowProc(EM_SETLIMITTEXT, 255);
	const char* s1 = *pRSWhere;
	if(s1)
	{
		SetWindowText(s1);
		DefWindowProc(EM_SETSEL, 0, -1);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Finder message handlers

LRESULT Finder::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_KEYDOWN, OnKey)
		ON_MSG(WM_CHAR, OnChar)
	BASEROUGHT 

}

void Finder::OnChar(TCHAR ch, int cRepeat)
{
	switch(ch)
	{
	case VK_RETURN:
	case VK_ESCAPE:
	case VK_TAB:
		return;
	}
	Default();
}

void Finder::OnKey(UINT vk, bool fDown, int cRepeat, UINT flags)
{
	switch(vk)
	{
	case VK_RETURN:
	case VK_ESCAPE:
		Save(vk == VK_RETURN);
		DestroyWindow();
		return;
	case VK_TAB:
		::SetFocus(::GetParent(*this));
		return;
	case VK_UP:
	case VK_DOWN:
		ResendMessage(::GetParent(*this));
		return;
	}
	Default();
}

void Finder::Save(bool data)
{
	int len;
	char* s1 = NULL;
	if(data && (len = GetWindowTextLenght()))
	{
		s1 = new char[len+1];
		GetWindowText(s1, len+1);
	}else
		s1 = NULL;
	pRSWhere->Set(s1);
	ifdel(s1);
	::InvalidateRect(::GetParent(*this), NULL, TRUE);
	((MainWnd*)GetApp()->GetMainWnd())->brk = true;
}

/*void Finder::OnEditUI(CmdUI* pCmdUI)
{
	switch((UINT)*pCmdUI)
	{
	case ID_EDIT_CUT:
	case ID_EDIT_COPY:
	case ID_EDIT_PASTE:
	case ID_EDIT_UNDO:
	case ID_EDIT_CLEAR:
		pCmdUI->Enable();
		break;
	default:
		pCmdUI->Enable(FALSE);
	}
}
*/