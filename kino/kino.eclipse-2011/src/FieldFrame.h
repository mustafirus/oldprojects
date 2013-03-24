// FieldFrame.h: interface for the FieldFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELDFRAME_H__D4762F72_FD10_11D1_A66A_204C4F4F5020__INCLUDED_)
#define AFX_FIELDFRAME_H__D4762F72_FD10_11D1_A66A_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Frame.h"
#include "Defines.h"
class TextFrame;
class DataContext;

class FieldFrame : public Frame
{
	RFieldControls	vCtrls;
	POINT point;
	SIZE size;
	int*	top;
	int*	height;
public:
//	FieldFrame(char*&str, DataContext* pdc);
	FieldFrame() : row_height(0), cdiv(0), focus(0), focused(false),
		space(defspace), top(0), height(0){};
	virtual ~FieldFrame(){ifdel(top); ifdel(height)};

	virtual void GetMinMax(PMINMAX pmm);
	virtual void SetPos(POINT pt, SIZE sz, HDWP hdwp = NULL);
	virtual void SetTabs(TabMap* pTab, TabPos first, TabPos& last);
	virtual void Draw(HDC hdc);


protected:

	int			row_height;
	int			cdiv;
	RECT		space;
	int			rowspace;

	int			focus;
	bool		focused;

static	int		cspace;
static	RECT	defspace;
static	int		defstyle;
static	int		rowspace_min;
static	int		rowspace_max;
	friend Frame* Frame::CreateFrame(char*&str, DataContext* pdc);
};

#endif // !defined(AFX_FIELDFRAME_H__D4762F72_FD10_11D1_A66A_204C4F4F5020__INCLUDED_)
