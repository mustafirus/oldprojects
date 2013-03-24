#pragma once

#include "Wnd.h"

class Edit : public Wnd  
{
	typedef Wnd BASECLASS;
protected:
	enum Mod{ KEY_CTRL = 1, KEY_SHIFT = 2};
	bool	edit;
	bool	focused;
	bool	hilighted;
public:
	Edit(DWORD dwExStyle, DWORD dwStyle, int x, int y, int nWidth, int nHeight, Wnd* pWndParent, Menu*	pm);
	Edit(DWORD dwExStyle, DWORD dwStyle, RECT* prect, Wnd* pWndParent, Menu*	pm);
	virtual ~Edit(){};

	bool IsModified(){return DefWindowProc(EM_GETMODIFY, 0, 0) > 0;};
	void SetFont(HFONT hFont, bool fRedraw = true);
	void SetEditMode(bool m);

// Message hendlers
	virtual void onKey(UINT vk, bool fDown, int cRepeat, UINT flags);
	virtual void onChar(TCHAR ch, int cRepeat);
	virtual void onKillFocus(HWND hwndNewFocus);
	virtual void onSetFocus(HWND hwndOldFocus);
	virtual void onLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags);

	void onNCPaint(HRGN hrgn);

// Overidables
	virtual void onUndo(){};
	virtual bool onBeginEdit(int mod){ return true; };
	virtual bool onEndEdit(){ return true; };

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

