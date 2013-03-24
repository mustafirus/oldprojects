#pragma once
#define WM_IDLEUPDATECMDUI  0x0363  // wParam == bDisableIfNoHandler
#define DISPATCH									\
	switch(message)									\
	{

#define ON_MSG(message, fn)							\
	case (message):									\
		return ON_##message((wParam), (lParam), (fn));

#define ROUGHT(fn)									\
	default:										\
		return fn(message, wParam, lParam);			\
	}

#define BASEROUGHT ROUGHT(BASECLASS::WindowProc)

//#include <windowsx.h>
/* bool OnCreate(LPCREATESTRUCT lpCreateStruct) */
#define ON_WM_CREATE(wParam, lParam, fn) \
	((fn)((LPCREATESTRUCT) lParam) ? 0L : -1)

/* void OnClose() */
#define ON_WM_CLOSE(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void OnDestroy() */
#define ON_WM_DESTROY(wParam, lParam, fn) \
    ((fn)(), 0L)

#define ON_WM_NCDESTROY(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void OnCommand(UINT id, HWND hwndCtl, UINT codeNotify) */
#define ON_WM_COMMAND(wParam, lParam, fn) \
	((fn)((int)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam)) ? 0L : DefWindowProc(WM_COMMAND, wParam, lParam))

/* void OnSysCommand(UINT cmd, int x, int y) */
#define ON_WM_SYSCOMMAND(wParam, lParam, fn) \
    ((fn)((UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L)

/* void OnParentNotify(UINT msg, HWND hwndChild, int idChild) */
#define ON_WM_PARENTNOTIFY(wParam, lParam, fn) \
    ((fn)((UINT)LOWORD(wParam), (HWND)(lParam), (UINT)HIWORD(wParam)), 0L)

/* void OnInitMenuPopup(HMENU hMenu, UINT item, BOOL fSystemMenu) */
#define ON_WM_INITMENUPOPUP(wParam, lParam, fn) \
    ((fn)((HMENU)(wParam), (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam)), 0L)

/* void OnSize(UINT nType, int cx, int cy) */
#define ON_WM_SIZE(wParam, lParam, fn) \
    ((fn)((UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L)

/* void OnUpdateUI() */
#define ON_WM_IDLEUPDATECMDUI(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags) */
#define ON_WM_MENUSELECT(wParam, lParam, fn)                  \
    ((fn)((HMENU)(lParam),  \
    (int)(LOWORD(wParam)),          \
    (HIWORD(wParam) & MF_POPUP) ? GetSubMenu((HMENU)lParam, LOWORD(wParam)) : 0L, \
    (UINT)(((short)HIWORD(wParam) == -1) ? 0xFFFFFFFF : HIWORD(wParam))), 0L)
///////////////

/* void OnPaint() */
#define ON_WM_PAINT(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void OnNCPaint(HRGN hrgn) */
#define ON_WM_NCPAINT(wParam, lParam, fn) \
    ((fn)((HRGN)(wParam)), 0L)

/* void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags) */
#define ON_WM_KEYDOWN(wParam, lParam, fn) \
    ((fn)((UINT)(wParam), true, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L)

/* void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags) */
#define ON_WM_KEYUP(wParam, lParam, fn) \
    ((fn)((UINT)(wParam), false, (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L)

/* void OnChar(TCHAR ch, int cRepeat) */
#define ON_WM_CHAR(wParam, lParam, fn) \
    ((fn)((TCHAR)(wParam), (int)(short)LOWORD(lParam)), 0L)

/* void OnLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags) */
#define ON_WM_LBUTTONDOWN(wParam, lParam, fn) \
    ((fn)(false, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void OnLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags) */
#define ON_WM_LBUTTONDBLCLK(wParam, lParam, fn) \
    ((fn)(true, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void OnLButtonUp(int x, int y, UINT keyFlags) */
#define ON_WM_LBUTTONUP(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void OnMouseMove(int x, int y, UINT keyFlags) */
#define ON_WM_MOUSEMOVE(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* int OnMouseActivate(HWND hwndTopLevel, UINT codeHitTest, UINT msg) */
#define ON_WM_MOUSEACTIVATE(wParam, lParam, fn) \
    (LRESULT)(DWORD)(int)(fn)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam))

/* NOTE: Usable only by MDI client windows */
/* void MDIActivate(bool fActive, HWND hwndActivate, HWND hwndDeactivate) */
#define ON_WM_MDIACTIVATE(wParam, lParam, fn) \
    ((fn)((bool)(lParam == (LPARAM)(HWND)*this), (HWND)(lParam), (HWND)(wParam)), 0L)

/* void OnEnable(HWND bool fEnable) */
#define ON_WM_ENABLE(wParam, lParam, fn) \
    ((fn)((bool)(wParam > 0)), 0L)

/* HBRUSH OnCtlColor(HDC hdc, HWND hwndChild, int type) */
#define ON_WM_CTLCOLOREDIT(wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_EDIT)

#define ON_WM_CTLCOLOREDIT_REFLECT(wParam, lParam, fn) \
    (LRESULT)SendMessage(fn)((HWND)(lParam), WM_CTLCOLOREDIT + WM_REFLECT_BASE, (HDC)(wParam), (HWND)(lParam))

/*
#define HANDLE_WM_CTLCOLORSTATIC(hwnd, wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(HBRUSH)(fn)((hwnd), (HDC)(wParam), (HWND)(lParam), CTLCOLOR_STATIC)
#define HANDLE_WM_CTLCOLOREDIT(hwnd, wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(HBRUSH)(fn)((hwnd), (HDC)(wParam), (HWND)(lParam), CTLCOLOR_EDIT)

#define HANDLE_WM_CTLCOLORLISTBOX(hwnd, wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(HBRUSH)(fn)((hwnd), (HDC)(wParam), (HWND)(lParam), CTLCOLOR_LISTBOX)

#define HANDLE_WM_CTLCOLORBTN(hwnd, wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(HBRUSH)(fn)((hwnd), (HDC)(wParam), (HWND)(lParam), CTLCOLOR_BTN)

#define HANDLE_WM_CTLCOLORDLG(hwnd, wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(HBRUSH)(fn)((hwnd), (HDC)(wParam), (HWND)(lParam), CTLCOLOR_DLG)

#define HANDLE_WM_CTLCOLORSCROLLBAR(hwnd, wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(HBRUSH)(fn)((hwnd), (HDC)(wParam), (HWND)(lParam), CTLCOLOR_SCROLLBAR)
*/

/* bool OnEraseBkgnd(HDC hdc) */
#define ON_WM_ERASEBKGND(wParam, lParam, fn) \
    (LRESULT)(DWORD)(bool)(fn)((HDC)(wParam))

/* void OnKillFocus(HWND hwndNewFocus) */
#define ON_WM_KILLFOCUS(wParam, lParam, fn) \
    ((fn)((HWND)(wParam)), 0L)

/* void OnSetFocus(HWND hwndOldFocus) */
#define ON_WM_SETFOCUS(wParam, lParam, fn) \
    ((fn)((HWND)(wParam)), 0L)

/* UINT OnNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp) */
#define ON_WM_NCCALCSIZE(wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(fn)((BOOL)(wParam), (NCCALCSIZE_PARAMS *)(lParam))

/* bool OnWindowPosChanging(LPWINDOWPOS lpwpos) */
#define ON_WM_WINDOWPOSCHANGING(wParam, lParam, fn) \
    (LRESULT)(DWORD)(BOOL)(fn)((LPWINDOWPOS)(lParam))

/* void OnWindowPosChanged(const LPWINDOWPOS lpwpos) */
#define ON_WM_WINDOWPOSCHANGED(wParam, lParam, fn) \
    ((fn)((const LPWINDOWPOS)(lParam)), 0L)

/* void OnTimer(UINT id) */
#define ON_WM_TIMER(wParam, lParam, fn) \
    ((fn)((UINT)(wParam)), 0L)

/* bool OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg) */
#define ON_WM_SETCURSOR(wParam, lParam, fn) \
    (LRESULT)(DWORD)( (bool)(fn)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)) )

/* void OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo) */
#define ON_WM_GETMINMAXINFO(wParam, lParam, fn) \
    ((fn)((LPMINMAXINFO)(lParam)), 0L)

/* void OnContextMenu(HWND hwndContext, UINT xPos, UINT yPos) */
#define ON_WM_CONTEXTMENU(wParam, lParam, fn) \
    ((fn)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L)

/* void OnCopy() */
#define ON_WM_COPY(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void OnPaste() */
#define ON_WM_PASTE(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void OnDestroyClipboard() */
#define ON_WM_DESTROYCLIPBOARD(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void OnHScroll(HWND hwndCtl, UINT code, int pos) */
#define ON_WM_HSCROLL(wParam, lParam, fn) \
    ((fn)((HWND)(lParam), (UINT)(LOWORD(wParam)), (int)(short)HIWORD(wParam)), 0L)

/* void OnVScroll(HWND hwndCtl, UINT code, int pos) */
#define ON_WM_VSCROLL(wParam, lParam, fn) \
    ((fn)((HWND)(lParam), (UINT)(LOWORD(wParam)),  (int)(short)HIWORD(wParam)), 0L)






/*
#define WM_REFLECT_BASE 0xBC00
#define ON_MSG_REFLECT(message, fn)					\
	case (message):									\
	{												\
	handled = true;									\
	lRes =											\
	ON_##message_REFLECT((wParam), (lParam), (fn));	\
	break;											\
	}

#define DISPATCH_MORE(fn)							\
	}												\
	if(!handled)									\
		lRes  = fn(message, wParam, lParam);		\
	switch(message)									\
	{

#define ON_MSG_MORE(message, fn)					\
	case (message):									\
	{												\
		handled = true;								\
		LRESULT lResP = 0;							\
		lResP = ON_##message((wParam),				\
					(lParam), (fn));				\
		if(handled)									\
			return lResP;							\
		else										\
			return lRes;							\
	}

#define END_DISPATCH_MORE							\
	}												\
	return lRes;
#define BASEDISP_MORE DISPATCH_MORE(BASECLASS::WindowProc)
*/
/*#ifndef __AFXPRIV_H__
#define WM_SIZEPARENT       0x0361  // lParam = &AFX_SIZEPARENTPARAMS
#define WM_INITIALUPDATE    0x0364  // (params unused) - sent to children
#define WM_MOUSEENTER	    0x0365  // wParam == bEnter- sent to children
struct X_SIZEPARENTPARAMS
{
	HDWP hDWP;       // handle for DeferWindowPos
	RECT rect;       // parent client rectangle (trim as appropriate)
	SIZE sizeTotal;  // total size on each side as layout proceeds
	BOOL bStretch;   // should stretch to fill all space
};
#endif

#define ON_MSG_TO_CHILD(message, first, last)		\
	case (message):									\
	{												\
	lRes = 	(SendMessageToDescendants((message),	\
	 (wParam), (lParam), (first), (last)), 0L);		\
	break;											\
	}

*/