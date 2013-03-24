// TabMap.cpp: implementation of the TabMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabMap.h"

TabDir TabMap::up = {-1, 0};
TabDir TabMap::down = {1, 0};
TabDir TabMap::left = {0, -1};
TabDir TabMap::right = {0, 1};
TabPos TabMap::first_pos = {0, 0};

static SIZE growby = {2, 4};
void TabMap::Grow()
{
	if(!pData)
	{
		pData = new Wnd*[(rows = growby.cy) * (cols = growby.cx)];
		memset(pData, 0, sizeof(Wnd**) * rows * cols);
	}
	else
	{
//		short mc = last_pos.c + (2 - (last_pos.c & 1));
		short mc = last_pos.c & 1;
		mc = 2 - mc;
		mc = last_pos.c + mc;
//		short mr = last_pos.r + (4 - (last_pos.r & 3));
		short mr = last_pos.r & 3;
		mr = 4 - mr;
		mr = last_pos.r + mr;
		Wnd** pNew= new Wnd*[mr * mc];
		memset(pNew, 0, sizeof(Wnd**) * mr * mc);
		int cr = min(rows, mr);
		int cc = min(cols, mc);
		Wnd** src = pData;
		Wnd** dst = pNew;
		while(cr--)
		{
			memcpy(dst, src, sizeof(Wnd**) * cc);
			dst += mc;
			src += cols;
		}
		delete pData;
		pData = pNew;
		rows = mr;
		cols = mc;
	}
}

Wnd* TabMap::GetNext(TabPos& pos, TabDir dir)
{
	TabPos p = pos + dir;
	Wnd* pWnd = GetAt(pos);
	while(p.r >=0 && p.r <= last_pos.r && p.c >=0 && p.c <= last_pos.c)
	{
		if(pWnd = GetAt(pos = p))
			return pWnd;
		else
			p = p + dir;
	}
	TabDir d1;
	bool lt = pos == first_pos;
	bool rb = pos == last_pos;
	if((lt || rb) && pWnd)
		return pWnd;
	if(lt)
		d1 = right;
	else if(rb)
		d1 = left;
	else if(dir == up)
		d1 = left;
	else if(dir == left)
		d1 = up;
	else if(dir == down)
		d1 = right;
	else if(dir == right)
		d1 = down;
	return	GetNext(pos, d1);
}

Wnd* TabMap::GetAt(TabPos pos)
{
	if(pos.c >= cols || pos.r >= rows)
		return NULL;
	return pData[pos.r * cols + pos.c];
}

void TabMap::SetAt(TabPos pos, Wnd* pWnd)
{
	last_pos.r = max(pos.r, last_pos.r);
	last_pos.c = max(pos.c, last_pos.c);
	if(pos.c >= cols || pos.r >= rows)
	{
		Grow();
	}
	//	SetSize(pos.r + 1, pos.c + 1);
	pData[pos.r * cols + pos.c] = pWnd;
}
