// FDC.h: interface for the FDC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FDC_H__62D8B952_4647_11D2_A720_204C4F4F5020__INCLUDED_)
#define AFX_FDC_H__62D8B952_4647_11D2_A720_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class StMdfr;
struct FStyle;
class Wnd;
#include "WndLib.h"

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
protected:
	void SetStyle(FStyle* pNewStyle);

};

#endif // !defined(AFX_FDC_H__62D8B952_4647_11D2_A720_204C4F4F5020__INCLUDED_)
