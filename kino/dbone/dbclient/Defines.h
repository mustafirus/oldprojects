#pragma once

#define _countof(array) (sizeof(array)/sizeof(array[0]))

struct RDIMS
{
	SIZE capt;
	SIZE data;
};

class Rect : public tagRECT
{
public:
	Rect(POINT point, SIZE  size)
	{ right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; }
	Rect(int l, int t, int r, int b)
	{ left = l; top = t; right = r; bottom = b; };
	operator RECT*(){return this;};
	operator SIZE(){SIZE sz = {right - left, bottom - top}; return sz;};
	void DeflateRect(int x, int y)
	{ ::InflateRect(this, -x, -y); };
	void DeflateRect(const RECT* lpRect)
	{
		left += lpRect->left;
		top += lpRect->top;
		right -= lpRect->right;
		bottom -= lpRect->bottom;
	};
	bool PtInRect(POINT pt){return ::PtInRect(this, pt)>0;}
	int Width( ) const{return right - left;};
	int Height( ) const{return bottom - top;};
};

class Point : public tagPOINT
{
public:
	Point(int x, int y){tagPOINT::x = x; tagPOINT::y = y;};
};

class Font
{
	HFONT		hFont;
	TEXTMETRIC	tm;
public:
	operator HGDIOBJ(){return hFont;}
	operator HFONT(){return hFont;}
	bool CreateFontIndirect(const LOGFONT* lpLogFont);
	bool CreatePointFont(int nPointSize, const char* lpszFaceName);
	bool CreatePointFontIndirect(const LOGFONT* lpLogFont);
	operator TEXTMETRIC*(){return &tm;};
};

class Brush
{
	HBRUSH	hBrush;
public:
	Brush() : hBrush(NULL){};
	Brush(COLORREF crColor){hBrush = ::CreateSolidBrush(crColor);};
	operator HBRUSH(){return hBrush;}
	bool CreateStockObject(int nIndex){hBrush = (HBRUSH)GetStockObject(nIndex); return hBrush > 0;};
};

typedef struct COLINFO_tag
{
	int			len;
	int			offset;
	int			type;
	int			intr;
	char		name[32];
}COLINFO, *PCOLINFO;
typedef struct TABLEINFO_tag
{
	int			cols;
	int			size;
	COLINFO*	pColInfo;	
}TABLEINFO, *PTABLEINFO;
