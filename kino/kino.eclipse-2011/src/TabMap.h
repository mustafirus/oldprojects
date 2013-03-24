// TabMap.h: interface for the TabMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABMAP_H__BE266605_49AD_11D2_A72E_204C4F4F5020__INCLUDED_)
#define AFX_TABMAP_H__BE266605_49AD_11D2_A72E_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
class Wnd;

struct TabPos
{
	short r;
	short c;
};
typedef TabPos TabDir;
inline TabPos operator+(TabPos pos, TabDir dir){pos.r += dir.r, pos.c += dir.c; return pos;};
inline bool operator==(TabDir d1, TabDir d2){return d1.r == d2.r && d1.c == d2.c;};
class TabMap
{
	int rows;
	int cols;
	int mr;
	int mc;
	Wnd** pData;
	void Grow();
	TabPos last_pos;
public:
	TabMap() : rows(0), cols(0), mr(0), mc(0), pData(NULL), last_pos(first_pos){};
	~TabMap(){if (pData) delete pData;};
	void SetSize(SIZE sz){/*mc = sz.cx; mr = sz.cy;*/};
	void SetSize(int r, int c){/*mr = r;mc = c;*/};
	void GetSize(SIZE& sz){sz.cx = cols, sz.cy = rows;};
	void SetAt(TabPos pos, Wnd* pWnd);
	Wnd* GetNext(TabPos& pos, TabDir dir);
	Wnd* GetAt(TabPos pos);
	static TabDir up;
	static TabDir down;
	static TabDir left;
	static TabDir right;
	static TabPos first_pos;
};

#endif // !defined(AFX_TABMAP_H__BE266605_49AD_11D2_A72E_204C4F4F5020__INCLUDED_)
