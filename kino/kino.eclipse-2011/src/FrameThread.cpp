// FrameThread.cpp: implementation of the FrameThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Application.h"
#include "MDIMain.h"
#include "FrameWnd.h"
#include "Record.h"
#include "Wait.h"
#include "Exception.h"
#include "FrameThread.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define GetApp() Application::GetApp()
FrameThread::FrameThread(FrameThread*& pHead) :
next(pHead), pprev(&pHead),
WinThread(true, true), pMainWnd(NULL), pRec(NULL)
{
	if(pHead)
		pHead->pprev = &next;
	pHead = this;
}

FrameThread::FrameThread(const char* tab, FrameThread*& pHead) :
next(pHead), pprev(&pHead),
WinThread(true, true), pMainWnd(NULL), pRec(NULL)
{
	try{
	dc.Create(tab);
	}catch(Exception* e)
	{
//		e->SetDelMem(this);
		throw e;
	}
	if(pHead)
		pHead->pprev = &next;
	pHead = this;
	Resume();
}

FrameThread::~FrameThread()
{
	*pprev = next; 
	if(next)
		next->pprev = pprev;
	auto_delete = false;
	if(pMainWnd)
	{
		PostMessage(*pMainWnd, WM_CLOSE, 0, 0);
	}
	if(GetCurrent() != this)
		WaitForSingleObject(*this, INFINITE);
	ifdel(pRec);
}

FrameThread* FrameThread::CreateThread()
{
	return new FrameThread(*pprev);
}

void FrameThread::CreateThread(RKey* prk, UINT formID)
{
	Wait ww;
	FrameThread* ptr = new FrameThread(*pprev);
	pRec = new Record(prk);
	prk = pRec->GetRKey();
	pRec->Load();
	ptr->dc.Create(prk, NULL, formID);
	ptr->Resume();
}

bool FrameThread::InitInstance()
{
/*	int c = 30;
	while(!dc.szForm && c--)
		Sleep(100);
*/	pMainWnd = new FrameWnd(&dc, NULL);
	return true;
}

bool FrameThread::PreTranslateMessage(MSG* pMsg)
{
	Wnd* pWnd = ((MDIMain*) GetApp()->GetMainWnd())->GetActiveChild();
	if(pWnd && pWnd->PreTranslateMessage(pMsg))
		return true;
	return GetApp()->PreTranslateMessage(pMsg);
};

/*FrameThread::FrameThread(RKey* prk, FrameThread*& pHead) :
next(pHead), pprev(&pHead),
WinThread(false, true), pMainWnd(NULL), pRec(NULL)
{
	dc.Create(prk);
	if(pHead)
		pHead->pprev = &next;
	pHead = this;
}
*/
