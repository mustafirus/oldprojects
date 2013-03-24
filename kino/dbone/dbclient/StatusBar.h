// StatusBar.h: interface for the StatusBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATUS_H__F84406A3_3F6D_11D2_A719_204C4F4F5020__INCLUDED_)
#define AFX_STATUS_H__F84406A3_3F6D_11D2_A719_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Wnd.h"
#include "Indicate.h"

#define SBPS_NORMAL     0x0000
#define SBPS_NOBORDERS  SBT_NOBORDERS
#define SBPS_POPOUT     SBT_POPOUT
#define SBPS_OWNERDRAW  SBT_OWNERDRAW
#define SBPS_DISABLED   0x04000000
#define SBPS_STRETCH    0x08000000  // stretch to fill status bar

#define SBPF_UPDATE 0x0001  // pending update of text

struct STATUSPANE
{
	UINT    nID;        // IDC of indicator: 0 => normal text area
	int     cxText;     // width of string area in pixels
						//   on both sides there is a 3 pixel gap and
						//   a one pixel border, making a pane 6 pixels wider
	UINT    nStyle;     // style flags (SBPS_*)
	UINT    nFlags;     // state flags (SBPF_*)
	char* strText;    // text in the pane
};

class StatusBar : public Wnd  
{
	int				nCount;
	STATUSPANE*		pSbp;
public:
	StatusBar(Wnd* pParentWnd, const UINT* pIDArray, int nIDCount,  UINT nID);
	virtual ~StatusBar();
	bool SetIndicators(const UINT* pIDArray, int nIDCount);
	void SetPaneText(int nIndex, LPCTSTR lpszNewText, bool bUpdate = true);
	void SetPaneText(int nIndex, UINT nID, bool bUpdate);
	void UpdateAllPanes(bool bUpdateRects, bool bUpdateText);
	void CalcInsideRect(RECT& rect, bool bHorz) const;
	int CommandToIndex(UINT nIDFind) const;

	void onUpdateUI();
	void onSize(UINT state, int cx, int cy);
	void onMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif // !defined(AFX_STATUS_H__F84406A3_3F6D_11D2_A719_204C4F4F5020__INCLUDED_)
