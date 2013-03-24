// StMdfr.cpp: implementation of the StMdfr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Styles.h"
#include "StMdfr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void StMdfr::ModifyStyle(FStyle* pst)
{
	if(!data)
		return;
	if(*data == '0')
		pst->Fcolor = GetColor(color1);
	else
		pst->Fcolor = GetColor(color2);
	if(focused)
		pst->Fcolor = pst->Fcolor | 0x00640064;
}
