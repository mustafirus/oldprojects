// ListFrame.h: interface for the ListFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LISTFRAME_H__C7785F73_F983_11D1_A658_204C4F4F5020__INCLUDED_)
#define AFX_LISTFRAME_H__C7785F73_F983_11D1_A658_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Frame.h"

class RecordSet;
class RGrid;

class ListFrame : public Frame
{
	RGrid*	pGrid;
public:
	ListFrame(char*&str, FrameWnd* pWnd);
	virtual ~ListFrame();

//	int GetCurRow();
	virtual void OnDraw(FDC* pDC);
	
	virtual bool SetFocus(POINT* pt);
	virtual bool MoveFocus(FocusDir dir);
	virtual void RemoveFocus(){};

protected:
	virtual void SetSizes();
	virtual void CalcSizes();
	virtual void SetPos(POINT pt);
	virtual bool Param(char* str);

	RecordSet*	pRec;
	int			focus_col;
	int			cols;
	int			rows;
	int*		lefts;
	int			row_height;

static	int		defstyle;
static	RECT	defspace;
};

#endif // !defined(AFX_LISTFRAME_H__C7785F73_F983_11D1_A658_204C4F4F5020__INCLUDED_)
