// Pedigree.h: interface for the Pedigree class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PEDIGREE_H__46390162_1BA8_11D2_A6B6_204C4F4F5020__INCLUDED_)
#define AFX_PEDIGREE_H__46390162_1BA8_11D2_A6B6_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Frame.h"
#include "RKey.h"
#include "Record.h"
class DogFrame;

class Pedigree : public Frame  
{
	Record*		pRec;
	RKey*		pRKey;
	int			focus;
	DogFrame*	dog[28];
	int			m_lang;
	bool		bPrint;
public:
//	Pedigree(){};
//	Pedigree(RKey* rk, bool print = false, int lang = 0);
	Pedigree(Wnd* pParent, RKey* prk);
	virtual ~Pedigree();

	virtual void GetMinMax(PMINMAX pmm){*pmm = minmax;};
	virtual void SetPos(POINT pt, SIZE sz, HDWP hdwp = NULL);
	virtual void SetTabs(TabMap* pTab, TabPos first, TabPos& last);
/*	void Set(int n);
	void SetRectDog(int ndog, LPRECT rect);
	void SetSizes();
	void SetPos(POINT pt);
*/	
/*	bool SetFocus(POINT* pt);
	bool MoveFocus(FocusDir dir);
	void RemoveFocus();
*/
//	bool Cmd(CmdCode cmd);
//	void OnDraw(HDC hDC);
//	void Print(CDC* pDC);
//	static int defstyle;
};

#endif // !defined(AFX_PEDIGREE_H__46390162_1BA8_11D2_A6B6_204C4F4F5020__INCLUDED_)
