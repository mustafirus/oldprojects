// Column.cpp: implementation of the Column class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "dblib.h"
#include "wndlib.h"
#include "resource.h"
//#include "StMdfr.h"
#include "Styles.h"
#include "Grid.h"
//#include "Wait.h"
//#include "Finder.h"
#include "Column.h"


//////////////////////////////////////////////////////////////////////
// Column
#ifdef BASECLASS
#undef BASECLASS
#endif
#define BASECLASS Wnd
#define COL_MIN_SIZE 3
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static RECT space = {3, 3, 0, 0};
Column::Column(Wnd* pWndParent, UINT nID, int& rs, int& dr, int& rh, int& hh, int st) :
rows(rs), row_height(rh), header_height(hh), style(st),
focused(false), pushed(false), sizing(false), pushing(false)
{
//	memset(&rsize, 0, sizeof(rsize));
	VERIFY(Create(0, NULL, WS_VISIBLE|WS_CHILD/*|WS_CLIPCHILDREN*/|WS_CLIPSIBLINGS,
				 0, 0, 0, 0, pWndParent, (HMENU) nID));
	memset(&minmax,0, sizeof(minmax));
};

/*void Column::InitDims()
{
	TEXTMETRIC* ptm = GetTextMetrics(style);
	dims.capt.cx = ptm->tmAveCharWidth * 10 * 3 / 2;
	dims.capt.cy = ptm->tmHeight * 6 / 5;
	dims.data.cx = ptm->tmAveCharWidth * 12 * 3 / 2;
	dims.data.cy = ptm->tmHeight * 6 / 5;
};
*/
/*void Column::GetDims(RDIMS* pdims)
{
	TEXTMETRIC* ptm = GetTextMetrics(style);
	pdims->capt.cx = ptm->tmAveCharWidth * 10 * 3 / 2;
	pdims->capt.cy = ptm->tmHeight * 6 / 5;
	pdims->data.cx = ptm->tmAveCharWidth * 12 * 3 / 2;
	pdims->data.cy = ptm->tmHeight * 6 / 5;
};
*/
/////////////////////////////////////////////////////////////
//Operations
/////////////////////////////////////////////////////////////
void Column::RedrawFrame()
{
		::RedrawWindow(*this, NULL, NULL, RDW_UPDATENOW|RDW_FRAME|RDW_INVALIDATE|RDW_ERASE);
/*
	RECT r;
	GetClientRect(*this, &r);
	r.bottom = header_height;
	RedrawWindow(*this, &r, NULL, RDW_INVALIDATE);
*/
}

/*void Column::GetRSize(SIZE* psz)
{
	if(rsize.cx && rsize.cy)
	{
		*psz = rsize;
		return;
	}
	RDIMS dims;
	GetDims(&dims);
	header_height = max(header_height, dims.capt.cy);
	row_height = max(row_height, dims.data.cy);
	psz->cx = max(dims.data.cx, dims.capt.cx);
	psz->cy = dims.data.cy * def_rows + dims.capt.cy + header_height;
	RECT r = {0,0,psz->cx, psz->cy};
	AdjustWindowRect(&r, GetStyle(), FALSE);
	psz->cx = r.right - r.left;
	psz->cy = r.bottom - r.top;
	rsize = *psz;
}

*/

bool Column::IsSizeGrip(int x, int y)
{
	RECT r;
	GetClientRect(*this, &r);
	return y > 0 && y < header_height && x > r.right - 5 && x <= r.right;
}

bool Column::IsHeader(int x, int y)
{
	RECT r;
	POINT pt = {x, y};
	GetClientRect(*this, &r);
	r.bottom = header_height;
	return PtInRect(&r, pt) > 0;
}

///////////////////////////////////////////////////////////
//Message handlers
///////////////////////////////////////////////////////////
BEGIN_COMMAND_MAP(Column, Wnd)
END_COMMAND_MAP
LRESULT Column::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_SETCURSOR, OnSetCursor)
		ON_MSG(WM_KILLFOCUS, OnKillFocus)
		ON_MSG(WM_SETFOCUS, OnSetFocus)
		ON_MSG(WM_MOUSEMOVE, OnMouseMove)
		ON_MSG(WM_LBUTTONDOWN, OnLButtonDown)
		ON_MSG(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		ON_MSG(WM_LBUTTONUP, OnLButtonUp)
		ON_MSG(WM_PAINT, OnPaint)
		ON_MSG(WM_KEYDOWN, OnKey)
	BASEROUGHT 
}

void Column::OnKey(UINT vk, bool fDown, int cRepeat, UINT flags)
{
	Default();
	((Grid*)GetParent())->OnKey(vk, fDown, cRepeat, flags);
}

void Column::OnKillFocus(HWND hwndNewFocus)
{
	RECT r;
	GetClientRect(*this, &r);
	r.bottom = header_height;
	InvalidateRect(&r);
	focused = false;
//	RedrawFrame();
	Default();
}

void Column::OnSetFocus(HWND hwndOldFocus)
{
	RECT r;
	GetClientRect(*this, &r);
	r.bottom = header_height;
	InvalidateRect(&r);
	focused = true;
	GetParent()->HiliteWindow();
//	RedrawFrame();
	Default();
}

bool Column::OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg)
{
	return Default() > 0;
}

void Column::OnLButtonDblClk(bool fDoubleClick, int x, int y, UINT keyFlags)
{
	if(IsHeader(x, y))
		return;
/*	if(MK_CONTROL & keyFlags)
		((RGrid*)GetParent())->Activate(VK_CONTROL);
	else if(MK_SHIFT & keyFlags)
		((RGrid*)GetParent())->Activate(VK_SHIFT);
	else
		((RGrid*)GetParent())->Activate(0);
*/	return;
}

void Column::OnLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags)
{
	int row = (y <= header_height) ? -1 : ((y-header_height)/row_height);
//	int rows = ((Grid*)GetParent())->GetRows();
	if(row < rows)
		((Grid*)GetParent())->OnActivate(row, this);
	if(IsSizeGrip(x, y))
	{
		SetCapture();
		RECT r;
		GetClientRect(*this, &r);
		{
			RECT rr;
			GetClientRect(::GetParent(*this), &rr);
			ClientToScreen(::GetParent(*this), ((POINT*)&rr) + 1);
			int right = rr.right;
			rr = r;
			ClientToScreen(*this, (POINT*)&rr);
			rr.left = rr.left + COL_MIN_SIZE;
			rr.right = right;
			rr.top = INT_MIN;
			rr.bottom = INT_MAX;
			::ClipCursor(&rr);
		}
		r.left = r.right - 1;
		hRgnSize = CreateRectRgnIndirect(&r);
		PointSize.x = x;
		PointSize.y = y;
		HDC hDC=::GetDC(*this);
		InvertRgn(hDC, hRgnSize);
		::ReleaseDC(*this, hDC);
		sizing = true;
	}else if(IsHeader(x, y))
	{
			SetCapture();
			pushing = pushed = true;
			RedrawFrame();
	}
}

void Column::OnMouseMove(int x, int y, UINT keyFlags)
{
	if(GetCapture() == this)
	{
		if(sizing)
		{
			HDC hDC=::GetDCEx(*this, NULL, DCX_PARENTCLIP);
			InvertRgn(hDC, hRgnSize);
			OffsetRgn(hRgnSize, x - PointSize.x, 0);
			InvertRgn(hDC, hRgnSize);
			
			::ReleaseDC(*this, hDC);
			PointSize.x = x;
			PointSize.y = y;
		}else if(pushing)
		{
			bool p = IsHeader(x, y);
			if(p != pushed)
			{
				pushed = p;
				RedrawFrame();
			}
		}
	}else
	{
		if(IsSizeGrip(x, y))
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
/*		else
			SetCursor(LoadCursor(NULL, IDC_ARROW));
*/	}
	Default();
}

void Column::OnLButtonUp(int x, int y, UINT keyFlags)
{
	if(GetCapture() == this)
	{
		if(sizing)
		{
			HDC hDC=::GetDCEx(*this, NULL, DCX_PARENTCLIP);
			InvertRgn(hDC, hRgnSize);
			::DeleteObject(hRgnSize);
			hRgnSize = NULL;
			::ReleaseDC(*this, hDC);
			::ReleaseCapture();
			::ClipCursor(NULL);
			minmax.szDes.cx = x;
			SetWindowPos(::GetParent(*this), NULL, 0, 0, 0, 0,
				SWP_NOACTIVATE|SWP_FRAMECHANGED|SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER);
			sizing = false;
		}else if(pushing)
		{
			::ReleaseCapture();
			if(pushed)
				OnHeaderPush();
			pushing = pushed = false;
			RedrawFrame();
		}
	}
}

void Column::OnPaint()
{
	PAINTSTRUCT paint;
	HDC hDC = BeginPaint(*this, &paint);
	SetBkMode(hDC, TRANSPARENT);
	FStyle Style;
	HFONT hf = NULL;
	COLORREF clr = INVALIDCOLOR;
	RECT r;
	int x = 0;
	int y = header_height;
	bool drawheader=false;
	if(GetClipBox(hDC, &r)== NULLREGION)
		return;
	int startrow;
	if(r.top < header_height)
	{
		drawheader = true;
		startrow = 0;
	}else
	{
		startrow = (r.top - header_height)/row_height;
		y = header_height + startrow * row_height;
	}
	int endrow = (r.bottom - header_height)/row_height + 1;
	endrow = min(endrow, rows);
	GetClientRect(*this, &r);
	::GetStyle(style, &Style);
	if(Style.pFont)
		hf = (HFONT)::SelectObject(hDC, (HGDIOBJ)(HFONT)*Style.pFont);
	if(Style.Fcolor != INVALIDCOLOR)
		clr = ::SetTextColor(hDC, Style.Fcolor);
	if(drawheader)
		OnDrawHeader(hDC, r);
	SetTextAlign(hDC, TA_BOTTOM|TA_LEFT);
	POINT pt[2] = {{r.left, 0},{r.right, 0}};
	HPEN hp = (HPEN)SelectObject(hDC, ::CreatePen(PS_SOLID, 0, RGB(128,128,128)));
	::GetStyle(GetFieldStyle(style, 2), &Style);
	if(Style.pFont)
		::SelectObject(hDC, (HGDIOBJ)(HFONT)*Style.pFont);
	if(Style.Fcolor != INVALIDCOLOR)
		::SetTextColor(hDC, Style.Fcolor);
	for(int i = startrow ; i < endrow; i++)
	{
		const char* s1 = GetData(i);
		if(ModifyStyle(&Style, i))
		{
			if(Style.pFont)
				::SelectObject(hDC, (HFONT) *Style.pFont);
			if(Style.Fcolor != INVALIDCOLOR)
				::SetTextColor(hDC, Style.Fcolor);
		}
		if(s1)
			TextOut(hDC, x + (row_height/5), (y += row_height)/* - (row_height/12)*/, s1, strlen(s1));
		pt[0].y = pt[1].y = y;
		::Polyline(hDC, pt, 2);
	}
	pt[0].x = r.right-1;
	pt[1].x = r.right-1;
	pt[0].y = r.top;
	pt[1].y = r.bottom;
	::Polyline(hDC, pt, 2);
	::DeleteObject(::SelectObject(hDC, hp));
	if(hf)
		SelectObject(hDC, hf);
	if(clr != INVALIDCOLOR)
		SetTextColor(hDC, clr);
	EndPaint(*this, &paint);
}

void Column::OnDrawHeader(HDC hDC, RECT rcClient)
{
	if(header_height)
	{
		rcClient.bottom = header_height;
		DrawFrameControl(hDC, &rcClient, DFC_BUTTON, DFCS_BUTTONPUSH|
			DFCS_ADJUSTRECT|(pushed ? DFCS_PUSHED : 0));
		SetTextAlign(hDC, TA_CENTER|TA_BOTTOM);
		TextOut(hDC, (rcClient.left + rcClient.right)/2, rcClient.bottom, GetCaption(), strlen(GetCaption()));
	}
}

//////////////////////////////////////////////////////////////////////
// RColumn
#if 0
#ifdef BASECLASS
#undef BASECLASS
#endif
#define BASECLASS Column
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RColumn::RColumn(RSField* prf,StMdfr* pst, Wnd* pWndParent, UINT nID, int& rs, int& dr, int& rh, int& hh, int st) :
Column(pWndParent, nID, rs, dr, rh, hh, st), pRField(prf), pFinder(NULL), pStMdfr(pst)
{
	style = ((Field*)*pRField)->style;
}

RColumn::~RColumn()
{
	ifdel(pStMdfr);
	ifdel(pFinder);
};

void RColumn::GetMinMax(PMINMAX pmm)
{
	if(minmax.szMax.cx == 0 && minmax.szMax.cy == 0)
	{
		TEXTMETRIC* ptm = GetTextMetrics(style);
		header_height = max(header_height, ptm->tmHeight * 6 / 5);
		int header_width = ptm->tmAveCharWidth * strlen(GetCaption()) * 3 / 2;

		ptm = GetTextMetrics(style);
		minmax.szMin.cx = max(header_width,
			ptm->tmAveCharWidth * ((Field*)*pRField)->scrlen * 3 / 2);
		minmax.szMax.cx = max(header_width,
			ptm->tmAveCharWidth * ((Field*)*pRField)->strlen * 3 / 2);
		minmax.szDes.cx = minmax.szMin.cx;

		row_height = max(row_height, ptm->tmHeight * 6 / 5);

		ROWINFO ri = ((RGrid*)GetParent())->GetRowInfo();
		minmax.szMin.cy = header_height + row_height * ri.min_rows;
		minmax.szMax.cy = header_height + row_height * ri.max_rows;
		minmax.szDes.cy = header_height + row_height * ri.max_rows;

		{
			RECT r = {0,0,minmax.szMin.cx, minmax.szMin.cy};
			AdjustWindowRect(&r, GetStyle(), FALSE);
			minmax.szMin.cx = r.right - r.left;
			minmax.szMin.cy = r.bottom - r.top;
		}
		{
			RECT r = {0,0,minmax.szMax.cx, minmax.szMax.cy};
			AdjustWindowRect(&r, GetStyle(), FALSE);
			minmax.szMax.cx = r.right - r.left;
			minmax.szMax.cy = r.bottom - r.top;
		}
		{
			RECT r = {0,0,minmax.szDes.cx, minmax.szDes.cy};
			AdjustWindowRect(&r, GetStyle(), FALSE);
			minmax.szDes.cx = r.right - r.left;
			minmax.szDes.cy = r.bottom - r.top;
		}
	}
	*pmm = minmax;
}
/////////////////////////////////////////////////////////////
//Operations
/////////////////////////////////////////////////////////////
const char* RColumn::GetData(int row)
{
	return (*pRField)[row];
};

const char* RColumn::GetCaption()
{
	return Record::pDict->GetCapt(*pRField);
}

void RColumn::OnDrawHeader(HDC hDC, RECT rcClient)
{
	Column::OnDrawHeader(hDC, rcClient);
	UINT iconID = 0;
	if(focused)
		iconID = IDI_EYE;
	else if(pRField->IsWhere())
		iconID = IDI_EYE1;
	else
		return;
	::DrawIconEx(hDC, 1, 1, ::LoadIcon(getApp()->getInstance(), MAKEINTRESOURCE(iconID)),
		header_height - 2, header_height - 2, 0, NULL, DI_NORMAL);
}
///////////////////////////////////////////////////////////
//Message handlers
///////////////////////////////////////////////////////////
BEGIN_COMMAND_MAP(RColumn, Column)
END_COMMAND_MAP

LRESULT RColumn::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_CHAR, OnChar)
		ON_MSG(WM_PARENTNOTIFY, OnParentNotify)
	BASEROUGHT 
}

void RColumn::OnParentNotify(UINT msg, HWND hwndChild, int idChild)
{
	if(msg == WM_DESTROY)
		::InvalidateRect(::GetParent(*this), NULL, TRUE);
}

void RColumn::OnChar(TCHAR ch, int cRepeat)
{
	switch(ch)
	{
	case VK_RETURN:
	case 0xa:
	case VK_ESCAPE:
	case VK_TAB:
		return;
	}
	if(!pFinder)
	{
		MSG msg = GetLastMsg();
		RECT r;
		GetClientRect(*this, &r);
		r.bottom = header_height;
		pFinder = new Finder(this, &r, *pRField, &pFinder);
		if(!pRField->IsWhere() && ch != VK_SPACE)
			pFinder->SendMessage(msg.message, msg.wParam, msg.lParam);
	}else
		ResendMessage(*pFinder);
}

void RColumn::OnKey(UINT vk, bool fDown, int cRepeat, UINT flags)
{
	if(pFinder)
		switch(vk)
	{
	case VK_RETURN:
	case VK_ESCAPE:
		{
			Wait ww;
		pFinder->Save(vk == VK_RETURN);
		if(pRField->IsDummy())
			pRField->Load();
		((RGrid*)GetParent())->OnResetMaxRows();
		pFinder->DestroyWindow();
		return;
		}
	case VK_TAB:
		pFinder->SetFocus();
		return;
	case VK_LEFT:
	case VK_RIGHT:
		pFinder->SetFocus();
		ResendMessage(*pFinder);
		return;
	}
	Column::OnKey(vk, fDown, cRepeat, flags);
}

bool RColumn::ModifyStyle(FStyle* pst, int row)
{
	if(!pStMdfr)
		return false;
	pStMdfr->SetData(((RSField*)pStMdfr->GetRField())->GetRow(row));
	pStMdfr->ModifyStyle(pst);
	return true;
}
#endif
