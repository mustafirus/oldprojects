// StMdfr.cpp: implementation of the StMdfr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Styles.h"
#include "StMdfr.h"

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
