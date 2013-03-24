// Wnd.cpp: implementation of the Wnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Application.h"
#include "CmdUI.h"
#include "Menu.h"
#include "string.h"
#include "Wnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define hhk Thread::GetCurrent()->hhk

Application* Wnd::getApp() {
	return Application::getApp();
}

void Wnd::RegisterClass()
{
	RegisterClass(0, 0, 0, 0, 0, "WND", 0);
}

void Wnd::RegisterClass(UINT style, HICON hIcon, HCURSOR hCursor,
						HBRUSH  hbrBackground, LPCTSTR lpszMenuName,
						LPCTSTR lpszClassName, HICON hIconSm)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = style;
	wc.lpfnWndProc=Wnd::WndProc;
    wc.cbClsExtra = 0;
	wc.cbWndExtra=10;
	wc.hInstance=Application::getApp()->getInstance();
    wc.hIcon = hIcon;
    wc.hCursor = hCursor;
	wc.hbrBackground = hbrBackground;
    wc.lpszMenuName = lpszMenuName; 
	wc.lpszClassName=lpszClassName;
    wc.hIconSm = hIconSm;
	VERIFY(::RegisterClassEx(&wc));
}

Wnd::Wnd(const char* classname, DWORD dwExStyle, const char* name, DWORD dwStyle,
				 int x, int y, int nWidth, int nHeight, Wnd* pWndParent,
				 Menu*	pm, LPVOID lpParam) : hWnd(NULL), pWndProc(NULL), _pMenu(pm)
{
	WinThread::HookWndCreate(this);
	
	HMENU hMenu = NULL;
	if ( _pMenu )
		hMenu = *_pMenu;
	if( !::CreateWindowEx(dwExStyle, classname ? classname : "WND", name, dwStyle, x, y, nWidth,
		nHeight, pWndParent ? pWndParent->hWnd : NULL, hMenu, getApp()->getInstance(),lpParam) )
		WinThread::UnhookWndCreate();
}
Wnd::~Wnd(){
	delete _pMenu;
//	::SetWindowLongPtr( hWnd, GWL_WNDPROC, (LONG_PTR)::DefWindowProc);
//	::SetWindowLongPtr( hWnd, GWL_USERDATA, NULL);
}

void Wnd::SendMessageToDescendants(UINT message, WPARAM wParam, LPARAM lParam, UINT nIDFirst, UINT nIDLast)
{
	HWND hwnd = GetWindow(hWnd, GW_CHILD);
	while(hwnd)
	{
		UINT nID = GetDlgCtrlID(hwnd);
		if(nID >= nIDFirst && (nIDLast == 0 || nID <= nIDLast))
			::SendMessage(hwnd, message, wParam, lParam);
		hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
	}
}

MSG Wnd::GetLastMsg()
{
	return getThread()->lastSentMsg;
};


///////////////////////////////////////////////////////////////////
// Message handlers
//////////////////////////////////////////////////////////////////
LRESULT Wnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_NCDESTROY, onNcDestroy)
		ON_MSG(WM_CLOSE, onClose);
		ON_MSG(WM_COMMAND, onCommand)
		ON_MSG(WM_INITMENUPOPUP, onInitMenuPopup)
		ON_MSG(WM_CTLCOLOREDIT, onCtlColor)
	ROUGHT(DefWindowProc)
}

bool Wnd::onCommand(UINT nID, HWND hwndCtl, UINT codeNotify)
{
	if(hwndCtl)
		return false;
	if(_pMenu){
		_pMenu->action(nID);
		return true;
	}
	return false;
}

void Wnd::onNcDestroy(){
	delete this;
}

HBRUSH Wnd::onCtlColor(HDC hdc, HWND hwndChild, int type)
{
	Wnd* pWnd = Wnd::FromHandle(hwndChild);
	HBRUSH hBrush;
	if(pWnd && (hBrush = pWnd->onCtlColorReflect(hdc, type)))
	{
//		handled = true;
		return hBrush;
	}
	else
		return (HBRUSH)Default();
}

void Wnd::onInitMenuPopup( HMENU hmenuPopup, UINT nIndex, BOOL bSysMenu )
{
	if(bSysMenu)
	{
		Default();
		return;
	}
	CmdUI cmdUI(hmenuPopup);
	int n = cmdUI.GetCount();//dunya
	Wnd* pWnd = GetFocus();
	if(n>0)
	while(n--)
	{
		onCmdUI(cmdUI[n]);
	}
	Default();
}

bool Wnd::onCmdUI(CmdUI* pCmdUI)
{
	if( _pMenu->isActive((UINT)*pCmdUI) )
		return pCmdUI->Enable(true), true;

	return pCmdUI->Enable(false), false;
}

LRESULT Wnd::DefWindowProc(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if(pWndProc)
		return ::CallWindowProc( pWndProc, hWnd, Msg, wParam, lParam); 
	else
		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT WINAPI Wnd::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	Wnd* pWnd = Wnd::FromHandle(hWnd);
	ASSERT(pWnd);
	WinThread* pThread = getThread();
	MSG oldMsg = pThread->lastSentMsg;   // save for nesting
	pThread->lastSentMsg.hwnd = hWnd;
	pThread->lastSentMsg.message = Msg;
	pThread->lastSentMsg.wParam = wParam;
	pThread->lastSentMsg.lParam = lParam;
	return pWnd->WindowProc(Msg, wParam, lParam);
}

void Wnd::invalidate_parent()
{
		RECT r;
		Wnd* pWnd = GetParent();
		GetWindowRect(*this, &r);
		pWnd->ScreenToClient(&r);
		pWnd->InvalidateRect(&r);
}

