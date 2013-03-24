#pragma once
#include "WinThread.h"

class FrameWnd;
class DataContext;

class FrameThread : public WinThread
{
	typedef WinThread base;
	FrameThread**	pprev;
	FrameThread*	next;

	FrameWnd*	pMainWnd;
	bool		_deleted;
//	const char* table;
	HWND		_hWndClient;
//	FrameThread(FrameThread*& pHead);

	DataContext*	_pdc;
public:
//	Record*		pRec;
	FrameThread(DataContext* pdc);
//	FrameThread(RKey* prk, FrameThread*& pHead);
	virtual ~FrameThread();

	virtual int run();
//Operations
//	void CreateThread(DataContext* pdc, UINT formID);
//	FrameThread* CreateThread();
//Helpers
	FrameWnd*	getThreadWnd()		{ return pMainWnd; } 
	void		clearThreadWnd()	{ pMainWnd = NULL; }
//Standard
	bool preTranslateMessage(MSG* pMsg);
	static FrameThread* getCurrent(){ return dynamic_cast<FrameThread*>( Thread::getCurrent() ); }
	static FrameThread* pHead;
};

