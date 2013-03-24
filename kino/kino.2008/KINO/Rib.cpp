// Rib.cpp: implementation of the RiB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Rib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool RiB::MoveCurRow(int rows)
{
	if(!rows)
		return true;
	if(size == 1)
		return false;
	if(abs(rows) >= size)
		rows = (size - 1) * (rows > 0 ? 1 : -1);
	currow+=rows;
	
	if(currow < 0)
	{
		pstart+=currow;
		currow = 0;
	}else if(currow >= psize)
	{
		pstart += currow - psize + 1;
		currow = psize - 1;
	}
	if(pstart < 0)
	{
		if(GetFlag(bof))
		{
			pstart = 0;
			return false;
		}else
			return Normalize(true);
	}else if(pstart > size - psize)
	{
		if(GetFlag(eof))
		{
			pstart = size - psize;
			return false;
		}else
			return Normalize(true);
		return true;
	}
	if((pstart < (size - psize)/4 && !GetFlag(bof))
		|| (pstart > (size - psize)*3/4 && !GetFlag(eof)))
			Normalize(false);
	return true;
}

bool RiB::GetFlag(Flag st)
{
	switch(st)
	{
	case eof:
		return (flag & EO_FLAG) > 0;
	case bof:
		return (flag & BO_FLAG) > 0;
	default:
		return false;
	}
};
void RiB::SetFlag(Flag st)
{
	switch(st)
	{
	case eof:
		flag |= EO_FLAG;
		break;
	case bof:
		flag |= BO_FLAG;
	}
};
void RiB::ResetFlag(Flag st)
{
	switch(st)
	{
	case eof:
		flag &= ~EO_FLAG;
		break;
	case bof:
		flag &= ~BO_FLAG;
		break;
	case all:
		flag = 0;
	}
};
