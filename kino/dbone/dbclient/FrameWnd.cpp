// FrameWnd.cpp : implementation file
//

#include "stdafx.h"

#include "MainWnd.h"

#include "DataContext.h"
#include "defines.h"
#include "Styles.h"
#include "Frame.h"
#include "FrameThread.h"
#include "Defines.h"
#include "TabMap.h"
#include "FDC.h"

#include "FrameWnd.h"

//#include "Grid.h"
//#include "RFieldControl.h"

#define MAXACCELS	10
#define BASECLASS MDIChild

// For new functionality

FrameWnd::FrameWnd(DataContext* pdc, FrameWnd* pPrev) : MDIChild("FRAMEWND", pdc->name()), _pdc(pdc),
next(NULL), prev(pPrev), hWndTracking(NULL), pRootFrame(NULL), curpos(TabMap::first_pos)
{
	if(prev)
		prev->next = this;
	if(next)
		next->prev = this;

	pdc->setWnd(this);
	pRootFrame = pdc->getFrame();
	ASSERT(pRootFrame);

	MINMAX mm;
	pRootFrame->GetMinMax(&mm);
	TabPos pos;

	_ASSERTE(_CrtCheckMemory());
	pRootFrame->SetTabs(&tabmap, TabMap::first_pos, pos);
	_ASSERTE(_CrtCheckMemory());
	
	POINT pt = {defspace.left, defspace.top};
	AdjustSize(mm.szDes);
	SetWindowPos(*this, NULL, 0,0, mm.szDes.cx, mm.szDes.cy, SWP_NOMOVE|
		SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW|SWP_NOSENDCHANGING|
		SWP_NOOWNERZORDER);

	if(prev)
		EnableWindow(*prev, FALSE);
	show();
}

void FrameWnd::AdjustSize(SIZE& sz)
{
	POINT pt = {-defspace.left, -defspace.top};
	sz.cx += defspace.left + defspace.right;
	sz.cy += defspace.top + defspace.bottom;
	Rect r(pt,sz);
	AdjustWindowRect(r, GetStyle(), FALSE);
	sz = r;
	RECT mc;
	GetClientRect(*(getMainWnd()), &mc);
	sz.cx = min(sz.cx, mc.right - 20);
	sz.cy = min(sz.cy, mc.bottom - 20);
}

FrameWnd::~FrameWnd()
{
/*  Не может следующее окно существовать без предыдущего
	if(prev)
		prev->next = next; 
	if(next)
		next->prev = prev;
*/
	if(prev)
		prev->next = NULL; 
	delete next;

//	if(getApp() == getApp()->getCurrent())
//	{
//		::SendMessage(::GetParent(*this), WM_MDIDESTROY, (WPARAM)(HWND)(*this), 0);
//	}else
//	{
//		::SendMessage(::GetParent(*this), WM_MDIDESTROY, (WPARAM)(HWND)(*this), 0);
////		return ::DestroyWindow(*this) > 0;
//	}
	delete pRootFrame;
	delete _pdc;

	Thread* p = Thread::getCurrent();
	FrameThread* pf = dynamic_cast<FrameThread*> (p);

	if(!pf)
		return;
	if(pf->getThreadWnd()== this){
		pf->clearThreadWnd();
		PostQuitMessage(0);
	}
}

LRESULT FrameWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	DISPATCH
		ON_MSG(WM_GETMINMAXINFO, onGetMinMaxInfo);
		ON_MSG(WM_MDIACTIVATE, onMDIActivate);
		ON_MSG(WM_WINDOWPOSCHANGED, onWindowPosChanged);
		ON_MSG(WM_SIZE, onSize);
		ON_MSG(WM_SETCURSOR, onSetCursor)
		ON_MSG(WM_SETFOCUS, onSetFocus)
		ON_MSG(WM_PAINT, onPaint);
		ON_MSG(WM_KEYDOWN, onKey);
		ON_MSG(WM_KEYUP, onKey);
	BASEROUGHT
}

void FrameWnd::onSetFocus(HWND hwndOldFocus)
{
	Default();
		Wnd* pWnd =	tabmap.GetAt(curpos);
		if(pWnd)
			pWnd->SetFocus();
}

void FrameWnd::onWindowPosChanged(const LPWINDOWPOS lpwpos)
{
	Default();
	bool nic;
	if(IsWindowVisible(*this))
	{
		if(nic = !IsIconic(*this))
		{
			ASSERT(pRootFrame);
			RECT r;
			GetClientRect(*this, &r);
			POINT pt = {defspace.left, defspace.top};
			SIZE& sz = *((SIZE*)((POINT*)&r)+1);//{r.right - defspace.left - defspace.right, lpwpos->cy - defspace.top - defspace.bottom};
			sz.cx = sz.cx  - defspace.left - defspace.right;
			sz.cy = sz.cy  - defspace.top - defspace.bottom;
			HDWP hdwp = BeginDeferWindowPos(10);
			pRootFrame->SetPos(pt, sz, hdwp);
			EndDeferWindowPos(hdwp);
		}
		ShowAll(nic);
	}
}

void FrameWnd::onSize(UINT nType, int cx, int cy)
{
	Default();
}


void FrameWnd::onGetMinMaxInfo(LPMINMAXINFO pmm)
{
	Default();
	if(pRootFrame)
	{
		MINMAX mm;
		POINT pt = {0,0};
		pRootFrame->GetMinMax(&mm);
		AdjustSize(mm.szMin);
		AdjustSize(mm.szMax);
		pmm->ptMinTrackSize = *(POINT*)&mm.szMin;
		pmm->ptMaxTrackSize = *(POINT*)&mm.szMax;
	}
}

bool FrameWnd::onSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg)
{
	if(hwndCursor == *this)
	{
		Wnd* pWnd = hWndTracking ? FromHandle(hWndTracking) : NULL;
		if(pWnd)
			pWnd->HiliteWindow(false);
		hWndTracking = NULL;
	}else
	{
	HWND hChild = hwndCursor;
	while(hwndCursor && (hwndCursor = ::GetParent(hwndCursor)) != *this)
		hChild = hwndCursor;
	if(hChild && hChild != hWndTracking)
	{
		Wnd* pWnd = hWndTracking ? FromHandle(hWndTracking) : NULL;
		if(pWnd)
			pWnd->HiliteWindow(false);
		hWndTracking = hChild;
		pWnd = hWndTracking ? FromHandle(hWndTracking) : NULL;
		if(pWnd)
			pWnd->HiliteWindow(true);
	}
	}
	return Default() > 0;
}

void FrameWnd::onMDIActivate(bool fActive, HWND hwndActivate, HWND hwndDeactivate)
{
	Default();
	if(fActive)
	{
		Wnd* pWnd =	tabmap.GetAt(curpos);
		if(pWnd)
			pWnd->SetFocus();
	}

	if(		( fActive && ((next && hwndDeactivate == *next) || (prev && hwndDeactivate == *prev))) ||
		(!fActive && ( (next && hwndActivate == *next) || (prev && hwndActivate == *prev)))		)
		return;
	ShowAll(fActive);
	HMENU hMenu = getMainWnd()->RefreshMenu();
}

void FrameWnd::ShowAll(bool sa_show)
{
	FrameWnd* pShow = prev;
	while(pShow)
	{
		if(sa_show)
			pShow->show();
		else
			pShow->hide();
		pShow = pShow->prev;
	}
}

void FrameWnd::onClose() 
{
	ASSERT(_pdc);
	if(_pdc->isModified())
	{
		if(bAutoSave)
			_pdc->save();
		else
		{
			switch(MessageBox(*this, "Сохранить изменения?", "Kino", MB_YESNOCANCEL|MB_ICONQUESTION))
			{
			case IDYES:
				_pdc->save();
				break;
			case IDCANCEL:
				return;
			}
		}
	}
	if(prev){
		::EnableWindow(*prev, TRUE);
		getMainWnd()->MDIActivate(prev);
	}
	destroy();
}

void FrameWnd::onPaint() 
{
//	PAINTSTRUCT paint;
//	HDC hdc = BeginPaint(*this, &paint);
	FDC dc(this);
	dc.SetBkMode(TRANSPARENT);
	pRootFrame->Draw(dc);
//	EndPaint(*this, &paint);
}

void FrameWnd::onKey(UINT nChar, bool fDown, int nRepCnt, UINT nFlags)
{
	TabDir dir ={0,0};
	switch(nChar)
	{
		case VK_HOME: break;
		case VK_UP: dir = TabMap::up; break;
		case VK_DOWN: dir = TabMap::down; break;
		case VK_LEFT: dir = TabMap::left; break;
		case VK_RIGHT: dir = TabMap::right; break;
		case VK_ESCAPE:
			if(fDown)
				close();
			return;
	}
	if(fDown){
		if(dir.r != 0 || dir.c != 0) {
			Wnd* pWnd =	tabmap.GetNext(curpos, dir);
			if(pWnd)
				pWnd->SetFocus();
			return;
		}
	}

	Default();
}

void FrameWnd::onLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags) {
	if(!fDoubleClick)
	if(keyFlags == MK_LBUTTON)
	{
		SetFocus();
	}
	Default();
}

void FrameWnd::onMouseMove(int x, int y, UINT keyFlags) {
}

//void FrameWnd::deleteIfTreadMain(FrameWnd* pfw) {
//	Thread* p = Thread::getCurrent();
//	FrameThread* pf = dynamic_cast<FrameThread*> (p);
//
//	if(!pf)
//		return;
//	if(pf->getThreadWnd()== pfw){
//		pf->clearThreadWnd();
//		PostQuitMessage(0);
//		delete pf;
//	}
//}

RECT FrameWnd::defspace = {5,5,5,5};


void FrameWnd::RegisterClass()
{
	Wnd::RegisterClass(CS_OWNDC|CS_DBLCLKS, 0, LoadCursor(NULL,IDC_ARROW),
		CreateSolidBrush(RGB(255,255,240)), 0, "FRAMEWND", 0);
}

void FrameWnd::BringToTop(bool top)
{
	HWND hwnd; 
	if(top)
		hwnd = HWND_TOP;
	else
	{
		hwnd = *next;
		ASSERT(hwnd);
	}
	SetWindowPos(*this, hwnd, -1,-1,-1,-1, 
		SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOSENDCHANGING);
}

bool FrameWnd::onCmdUI(CmdUI* pCmdUI)
{
	Wnd* pWnd = GetFocus();
	Wnd* pWndParent;
	if(IsChild(pWnd))
	{
		while(pWnd && (pWndParent = pWnd->GetParent()) != this)
			pWnd = pWndParent;
	}
		
	if(pWnd && pWnd->onCmdUI(pCmdUI))
		return true;
	return MDIChild::onCmdUI(pCmdUI);
}

bool FrameWnd::PreTranslateMessage(MSG* pMsg)
{
//	return (hAccel ? (::TranslateAccelerator(*this, hAccel, pMsg)>0) : false);
	return false;
}

