// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Thread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*	// Non microsoft implementation
DWORD Thread::ThreadPtrIndex = -1; */

	// Microsoft implementation
Thread* Thread::pCurentThread = NULL;

void Thread::Attach()
{
/*	// Non microsoft implementation
	if(ThreadPtrIndex == -1)		
		ThreadPtrIndex = TlsAlloc();
	ASSERT(TlsGetValue(ThreadPtrIndex) == NULL);
	TlsSetValue(ThreadPtrIndex, this); */
	// Microsoft implementation
	pCurentThread = this;

	hThread	= ::GetCurrentThread();
	nThreadID = ::GetCurrentThreadId();
}

void Thread::Detach()
{
/*	// Non microsoft implementation
	ASSERT(ThreadPtrIndex != -1 && TlsGetValue(ThreadPtrIndex));
	TlsSetValue(ThreadPtrIndex, NULL);*/
	// Microsoft implementation
	pCurentThread = NULL;
	hThread	= NULL;
	nThreadID = 0;
}

Thread* Thread::GetCurrent()
{
/*	// Non microsoft implementation
	return (Thread*)TlsGetValue(ThreadPtrIndex);  */
	// Microsoft implementation
	return pCurentThread;
}

Thread::Thread(bool suspended, bool autodel) :
hThread(NULL), nThreadID(0), pWndCreate(NULL), hhk(NULL), auto_delete(autodel)
{
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Start, this,
		suspended ? CREATE_SUSPENDED : 0, &nThreadID);
};

DWORD WINAPI Thread::Start(Thread* pThread)
{
	pThread->pCurentThread = pThread;
	int exc = pThread->exit_code = pThread->Run();
	if(pThread->auto_delete)
		delete pThread;
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
