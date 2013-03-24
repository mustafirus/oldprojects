// WinThread.cpp: implementation of the WinThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Wnd.h"
#include "Application.h"
#include "Exception.h"
#include "WinThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int WinThread::Run()
{
	try{
	bool Idle;
	if(!InitInstance())
		return ExitInstance(-1);
	while(::GetMessage(&msg, NULL, 0,0 ))
	{
		if(IsIdleMessage(&msg))
			Idle = true;
		if(!PreTranslateMessage(&msg))
		{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
		}
		if(Idle && !::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
			Idle = OnIdle();
	}
	}catch(Exception* e)
	{
		e->Effect();
		return ExitInstance(-2);
	}catch(...)
	{
		return ExitInstance(-3);
	}
	return ExitInstance((int)msg.wParam);
}
/*	
bool WinThread::PreTranslateMessage(MSG* pMsg)
{
	Application* pApp = Application::GetApp();
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

bool WinThread::IsIdleMessage(MSG* pMsg)
{
	if(pMsg->message == WM_MOUSEMOVE ||
		(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) ||
		pMsg->message == WM_TIMER)
		return true;
	return false;
}
