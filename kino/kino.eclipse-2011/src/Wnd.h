// Wnd.h: interface for the Wnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WND_H__E7D87214_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
#define AFX_WND_H__E7D87214_272F_11D2_A6D9_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WndX.h"
#include "wnd_cmdX.h"
class CmdUI;
#ifdef STRICT
typedef WNDPROC XWNDPROC;
#else
typedef FARPROC XWNDPROC;
#endif
class WinThread;
class Application;
class Wnd  
{
	HWND hWnd;
	XWNDPROC pWndProc;
protected:
//	bool handled;
//Operations 
protected:
	const X_CMDMAP_ENTRY* GetCmdEntry(UINT nID);
	const X_CMDMAP_ENTRY* GetReflectEntry(UINT codeNotify);
public:
	Wnd() : hWnd(NULL), pWndProc(NULL)/*, handled(false)*/{};
	virtual ~Wnd(){};

	virtual bool Create(DWORD dwExStyle, LPCTSTR lpWindowName, DWORD dwStyle,
				 int x, int y, int nWidth, int nHeight, Wnd* pWndParent,
				 HMENU hMenu, LPVOID lpParam = NULL);
// Operators
			operator HWND(){if(this) return hWnd; else return NULL;};
 

//Overidables
	virtual void PostNcDestroy(){delete this;};
	virtual HACCEL GetAccel(){/*No default accel*/return NULL;};
	virtual bool DestroyWindow(){return ::DestroyWindow(hWnd)>0;};
	virtual void SendMessageToDescendants(UINT message, WPARAM wParam = 0, LPARAM lParam = 0, UINT nIDFirst = 0, UINT nIDLast = 0);
	virtual void HiliteWindow(bool hi = true){};
	virtual bool PreTranslateMessage(MSG* pMsg){return false;};
	virtual void UpdateData(Wnd* pSender){};

//Reppers
			void ScreenToClient(RECT* prect){::MapWindowPoints(NULL, hWnd, (POINT*)prect, 2);};
			bool ShowWindow(int nCmdShow){ASSERT(::IsWindow(hWnd));return ::ShowWindow(hWnd, nCmdShow) > 0;};
			bool InvalidateRect(const RECT *lpRect, bool bErase = true ){return ::InvalidateRect(hWnd, lpRect, bErase )>0;};
			bool UpdateWindow(){ return ::UpdateWindow(hWnd)>0;};
			DWORD GetStyle()const{return ::GetWindowLong(hWnd, GWL_STYLE);};
			LRESULT SendMessage( UINT message, WPARAM wParam = 0, LPARAM lParam = 0 ){return ::SendMessage(hWnd, message, wParam, lParam);};
			LRESULT ResendMessage(HWND hWnd){MSG msg = GetLastMsg(); return ::SendMessage(hWnd, msg.message, msg.wParam, msg.lParam);};
			LRESULT RepostMessage(HWND hWnd){MSG msg = GetLastMsg(); return ::PostMessage(hWnd, msg.message, msg.wParam, msg.lParam);};
			Wnd* SetFocus(){return FromHandle(::SetFocus(hWnd));};
	static	Wnd* GetFocus(){return FromHandle(::GetFocus());};
			Wnd* GetParent(){return FromHandle(::GetParent(hWnd));};
			bool IsChild(Wnd* pWnd){return ::IsChild(hWnd, *pWnd) > 0;};
			
			Wnd* GetCapture(){return FromHandle(::GetCapture());};
			void SetCapture(){::SetCapture(hWnd);};
			int GetWindowText(char* pszText, int cchTextMax)
			{return DefWindowProc(WM_GETTEXT, (WPARAM) cchTextMax, (LPARAM) pszText);};
			int GetWindowTextLenght()
			{return DefWindowProc(WM_GETTEXTLENGTH);};
			int SetWindowText(const char* pszText)
			{return DefWindowProc(WM_SETTEXT, (WPARAM) 0, (LPARAM) pszText);};

//Message handlers
	virtual LRESULT WindowProc(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual bool OnCommand(UINT id, HWND hwndCtl, UINT codeNotify);
	virtual HBRUSH OnCtlColor(HDC hdc, HWND hwndChild, int type);
	virtual HBRUSH OnCtlColorReflect(HDC hdc, int type){return NULL;};
	virtual void OnInitMenuPopup( HMENU hmenuPopup, UINT nIndex, BOOL bSysMenu );
	virtual void OnNcDestroy(){PostNcDestroy();};
	virtual bool OnCmdUI(CmdUI* pCmdUI);

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	virtual LRESULT Default(){MSG msg = GetLastMsg(); return DefWindowProc(msg.message, msg.wParam, msg.lParam);};
	static  LRESULT WINAPI WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

// Static operations	
	static WinThread* GetThread();
	static Application* GetApp();
	static MSG GetLastMsg();
	static Wnd* FromHandle(HWND hWnd){return (Wnd*)GetWindowLong(hWnd, GWL_USERDATA);};
	
	static void RegisterClass();
	static void RegisterClass(UINT style, HICON hIcon, HCURSOR hCursor,
						HBRUSH  hbrBackground, LPCTSTR lpszMenuName,
						LPCTSTR lpszClassName, HICON hIconSm);
	virtual const char* GetClassName(){return "WND";};

	friend LRESULT CALLBACK WndCreateHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	DECLARE_COMMAND_MAP()
};

void HookWndCreate(Wnd* pWnd);
void UnhookWndCreate();

#endif // !defined(AFX_WND_H__E7D87214_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
