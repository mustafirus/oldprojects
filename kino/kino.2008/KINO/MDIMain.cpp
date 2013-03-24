// MDIMain.cpp: implementation of the MDIMain class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Application.h"
#include "MDIChild.h"
#include "MDIMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GetApp() Application::GetApp()
#define CLIENT_ID	0x100
BEGIN_COMMAND_MAP(MDIMain, Wnd)
END_COMMAND_MAP

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MDIMain::MDIMain(UINT resID) : hWndClient(NULL)
{
	char* s1 = GetApp()->LoadString(resID);
	hAccel = GetApp()->LoadAccelerators(resID);
	Create(WS_EX_OVERLAPPEDWINDOW, s1, WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
				 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
				 GetApp()->LoadMenu(resID), NULL);
	delete s1;
}

void MDIMain::MDIActivate(MDIChild* pWnd)
{
	::SendMessage (hWndClient, WM_MDIACTIVATE, (WPARAM)(HWND)*pWnd, 0) ;
}

bool MDIMain::CreateChildFrame(MDIChild* pWnd, const char* szTitle, DWORD dwStyle, int x, int y, int cx, int cy, LPARAM lParam /*= NULL*/)
{
	Application* pApp = GetApp();
	HWND hwndChild = NULL;
	HookWndCreate(pWnd);
	if(pApp->GetCurrent() == pApp)
	{
		MDICREATESTRUCT     mdicreate;
		mdicreate.szClass = pWnd->GetClassName();
		mdicreate.szTitle = szTitle;
		mdicreate.hOwner  = pApp->GetInstance();
		mdicreate.x       = x;
		mdicreate.y       = y;
		mdicreate.cx      = cx;
		mdicreate.cy      = cy;
		mdicreate.style   = dwStyle;
		mdicreate.lParam  = lParam ;
		
		hwndChild = (HWND) ::SendMessage (hWndClient,
			WM_MDICREATE, 0, (LPARAM) (LPMDICREATESTRUCT) &mdicreate) ;
	}else
	{
		hwndChild = CreateMDIWindow((char*)pWnd->GetClassName(), (char*)szTitle,
			dwStyle, x, y, cx, cy, hWndClient, pApp->GetInstance(), lParam);
	}
		if(hwndChild)
			return true;
		else
		{
			UnhookWndCreate();
			return false;
		}
}

LRESULT MDIMain::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_SIZE, OnSize)
		ON_MSG(WM_CREATE, OnCreate)
		ON_MSG(WM_DESTROY, OnDestroy)
//		ON_MSG_TO_CHILD(WM_MENUSELECT, 0, CLIENT_ID-1)
	ROUGHT(Wnd::WindowProc)
}

bool MDIMain::OnCommand(UINT nID, HWND hwndCtl, UINT codeNotify)
{
	if(Wnd::OnCommand(nID, hwndCtl, codeNotify))
		return true;
	MDIChild* pWnd = GetActiveChild();
	if(pWnd)
		RepostMessage(*pWnd);//, WM_COMMAND, MAKELONG(nID, codeNotify), hwndCtl);
	return false;
}

void MDIMain::OnSize(UINT state, int cx, int cy)
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

bool MDIMain::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    CLIENTCREATESTRUCT  clientcreate;
	clientcreate.hWindowMenu  = NULL ;
	clientcreate.idFirstChild = ID_FIRSTCHILD ;
	
	hWndClient = CreateWindow ("MDICLIENT", NULL,
		WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		0, 0, 0, 0, *this, (HMENU) CLIENT_ID, GetApp()->GetInstance(),
		(LPSTR) &clientcreate) ;
	return true;
}

void MDIMain::OnDestroy()
{
	if(GetApp()->GetMainWnd() == this)
		PostQuitMessage(0);
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

