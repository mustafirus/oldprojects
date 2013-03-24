// DogFrame.h: interface for the DogFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOGFRAME_H__9655C042_1AAA_11D2_A6B4_204C4F4F5020__INCLUDED_)
#define AFX_DOGFRAME_H__9655C042_1AAA_11D2_A6B4_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Frame.h"
#include "StMdfr.h"
typedef bool			Sex;
class RKey;
#include "EditControl.h"

class DogFrame : public EditControl
{
	RKey*		pRKey;
	Record*		pRec;
	char*		id;
	char*		sex;
	StMdfr*		pStMdfr;
public:
	DogFrame(RKey* prk, Record* pr, const char* id, const char* sex, Wnd* pWndParent, UINT nID);
	virtual ~DogFrame(){};

//	void SetData(const char* str);
	void GetMinMax(PMINMAX pmm){};
	void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);
	void Activate(UINT syskey);
/*	bool SetFocus(POINT* pt){if(!Hit(pt))return false; Redraw(); return focused = true;};
	bool SetFocus(){Redraw(); return focused = true;};
	void RemoveFocus(){ Redraw(); focused = false;};

	int  GetParent(Sex s){return parent[s];};
	void Set(int iid, int lines, int lang){char s;GetDog(id = iid, lines, lang, str, s, parent); sex = s > 0; pStMdfr->SetData(s);};
	void RemoveSmthng(){int n = str.Find('\n'); if(n>0) str.SetAt(n,' '); n = str.ReverseFind('\n'); if(n>0) str.SetAt(n,' ');};
	int  GetID(){return id;};
	void OnDraw(HDC* hDC);
	void OnPrint(HDC* hDC);
	int  OnSetStyle(HDC* hDC)
	{
		pStMdfr->focused = focused;
		return pDC->SetStyle(style + (focused ? 1 : 0), pStMdfr);
	};

	static bool GetDog(int id, int lines, int lang, CString& str, char& s, int* parents);
	static int defstyle;
*/	virtual const char* GetClassName(){return "STATIC";};
};

#endif // !defined(AFX_DOGFRAME_H__9655C042_1AAA_11D2_A6B4_204C4F4F5020__INCLUDED_)
