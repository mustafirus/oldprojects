// FrameThread.h: interface for the FrameThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAMETHREAD_H__4DD0E293_4541_11D2_A71F_204C4F4F5020__INCLUDED_)
#define AFX_FRAMETHREAD_H__4DD0E293_4541_11D2_A71F_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WinThread.h"
#include "DataContext.h"

class FrameWnd;

class FrameThread : public WinThread
{
	FrameThread**	pprev;
	FrameThread*	next;

	FrameWnd*	pMainWnd;
//	const char* table;
	FrameThread(FrameThread*& pHead);
public:
	Record*		pRec;
	DataContext dc;
	FrameThread(const char* tab, FrameThread*& pHead);
//	FrameThread(RKey* prk, FrameThread*& pHead);
	virtual ~FrameThread();
//Operations
	void CreateThread(RKey* prk, UINT formID);
	FrameThread* CreateThread();
//Helpers
	FrameWnd* GetThreadWnd(){return pMainWnd;};
//Standard
	virtual bool OnIdle(){return false;};
	virtual bool InitInstance();
	bool PreTranslateMessage(MSG* pMsg);
	static FrameThread* GetCurrent(){return (FrameThread*)Thread::GetCurrent();};
};

#endif // !defined(AFX_FRAMETHREAD_H__4DD0E293_4541_11D2_A71F_204C4F4F5020__INCLUDED_)
