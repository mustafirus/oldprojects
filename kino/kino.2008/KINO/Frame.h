// Frame.h: interface for the Frame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAME_H__BF1FD714_F276_11D1_A640_204C4F4F5020__INCLUDED_)
#define AFX_FRAME_H__BF1FD714_F276_11D1_A640_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "Set.h"

typedef struct tagMINMAX {
    SIZE szMin;
    SIZE szMax;
	SIZE szDes;
} MINMAX, *PMINMAX;
typedef SIZE ADJ;
class DataContext;
class TabMap;
struct TabPos;
#include "TabMap.h"
class Frame  
{
protected:
	TabPos pos;
	MINMAX minmax;
	void AdjSize(const MINMAX& mm, SIZE& sz, const ADJ& adj);
	void GetAdj(const SIZE& sz, ADJ& adj);
	Frame(){memset(&minmax, 0, sizeof(minmax));};
public:
	virtual ~Frame(){};

	virtual void GetMinMax(PMINMAX pmm) = 0;
	virtual void SetPos(POINT pt, SIZE sz, HDWP hdwp = NULL) = 0;
	virtual void SetTabs(TabMap* pTab, TabPos first, TabPos& last) = 0;

	virtual void Draw(HDC hdc){};

	static Frame* CreateFrame(char*&str, DataContext* pdc);
};
typedef Set<Frame> Frames;

class VecFrame : public Frame  
{
	Frames	pFrames;
	int		focus;
	bool horz;
public:
	VecFrame(){};
	virtual ~VecFrame();
	void Add(Frame* pFrame){pFrames.Add(pFrame);};
	virtual void GetMinMax(PMINMAX pmm);
	virtual void SetPos(POINT pt, SIZE sz, HDWP hdwp = NULL);
	virtual void SetTabs(TabMap* pTab, TabPos first, TabPos& last);
	virtual void Draw(HDC hdc);
	friend Frame* Frame::CreateFrame(char*&str, DataContext* pdc);
};

#endif // !defined(AFX_FRAME_H__BF1FD714_F276_11D1_A640_204C4F4F5020__INCLUDED_)
