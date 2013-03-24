#if !defined(AFX_FINDER_H__6DFE2D72_0703_11D2_A688_204C4F4F5020__INCLUDED_)
#define AFX_FINDER_H__6DFE2D72_0703_11D2_A688_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Finder.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Finder window
class RSField;

class Finder : public Edit
{
//	HACCEL		m_hAccelTable;       // accelerator table
	RSWhere*	pRSWhere;
	Finder**	ppFinder;
//	int			counter;
// Construction
public:
	Finder(Wnd* pParentWnd, RECT* r, RSWhere* prsw, Finder** ppf);
	virtual ~Finder(){*ppFinder = NULL;};
	void Save(bool empty);

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void OnChar(TCHAR ch, int cRepeat);
	void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDER_H__6DFE2D72_0703_11D2_A688_204C4F4F5020__INCLUDED_)
