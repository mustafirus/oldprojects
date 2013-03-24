// Edit.cpp: implementation of the Edit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "defines.h"
#include "Edit.h"


Edit::Edit(DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, Wnd* pWndParent, Menu*	pm) :
Wnd( "EDIT", dwExStyle, "", dwStyle, x,  y, nWidth, nHeight, pWndParent,
				 pm, NULL), edit(false), focused(false), hilighted(false)
{
}

Edit::Edit(DWORD dwExStyle, DWORD dwStyle, RECT* pr, Wnd* pWndParent, Menu*	pm) :
Wnd( "EDIT", dwExStyle, "", dwStyle, pr->left,  pr->top, pr->right - pr->left, pr->bottom - pr->top, pWndParent,
				 pm, NULL), edit(false), focused(false), hilighted(false)

{
}

LRESULT Edit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
//	switch(message){
//	case WM_INPUTLANGCHANGE:
//	case WM_INPUTLANGCHANGEREQUEST:
//		return ::DefWindowProc(*this, message, wParam, lParam);
//	}
	DISPATCH
		ON_MSG(WM_NCPAINT, onNCPaint)
		ON_MSG(WM_KILLFOCUS, onKillFocus)
		ON_MSG(WM_LBUTTONDBLCLK, onLButtonDown)
		ON_MSG(WM_LBUTTONDOWN, onLButtonDown)
		ON_MSG(WM_KEYDOWN, onKey)
		ON_MSG(WM_KEYUP, onKey)
		ON_MSG(WM_CHAR, onChar)
		ON_MSG(WM_SETFOCUS, onSetFocus)
//		NOTYFY(WM_INPUTLANGCHANGE)
	BASEROUGHT
}

void Edit::SetFont(HFONT hFont, bool fRedraw)
{
	DefWindowProc(WM_SETFONT, (WPARAM) hFont, MAKELPARAM(fRedraw, 0));
}

void Edit::SetEditMode(bool m)
{
	if(edit=m)
	{
		ShowCaret(*this);
//		DefWindowProc( WM_KEYDOWN, VK_END );
		DefWindowProc( WM_SETTEXT, 0, (LPARAM)"" );
 	}else
	{
		DefWindowProc(EM_SETSEL);
		HideCaret(*this);
	}
	invalidate_parent();
}

void Edit::onKey(UINT vk, bool fDown, int cRepeat, UINT flags) {

	switch(vk)
	{
	case VK_ESCAPE:
		if(fDown && edit) {
			onUndo();
			SetEditMode(false);
		}else{
			ResendMessage(::GetParent(*this));
		}
		return;
	case VK_RETURN:
		if(fDown){
			SetEditMode(!edit);
			if(edit) {
				int mod = 0;
				if(::GetKeyState(VK_CONTROL)&0x8000)
					mod &= KEY_CTRL;
				if(GetKeyState(VK_SHIFT)&0x8000)
					mod &= KEY_SHIFT;
				if(onBeginEdit(mod)){
					SetEditMode(true);
				}
			}else {
				if(onEndEdit()){
					SetEditMode(false);
				}
			}
		}
		return;
	case VK_DELETE:
		if(!edit) {
			if(fDown){
				DefWindowProc( EM_SETSEL, 0, -1 );
				DefWindowProc( WM_CLEAR );
				invalidate_parent();
			}
		}
		return;
	case VK_PRIOR:
	case VK_NEXT:
	case VK_TAB:
	case VK_UP:
	case VK_DOWN:
	case VK_SELECT:
	case VK_PRINT:
	case VK_EXECUTE:
	case VK_SNAPSHOT:
		ResendMessage(::GetParent(*this));
		return;
	}

	if(!edit){
		switch(vk){
			case VK_BACK:
			case VK_SPACE:
				if(onBeginEdit(0)){
					SetEditMode(true);
					Default();
				}
				return;
			default:
				if( (vk >= '0' && vk <= 'Z') ||
					(vk >= VK_OEM_1 && vk <= VK_OEM_8) ||
					(vk >= VK_NUMPAD0 && vk <= VK_DIVIDE) ){
						if(onBeginEdit(0)){
							SetEditMode(true);
							Default();
						}
						return;
					}
		}
		ResendMessage(::GetParent(*this));
		return;
	}
	Default();

//		if( (vk >= '0' && vk <= 'Z') || (vk >= VK_OEM_1 && vk <= VK_OEM_8)  || vk == VK_BACK || vk == VK_SPACE ) {
//			TCHAR lpString[1024];
//			int n = GetKeyNameText( MAKELONG(cRepeat, flags), lpString, 1024);
//			if (fDown){
//				TRACE2( "Key %s - %x\n", lpString, vk );
//			}
//		}
}

void Edit::onChar(TCHAR ch, int cRepeat) {
//	switch(ch)
//	{
//	case VK_RETURN:
//	case VK_ESCAPE:
//		return;
//	}
	Default();
}

void Edit::onKillFocus(HWND hwndNewFocus) {
	if(edit){
		SetEditMode(false);
		onEndEdit();
	}
	Default();
	focused = false;
	invalidate_parent();
}

void Edit::onLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags) {
	if(GetFocus() != this){
		SetFocus();
	}
	if(fDoubleClick){
		SetEditMode(true);
	}
	if(edit) {
		Default();
		return;
	}
}

void Edit::onSetFocus(HWND hwndOldFocus)
{
	Default();
	focused = true;
	invalidate_parent();
	HideCaret(*this);
}

void Edit::onNCPaint(HRGN hrgn)
{
	HDC hDC = ::GetWindowDC(*this);
	RECT r;
	GetWindowRect(*this, &r);
	OffsetRect(&r, -r.left, -r.top);
	//ScreenToClient(&r);
	if(focused)
	{
		POINT ppt1[] = {
			{r.left, r.bottom-1},
			{r.right-1, r.bottom-1},
			{r.right-1, r.top},
			{r.left, r.top},
			{r.left, r.bottom-1}};
		SelectObject(hDC, ::GetStockObject(BLACK_BRUSH));
//		SelectObject(hDC, ::GetStockObject(BLACK_PEN));
		SelectObject(hDC, ::CreatePen(PS_SOLID, 2, RGB(0,0,255)));
		Polyline( hDC, ppt1, _countof(ppt1)); 
//		Rectangle(hDC, r.left, r.top, r.right, r.bottom);
		DeleteObject(SelectObject(hDC, ::GetStockObject(BLACK_PEN)));
	}else if(!hilighted)
	{
		r.bottom -=1;
		r.right  -=1;
		POINT ppt1[3] = {{r.left, r.bottom}, {r.right, r.bottom},{r.right, r.top}};
		Polyline( hDC, ppt1, _countof(ppt1)); 
	}else
	{
		r.bottom -=1;
		r.right  -=1;
		POINT ppt1[] = {{r.left, r.bottom}, {r.right, r.bottom},{r.right, r.top}};
		POINT ppt2[] = {{r.left, r.bottom-1}, {r.right-1, r.bottom-1},{r.right-1, r.top}, {r.left, r.top}, {r.left, r.bottom-1}};
		POINT ppt3[] = {{r.left-1, r.bottom-2}, {r.right-2, r.bottom-2},{r.right-2, r.top-1}};
		SelectObject(hDC, ::GetStockObject(WHITE_PEN));
		Polyline( hDC, ppt3, _countof(ppt3)); 
		SelectObject(hDC, ::CreatePen(PS_SOLID, 0, RGB(150,150,150)));
		Polyline( hDC, ppt2, _countof(ppt2));
		DeleteObject(SelectObject(hDC, ::GetStockObject(BLACK_PEN)));
		Polyline( hDC, ppt1, _countof(ppt1)); 
		
	}
	ReleaseDC(*this, hDC);
}
