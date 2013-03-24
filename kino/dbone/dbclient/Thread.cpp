// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Thread.h"

Thread* Thread::pCurentThread = NULL;

void Thread::attach()
{
	pCurentThread = this;
	hThread	= ::GetCurrentThread();
	nThreadID = ::GetCurrentThreadId();
}

void Thread::detach()
{
	pCurentThread = NULL;
	hThread	= NULL;
	nThreadID = 0;
}

Thread::Thread(bool suspended) :
hThread(NULL), nThreadID(0)
{
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)start, this,
		suspended ? CREATE_SUSPENDED : 0, &nThreadID);
};

DWORD WINAPI Thread::start(Thread* pThread)
{
	pThread->pCurentThread = pThread;
	int exc = pThread->run();
	return exc;
}

Thread::~Thread()
{
	if(hThread)
	{
		if(pCurentThread != this)
		{
			DWORD status;
			GetExitCodeThread(hThread, &status);
			if(status == STILL_ACTIVE)
				TerminateThread(hThread, -1);
		}
		CloseHandle(hThread);
	}
};
