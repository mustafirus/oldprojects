#pragma once

#include "Thread.h"

class WinThread : public Thread
{
	MSG msg;
protected:
	WinThread() : pWndCreate(NULL), hhk(NULL){};
	Wnd* pWndCreate;
	HHOOK hhk;
public:
	MSG lastSentMsg;
	WinThread(bool suspended) : Thread(suspended), pWndCreate(NULL), hhk(NULL)
	{memset(&msg, 0, sizeof(MSG));memset(&lastSentMsg, 0, sizeof(MSG));};
	virtual ~WinThread(){};

	virtual int run();
	virtual bool preTranslateMessage(MSG* pMsg) = 0;
	virtual bool onIdle() { return false; } // true - has idle job, false hasn't
	virtual bool isIdleMessage(MSG* pMsg);

	static WinThread* getCurrent(){return dynamic_cast<WinThread*>( Thread::getCurrent()) ; }

	static LRESULT CALLBACK WndCreateHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	static void HookWndCreate(Wnd* pWnd);
	static void UnhookWndCreate();
};
