#pragma once

class StMdfr;
struct FStyle;
class Wnd;

class FDC
{
	int		style;
	FStyle	Style;
	bool	modified;
	HDC		hDC;
	Wnd*	pWnd;

public:
	FDC(Wnd* pWnd, HDC hDC = NULL);
	~FDC(){::ReleaseDC(*pWnd, hDC);};
	void DrawButton(LPRECT rect, bool pushed = false);
	void DrawText(LPRECT pRect, const char* str);
	void FillRect(LPRECT rect);
	void DrawRect(LPRECT rect);
	void Border(LPRECT rect);
	int  SetStyle(int style, StMdfr* psr = NULL);
	bool RectVisible(LPRECT pr){return ::RectVisible(hDC, pr) ? true : false;};
	int SetBkMode(int iBkMode){return ::SetBkMode(hDC, iBkMode);};
	operator HDC() { return hDC; }
protected:
	void SetStyle(FStyle* pNewStyle);

};

