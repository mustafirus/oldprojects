// FieldFrame.cpp: implementation of the FieldFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Defines.h"
#include "Edit.h"
#include "Styles.h"
//#include "FDC.h"
#include "FrameWnd.h"
//#include "RFieldControl.h"
#include "FieldFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GrowBy 4
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int	 FieldFrame::cspace   = 4;
RECT FieldFrame::defspace = {2,2,2,2};
int  FieldFrame::defstyle = 1;
int	FieldFrame::rowspace_min = 10;
int	FieldFrame::rowspace_max = 40;


/*FieldFrame::FieldFrame(char*&str, DataContext* pdc) :
row_height(0), cdiv(0), focus(0), focused(false), space(defspace)
{
//	style = defstyle;
//	Init(str);
	*strchr(str, '>') = 0;
	pWnd->GetControls(str, vCtrls);
	*str = '>';
}
*/
void FieldFrame::GetMinMax(PMINMAX pmm)
{
	if(*(UINT*)&minmax == 0)
	{
		if(!vCtrls.count())
		{
			memset(&minmax, 0, sizeof(minmax));
			*pmm = minmax;
			return;
		}
	
		int clen = 0; //caption max len in pixels
		for(int i = 0; i < vCtrls.count(); i++)
		{
			SIZE csz = {0,0};
			vCtrls[i]->GetMinMax(pmm);
//			vCtrls[i]->GetCaptionSize(csz);
			clen = max(clen, csz.cx);
			minmax.szMin.cx = max(minmax.szMin.cx, pmm->szMin.cx);
			minmax.szMax.cx = max(minmax.szMax.cx, pmm->szMax.cx);
			minmax.szDes.cx = max(minmax.szDes.cx, pmm->szDes.cx);
			minmax.szMin.cy += pmm->szMin.cy;
			minmax.szMax.cy += pmm->szMax.cy;
			minmax.szDes.cy += pmm->szDes.cy;
		}
		minmax.szMin.cx += (cdiv=clen) + cspace + space.left + space.right;
		minmax.szMax.cx += (cdiv=clen) + cspace + space.left + space.right;
		
		minmax.szMin.cy += (minmax.szMin.cy * rowspace_min / (100 * vCtrls.count())) * (vCtrls.count() - 1);
		minmax.szMax.cy += (minmax.szMax.cy * rowspace_max / (100 * vCtrls.count())) * (vCtrls.count() - 1);
		minmax.szDes.cy += (minmax.szDes.cy * rowspace_max / (100 * vCtrls.count())) * (vCtrls.count() - 1);
		minmax.szMin.cy += space.top + space.bottom;
		minmax.szMax.cy += space.top + space.bottom;
		minmax.szDes.cy += space.top + space.bottom;
		
		top = new int[vCtrls.count()];
		height = new int[vCtrls.count()];
	}

	*pmm = minmax;
}

void FieldFrame::SetPos(POINT pt, SIZE sz, HDWP hdwp)
{
	point = pt;
	size = sz;
	ADJ adj;
	GetAdj(sz, adj);
	int coff = 0;
	pt.x += space.left + cdiv + cspace;
	pt.y += space.top;
	rowspace = rowspace_min + (rowspace_max  * adj.cy - rowspace_min * adj.cy) / 100;

	int cx = sz.cx;
	for(int i=0; i<vCtrls.count(); i++)
	{
		MINMAX	chmm;
		vCtrls[i]->GetMinMax(&chmm);
		AdjSize(chmm, sz, adj);
//		sz.cx = cx;
		vCtrls[i]->SetPos(pt, sz, hdwp);
		top[i] = pt.y;
		height[i] = sz.cy;
		pt.y += sz.cy + rowspace/sz.cy;
	}
}
void FieldFrame::SetTabs(TabMap* pTab, TabPos first, TabPos& last)
{
	SIZE sz;
	pTab->GetSize(sz);
	sz.cx += 1;
	sz.cy += vCtrls.count();
	pTab->SetSize(sz);

	TabPos maxpos = first;
	for(int i = 0; i < vCtrls.count() ; i++)
	{
		vCtrls[i]->SetTabs(pTab, first, last);
		first.r = last.r + 1;
		maxpos.r = max(maxpos.r, last.r);
		maxpos.c = max(maxpos.c, last.c);
	}
	last = maxpos;
}

void FieldFrame::Draw(HDC hdc)
{
	FStyle Style;
	GetStyle(defstyle, &Style);
	HFONT hf = NULL;
	COLORREF clr = INVALIDCOLOR;
	if(Style.pFont)
		hf = (HFONT) ::SelectObject(hdc, *Style.pFont);
	if(Style.Fcolor != INVALIDCOLOR)
		clr = ::SetTextColor(hdc, Style.Fcolor);
////////// Captions ///////////////////////////
	RECT r;
	r.left = point.x + space.left;
	r.right = r.left + cdiv;
	for(int i=0; i<vCtrls.count(); i++)
	{
		r.top = top[i];
		r.bottom = r.top + height[i];
		if(!RectVisible(hdc, &r))
			continue;
		::DrawText(hdc, "" /*vCtrls[i]->GetCaption()*/, -1, &r, 0);
		vCtrls[i]->Draw(hdc);
	}
	if(hf)
		SelectObject(hdc, hf);
	if(clr != INVALIDCOLOR)
		SetTextColor(hdc, clr);
}

/*void FieldFrame::SetSize(int x, int y)
{
	ASSERT(x>=rsize.cx && y>=rsize.cy);
	space.right = x - rsize.cx;
	space.bottom = y - rsize.cy;
	size.cx = x;size.cy = y;
};

void FieldFrame::SetSize(SIZE sz)
{
	SetSize(sz.cx, sz.cy);
};
*/
/*void FieldFrame::SetPos(POINT pt)
{
	point = pt;
	int coff = 0;
	Rect r = GetRect();
	r.DeflateRect(&space);
	r.left += cdiv + cspace;
//	r.OffsetRect(point);
	r.bottom = r.top + coff;
	for(int i=0; i<vCtrls.count(); i++)
	{
		r.top = r.bottom;
		r.bottom = r.top + row_height;
		vCtrls[i]->SetPos(r);
	}
};
*/
/*void FieldFrame::SetPos(int x, int y)
{
	point.x = x;
	point.y = y;
	int coff = 0;
	Rect r = GetRect();
	r.DeflateRect(&space);
	r.left += cdiv + cspace;
//	r.OffsetRect(point);
	r.bottom = r.top + coff;
	for(int i=0; i<vCtrls.count(); i++)
	{
		r.top = r.bottom;
		r.bottom = r.top + row_height;
		vCtrls[i]->SetPos(r);
	}
};
*/
/*Rect FieldFrame::GetFieldRect(int n)
{
	ASSERT(n >= 0 && n < vCtrls.count());
	int coff = 0;
	return Rect(point.x + space.left + cdiv + cspace, point.y + space.top + n * row_height + coff,
		point.x + size.cx - space.right, point.y + space.top + (n+1) * row_height + coff);
}
*/
/*bool FieldFrame::MoveFocus(FocusDir dir)
{
/////////////////////////////////////////
//	Redraw(GetFieldRect(focus));
/////////////////////////////////////////
	if(focused)
	{
		switch(dir)
		{
		case up:
			focused = focus > 0;
			if(focused)
				focus--;
			break;
		case down:
			focused = focus < vCtrls.GetCount()-1;
			if(focused)
				focus++;
			break;
		default:
			focused = !focused;
		}
	}else
		focused = true;
/////////////////////////////////////////
//	Redraw(GetFieldRect(focus));
/////////////////////////////////////////
	return focused;
};
*/