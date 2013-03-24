// FrameThread.cpp: implementation of the FrameThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrameWnd.h"
#include "Application.h"
#include "MDIMain.h"

#include "FrameThread.h"

FrameThread* FrameThread::pHead = NULL;

FrameThread::FrameThread(DataContext* pdc) :
next(pHead), pprev(&pHead),
WinThread(false), pMainWnd(NULL), _pdc(pdc), _deleted(false)
{
	if(pHead)
		pHead->pprev = &next;
	pHead = this;

	Wnd* p = Application::getApp()->getMainWnd();
	MDIMain* pm = dynamic_cast<MDIMain*> (p);
	ASSERT( pm );
	_hWndClient = pm->getClient();
}

FrameThread::~FrameThread()
{
	*pprev = next; 
	if(next)
		next->pprev = pprev;

	_deleted = true;
	MDIMain* p = (MDIMain*) Application::getApp()->getMainWnd();
	BOOL b = AttachThreadInput( GetCurrentThreadId(),  GetWindowThreadProcessId( *p, NULL ), TRUE );
	if(pMainWnd)
		pMainWnd->close();
//		FrameWnd* p = pMainWnd;
//		pMainWnd = NULL;
//		PostMessage(*pMainWnd, WM_CLOSE, 0, 0);
//	}
	HWND h = ::SetFocus(p->getClient());
	if ( !h ){
		DWORD err = GetLastError();
	}
	if(getCurrent() != this)
		WaitForSingleObject(*this, INFINITE);
}

int FrameThread::run(){
	DWORD app_id  = *Application::getApp();
	DWORD this_id = *this;
	BOOL b = AttachThreadInput( this_id, app_id, TRUE );
	pMainWnd = new FrameWnd(_pdc);
//	Wnd* pWnd = new MDIChild(NULL, "test");
//	pWnd->show();
	int ret = base::run();
	if(!_deleted)
		delete this;
	b = AttachThreadInput( this_id, app_id, FALSE );
	return ret;
}

bool FrameThread::preTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message){
		case WM_INPUTLANGCHANGEREQUEST:{
//			DWORD app_id  = *Application::getApp();
//			DWORD this_id = *this;
//			BOOL b = AttachThreadInput( this_id, app_id, TRUE );
//			LRESULT res = ::PostMessage( _hWndClient, pMsg->message, pMsg->wParam, pMsg->lParam );
//			b = true;
		}
//			::DefWindowProc(*pMainWnd, pMsg->message, pMsg->wParam, pMsg->lParam );
			return false;
		case WM_INPUTLANGCHANGE:
			return false;
		default:
	return TranslateMDISysAccel( _hWndClient, pMsg ) != 0;
	}

	Wnd* pWnd = ((MDIMain*) Application::getApp()->getMainWnd())->getActiveChild();
	if(pWnd && pWnd->preTranslateMessage(pMsg))
		return true;
	return Application::getApp()->preTranslateMessage(pMsg);
};
