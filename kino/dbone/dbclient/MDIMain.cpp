#include "stdafx.h"
#include "Application.h"
#include "MDIChild.h"
#include "MDIMain.h"

#define CLIENT_ID	0x100
#define ID_FIRSTCHILD                   0xE000


MDIMain::MDIMain(const char* name, Menu* pm) : Wnd(NULL, WS_EX_OVERLAPPEDWINDOW, name,
								   WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
								   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
								   pm, NULL),
								   hWndClient(NULL)
{
    CLIENTCREATESTRUCT  clientcreate;
	clientcreate.hWindowMenu  = NULL ;
	clientcreate.idFirstChild = ID_FIRSTCHILD ;
	
	hWndClient = CreateWindow ("MDICLIENT", NULL,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL,
		0, 0, 0, 0, *this, (HMENU) CLIENT_ID, getApp()->getInstance(),
		(LPSTR) &clientcreate) ;
}
MDIMain::~MDIMain(){
	PostQuitMessage(0);
	getApp()->clearMainWnd();
}

void MDIMain::MDIActivate(MDIChild* pWnd)
{
	::SendMessage (hWndClient, WM_MDIACTIVATE, (WPARAM)(HWND)*pWnd, 0) ;
}

LRESULT MDIMain::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_SIZE, onSize)
//		ON_MSG_TO_CHILD(WM_MENUSELECT, 0, CLIENT_ID-1)
	ROUGHT(Wnd::WindowProc)
}

bool MDIMain::onCommand(UINT nID, HWND hwndCtl, UINT codeNotify)
{
	if(Wnd::onCommand(nID, hwndCtl, codeNotify))
		return true;
	MDIChild* pWnd = getActiveChild();
	if(pWnd)
		RepostMessage(*pWnd);//, WM_COMMAND, MAKELONG(nID, codeNotify), hwndCtl);
	return false;
}

void MDIMain::onSize(UINT state, int cx, int cy)
{
	HWND hwnd = GetWindow(*this, GW_CHILD);
	RECT rect;
	RECT free;
	GetClientRect(*this, &free);
	while(hwnd)
	{
		UINT nID = GetDlgCtrlID(hwnd);
		if(nID < CLIENT_ID)
		{
			::SendMessage(hwnd, WM_SIZE, 0, 0);
			::GetWindowRect(hwnd, &rect);
			ScreenToClient(&rect);
			SubtractRect(&free, &free, &rect);
		}
		hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
	}
//	DefWindowProc(WM_SIZE, 0, 0);
	SetWindowPos(hWndClient, HWND_BOTTOM, free.left, free.top,
		free.right-free.left, free.bottom - free.top, SWP_FRAMECHANGED|SWP_DRAWFRAME);
}

LRESULT MDIMain::DefWindowProc(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return ::DefFrameProc(*this, hWndClient, Msg, wParam, lParam);
}

bool MDIMain::PreTranslateMessage(MSG* pMsg)
{
	if(::TranslateMDISysAccel(hWndClient, pMsg))
		return true;
	return (hAccel ? (::TranslateAccelerator(*this, hAccel, pMsg)>0) : false);
};

