#pragma once
#include "MDIChild.h"
#include "TabMap.h"

class DataContext;
class Frame;
class MDIMain;

class FrameWnd : public MDIChild
{
	FrameWnd*	prev;
	FrameWnd*	next;
//Members
	DataContext*	_pdc;
	Frame*		pRootFrame;

	HWND		hWndTracking;
	TabMap		tabmap;
	TabPos		curpos;
	bool		bAutoSave; 
public:
	FrameWnd(DataContext* pdc, FrameWnd* pPrev = NULL);
	FrameWnd(/*ScreenAction* pa, char* str, CreateContext* pContext*/);
	virtual ~FrameWnd(); // Destruct by DestroyWindow

	DataContext* context(){ return _pdc; }
private:
	virtual void onClose();

public:
//	ScreenAction*	pAction;
public:

// Attributes
public:

// Interface
// Operations
public:
	void AdjustSize(SIZE& sz);
//	char* GetControls(char* str, RFieldControls& vCtrls);
//	void Action(Record* pr, RField* prf, int nID);
	bool onCmdUI(CmdUI* pCmdUI);
	void SetPos(TabPos pos){curpos = pos;};

	void UpdateMenu(HMENU hmenu);

//	operator Record*(){return pRec;};
//	operator RecordSet*(){return (RecordSet*)pRec;};

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
	void onSetFocus(HWND hwndOldFocus);
	void onGetMinMaxInfo(LPMINMAXINFO lpMinMaxInfo);
	void onSize(UINT nType, int cx, int cy);
	void onWindowPosChanged(const LPWINDOWPOS lpwpos);
	void onPaint();
	void onMDIActivate(bool fActive, HWND hwndActivate, HWND hwndDeactivate);
	void onKey(UINT vk, bool fDown, int cRepeat, UINT flags);
	void onLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags);
	void onMouseMove(int x, int y, UINT keyFlags);
	bool onSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg);
	
// Operations
protected:
	void ShowAll(bool sa_show);


//Destruction


	static RECT defspace;
public:
	void BringToTop(bool top);
	static void RegisterClass();
//	virtual const char* getClassName(){return "FRAMEWND";};
};
