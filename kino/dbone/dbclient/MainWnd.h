#pragma once

class FrameThread;
#include "FrameWnd.h"
#include "MDIMain.h"
//#define CF_RECORD	CF_PRIVATEFIRST
//#define CF_RKEY		(CF_PRIVATEFIRST + 1)

class Connection;
class FrameWnd;
class StatusBar;

class MainWnd : public MDIMain  
{
	StatusBar*		psb;
	FrameThread*	pThread;
	Connection*		_pcon;

public:
	MainWnd(Menu* pm) ;
	virtual ~MainWnd();

	FrameWnd* getActiveChild() { return dynamic_cast<FrameWnd*>(MDIMain::getActiveChild()); }

	bool brk;
	void Wait(bool w = true);
//	void CreateThread(RKey* prk);
protected:

// Message handlers
	virtual LRESULT WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam);
	void onMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);
	bool onSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg);

public:
	void onFileNew();
	void onExit(){ close(); }
	void onCloseAll();
};

