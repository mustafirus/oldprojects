// Styles.h: interface for the Styles class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STYLES_H__4513AD12_FAE3_11D1_A662_204C4F4F5020__INCLUDED_)
#define AFX_STYLES_H__4513AD12_FAE3_11D1_A662_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define		INVALIDCOLOR	-1

class Font;
class Brush;

struct FStyle
{
	int			style;
	Font*		pFont;
	COLORREF	Fcolor;
	Brush*		pBrush;
	int			Border;
};

struct FontData
{
	char* FaceName;
	int   PointSize;
	char  St;
};

struct FStyleMap
{
	int nFont;
	int nFcolor;
	int nBrush;
	int nBorder;
};

struct StateStyleMap
{
	int		states;
	int*	styles;
};

struct FieldStyleMap
{
	int		states;
	int*	state; //0 - normal, 1 - focused, 2 - selected
};


TEXTMETRIC* GetTextMetrics(int num);
void GetStyle(int num, /*int state, */FStyle* pStyle);
int GetFieldStyle(int num, int state);
COLORREF GetColor(int num);
void InitStyles();
void FreeStyles();

#endif // !defined(AFX_STYLES_H__4513AD12_FAE3_11D1_A662_204C4F4F5020__INCLUDED_)
