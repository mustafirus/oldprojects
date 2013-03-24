// MainWnd.cpp: implementation of the MainWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "kinores.h"
#include "FrameThread.h"
#include "FrameWnd.h"
#include "Exception.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BASECLASS MDIMain
static UINT ind[4] = {(UINT)ID_INDICATOR_TEXT,
		(UINT)ID_INDICATOR_CAPS, (UINT)ID_INDICATOR_NUM, 
		(UINT)ID_INDICATOR_SCRL};
static UINT icount = sizeof ind / sizeof(UINT);
static char* cat[]=
{"dog", "baby", "fucking", "gofuck", "show", "owner", "street",
 "city", "color", "breed", "refery","club", "factory", "Forms"};

BEGIN_COMMAND_MAP(MainWnd, MDIMain)
	ON_COMMAND_RANGE(ID_CAT_FIRST, ID_CAT_LAST, OnCat)
	ON_COMMAND(ID_FILE_EXIT, OnClose)
END_COMMAND_MAP


void MainWnd::OnCat(UINT nID)
{
/*	if(nID == ID_CAT_GOFUCK)
	{
		Wnd* pWnd = FromHandle((HWND)::SendMessage(hWndClient, WM_MDIGETACTIVE, 0, 0));
		if(pWnd)
			pWnd->DestroyWindow();
		return;
	}
*/
	//	new FrameWnd(cat[nID - (ID_CAT_FIRST)], NULL);
	try{
	new FrameThread(cat[nID - (ID_CAT_FIRST)], pThread);
	}catch(Exception* e)
	{
		e->Effect();
	}
}

/*void MainWnd::CreateThread(RKey* prk)
{
	new FrameThread(prk, pThread);
}
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MainWnd::MainWnd(UINT resID) : MDIMain(resID), pThread(NULL), brk(false)
{
	psb = new StatusBar(this, ind, icount, 11);
}

MainWnd::~MainWnd()
{
}

LRESULT MainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_SETCURSOR, OnSetCursor)
		ON_MSG(WM_CLOSE, OnClose)
		ON_MSG(WM_MENUSELECT, OnMenuSelect)
	BASEROUGHT 
}

void MainWnd::OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	ResendMessage(*psb);
}

bool MainWnd::OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg)
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
		if(pWnd)
			pWnd->BringToTop(top);
		return true;
	}
	return Default() > 0;
}

void MainWnd::PostNcDestroy()
{
	while(pThread)
		delete pThread;
	MDIMain::PostNcDestroy();
}

void MainWnd::OnClose()
{
	ShowWindow(SW_HIDE);
	DestroyWindow();
	PostQuitMessage(0);
}
bool MainWnd::OnCmdUI(CmdUI* pCmdUI)
{
	if(((UINT)*pCmdUI) > 0xF000)
		return true;
	MDIChild* pWnd = GetActiveChild();
	if(pWnd && pWnd->OnCmdUI(pCmdUI))
		return true;
	return MDIMain::OnCmdUI(pCmdUI);
}

void MainWnd::Wait(bool w /* = true*/)
{
	psb->SetPaneText(0, w ? ID_WAIT_MESSAGE : ID_READY_MESSAGE, true);
}

void MainWnd::OnInitMenuPopup( HMENU hmenuPopup, UINT nIndex, BOOL bSysMenu )
{
	if(nIndex == 1)
	{
		FrameWnd* pWnd = (FrameWnd*)GetActiveChild();
		if(pWnd)
		{
			pWnd->UpdateMenu(hmenuPopup);
		}else
		{
			MENUITEMINFO mi;
			memset(&mi, 0, sizeof(mi));
			mi.cbSize = sizeof(mi);
			mi.fMask = MIIM_STATE;
			mi.fState = MFS_DISABLED;
			SetMenuItemInfo(hmenuPopup, GetMenuItemCount(hmenuPopup)-1,
				TRUE, &mi);
		}
	}
	BASECLASS::OnInitMenuPopup(hmenuPopup, nIndex, bSysMenu);
}
