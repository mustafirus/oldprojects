// Column.h: interface for the Column class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLUMN_H__2E28BAF5_43DC_11D2_A71D_204C4F4F5020__INCLUDED_)
#define AFX_COLUMN_H__2E28BAF5_43DC_11D2_A71D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Wnd.h"
#include "Frame.h"
#include "Defines.h"
class Edit;
struct FStyle;

class Column : public Wnd  
{
	int& rows;

	HRGN  hRgnSize;
	POINT PointSize;
	bool pushed;
	bool sizing;
	bool pushing;
protected:
	int& row_height;
	MINMAX minmax;
	bool focused;
	int& header_height;
	int  style;
//	RDIMS dims;
//	SIZE  rsize;
//	virtual void InitDims();
//	virtual void GetDims(RDIMS* pdims);
public:
	Column(Wnd* pWndParent, UINT nID, int& rs, int& dr, int& rh, int& hh, int st);
	virtual ~Column(){};

//Operations
	void RedrawFrame();
	virtual void OnHeaderPush(){};
	virtual void OnDrawHeader(HDC hDC, RECT rcClient);
	virtual bool ModifyStyle(FStyle* pst, int row){return false;};
//	virtual bool SetFocus(bool f){focused = f; RedrawFrame(); return focused;};
//	void GetRSize(SIZE* psz);
	virtual void GetMinMax(PMINMAX pmm){};
	bool IsSizeGrip(int x, int y);
	bool IsHeader(int x, int y);
	virtual const char* GetData(int row){return "ÐÛÆÅ-ÏÅÃÈÉ Ñ ×ÅÐÍÛÌ";};
	virtual const char* GetCaption(){return "Caption";};
//Message handlers
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void OnChar(TCHAR ch, int cRepeat);
	virtual void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);
	void OnKillFocus(HWND hwndNewFocus);
	void OnSetFocus(HWND hwndOldFocus);
	bool OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg);
	void OnMouseMove(int x, int y, UINT keyFlags);
	void OnLButtonDblClk(bool fDoubleClick, int x, int y, UINT keyFlags);
	void OnLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags);
	void OnLButtonUp(int x, int y, UINT keyFlags);
	void OnPaint();
//Register
	virtual const char* GetClassName(){return "COLUMN";};
	static void RegisterClass(){Wnd::RegisterClass(CS_SAVEBITS|CS_PARENTDC|CS_DBLCLKS	, 0, 
		GetApp()->LoadCursor(IDC_HAND), 0, 0,"COLUMN", 0);};

DECLARE_COMMAND_MAP()
};


class RSField;
class Finder;
class pStMdfr;

class RColumn : public Column
{
	friend class RGrid;
	RSField*	pRField;
	Finder*		pFinder;
	StMdfr*		pStMdfr;
protected:
//	virtual void GetDims(RDIMS* pdims);
//	virtual void InitDims();
public:
	RColumn(RSField* prf,StMdfr* pst, Wnd* pWndParent, UINT nID, int& rs, int& dr, int& rh, int& hh, int st);
	~RColumn();

	const char* GetData(int row);
	const char* GetCaption();
	void GetMinMax(PMINMAX pmm);
	
	void OnDrawHeader(HDC hDC, RECT rcClient);
	bool ModifyStyle(FStyle* pst, int row);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void OnParentNotify(UINT msg, HWND hwndChild, int idChild);
	void OnChar(TCHAR ch, int cRepeat);
	void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);

	DECLARE_COMMAND_MAP()
};

#endif // !defined(AFX_COLUMN_H__2E28BAF5_43DC_11D2_A71D_204C4F4F5020__INCLUDED_)
