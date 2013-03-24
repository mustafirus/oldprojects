// EditControl.cpp: implementation of the EditControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "WinThread.h"
#include "FrameWnd.h"
#include "EditControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BASECLASS	Edit
//#define BORDER		3
BEGIN_COMMAND_MAP(EditControl, BASECLASS)
END_COMMAND_MAP
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RECT EditControl::border = {3,1,3,2};
EditControl::EditControl(Wnd* pWndParent, DWORD dwStyle, UINT nID) :
Edit(0, dwStyle|WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL, 0,0,0,0, pWndParent, nID),
edit(false), focused(false), hilighted(false)
{

}

EditControl::~EditControl()
{

}

LRESULT EditControl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_NCCALCSIZE, OnNCCalcSize)
		ON_MSG(WM_NCPAINT, OnNCPaint)
		ON_MSG(WM_KILLFOCUS, OnKillFocus)
		ON_MSG(WM_LBUTTONDOWN, OnLButtonDown)
		ON_MSG(WM_KEYDOWN, OnKey)
		ON_MSG(WM_CHAR, OnChar)
//		ON_MSG(WM_MOUSEMOVE, OnMouseMove);
		ON_MSG(WM_PAINT, OnPaint)
		ON_MSG(WM_SETFOCUS, OnSetFocus)
	BASEROUGHT
}

void EditControl::OnChar(TCHAR ch, int cRepeat)
{
	switch(ch)
	{
	case VK_RETURN:
	case VK_ESCAPE:
		return;
	}
	Default();
}

UINT EditControl::OnNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp)
{
	RECT& r = *(RECT*)lpcsp;
	r.left += border.left;
	r.top += border.top;
	r.right -= border.right;
	r.bottom -= border.bottom;
	return Default();
}

void EditControl::OnNCPaint(HRGN hrgn)
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

void EditControl::HiliteWindow(bool hi)
{
	if(focused)
	{
		hilighted = false;
		return;
	}
	hilighted = hi;
	Invalidate();
}

void EditControl::OnKillFocus(HWND hwndNewFocus)
{
	Default();
	focused = false;
	Invalidate();
}

void EditControl::OnSetFocus(HWND hwndOldFocus)
{
	Default();
	focused = true;
	Invalidate();
	HideCaret(*this);
}

void EditControl::OnLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags)
{
	if(edit)
	{
		Default();
		return;
	}
	if(GetFocus() != this)
	{
		((FrameWnd*)GetParent())->SetPos(pos);
		SetFocus();
	}
}

void EditControl::Invalidate()
{
		RECT r;
		Wnd* pWnd = GetParent();
		GetWindowRect(*this, &r);
		pWnd->ScreenToClient(&r);
		pWnd->InvalidateRect(&r);
}

void EditControl::SetEditMode(bool m)
{
	if(edit=m)
	{
		ShowCaret(*this);
		Invalidate();
	}else
	{
		DefWindowProc(EM_SETSEL);
		HideCaret(*this);
		Invalidate();
	}
}

void EditControl::OnPaint() 
{
/*	Default();
	HDC hDC = ::GetDC(*this);
	RECT r;
	GetClientRect(*this, &r);
	if(focused)
	{
		SelectObject(hDC, ::GetStockObject(NULL_BRUSH));
		Rectangle(hDC, r.left, r.top, r.right, r.bottom);
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
*/	Default();
}

void EditControl::OnMouseMove(int x, int y, UINT keyFlags)
{
/*	if(GetCapture() == *this)
	{
		POINT pt = {x,y};
		RECT r;
		GetClientRect(*this, &r);
		if(PtInRect(&r, pt))
			return;
		else
		{
			Hilight(false);
			::ReleaseCapture();
		}
	}else
	{
		POINT pt = {x,y};
		RECT r;
		GetClientRect(*this, &r);
		if(PtInRect(&r, pt))
		{
			Hilight(true);
			SetCapture();
		}
		else
			return;
	}
*/
}

void EditControl::OnKey(UINT vk, bool fDown, int cRepeat, UINT flags)
{
	switch(vk)
	{
	case VK_LEFT:
	case VK_RIGHT:
	case VK_INSERT:
		if(edit)
			goto ddf;
	case VK_PRIOR:
	case VK_NEXT:
	case VK_END:
	case VK_TAB:
	case VK_HOME:
	case VK_UP:
	case VK_DOWN:
	case VK_SELECT:
	case VK_PRINT:
	case VK_EXECUTE:
	case VK_SNAPSHOT:
		ResendMessage(::GetParent(*this));
ddf:default:
		Default();
	}
}

HBRUSH EditControl::OnCtlColorReflect(HDC hdc, int type)
{
	if(edit)
		return NULL;
	else
	{
		SetBkMode(hdc, TRANSPARENT);
		return ::GetStockObject(HOLLOW_BRUSH);
	}
}

void EditControl::SetTabs(TabMap* pTab, TabPos first, TabPos& last)
{
	last = pos = first;
	pTab->SetAt(pos, this);
}

void EditControl::SetPos(POINT pt, SIZE sz, HDWP hdwp)
{
	if(hdwp)
		hdwp = DeferWindowPos(hdwp, *this, NULL, pt.x, pt.y,
		sz.cx, sz.cy, SWP_NOZORDER);
	else
		::SetWindowPos(*this, NULL, pt.x, pt.y,
		sz.cx, sz.cy, SWP_NOZORDER);
}
