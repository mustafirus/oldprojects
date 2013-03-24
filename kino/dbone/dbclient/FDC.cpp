// FDC.cpp: implementation of the FDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "defines.h"
#include "Styles.h"
#include "StMdfr.h"
#include "Wnd.h"
#include "FDC.h"

FDC::FDC(Wnd* pwnd, HDC hdc /*= NULL*/) :
pWnd(pwnd), style(0), modified(false)
{
	PAINTSTRUCT ps;
	if(hdc)
		hDC = hdc;
	else
		hDC = ::BeginPaint( *pWnd, &ps); 
 

	GetStyle(style, /*state, */&Style);
	SetStyle((FStyle*)NULL);
}

int FDC::SetStyle(int n, StMdfr* psr /*= NULL*/)
{
	if(style == n && !psr && !modified)
		return style;
	int old_style = style;
	style = n;
	FStyle NewStyle;
	GetStyle(style, &NewStyle);
	if(psr)
	{
		psr->ModifyStyle(&NewStyle);
		modified = true;
	}
	SetStyle(&NewStyle);
	return old_style;
}
void FDC::SetStyle(FStyle* pNewStyle)
{
	if(pNewStyle == NULL)
	{
		if(Style.pFont)
			SelectObject(hDC, Style.pFont);
		if(Style.Fcolor != INVALIDCOLOR)
			SetTextColor(hDC, Style.Fcolor);
		if(Style.pBrush)
			SelectObject(hDC, Style.pBrush);
		return;
	}

	if(Style.pFont != pNewStyle->pFont)
	{
		Style.pFont = pNewStyle->pFont;
		if(Style.pFont)
			SelectObject(hDC, Style.pFont);
	}
	if(Style.Fcolor != pNewStyle->Fcolor)
	{
		Style.Fcolor = pNewStyle->Fcolor;
		if(Style.Fcolor != INVALIDCOLOR)
		SetTextColor(hDC, Style.Fcolor);
	}
	if(Style.pBrush != pNewStyle->pBrush)
	{
		Style.pBrush = pNewStyle->pBrush;
		if(Style.pBrush)
			SelectObject(hDC, Style.pBrush);
	}
	if(Style.Border != pNewStyle->Border)
	{
		Style.Border = pNewStyle->Border;
	}
}

void FDC::DrawText(LPRECT pRect, const char * str)
{
	::DrawText(hDC, str, -1, pRect, 0);
}

void FDC::FillRect(LPRECT rect)
{
//	pDC->FillRect(rect, pbr);
}

void FDC::DrawRect(LPRECT rect)
{
//	pDC->FrameRect(rect,pbr);
}

void FDC::Border(LPRECT rect)
{
	switch(Style.Border)
	{
	case 0:
		return;
	case -1:
		if(Style.pBrush)
			::FrameRect(hDC, rect, *Style.pBrush);
		return;
	default:
		::DrawEdge(hDC, rect, LOWORD(Style.Border), HIWORD(Style.Border));
	}
}

void FDC::DrawButton(LPRECT rect, bool pushed)
{
	RECT r = *rect;
	DrawFrameControl(hDC, &r ,DFC_BUTTON,
		DFCS_BUTTONPUSH|DFCS_ADJUSTRECT|(pushed ? DFCS_PUSHED:0));
}
