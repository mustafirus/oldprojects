// Pedigree.cpp: implementation of the Pedigree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "RKey.h"
#include "Wnd.h"
#include "DogFrame.h"
#include "Action.h"
#include "Exception.h"
#include "TempRecord.h"
#include "Pedigree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PEDIGREE_FIRST		10
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//Pedigree::Pedigree(RKey* prk, bool print /*= false*/, int lang /*= 0*/) :
/*rec(prk), pRKey(prk),*/ //focus(0), m_lang(lang), bPrint(print)
/*{
//	pRKey = rec.GetRKey();
//	Set(atoi(rkey.GetSrcData()));
	minmax.szMin.cx = 600;
	minmax.szMin.cy = 400;
	minmax.szMax.cx = 900;
	minmax.szMax.cy = 600;
	minmax.szDes.cx = 600;
	minmax.szDes.cy = 400;
}
*/
Pedigree::Pedigree(Wnd* pParent, RKey* prk)
{
	try{
		pRec = new Record(prk);
		pRKey = pRec->GetRKey();
		pRec->Load();
		memset(dog, 0, sizeof(dog));
		minmax.szMin.cx = 600;
		minmax.szMin.cy = 400;
		minmax.szMax.cx = 900;
		minmax.szMax.cy = 600;
		minmax.szDes.cx = 600;
		minmax.szDes.cy = 400;
		TempRecord tr("{CALL  sp_pedigree(?)}", prk); 
		UINT nID = 0;
		while(tr.MoveNext())
		{
			nID = atoi(tr[0]);
			ASSERT(nID>=0 && nID < 28);
			dog[nID] = new DogFrame(pRKey, pRec, tr[2], tr[3], pParent, nID + PEDIGREE_FIRST);
			dog[nID]->SetWindowText(tr[1]);
		}
	}catch(Exception* e)
	{
//		e->SetDelMem(this);
		throw e;
	}
}

Pedigree::~Pedigree()
{

}
#pragma optimize( "g", off)
void Pedigree::SetPos(POINT point, SIZE size, HDWP hdwp)
{
	SIZE sz;
	POINT pt;
	int roh = size.cy / 16;
	int coh[3] = {size.cx * 5 / 19,size.cx * 6 / 19,size.cx * 8 / 19};
	int col[3] = {0, coh[0], coh[0]+coh[1]};

	int r = 0;
	int c = 0;
	int rn = 0;
	for(int i = 4; i < 32; i++)
	{
		if(dog[i-4])
		{
			{
				r = (r = i & 16)>>4 | (~(r=r>>1) & (r |= i & 8)) >> 2 | (~r>>1 & i & 4);
				c = ((c = i & 16)>>4)*3 + ((~(c=c>>1) & (c |= i & 8)) >> 2) + ((~c>>1 & i & 4)>>2) - 1;
			}
			sz.cy = r * roh;
			sz.cx = coh[c];
			pt.x = point.x + col[c];
			pt.y = point.y + ((i << (2 - c)) & 15) * roh;
			dog[i-4]->SetPos(pt, sz, hdwp);
		}
	}
}

#pragma optimize( "g", off)
void Pedigree::SetTabs(TabMap* pTab, TabPos first, TabPos& last)
{
	last.c = first.c + 2;
	last.r = first.r + 15;
	int c = 0;
	for(int i = 4; i < 32; i++)
	{
		if(!dog[i-4])
			continue;
		TabPos f,l;
		int c=
		c = ((c = i & 16)>>4)*3 + ((~(c=c>>1) & (c |= i & 8)) >> 2) + ((~c>>1 & i & 4)>>2) - 1;
		f.c = first.c + c;
		f.r = first.r + ((i << (2 - c)) & 15);
		if(dog[i-4])
		dog[i-4]->SetTabs(pTab, f, l);
	}
}

/*
bool Pedigree::Cmd(CmdCode cmd)
{
	if(cmd == fDefault)
	{
		rkey.SetSrcData(dog[focus].GetID());
		return pWnd->pAction->UserAction(0, &rkey);//pWnd->CreateFrame(pRFields[focus]->GetRKey(), GetFieldRect(focus));
	}
	return false;
}
*/
/*void Pedigree::Set(int n)
{
	dog[0].Set(n, 1, m_lang);
	int lines = 4;
	for(int i = 1; i < 31; i++)
	{
		if(i == 3) lines = 3;
		else if(i == 7) lines = 3;
		else if(i == 15) lines = 1;
		dog[i].Set(dog[((i+1) >> 1) - 1].GetParent((i+1) & 1), lines, m_lang);
	}
	if(!bPrint)
	{
		dog[1].RemoveSmthng();
		dog[2].RemoveSmthng();
	}
}
*/
/*void Pedigree::SetRectDog(int ndog, LPRECT rect)
{
	dog[ndog].SetPos(rect->left, rect->top);
	dog[ndog].SetSize(rect->right-rect->left, rect->bottom-rect->top);
}

void Pedigree::SetSizes()
{
	int rows = bPrint ? 16 : 19;
	int roh = size.cy / rows;
	int coh;//5,6,8
	
	if(!bPrint)
	{
		dog[0].SetSize(size.cx, roh);
		roh *= 2;
		dog[1].SetSize(size.cx/2, roh);
		dog[2].SetSize(size.cx/2, roh);
	}

	coh = size.cx * 5 / 19;
	roh = size.cy * 4 / rows;
	for(int i = 3; i < 7; i++)
		dog[i].SetSize(coh, roh);
	coh = size.cx * 6 / 19;
	roh = size.cy * 2 / rows;
	for(i = 7; i < 15; i++)
		dog[i].SetSize(coh, roh);
	coh = size.cx * 8 / 19;
	roh = size.cy / rows;
	for(i = 15; i < 31; i++)
		dog[i].SetSize(coh, roh);
}
*/
/*void Pedigree::SetPos(POINT pt)
{
	Frame::SetPos(pt);
	if(!bPrint)
	{
		dog[0].SetPos(pt);
		dog[1].SetPos(pt.x, pt.y + size.cy/19);
		dog[2].SetPos(pt.x + size.cx/2, pt.y + size.cy/19);
	}
	int d = 3;
	int roh = size.cy * 4 / (bPrint ? 18 : 19);
	int col;
	int row;
	int start = bPrint ? (pt.y) : (pt.y + size.cy * 3 / 19);
	for(int i = 0; i < 3; i++)
	{
		switch(i)
		{
		case 0:col=pt.x;break;
		case 1:col=pt.x + size.cx * 5 / 19;break;
		case 2:col=pt.x + size.cx * 11 / 19;
		}
		row = start;
		for(int j = 0; j < 4 << i; j++)
		{
			dog[d++].SetPos(col, row);
			row+=roh;
		}
		roh = roh>>1;
	}
}
*/
/*void Pedigree::OnDraw(FDC* pDC)
{
	for(int i = 0; i < 31; i++)
		dog[i].Draw(pDC);
}

void Pedigree::Print(CDC* pDC)
{
	for(int i = 1; i < 31; i++)
		dog[i].OnPrint(pDC);
}
*/
/*bool Pedigree::SetFocus(POINT* pt)
{
	if(!Hit(pt))
		return false;
	int i=0;
	while(i<31 && !dog[i].SetFocus(pt))i++;
	if(i!=31)
	{
		dog[focus].RemoveFocus();
		focus = i;
		return true;
	}
	else
		return false;
}
*/
/*bool Pedigree::MoveFocus(FocusDir dir)
{
	bool ret = true;
	dog[focus].RemoveFocus();
	if(focus == 0)
	{
		if(dir == down)
			focus++;
		else
			ret = false;
	}else if(dir == up && (focus == 1 || focus == 2))
	{
			focus = 0;
	}else if(focus == 1)
	{
		if(dir == down)
			focus = 3;
		else if(dir == right)
			focus = 2;
		else
			ret = false;
	}else if(focus == 2)
	{
		if(dir == down)
			focus = 7;
		else if(dir == left)
			focus = 1;
		else
			ret = false;
	}else
	{
		focus += 1;
		switch(dir)
		{
		case up:	focus--;		break;
		case down:	focus++;		break;
		case left:	focus >>= 1;	break;
		case right:	focus <<= 1;	break;
		case home:	focus = -2;		break;
		case end:	focus = 28;		break;
		}
		focus -= 1;
		if(focus == 2 && dir == up)
			focus = 1;
		else if(focus == 6 && dir == up)
			focus = 1;
		else if(focus == 14 && dir == up)
			focus = 2;
	}

	if(focus > 30)
	{
		focus = 30;
		ret = false;
	}
	dog[focus].SetFocus();
	return ret;
}
*/
/*void Pedigree::RemoveFocus()
{
	dog[focus].RemoveFocus();
}
*/