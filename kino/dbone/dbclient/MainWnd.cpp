// MainWnd.cpp: implementation of the MainWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "indicate.h"
#include "FrameThread.h"
#include "DataContext.h"
#include "StatusBar.h"
#include "FrameWnd.h"
#include "MainWnd.h"

#define BASECLASS MDIMain
static UINT ind[4] = {(UINT)ID_INDICATOR_TEXT,
		(UINT)ID_INDICATOR_CAPS, (UINT)ID_INDICATOR_NUM, 
		(UINT)ID_INDICATOR_SCRL};
static UINT icount = sizeof ind / sizeof(UINT);



void MainWnd::onFileNew(){
//	Wnd* pWnd = new MDIChild(NULL, "test");
//	pWnd->show();
	new FrameThread(new TestDC);
}
void MainWnd::onCloseAll() {
	while(FrameThread::pHead) delete FrameThread::pHead; 
}

/*void MainWnd::CreateThread(RKey* prk)
{
	new FrameThread(prk, pThread);
}
*/

MainWnd::MainWnd(Menu* pm) : MDIMain("DBCLIENT", pm ), pThread(NULL), brk(false)
{
	psb = new StatusBar(this, ind, icount, 11);
}

MainWnd::~MainWnd()
{
}

LRESULT MainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
//		ON_MSG(WM_ACTIVATE, onSetCursor)
		ON_MSG(WM_SETCURSOR, onSetCursor)
		ON_MSG(WM_MENUSELECT, onMenuSelect)
	BASEROUGHT 
}

void MainWnd::onMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	psb->onMenuSelect( hmenu, item, hmenuPopup, flags);
}

bool MainWnd::onSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg)
{
	if(hwndCursor != hWndClient)
		return Default() > 0;
	bool top;
	if(msg == WM_LBUTTONDOWN)
		top = true;
	else if(msg == WM_LBUTTONUP)
		top = false;
	else
		return false;
	POINT pt;
	GetCursorPos(&pt);
	::ScreenToClient(hWndClient, &pt);
	HWND hwnd = ChildWindowFromPointEx(hWndClient, pt, CWP_SKIPINVISIBLE);
	HWND hwndActive = (HWND)::SendMessage(hWndClient, WM_MDIGETACTIVE, 0, 0);
	if(hwnd && hwnd != hWndClient && hwnd != hwndActive)
	{
		FrameWnd* pWnd = (FrameWnd*)Wnd::FromHandle(hwnd);
//		if(pWnd)
//			pWnd->BringToTop(top);
		return true;
	}
	return Default() > 0;
}

void MainWnd::Wait(bool w /* = true*/)
{
	psb->SetPaneText(0, w ? ID_WAIT_MESSAGE : ID_READY_MESSAGE, true);
}

