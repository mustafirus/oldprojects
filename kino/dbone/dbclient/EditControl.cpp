// EditControl.cpp: implementation of the EditControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinThread.h"
#include "defines.h"
#include "FrameWnd.h"
#include "EditControl.h"


RECT EditControl::border = {3,1,3,2};


EditControl::EditControl(Wnd* pWndParent, DWORD dwStyle, Menu*	pm) :
Edit(0, dwStyle|WS_VISIBLE|WS_CHILD|ES_AUTOHSCROLL, 0,0,0,0, pWndParent, pm)
{

}

EditControl::~EditControl()
{

}

LRESULT EditControl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_NCCALCSIZE, onNCCalcSize)
	BASEROUGHT
}


UINT EditControl::onNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp)
{
	RECT& r = *(RECT*)lpcsp;
	r.left += border.left;
	r.top += border.top;
	r.right -= border.right;
	r.bottom -= border.bottom;
	return (UINT) Default();
}


void EditControl::HiliteWindow(bool hi)
{
	if(focused)
	{
		hilighted = false;
		return;
	}
	hilighted = hi;
	invalidate_parent();
}


HBRUSH EditControl::onCtlColorReflect(HDC hdc, int type)
{
	if(edit)
		return NULL;
	else
	{
		SetBkMode(hdc, TRANSPARENT);
		return (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
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

void EditControl::onSetFocus(HWND hwndOldFocus)
{
	ASSERT(hwndOldFocus != *this);
	((FrameWnd*)GetParent())->SetPos(pos);
	BASECLASS::onSetFocus(hwndOldFocus);
}
