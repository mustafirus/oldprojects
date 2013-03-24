#if !defined(AFX_FRAMEWND_H__D4762F74_FD10_11D1_A66A_204C4F4F5020__INCLUDED_)
#define AFX_FRAMEWND_H__D4762F74_FD10_11D1_A66A_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FrameWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FrameWnd frame

class WinHandler;
class RKey;
class Table;
class FormAction;
class Frame;
class RecordSet;
class Record;
class RLink;
class FrameThread;
class MainWnd;
class DataContext;

#include "MDIChild.h"
#include "Defines.h"
#include "TabMap.h"

class FrameWnd : public MDIChild
{
	FrameWnd*	prev;
	FrameWnd*	next;
//Members
	HACCEL		hAccel;       // accelerator table
	HMENU		hMenu;       // context menu
	Frame*		pRootFrame;
	Record*		pRec;
	Record*		pRecdef;
	RLink*		pRLink;
	RFieldControls vCtrls;
	HWND		hWndTracking;
	TabMap		tabmap;
	TabPos		curpos;
	bool		bAutoSave; 
public:
	FrameWnd(DataContext* pdc, FrameWnd* pPrev);
protected:
	LRESULT		lRes;
//	virtual void Init(char* str, CreateContext* pContext);
public:
//	ScreenAction*	pAction;
public:
	FrameWnd(/*ScreenAction* pa, char* str, CreateContext* pContext*/);

// Attributes
public:

// Interface
// Operations
public:
	void AdjustSize(SIZE& sz);
	char* GetControls(char* str, RFieldControls& vCtrls);
	void Action(Record* pr, RField* prf, int nID);
	virtual bool DestroyWindow();
	bool OnCmdUI(CmdUI* pCmdUI);
	void SetPos(TabPos pos){curpos = pos;};

	void UpdateMenu(HMENU hmenu);

	operator Record*(){return pRec;};
	operator RecordSet*(){return (RecordSet*)pRec;};

	bool AskSave_n_Exit();
//	void Save();
//	void SetModified(){modified = true;};
//	void Show(bool show);
//	Wnd* GetWnd(){return this;};
//	Frame* GetFrame(){return pRootFrame;}

//	bool Start();
	virtual bool PreTranslateMessage(MSG* pMsg);
protected:

// Implementation
// Message handlers
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	bool OnCommand(UINT nID, HWND hwndCtl, UINT codeNotify);
	void OnSetFocus(HWND hwndOldFocus);
	void OnGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo);
	void OnSize(UINT nType, int cx, int cy);
	void OnWindowPosChanged(const LPWINDOWPOS lpwpos);
	bool OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnPaint();
	void OnMDIActivate(bool fActive, HWND hwndActivate, HWND hwndDeactivate);
	void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);
	void OnLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags);
	void OnMouseMove(int x, int y, UINT keyFlags);
	void OnClose();
	void OnCopy();
	void OnPaste();
	void OnDestroy();
	bool OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg);
	
// Operations
protected:
	void ShowAll(bool sa_show);
	FrameWnd* GetThreadWnd();
	MainWnd*  GetMainWnd();

//Destruction
	virtual ~FrameWnd(); // Destruct by DestroyWindow

	
//	bool OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	bool OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
//	bool OnLButtonDown(UINT nFlags, CPoint point);
//	bool OnLButtonDblClk(UINT nFlags);
//	void OnNorm(RecordSet* pRec);


	static RECT defspace;
public:
	void BringToTop(bool top);
	static void RegisterClass();
	virtual const char* GetClassName(){return "FRAMEWND";};
DECLARE_COMMAND_MAP()
	void OnCommands(UINT nID);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEWND_H__D4762F74_FD10_11D1_A66A_204C4F4F5020__INCLUDED_)
