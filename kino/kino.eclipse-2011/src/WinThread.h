// WinThread.h: interface for the WinThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINTHREAD_H__28FAAAB2_4183_11D2_A71A_204C4F4F5020__INCLUDED_)
#define AFX_WINTHREAD_H__28FAAAB2_4183_11D2_A71A_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Thread.h"

class WinThread : public Thread
{
	MSG msg;
protected:
	WinThread(){};
public:
	MSG lastSentMsg;
	WinThread(bool suspended, bool autodel) : Thread(suspended, autodel)
	{memset(&msg, 0, sizeof(MSG));memset(&lastSentMsg, 0, sizeof(MSG));};
	virtual ~WinThread(){};

	virtual bool InitInstance(){return true;};
	virtual int ExitInstance(int code = 0){return code;};
	virtual int Run();
	virtual bool PreTranslateMessage(MSG* pMsg){return false;};
	virtual bool OnIdle() = 0; // true - has idle job, false hasn't
	virtual bool IsIdleMessage(MSG* pMsg);
};
#endif // !defined(AFX_WINTHREAD_H__28FAAAB2_4183_11D2_A71A_204C4F4F5020__INCLUDED_)
