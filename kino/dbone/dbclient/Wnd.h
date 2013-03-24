#pragma once

#include "WndX.h"
#include "string.h"
#include "WinThread.h"
class CmdUI;
#ifdef STRICT
typedef WNDPROC XWNDPROC;
#else
typedef FARPROC XWNDPROC;
#endif
class WinThread;
class Application;
class Menu;

class Wnd  
{
	friend LRESULT CALLBACK WinThread::WndCreateHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	XWNDPROC pWndProc;
	Menu*	_pMenu;
//	bool handled;
//Operations 
	HWND hWnd;

//	const X_CMDMAP_ENTRY* getCmdEntry(UINT nID);
//	const X_CMDMAP_ENTRY* getReflectEntry(UINT codeNotify);

public:
	Wnd() : hWnd(NULL), pWndProc(NULL), _pMenu(NULL){};
	virtual ~Wnd();

	Wnd(const char* classname, DWORD dwExStyle, const char* name, DWORD dwStyle,
				 int x, int y, int nWidth, int nHeight, Wnd* pWndParent,
				 Menu*	pMenu, LPVOID lpParam = NULL);
// Operators
	
	operator HWND(){ ASSERT(this); if(this) return hWnd; else return NULL;};
 

//Overidables

	virtual HACCEL GetAccel(){/*No default accel*/return NULL;};
	virtual void SendMessageToDescendants(UINT message, WPARAM wParam = 0, LPARAM lParam = 0, UINT nIDFirst = 0, UINT nIDLast = 0);
	virtual void HiliteWindow(bool hi = true){};
	virtual bool preTranslateMessage(MSG* pMsg){return false;}
	virtual void UpdateData(Wnd* pSender){};


//Reppers
			void invalidate_parent();
			void show() { ASSERT(::IsWindow(hWnd)); ::ShowWindow(hWnd, SW_SHOW); }
			void hide() { ASSERT(::IsWindow(hWnd)); ::ShowWindow(hWnd, SW_HIDE); }
			void close(){ ASSERT(::IsWindow(hWnd)); SendMessage(WM_CLOSE);}
	virtual void destroy(){ ASSERT(::IsWindow(hWnd)); hide(); ::DestroyWindow(hWnd);}

			void ScreenToClient(RECT* prect){::MapWindowPoints(NULL, hWnd, (POINT*)prect, 2);};
			bool InvalidateRect(const RECT *lpRect, bool bErase = true ){return ::InvalidateRect(hWnd, lpRect, bErase )>0;};
			bool UpdateWindow(){ return ::UpdateWindow(hWnd)>0;};
			DWORD GetStyle()const{return ::GetWindowLong(hWnd, GWL_STYLE);};
			Wnd* SetFocus(){return FromHandle(::SetFocus(hWnd));};
	static	Wnd* GetFocus(){return FromHandle(::GetFocus());};
			Wnd* GetParent(){return FromHandle(::GetParent(hWnd));};
			bool IsChild(Wnd* pWnd){return ::IsChild(hWnd, *pWnd) > 0;};
			
			Wnd* GetCapture(){return FromHandle(::GetCapture());};
			void SetCapture(){::SetCapture(hWnd);};
			void GetWindowText(char* pszText, int cchTextMax)
			{ DefWindowProc(WM_GETTEXT, (WPARAM) cchTextMax, (LPARAM) pszText); };
			int GetWindowTextLenght()
			{return (int)DefWindowProc(WM_GETTEXTLENGTH);};
			void SetWindowText(const char* pszText)
			{ DefWindowProc(WM_SETTEXT, (WPARAM) 0, (LPARAM) pszText); };
protected:
			LRESULT SendMessage( UINT message, WPARAM wParam = 0, LPARAM lParam = 0 ){return ::SendMessage(hWnd, message, wParam, lParam);};
			LRESULT ResendMessage(HWND hWnd){MSG msg = GetLastMsg(); return ::SendMessage(hWnd, msg.message, msg.wParam, msg.lParam);};
			LRESULT RepostMessage(HWND hWnd){MSG msg = GetLastMsg(); return ::PostMessage(hWnd, msg.message, msg.wParam, msg.lParam);};


//Message handlers
public:
	virtual bool onCmdUI(CmdUI* pCmdUI);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual void onNcDestroy();
	virtual void onClose()		{ destroy(); }

	virtual bool onCommand(UINT id, HWND hwndCtl, UINT codeNotify);
	virtual HBRUSH onCtlColor(HDC hdc, HWND hwndChild, int type);
	virtual HBRUSH onCtlColorReflect(HDC hdc, int type){return NULL;};
	virtual void onInitMenuPopup( HMENU hmenuPopup, UINT nIndex, BOOL bSysMenu );

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual LRESULT Default(){MSG msg = GetLastMsg(); return DefWindowProc(msg.message, msg.wParam, msg.lParam);};
	static  LRESULT WINAPI WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

// Static operations	
	static WinThread* getThread() { return WinThread::getCurrent(); }
	static Application* getApp();
	static Wnd* FromHandle(HWND hWnd){return (Wnd*) (LONG_PTR)::GetWindowLongPtr(hWnd, GWL_USERDATA);};


	static MSG GetLastMsg();
	static void RegisterClass(UINT style, HICON hIcon, HCURSOR hCursor,
						HBRUSH  hbrBackground, LPCTSTR lpszMenuName,
						LPCTSTR lpszClassName, HICON hIconSm);

public:	
	static void RegisterClass();
//	virtual const char* getClassName(){return "WND";};
//	friend LRESULT CALLBACK WndCreateHookProc(int nCode, WPARAM wParam, LPARAM lParam);
//	DECLARE_COMMAND_MAP()
};

