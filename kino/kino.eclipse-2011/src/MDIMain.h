// MDIMain.h: interface for the MDIMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDIMAIN_H__E7D87215_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
#define AFX_MDIMAIN_H__E7D87215_272F_11D2_A6D9_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Wnd.h"
class MDIChild;
#define ID_FIRSTCHILD                   0xE000

class MDIMain : public Wnd  
{
	HACCEL hAccel;
protected:
	HWND	hWndClient;
//	MDIMain();
public:
	MDIMain(UINT resID);
	virtual ~MDIMain(){};

//Operations 
	void MDIActivate(MDIChild* pWnd);
	HMENU RefreshMenu(){return (HMENU)::SendMessage (hWndClient, WM_MDIREFRESHMENU, 0, 0);};
	bool CreateChildFrame(MDIChild* pWnd, const char* szTitle, DWORD dwStyle, int x, int y, int cx, int cy, LPARAM lParam = NULL);
	MDIChild* GetActiveChild(){return (MDIChild*)FromHandle((HWND) ::SendMessage (hWndClient, WM_MDIGETACTIVE, 0, 0));};

protected:
	virtual LRESULT WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual bool OnCommand(UINT id, HWND hwndCtl, UINT codeNotify);
	virtual bool OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnSize(UINT state, int cx, int cy);
	virtual void OnDestroy();

	virtual bool PreTranslateMessage(MSG* pMsg);

	DECLARE_COMMAND_MAP()
};

#endif // !defined(AFX_MDIMAIN_H__E7D87215_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
