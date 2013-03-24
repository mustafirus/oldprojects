// Grid.cpp: implementation of the Grid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Dblib.h"
#include "Application.h"
#include "Resource.h"
#include "MainWnd.h"
#include "KinoRes.h"
#include "StMdfr.h"
#include "Column.h"
#include "Action.h"
#include "TabMap.h"
#include "Wait.h"
#include "Grid.h"

#define BASECLASS Wnd
#define BORDER	7
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/////////////////////////////////////////////////
// Construction/Destruction
// Grid
Grid::Grid(int defrows, Wnd* pWndParent, UINT nID) :
rows(0), row_height(0), def_rows(defrows), header_height(0), focus(0)
{
//	memset(&rsize, 0, sizeof(rsize));
	VERIFY(Create(0, NULL, WS_CHILD|WS_VISIBLE|WS_HSCROLL|WS_VSCROLL,
				 0, 0, 0, 0, pWndParent, (HMENU) nID)); 
}
Grid::~Grid()
{
};
/////////////////////////////////////////////////////////////
//Operations
/////////////////////////////////////////////////////////////
void Grid::RedrawFrame()
{
	RECT r;
	GetWindowRect(*this, &r);
	ScreenToClient(&r);
	HRGN hr1 = ::CreateRectRgnIndirect(&r);
	InflateRect(&r, -BORDER, -BORDER);
	HRGN hr2 = ::CreateRectRgnIndirect(&r);
	::CombineRgn(hr1, hr1, hr2, RGN_DIFF);
	::RedrawWindow(*this, NULL, hr1, /*RDW_UPDATENOW||RDW_ERASE|
		RDW_ALLCHILDREN*/RDW_INVALIDATE|RDW_FRAME);
	::DeleteObject(hr1);
	::DeleteObject(hr2);
}

/*void Grid::GetRSize(SIZE* psz)
{
	for(int i = 0; i < cols.GetCount(); i++)
	{
		cols[i]->GetRSize(psz);
		rsize.cx += psz->cx;
		rsize.cy = max(rsize.cy, psz->cy);
	}
	RECT r = {0,0,rsize.cx, rsize.cy};
	AdjustWindowRect(&r, GetStyle(), FALSE);
	InflateRect(&r, BORDER, BORDER);
	rsize.cx = r.right - r.left + 1;
	rsize.cy = r.bottom - r.top + 1;
	*psz = rsize;
}
*/
ROWINFO Grid::GetRowInfo()
{
	ROWINFO r = {5, 20, 10, 0};
	return r;
}
void Grid::GetMinMax(PMINMAX pmm)
{
	if(minmax.szMax.cx != 0 && minmax.szMax.cy != 0)
	{
		*pmm = minmax;
		return;
	}
	minmax.szMin.cx = 20;//pmm->szMin.cx;
	minmax.szMax.cx = GetSystemMetrics(SM_CXMAXTRACK) - 2*BORDER - 20;//pmm->szMax.cx;
	for(int i = 0; i < cols.GetCount(); i++)
	{
		cols[i]->GetMinMax(pmm);
		minmax.szDes.cx += pmm->szDes.cx;
		minmax.szMin.cy = max(minmax.szMin.cy, pmm->szMin.cy);
		minmax.szMax.cy = max(minmax.szMax.cy, pmm->szMax.cy);
		minmax.szDes.cy = max(minmax.szDes.cy, pmm->szDes.cy);
	}
	minmax.szDes.cx = min(minmax.szDes.cx, minmax.szMax.cx);
	minmax.szDes.cx = max(minmax.szDes.cx, minmax.szMin.cx);
	minmax.szMin.cx += 2*BORDER + 16;
	minmax.szMin.cy += 2*BORDER;
	minmax.szMax.cx += 2*BORDER + 16;
	minmax.szMax.cy += 2*BORDER;
	minmax.szDes.cx += 2*BORDER + 16;
	minmax.szDes.cy += 2*BORDER;
	*pmm = minmax;
}

void Grid::SetTabs(TabMap* pTab, TabPos first, TabPos& last)
{
	last = pos = first;
	pTab->SetAt(pos, this);
}

void Grid::OnWindowPosChanged(const LPWINDOWPOS pwpos)
{
	if(row_height && pwpos->cy)
		Reposition();
	Default();
}
void Grid::Reposition()
{
	HDWP hWinPosInfo = ::BeginDeferWindowPos(cols.GetCount());
	ASSERT(hWinPosInfo);
//	SIZE sz;
	RECT r;
	GetClientRect(*this, &r);
	int x = 0;
	int fl = 0;
	int fr = 0;
	for(int i = 0; i < cols.GetCount(); i++)
	{
		MINMAX mm;
		cols[i]->GetMinMax(&mm);
		if(i == focus)
			fl = x;
		x += mm.szDes.cx;
		if(i == focus)
			fr = x;
	}
	minmax.szDes.cx = x;
	x -= r.right;
	if(x>0)
	{
		EnableScrollBar(*this, SB_HORZ, ESB_ENABLE_BOTH);
		SCROLLINFO si;
		si.cbSize = sizeof SCROLLINFO;
		si.fMask = SIF_PAGE|SIF_RANGE;
		si.nMin = 0;
		si.nMax = x;
		si.nPage = 0;
		SetScrollInfo(*this, SB_HORZ, &si, TRUE);
	}else
		EnableScrollBar(*this, SB_HORZ, ESB_DISABLE_BOTH);

	int left;
	int right = -GetScrollPos(*this,SB_HORZ);
	if((right + fr) > r.right)
	{
		right = r.right - fr;
	}
	if((right + fl) < 0)
	{
		right = - fl;
	}
	SetScrollPos(*this,SB_HORZ, -right, TRUE);
	int c = cols.GetCount();
	fmin = -1;
	fmax = c + 1;
	for(i = 0; i < c; i++)
	{
		MINMAX mm;
		cols[i]->GetMinMax(&mm);
		left = right;
		right += mm.szDes.cx;
		if(left >= 0 && fmin < 0)
			fmin = i;
		if(right > r.right && fmax > c)
			fmax = i - 1;
		hWinPosInfo = DeferWindowPos(hWinPosInfo, *cols[i], NULL,
			left, 0, mm.szDes.cx, r.bottom - r.top, SWP_NOACTIVATE|SWP_NOZORDER);
	}
//	minmax.szDes.cx = right;

//	if(right > pwpos->cx)
//		ShowScrollBar(
	rows = (r.bottom - r.top - header_height /*- row_height/2*/)/row_height /*+ 1*/;
	VERIFY(::EndDeferWindowPos(hWinPosInfo));
	InvalidateRect(NULL);
}

void Grid::OnResetMaxRows()
{
	if(ri.max_rows > rows)
	{
		EnableScrollBar(*this, SB_VERT, ESB_ENABLE_BOTH);
		SCROLLINFO si;
		si.cbSize = sizeof SCROLLINFO;
		si.fMask = SIF_PAGE|SIF_RANGE;
		si.nMin = 0;
		si.nMax = ri.max_rows;
		si.nPage = rows;
		SetScrollInfo(*this, SB_VERT, &si, TRUE);
	}else
		EnableScrollBar(*this, SB_VERT, ESB_DISABLE_BOTH);
}
///////////////////////////////////////////////////////////
//Message handlers
///////////////////////////////////////////////////////////
BEGIN_COMMAND_MAP(Grid, Wnd)
	ON_COMMAND(ID_EDIT_COPY, OnCopy)
END_COMMAND_MAP
LRESULT Grid::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_KEYDOWN, OnKey)
		ON_MSG(WM_SETFOCUS, OnSetFocus)
		ON_MSG(WM_MOUSEACTIVATE, OnMouseActivate)
		ON_MSG(WM_NCPAINT, OnNCPaint)
		ON_MSG(WM_NCCALCSIZE, OnNCCalcSize)
		ON_MSG(WM_WINDOWPOSCHANGED, OnWindowPosChanged)
		ON_MSG(WM_PAINT, OnPaint)
		ON_MSG(WM_CONTEXTMENU, OnContextMenu)
		ON_MSG(WM_COPY, OnCopy)
		ON_MSG(WM_HSCROLL, OnHScroll)
		ON_MSG(WM_VSCROLL, OnVScroll)
	BASEROUGHT
}
void Grid::OnHScroll(HWND hwndCtl, UINT code, int pos)
{
	switch(code)
	{
	case SB_LINELEFT:
		SetScrollPos(*this, SB_HORZ, GetScrollPos(*this, SB_HORZ) - 1, TRUE);
		Reposition();
		break;
	case SB_LINERIGHT:
		SetScrollPos(*this, SB_HORZ, GetScrollPos(*this, SB_HORZ) + 1, TRUE);
		Reposition();
		break;
	case SB_PAGELEFT:
		SetScrollPos(*this, SB_HORZ, GetScrollPos(*this, SB_HORZ) - 100, TRUE);
		Reposition();
		break;
	case SB_PAGERIGHT:
		SetScrollPos(*this, SB_HORZ, GetScrollPos(*this, SB_HORZ) + 100, TRUE);
		Reposition();
		break;
	case SB_THUMBPOSITION:
		SetScrollPos(*this, SB_HORZ, pos, TRUE);
		Reposition();
		break;
		
	}
}

void Grid::OnVScroll(HWND hwndCtl, UINT code, int pos)
{
	switch(code)
	{
	case SB_LINEUP:
		SetScrollPos(*this, SB_VERT, GetScrollPos(*this, SB_VERT) - 1, TRUE);
		break;
	case SB_LINEDOWN:
		SetScrollPos(*this, SB_VERT, GetScrollPos(*this, SB_VERT) + 1, TRUE);
		break;
	case SB_PAGEUP:
		SetScrollPos(*this, SB_VERT, GetScrollPos(*this, SB_VERT) - rows, TRUE);
		break;
	case SB_PAGEDOWN:
		SetScrollPos(*this, SB_VERT, GetScrollPos(*this, SB_VERT) + rows, TRUE);
		break;
	case SB_THUMBPOSITION:
		SetScrollPos(*this, SB_VERT, pos, TRUE);
		break;
	}
}

void Grid::OnKey(UINT vk, bool fDown, int cRepeat, UINT flags)
{
	bool WE = false;
	int newf;
	switch(vk)
	{
	case VK_LEFT:
		newf = focus > 0 ? focus - 1 : focus;
		WE = true;
		break;
	case VK_RIGHT:
		newf = focus < cols.GetCount() - 1 ? focus + 1 : focus;
		WE = true;
		break;
	}
	if(WE)
	{
		cols[focus=newf]->SetFocus();
		if(focus < fmin || focus > fmax)
			Reposition();
	}
}

void Grid::OnSetFocus(HWND hwndOldFocus)
{
	cols[focus]->SetFocus();
	HiliteWindow();
	Default();
}

void Grid::OnActivate(int row, Column* col)
{
	if(row>=0 && row != GetCurRow())
	{
		RECT r;
		GetClientRect(*this, &r);
		r.top = header_height + row_height * GetCurRow() + 1;
		r.bottom = r.top + row_height;
		InvalidateRect(&r);
		OnSetRow(row);
		r.top = header_height + row_height * row;
		r.bottom = r.top + row_height;
		InvalidateRect(&r);
	}
	for(int i = 0; i<cols.GetCount(); i++)
	{
		if(cols[i] != col)
			continue;
		if(focus != i)
		{
			focus = i;
			col->SetFocus();
			break;
		}
	}
}

int Grid::OnMouseActivate(HWND hwndTopLevel, UINT codeHitTest, UINT msg)
{
//	SetFocus();
	return Default();
}

void Grid::OnNCPaint(HRGN hrg)
{
	Default();
	RECT rect, r;
	GetWindowRect(*this, &rect);
	ScreenToClient(&rect);
	GetClientRect(*this, &r);
	r.left = r.right - rect.left;
	r.top = r.bottom - rect.top;
	OffsetRect(&rect, -rect.left, -rect.top);
	*(((POINT*)&r)+1) = *(((POINT*)&rect)+1);

	HDC hdc = ::GetWindowDC(*this);
	HRGN hr1 = ::CreateRectRgnIndirect(&r);
	SelectClipRgn(hdc, hr1);
	::DeleteObject(hr1);
	::SendMessage(::GetParent(*this), WM_ERASEBKGND, (WPARAM)hdc, 0);

	r = rect;
		 hr1 = ::CreateRectRgnIndirect(&r);
	InflateRect(&r, -BORDER, -BORDER);
	HRGN hr2 = ::CreateRectRgnIndirect(&r);
	::CombineRgn(hr1, hr1, hr2, RGN_DIFF);
	::SelectClipRgn(hdc, hr1);
	::DeleteObject(hr1);
	::DeleteObject(hr2);
	r = rect;
	FillRect(hdc, &r, GetStockObject(LTGRAY_BRUSH));
	if(hilited)
	{
		DrawEdge(hdc, &r, EDGE_SUNKEN,BF_TOPLEFT);
		DrawEdge(hdc, &r, EDGE_BUMP,BF_BOTTOMRIGHT);
	}
	else
	{
		DrawEdge(hdc, &r, EDGE_RAISED,BF_RECT);
	}
	::ReleaseDC(*this, hdc);
}

UINT Grid::OnNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp)
{
	LRESULT l = Default();
	InflateRect((RECT*)lpcsp, -BORDER, -BORDER);
	return l;
}

void Grid::OnDraw(HDC hDC)
{
	RECT r;
	GetClientRect(*this, &r);
	POINT pt[3] = {{r.left, r.top},{r.left, r.bottom-1}, {r.right-1, r.bottom-1}};
	int cr = GetCurRow();
	if(cr >= 0)
	{
		r.top = header_height + row_height * GetCurRow();
		r.bottom = r.top + row_height;
		r.right = minmax.szDes.cx;
		Rectangle(hDC, r.left, r.top, r.right, r.bottom);
	}
	::Polyline(hDC, pt, 3);
}
void Grid::OnPaint()
{
	PAINTSTRUCT paint;
	HDC hDC = BeginPaint(*this, &paint);
	OnDraw(hDC);
	EndPaint(*this, &paint);
}

/////////////////////////////////////////////////////////////////
// Construction/Destruction
// RGrid

RGrid::RGrid(char* str, RecordSet* pr, int defrows, Wnd* pWndParent, UINT nID) :
Grid(defrows, pWndParent, nID), pRec(pr)
{
	RFields rf;
	StMdfr* pStMdfr = NULL;
/*	char* s1=strchr(str, '>');
	*s1 = 0;
	*/	
	pRec->GetRFields(str, rf);
//	*s1 = '>';
	for(int i = 0; i < rf.GetCount(); i++)
	{
		if(((Field*)*(rf[i]))->stmdf)
		{
			char f[30]; int n,m;
			VERIFY(sscanf(((Field*)*(rf[i]))->stmdf,
				" %[^, ] , %d , %d",f,&m,&n) == 3);
			RFields rrf;
			pRec->GetRFields(f, rrf, ( (QTable*)(*(QField*)*(rf[i])) ) );
			ASSERT(rrf.GetCount() == 1);
			pStMdfr = new StMdfr(rrf[0], m, n);
//			pStMdfr->SetData(*(pStMdfr->GetRField()));
		}else
			pStMdfr = NULL;
		cols.Add(	
			new RColumn( ((RSField*)rf[i]), pStMdfr, this, i+1,
			rows, def_rows, row_height, header_height, 0));
	}
	OnResetMaxRows();
	pRec->Load();
	cols[focus]->SetFocus();
}
///////////////////////////////////////////////////////////
//Message handlers
///////////////////////////////////////////////////////////
#include "FrameWnd.h"

BEGIN_COMMAND_MAP(RGrid, Grid)
	ON_COMMAND(ID_FORM_CARD, OnCard)
END_COMMAND_MAP

void RGrid::OnCard()
{
	Action::GetAction()->Exec(((FrameWnd*)GetParent()), pRec, NULL, ID_FORM_CARD);
}

void RGrid::OnPaint()
{
/*	if(pRec->IsDummy())
		if(!pRec->Load())
		{
			ValidateRect(*this, NULL);
			return;
		}
*/
/*	if(rows)
		pRec->SetPageSize(def_rows);
	rows = pRec->GetPageSize();
*/	Grid::OnPaint();
}
void RGrid::OnKey(UINT vk, bool fDown, int cRepeat, UINT flags)
{
	Wait w;
	if(!fDown)
		return;
	Grid::OnKey(vk, fDown, cRepeat, flags);
	bool NS = false;
	int old = pRec->GetPageCur();
	switch(vk)
	{
	case VK_UP:
		NS = pRec->MoveCur(-1);
		break;
	case VK_DOWN:
		NS = pRec->MoveCur(1);
		break;
	case VK_PRIOR:
		NS = pRec->MoveCur(-rows);
		break;
	case VK_NEXT:
		NS = pRec->MoveCur(rows);
		break;
	case VK_HOME:
		NS = pRec->MoveCur(INT_MIN);
		break;
	case VK_END:
		NS = pRec->MoveCur(INT_MAX);
		break;
	case VK_RETURN:
		{
			if(GetKeyState(VK_CONTROL)&0x8000)
				Activate(VK_CONTROL);
			else if(GetKeyState(VK_SHIFT)&0x8000)
				Activate(VK_SHIFT);
			else
				Activate(0);
			return;
		}
	case VK_INSERT:
		if(GetKeyState(VK_CONTROL)&0x8000  || GetKeyState(VK_SHIFT)&0x8000)
			break;
		Action::GetAction()->Exec( (FrameWnd*)GetParent(), pRec, NULL, ID_FORM_CARD|ACT_FLAG_BLANK);
//		((FrameWnd*)GetParent())->Action(pRec, NULL, -3);
//		{ResendMessage(::GetParent(*this));return;}
		return;
	case VK_DELETE:
		if(GetKeyState(VK_CONTROL)&0x8000  || GetKeyState(VK_SHIFT)&0x8000)
			break;
		pRec->Delete();
		InvalidateRect(NULL);
//		{ResendMessage(::GetParent(*this));return;}
		break;
	}
	if(NS)
	{
		RECT r;
		GetClientRect(*this, &r);
		int newc = pRec->GetPageCur();
		if(old == newc)
		{
			r.top += header_height;
			InvalidateRect(&r);
		}else
		{
			r.top = header_height + old * row_height + 1;
			r.bottom = r.top + row_height;
			InvalidateRect(&r);
			r.top = header_height + newc * row_height + 1;
			r.bottom = r.top + row_height;
			InvalidateRect(&r);
		}
	}else
	ResendMessage(::GetParent(*this));
}

void RGrid::OnWindowPosChanged(const LPWINDOWPOS pwpos)
{
	Grid::OnWindowPosChanged(pwpos);
	if(row_height && pwpos->cy)
	{
		if(rows)
			pRec->SetPageSize(rows);
		rows = pRec->GetPageSize();
	}
}

int RGrid::GetCurRow()
{
	return pRec->GetPageCur();
}

void RGrid::OnSetRow(int row)
{
	pRec->SetPageCur(row);
}

void RGrid::OnResetMaxRows()
{
	Grid::OnResetMaxRows();
}

void RGrid::OnVScroll(HWND hwndCtl, UINT code, int pos)
{
	bool NS = false;
	int old = pRec->GetPageCur();
	switch(code)
	{
	case SB_LINEUP:
		NS = pRec->MoveCur(-1);
		break;
	case SB_LINEDOWN:
		NS = pRec->MoveCur(1);
		break;
	case SB_PAGEUP:
		NS = pRec->MoveCur(-rows);
		break;
	case SB_PAGEDOWN:
		NS = pRec->MoveCur(rows);
		break;
	case SB_THUMBPOSITION:
		return;
	}
	Grid::OnVScroll(hwndCtl, code, pos);
	if(NS)
	{
		int pos = GetScrollPos(*this, SB_VERT);
		if( pos >= ri.max_rows)
			SetScrollPos(*this, SB_HORZ, pos - 3, TRUE);
		if( pos <= 1)
			SetScrollPos(*this, SB_HORZ, 3, TRUE);
		RECT r;
		GetClientRect(*this, &r);
		int newc = pRec->GetPageCur();
		if(old == newc)
		{
			r.top += header_height;
			InvalidateRect(&r);
		}else
		{
			r.top = header_height + old * row_height + 1;
			r.bottom = r.top + row_height;
			InvalidateRect(&r);
			r.top = header_height + newc * row_height + 1;
			r.bottom = r.top + row_height;
			InvalidateRect(&r);
		}
	}
}

void RGrid::OnContextMenu(HWND hwndContext, UINT xPos, UINT yPos)
{
	HMENU hm = GetApp()->LoadMenu(IDR_GRID);
	HMENU sm = GetSubMenu(hm, 0);


	QTable* pqt = *((Query*)*pRec);
	const char* str;
	MENUITEMINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	mi.fMask = MIIM_TYPE;
	mi.fType = MFT_SEPARATOR;
	InsertMenuItem(sm, 0, TRUE, &mi);
	int i = 0;
	while(str = pqt->GetName(i++))
	{
		mi.fType = MFT_STRING;
		mi.dwTypeData = (char*)str;
		InsertMenuItem(sm, 0, TRUE, &mi);
	}
	SendMessage(WM_INITMENUPOPUP, (WPARAM)sm, 0);
	UINT n = TrackPopupMenu(sm, TPM_LEFTALIGN|TPM_TOPALIGN|
		/*TPM_NONOTIFY|TPM_RETURNCMD|*/TPM_LEFTBUTTON,
		xPos, yPos, 0, *this, NULL);
/*	switch(n)
	{
	case ID_EDIT_COPY:
		OnCopy();
	}
*/
}

void RGrid::OnCopy()
{
	Wait w;
	pRec->Copy(*this);
}


void RGrid::Activate(UINT syskey)
{
	if(pRec->Save())
	{
		GetParent()->SendMessage(WM_CLOSE);
		return;
	}
	UINT id = ID_FORM_CARD;
	switch(syskey)
	{
	case VK_CONTROL:
		id |= ACT_FLAG_THREAD;
		break;
/*	case VK_SHIFT:
		id = -2;
*/	}
	Action::GetAction()->Exec(((FrameWnd*)GetParent()), pRec, NULL, id);
}

/*bool FrameWnd::OnCmdUI(CmdUI* pCmdUI)
{
	if(GetFocus()->OnCmdUI(pCmdUI))
		return true;
	UINT nID = *pCmdUI;
	if(nID >= ID_EDIT_FIRST && nID <= ID_EDIT_LAST)
	{
		pCmdUI->Enable();
		return true;
	}
	return MDIChild::OnCmdUI(pCmdUI);
}
*/