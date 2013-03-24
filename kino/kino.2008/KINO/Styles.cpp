// Styles.cpp: implementation of the Styles class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Defines.h"
#include "Styles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define		MY_FIRST_BRUSH	6
#define		INVALIDCOLOR	-1

#define		FONTNUM		4
#define		COLORNUM	5
#define		BRUSHNUM	1 // 0-for NULL Brush, 1 - for BackGround 2 - BlackBrash
#define		BORDERNUM	4
static int	gFonts	=	0;
static int	gColors =	COLORNUM;
static int	gBrushs =	0;
static int	gBorders=	BORDERNUM;

static FontData	gFontData[FONTNUM]=
{
	"Times New Roman Cyr", 100, 'B',
	"Arial Cyr", 100, 'N',
	"Arial Cyr", 100, 'B',
	"Arial Cyr", 100, 'I'
};

static LOGFONT*	gLogFont = NULL;
static int		gLogFonts = 0;

static COLORREF	gBrushData[BRUSHNUM]={0x000000ff};

static Font**	gFontPalette = NULL;
static COLORREF	gColorPalette[COLORNUM]={0, 0xff, RGB(0, 0, 255), RGB(255, 0, 0), RGB(125, 40, 40)};
static Brush*	gBrushPalette[BRUSHNUM+MY_FIRST_BRUSH];
static int		gBorderPalette[BORDERNUM]=
{
	-1, 
	MAKELONG(BDR_RAISEDINNER,BF_BOTTOMRIGHT),
	MAKELONG(EDGE_RAISED,BF_RECT),
	MAKELONG(BDR_RAISEDINNER,BF_LEFT|BF_RIGHT)
};


static FStyleMap gFStylePalete[]=
{//	Font	Color	Brush		Border
	{0,		0,		-1,			0},		//Default Frame
	{0,		0,		-1,			0},		//Field Frame
	{0,		0,		-1,			1},		//List Frame
	{0,		0,		-1,			1},		//List Data Frame
	{1,		0,		-1,			0},		//Select Frame

	{1,		4,		-1,			0},		//Focused Select Frame
	{1,		0,		-1,			1},		//DogFrame
	{1,		0,		4,			0},		//Focused Dog Frame
	{0,		0,		-1,			0},		//Titul Frame
	{0,		0,		-1,			2},		//Focused Titul Frame

//-------------------------------------------------------
// Forms 10 - 19
//	Font	Color	Brush		Border
	{1,		0,		-1,			1},		//All data
	{1,		4,		-1,			2},		//Focused All data
	{2,		0,		-1,			1},		//Breed
	{2,		4,		-1,			2},		//Focused Breed
	{3,		0,		-1,			1},		//Parent

	{3,		4,		-1,			2},		//Focused Parent
	{0,		0,		-1,			0},		//Reserved
	{0,		0,		-1,			0},		//Reserved
	{0,		0,		-1,			0},		//Reserved
	{0,		0,		-1,			0},		//Reserved
//-------------------------------------------------------
// Lists 20 - 29
//	Font	Color	Brush		Border
	{1,		0,		-1,			3},		//All data
	{1,		4,		-1,			3},		//Focused All data
	{2,		0,		-1,			3},		//Breed
	{2,		4,		-1,			3},		//Focused Breed
	{3,		0,		-1,			3},		//Parent

	{3,		4,		-1,			3},		//Focused Parent
	{1,		0,		-1,			0},		//Catalog
	{1,		4,		-1,			0},		//Focused Catalog
	{0,		0,		-1,			0},		//Reserved
	{0,		0,		-1,			0}		//Reserved
};
static int Default[]	=	{10,11,20,21};
static int Breed[]		=	{12,13,22,23};
static int Parent[]		=	{14,15,24,25};
static int Catalog[]	=	{10,11,26,27};

/*int		  Frame::defstyle = 0;
int   DataFrame::defstyle = 3;
int    DogFrame::defstyle = 6;
int    Pedigree::defstyle = 6;
*/
//int    TitulFrame::defstyle = 8;

static FieldStyleMap gFieldStyleMap[] =
{
	{4,	Default},
	{4,	Breed},
	{4,	Parent},
	{4,	Catalog}
};

int GetFieldStyle(int num, int state)
{
	ASSERT(state < gFieldStyleMap[num].states);
	return gFieldStyleMap[num].state[state];
}

COLORREF GetColor(int num)
{
	if(num >= 0 && num < COLORNUM)
		return gColorPalette[num];
	else
		return 0;
}

TEXTMETRIC* GetTextMetrics(int num)
{
	return (gFStylePalete[num].nFont >= 0 &&
		gFStylePalete[num].nFont < gFonts) ? 
		*(gFontPalette[gFStylePalete[num].nFont]) : (TEXTMETRIC*)NULL;
}

void GetStyle(int num, /*int state, */FStyle* pStyle)
{
	FStyleMap& map	=	gFStylePalete[num /*gStylePalete[num].styles[state]*/];
	pStyle->pFont	=	(map.nFont >= 0 && map.nFont < gFonts) ? 
		gFontPalette[map.nFont] : NULL;
	pStyle->Fcolor	=	(map.nFcolor >= 0 && map.nFcolor < gColors) ?
		gColorPalette[map.nFcolor] : INVALIDCOLOR;
	pStyle->pBrush	=	(map.nBrush >= 0 && map.nBrush < gBrushs) ?
		gBrushPalette[map.nBrush] : NULL;
	pStyle->Border	=	(map.nBorder >= 0 && map.nBorder < gBorders)?
		gBorderPalette[map.nBorder] :NULL;
}

static void BrushInit()
{
	for(int i = 0; i < MY_FIRST_BRUSH;i++)
	{
		gBrushPalette[i] = new Brush;
		gBrushPalette[i]->CreateStockObject(i);
	}
	for(i = MY_FIRST_BRUSH; i < MY_FIRST_BRUSH + BRUSHNUM; i++)
	{
		gBrushPalette[i] = new Brush((COLORREF)gBrushData[i-MY_FIRST_BRUSH]);
	}
	gBrushs = MY_FIRST_BRUSH + BRUSHNUM;
}
static void BrushFree()
{
	for(int i = 0; i < gBrushs; i++)
	{
		delete gBrushPalette[i];
	}
	gBrushs = 0;
}

static void FontInit()
{
	if(gLogFont && gLogFonts)
	{
		gFontPalette = new Font*[gLogFonts];
		for(int i = 0; i < gLogFonts; i++)
		{
			gFontPalette[i] = new Font;
			gFontPalette[i]->CreateFontIndirect(&gLogFont[i]);
		}
		gFonts = gLogFonts;
		gLogFonts = 0;
		delete[] gLogFont;
	}else
	{
		gFontPalette = new Font*[FONTNUM];
		for(int i = 0; i < FONTNUM; i++)
		{
			gFontPalette[i] = new Font;
			gFontPalette[i]->CreatePointFont( gFontData[i].PointSize, gFontData[i].FaceName);
		}
		gFonts = FONTNUM;
	}
}

static void FontFree()
{
	for(int i = 0; i < gFonts; i++)
	{
		delete gFontPalette[i];
	}
	delete[] gFontPalette;
	gFonts = 0;
}

void LoadFontData()
{
	HANDLE hFile = CreateFile("Fonts.lgf", GENERIC_READ, 0, NULL, OPEN_ALWAYS, 0, NULL);
	if(!hFile || !GetFileSize(hFile, NULL))
		return;
	int count = 0;
	DWORD dwrd;
	ReadFile(hFile, &count, sizeof(count), &dwrd, NULL);
	if(count)
	{
		gLogFont = new LOGFONT[count];
		ReadFile(hFile, gLogFont, sizeof(LOGFONT)*count, &dwrd, NULL);
	}
	gLogFonts = count;
}

void InitStyles()
{
	BrushInit();
	LoadFontData();
	FontInit();
}

void LoadStyles()
{
}
void SaveStyles()
{
}

void FreeStyles()
{
	BrushFree();
	FontFree();
}

bool Font::CreateFontIndirect(const LOGFONT* lpLogFont)
{
	HDC hDC = ::GetDC(NULL);
	hFont = ::CreateFontIndirect(lpLogFont);
	SelectObject(hDC, hFont);
	GetTextMetrics(hDC, &tm);
	ReleaseDC(NULL, hDC);
	return hFont > 0;
};

bool Font::CreatePointFontIndirect(const LOGFONT* lpLogFont)
{
	HDC hDC = ::GetDC(NULL);
	
	// convert nPointSize to logical units based on pDC
	LOGFONT logFont = *lpLogFont;
	POINT pt;
	pt.y = ::GetDeviceCaps(hDC, LOGPIXELSY) * logFont.lfHeight;
	pt.y /= 720;    // 72 points/inch, 10 decipoints/point
	::DPtoLP(hDC, &pt, 1);
	POINT ptOrg = { 0, 0 };
	::DPtoLP(hDC, &ptOrg, 1);
	logFont.lfHeight = -abs(pt.y - ptOrg.y);
	ReleaseDC(NULL, hDC);
	return CreateFontIndirect(&logFont);
}

bool Font::CreatePointFont(int nPointSize, const char* lpszFaceName)
{
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfHeight = nPointSize;
	lstrcpyn(logFont.lfFaceName, lpszFaceName, _countof(logFont.lfFaceName));
	return CreatePointFontIndirect(&logFont);
}
