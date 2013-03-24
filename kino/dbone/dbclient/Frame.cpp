// Frame.cpp: implementation of the Frame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabMap.h"
#include "Frame.h"


VecFrame::~VecFrame()
{
	for(int i = 0; i < pFrames.GetCount(); i++)
		delete pFrames[i];
}

void VecFrame::GetMinMax(PMINMAX pmm)
{
	if(minmax.szDes.cx ==0 && minmax.szDes.cy == 0)
	{
		for(int i = 0; i < pFrames.GetCount(); i++)
		{
			pFrames[i]->GetMinMax(pmm);
			if(horz)
			{
				minmax.szMin.cx += pmm->szMin.cx;
				minmax.szMin.cy = max(minmax.szMin.cy, pmm->szMin.cy);
				minmax.szMax.cx += pmm->szMax.cx;
				minmax.szMax.cy = max(minmax.szMax.cy, pmm->szMax.cy);
				minmax.szDes.cx += pmm->szDes.cx;
				minmax.szDes.cy = max(minmax.szDes.cy, pmm->szDes.cy);
			}else
			{
				minmax.szMin.cx = max(minmax.szMin.cx, pmm->szMin.cx);
				minmax.szMin.cy += pmm->szMin.cy;
				minmax.szMax.cx = max(minmax.szMax.cx, pmm->szMax.cx);
				minmax.szMax.cy += pmm->szMax.cy;
				minmax.szDes.cx = max(minmax.szDes.cx, pmm->szDes.cx);
				minmax.szDes.cy += pmm->szDes.cy;
			}
		}
	}
	*pmm = minmax;
}
void Frame::AdjSize(const MINMAX& mm, SIZE& sz, const ADJ& adj)
{
	sz.cx = mm.szMin.cx + (mm.szMax.cx  * adj.cx - mm.szMin.cx * adj.cx) / 100;
	sz.cy = mm.szMin.cy + (mm.szMax.cy  * adj.cy - mm.szMin.cy * adj.cy) / 100;
}
void Frame::GetAdj(const SIZE& sz, ADJ& adj)
{
	if(minmax.szMax.cx == minmax.szMin.cx)
		adj.cx = 0;
	else
		adj.cx = (sz.cx * 100 - minmax.szMin.cx * 100) / (minmax.szMax.cx - minmax.szMin.cx);
	if(minmax.szMax.cy == minmax.szMin.cy)
		adj.cx = 0;
	else
		adj.cy = (sz.cy * 100 - minmax.szMin.cy * 100) / (minmax.szMax.cy - minmax.szMin.cy);
	if(adj.cx < 0)
		adj.cx = 0;
	if(adj.cx > 100)
		adj.cx = 100;
	if(adj.cy < 0)
		adj.cy = 0;
	if(adj.cy > 100)
		adj.cy = 100;
}

void VecFrame::SetPos(POINT pt, SIZE sz, HDWP hdwp)
{
	ADJ adj;
	GetAdj(sz, adj);
	int off = 0;
	for(int i = 0; i < pFrames.GetCount(); i++)
	{
		MINMAX	chmm;
		SIZE	chsz;
		pFrames[i]->GetMinMax(&chmm);
		AdjSize(chmm, chsz, adj);
		if(horz)
		{
			pt.x+=off;
			chsz.cy = sz.cy;
		}else
		{
			pt.y+=off;
			chsz.cx = sz.cx;
		}
		pFrames[i]->SetPos(pt, chsz, hdwp);
		off = horz ? chsz.cx : chsz.cy;
	}
}

void VecFrame::SetTabs(TabMap* pTab, TabPos first, TabPos& last)
{
	SIZE sz;
	pTab->GetSize(sz);
	if(horz)
	{
		sz.cx += pFrames.GetCount();
		sz.cy += 1;
/*		if((first.c + pFrames.GetCount()) >= sz.cx)
			sz.cx = first.c + pFrames.GetCount() + 1;
		if(first.r >= sz.cy)
			sz.cy += 1;
*/	}else
	{
		sz.cx += 1;
		sz.cy = pFrames.GetCount();
/*		if(first.c >= sz.cx)
			sz.cx += 1;
		if((first.r + pFrames.GetCount()) >= sz.cy)
			sz.cy = first.c + pFrames.GetCount() + 1;
*/	}
	pTab->SetSize(sz);
	TabPos maxpos = first;
	for(int i = 0; i < pFrames.GetCount(); i++)
	{
		pFrames[i]->SetTabs(pTab, first, last);
		if(horz)
		{
			first.c = last.c + 1;
		}else
		{
			first.r = last.r + 1;
		}
		maxpos.r = max(maxpos.r, last.r);
		maxpos.c = max(maxpos.c, last.c);
	}
	last = maxpos;
}

void VecFrame::Draw(HDC hdc)
{
	for(int i = 0; i < pFrames.GetCount(); i++)
		pFrames[i]->Draw(hdc);
}
