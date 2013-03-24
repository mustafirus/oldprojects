// Wnd.cpp: implementation of the Wnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Exception.h"
#include "Application.h"
#include "Log.h"
#include "CmdUI.h"
#include "Wnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define hhk Thread::GetCurrent()->hhk

static LRESULT CALLBACK WndCreateHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch(nCode)
	{
	case HCBT_CREATEWND:
		{
			Thread* ptr = Thread::GetCurrent();
			ASSERT(ptr && ptr->pWndCreate);
			if(!::IsWindow((HWND) wParam))
				return CallNextHookEx(hhk, nCode, wParam, lParam);
			ptr->pWndCreate->hWnd = (HWND) wParam;
			long pfn = ::GetWindowLong((HWND)wParam, GWL_WNDPROC);
			if(pfn != (long)Wnd::WndProc)
			{
				ptr->pWndCreate->pWndProc = (XWNDPROC)pfn;
				::SetWindowLong((HWND)wParam, GWL_WNDPROC, (long)Wnd::WndProc);
			}
			::SetWindowLong((HWND) wParam,GWL_USERDATA, (long)ptr->pWndCreate);
			ptr->pWndCreate = NULL;
			VERIFY(UnhookWindowsHookEx(hhk));
			hhk = NULL;
			return 0;
		}
	default: 
		return CallNextHookEx(hhk, nCode, wParam, lParam); 
	}
}

void HookWndCreate(Wnd* pWnd)
{
	ASSERT(hhk == NULL);
	hhk = SetWindowsHookEx(WH_CBT, (HOOKPROC) WndCreateHookProc, 
		Application::GetApp()->GetInstance(), ::GetCurrentThreadId());
	ASSERT(Thread::GetCurrent()->pWndCreate == NULL);
	Thread::GetCurrent()->pWndCreate = pWnd;
}

void UnhookWndCreate()
{
	if(hhk)
	{
		VERIFY(UnhookWindowsHookEx(hhk));
		hhk = NULL;
	}
	_RPT0(_CRT_WARN, "Error create Wnd");
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
	wc.hInstance=Application::GetApp()->GetInstance();
    wc.hIcon = hIcon;
    wc.hCursor = hCursor;
	wc.hbrBackground = hbrBackground;
    wc.lpszMenuName = lpszMenuName; 
	wc.lpszClassName=lpszClassName;
    wc.hIconSm = hIconSm;
	VERIFY(::RegisterClassEx(&wc));
}

bool Wnd::Create(DWORD dwExStyle, LPCTSTR lpWindowName, DWORD dwStyle,
				 int x, int y, int nWidth, int nHeight, Wnd* pWndParent,
				 HMENU hMenu, LPVOID lpParam)
{
	HookWndCreate(this);
	if(!::CreateWindowEx(dwExStyle, GetClassName(), lpWindowName, dwStyle, x, y, nWidth,
		nHeight, pWndParent ? pWndParent->hWnd : NULL, hMenu, GetApp()->GetInstance(),lpParam))
		UnhookWndCreate();
 	return true;
}
///////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////
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

Application* Wnd::GetApp()
{
	return Application::GetApp();
}

WinThread* Wnd::GetThread()
{
	return (WinThread*) Thread::GetCurrent();
};

MSG Wnd::GetLastMsg()
{
	return GetThread()->lastSentMsg;
};


const X_CMDMAP_ENTRY* Wnd::GetCmdEntry(UINT nID)
{
	const X_CMDMAP* pCmdMap;
	pCmdMap = GetCmdMap();
	do
	{
		const X_CMDMAP_ENTRY* pEntry = pCmdMap->lpEntries;
		while(pEntry->pfn)
		{
			if(nID >= pEntry->nID && nID <= pEntry->nLastID)
			{
				return pEntry;
			}
			pEntry++;
		}
	}while(pCmdMap = pCmdMap->pBaseMap);
	return NULL;
}

const X_CMDMAP_ENTRY* Wnd::GetReflectEntry(UINT codeNotify)
{
	const X_CMDMAP* pCmdMap;
	pCmdMap = GetCmdMap();
	do
	{
		const X_CMDMAP_ENTRY* pEntry = pCmdMap->lpEntries;
		while(pEntry->pfn)
		{
			if(pEntry->nID == pEntry->nLastID == 0 && pEntry->nCode == codeNotify)
			{
				return pEntry;
			}
			pEntry++;
		}
	}while(pCmdMap = pCmdMap->pBaseMap);
	return NULL;
}

/////////////////////////////////////////////////////////////
//BEGIN_COMMAND_MAP(CLASSNAME, BASECLASSNAME)
//END_COMMAND_MAP
const X_CMDMAP* Wnd::GetCmdMap() const 
{
	return &Wnd::cmdMap;
}
const X_CMDMAP Wnd::cmdMap = { NULL, &Wnd::_cmdEntries[0] };
const X_CMDMAP_ENTRY Wnd::_cmdEntries[] = 
{
	{0, 0, 0, (X_PCMDFN)0 } 
};
///////////////////////////////////////////////////////////////////
// Message handlers
//////////////////////////////////////////////////////////////////
LRESULT Wnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_COMMAND, OnCommand)
		ON_MSG(WM_INITMENUPOPUP, OnInitMenuPopup)
		ON_MSG(WM_CTLCOLOREDIT, OnCtlColor)
		ON_MSG(WM_NCDESTROY, OnNcDestroy)
	ROUGHT(DefWindowProc)
}

bool Wnd::OnCommand(UINT nID, HWND hwndCtl, UINT codeNotify)
{
	const X_CMDMAP_ENTRY* pEntry;
	if(hwndCtl)
		return false;
	pEntry = GetCmdEntry(nID);
	if(!pEntry)
		return false;
	if(pEntry->nCode & X_CMDMAP_CODE_RANGE)
		(this->*((X_PCMDFNEX)pEntry->pfn))(nID);
	else
		(this->*(pEntry->pfn))();
	return true;
}

HBRUSH Wnd::OnCtlColor(HDC hdc, HWND hwndChild, int type)
{
	Wnd* pWnd = Wnd::FromHandle(hwndChild);
	HBRUSH hBrush;
	if(pWnd && (hBrush = pWnd->OnCtlColorReflect(hdc, type)))
	{
//		handled = true;
		return hBrush;
	}
	else
		return (HBRUSH)Default();
}

void Wnd::OnInitMenuPopup( HMENU hmenuPopup, UINT nIndex, BOOL bSysMenu )
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
/*		if(pWnd && pWnd != this)
		{
			if(!pWnd->OnCmdUI(cmdUI[n]))
				OnCmdUI(&cmdUI);
		}else
*/		OnCmdUI(cmdUI[n]);
	}
	Default();
}

bool Wnd::OnCmdUI(CmdUI* pCmdUI)
{
	if((GetCmdEntry(*pCmdUI) == NULL))
		return false;
	pCmdUI->Enable(true);
	return true;
}

LRESULT Wnd::DefWindowProc(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if(pWndProc)
		return ::CallWindowProc( pWndProc, hWnd, Msg, wParam, lParam); 
	else
		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
}
extern Log warnlog;
LRESULT WINAPI Wnd::WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	try{
	Wnd* pWnd = Wnd::FromHandle(hWnd);
	ASSERT(pWnd);
	WinThread* pThread = GetThread();
	MSG oldMsg = pThread->lastSentMsg;   // save for nesting
	pThread->lastSentMsg.hwnd = hWnd;
	pThread->lastSentMsg.message = Msg;
	pThread->lastSentMsg.wParam = wParam;
	pThread->lastSentMsg.lParam = lParam;
	return pWnd->WindowProc(Msg, wParam, lParam);
	}catch(Exception* e)
	{
		e->Effect();
		return 0;
	}catch(...)
	{
		warnlog.printf("Unknown Exception in WndProc");
		warnlog.Dump();
		return 0;
	}
}

////////////////////////////////////////////////////////////////////
