#include "stdafx.h"
#include "Wnd.h"
#include "Application.h"

#include "WinThread.h"

//void* WinThread::operator new(size_t sz){
//	return ::operator new(sz);
//}
//
//void WinThread::operator delete(void* p){
//	::operator delete(p);
//}

int WinThread::run()
{
	bool Idle = false;
	while(::GetMessage(&msg, NULL, 0,0 ))
	{
		if(isIdleMessage(&msg))
			Idle = true;
		if(!preTranslateMessage(&msg))
		{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
		}
		if(Idle && !::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
			Idle = onIdle();
	}
	return 0;
}
/*	
bool WinThread::PreTranslateMessage(MSG* pMsg)
{
	Application* pApp = Application::getApp();
	if(pApp != this)
	{
		return pApp->PreTranslateMessage(pMsg);
	}else
	{
		Wnd* pWnd = Wnd::FromHandle(pMsg->hwnd);
		return pWnd ? pWnd->PreTranslateMessage(pMsg) : false;
	}
};
*/

bool WinThread::isIdleMessage(MSG* pMsg)
{
	if(pMsg->message == WM_MOUSEMOVE ||
		(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) ||
		pMsg->message == WM_TIMER)
		return true;
	return false;
}

LRESULT CALLBACK WinThread::WndCreateHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	switch(nCode)
	{
	case HCBT_CREATEWND:
		{
#pragma warning( push )
#pragma warning( disable : 4244 )
			WinThread* pwt = WinThread::getCurrent();
			ASSERT(pwt && pwt->pWndCreate);


			if(!::IsWindow((HWND) wParam))
				return CallNextHookEx(pwt->hhk, nCode, wParam, lParam);

			pwt->pWndCreate->hWnd = (HWND) wParam;
			LONG_PTR pfn = ::GetWindowLongPtr((HWND)wParam, GWL_WNDPROC);
			if(pfn != (LONG_PTR)Wnd::WndProc)
			{
				pwt->pWndCreate->pWndProc = (XWNDPROC)pfn;
				::SetWindowLongPtr((HWND)wParam, GWL_WNDPROC, (LONG_PTR)Wnd::WndProc);
			}
			::SetWindowLongPtr((HWND) wParam,GWL_USERDATA, (LONG_PTR)pwt->pWndCreate);
			VERIFY(UnhookWindowsHookEx(pwt->hhk));
			pwt->hhk = NULL;
			pwt->pWndCreate = NULL;
			return 0;
#pragma warning( pop )
		}
	default: 
		return CallNextHookEx(WinThread::getCurrent()->hhk, nCode, wParam, lParam); 
	}
}
void WinThread::HookWndCreate(Wnd* pWnd)
{
	WinThread* pwt = WinThread::getCurrent();
	ASSERT(pwt->hhk == NULL);
	ASSERT(pwt->pWndCreate == NULL);
	pwt->hhk = SetWindowsHookEx(WH_CBT, (HOOKPROC) WinThread::WndCreateHookProc, 
		Application::getApp()->getInstance(), ::GetCurrentThreadId());
	pwt->pWndCreate = pWnd;
}

void WinThread::UnhookWndCreate()
{
	WinThread* pwt = WinThread::getCurrent();
	if(pwt->hhk)
	{
		VERIFY(UnhookWindowsHookEx(pwt->hhk));
		pwt->hhk = NULL;
		pwt->pWndCreate = NULL;
	}
	_RPT0(_CRT_WARN, "Error create Wnd");
}
