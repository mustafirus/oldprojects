#pragma once

#include "Frame.h"
#include "my_vector.h"

//class TextFrame;
//class DataContext;

class FieldFrame : public Frame
{
	typedef vector<Frame*> Frames;

	Frames			vCtrls;
	POINT point;
	SIZE size;
	int*	top;
	int*	height;
public:
//	FieldFrame(char*&str, DataContext* pdc);
	FieldFrame() : row_height(0), cdiv(0), focus(0), focused(false),
		space(defspace), top(0), height(0){};
	virtual ~FieldFrame(){ delete top; delete height; }

	virtual void addchild(Frame* pf) { vCtrls.add(pf); };
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
};

