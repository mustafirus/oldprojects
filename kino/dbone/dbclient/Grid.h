// Grid.h: interface for the Grid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_H__2E28BAF4_43DC_11D2_A71D_204C4F4F5020__INCLUDED_)
#define AFX_GRID_H__2E28BAF4_43DC_11D2_A71D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Wnd.h"
#include "Frame.h"
#include "Defines.h"

//#define GRIDDEBUG
typedef struct ROWINFO_tag
{
	short min_rows;
	short max_rows;
	short des_rows;
	short reserved;
}ROWINFO, *PROWINFO;
class Grid : public Wnd, public Frame
{
	HACCEL hAccel;
//	SIZE	rsize;
protected:
	int rows;
	int row_height;
	int def_rows;
	int header_height;
	Columns cols;
	bool	hilited;
	int		focus;
	int		fmin;
	int		fmax;
	ROWINFO ri;
	virtual HACCEL GetAccel(){return hAccel;};
public:

	Grid(int defrows, Wnd* pWndParent, UINT nID);
	virtual ~Grid();
//Operations
	void Reposition();
	void RedrawFrame();
	void GetRSize(SIZE* sz);
	void SetSize(int cx, int cy){SetWindowPos(*this, NULL, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE|SWP_NOACTIVATE);};
//	void SetPos(int x, int y){SetWindowPos(*this, NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE);};
	virtual void HiliteWindow(bool hi = true){hilited = hi;RedrawFrame();};
	virtual int GetCurRow(){return 0;};
	ROWINFO GetRowInfo();

	virtual void PostNcDestroy(){};
//Inherited overidables
	virtual void OnSetRow(int row){};
	virtual void OnResetMaxRows();
//Frame overidables
	virtual void GetMinMax(PMINMAX pmm);
	virtual void SetPos(POINT pt, SIZE sz, HDWP hdwp = NULL)
	{SetWindowPos(*this, NULL, pt.x, pt.y, sz.cx, sz.cy,
	SWP_NOZORDER|SWP_NOACTIVATE);};
	virtual void SetTabs(TabMap* pTab, TabPos first, TabPos& last);
//Message handlers
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnHScroll(HWND hwndCtl, UINT code, int pos);
	virtual void OnVScroll(HWND hwndCtl, UINT code, int pos);
			void OnActivate(int row, Column* col);
	virtual void OnContextMenu(HWND hwndContext, UINT xPos, UINT yPos){};
	virtual void OnCopy(){};
	virtual void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);
			void OnSetFocus(HWND hwndOldFocus);
			int OnMouseActivate(HWND hwndTopLevel, UINT codeHitTest, UINT msg);
	virtual void OnWindowPosChanged(const LPWINDOWPOS lpwpos);
			void OnNCPaint(HRGN hrgn);
			UINT OnNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp);
//			void OnSize(UINT nType, int cx, int cy);
	virtual void OnPaint();
	virtual void OnDraw(HDC hdc);

	static void RegisterClass()
	{Wnd::RegisterClass(0, 0, 0,
		CreateSolidBrush(RGB(255,255,240)), 0,"GRID", 0);};
	
	DECLARE_COMMAND_MAP()
};

class RecordSet;

#if 0
class RGrid : public Grid
{
	RecordSet*	pRec;
public:
	void Activate(UINT syskey);
	RGrid(char* str, RecordSet* pr, int defrows, Wnd* pWndParent, UINT nID);
	~RGrid(){};

//Operations

	int GetCurRow();
	void OnPaint();
	void OnSetRow(int row);
	virtual void OnResetMaxRows();
	void OnVScroll(HWND hwndCtl, UINT code, int pos);
	void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);
	void OnContextMenu(HWND hwndContext, UINT xPos, UINT yPos);
	void OnCopy();
	void OnWindowPosChanged(const LPWINDOWPOS lpwpos);
	virtual void PostNcDestroy(){delete pRec;};

	void OnCard();
	DECLARE_COMMAND_MAP()
};
#endif //0
#endif // !defined(AFX_GRID_H__2E28BAF4_43DC_11D2_A71D_204C4F4F5020__INCLUDED_)
