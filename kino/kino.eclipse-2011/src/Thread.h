// Thread.h: interface for the Thread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__E7D87217_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
#define AFX_THREAD_H__E7D87217_272F_11D2_A6D9_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class Wnd;

#define ThreadPtr __declspec( thread ) static Thread*

class Thread
{
//	static DWORD ThreadPtrIndex; // Non microsoft implementation
	ThreadPtr pCurentThread;
	HANDLE	hThread;
	DWORD	nThreadID;
protected:
	int		exit_code;
	bool	auto_delete;
	Thread() : hThread(NULL), nThreadID(0), pWndCreate(NULL), hhk(NULL){};
//Attributes
public:
	Wnd* pWndCreate;
	HHOOK hhk;
//Construction
public:
	Thread(bool suspended, bool autodel);
	virtual ~Thread();
//Operations
	virtual void Attach();
	virtual void Detach();
	virtual int Run(){return -1;};
	virtual void Resume(){ResumeThread(hThread);};
	operator HANDLE(){return hThread;};
	static Thread* GetCurrent();
private:
	static DWORD WINAPI Start(Thread* pThread);
};

#endif // !defined(AFX_THREAD_H__E7D87217_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
