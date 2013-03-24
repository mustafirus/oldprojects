#pragma once

class Wnd;

//#define ThreadPtr __declspec( thread ) static Thread*

class Thread
{
//	static DWORD ThreadPtrIndex; // Non microsoft implementation
	HANDLE	hThread;
	DWORD	nThreadID;
protected:
	Thread() : hThread(NULL), nThreadID(0){};
	virtual void attach();
	virtual void detach();

public:
	Thread(bool suspended);
	virtual ~Thread();

	virtual int run(){return -1;};
	virtual void resume(){ResumeThread(hThread);};
	operator HANDLE(){return hThread;};
	operator DWORD(){return nThreadID;};
	static Thread* getCurrent(){ return pCurentThread; }

private:

	static DWORD WINAPI start(Thread* pThread);
	__declspec( thread ) static Thread* pCurentThread;
};

