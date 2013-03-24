#pragma once

#include "Wnd.h"
class MDIChild;

class MDIMain : public Wnd  
{
	HACCEL hAccel;
protected:
	HWND	hWndClient;
//	MDIMain();
public:
	MDIMain(const char* name, Menu* pm);
	virtual ~MDIMain();

	HWND getClient(){ return hWndClient; }
//Operations 
	void MDIActivate(MDIChild* pWnd);
	HMENU RefreshMenu(){return (HMENU)::SendMessage (hWndClient, WM_MDIREFRESHMENU, 0, 0);};
	MDIChild* getActiveChild(){return (MDIChild*)FromHandle((HWND) ::SendMessage (hWndClient, WM_MDIGETACTIVE, 0, 0));};


protected:
	virtual LRESULT WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool onCommand(UINT id, HWND hwndCtl, UINT codeNotify);
	virtual void onSize(UINT state, int cx, int cy);


	virtual bool PreTranslateMessage(MSG* pMsg);
};

