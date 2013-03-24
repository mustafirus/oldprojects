// StatusBar.cpp: implementation of the StatusBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Application.h"
#include "StatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "comctl32.lib")
//#define TRACE1(X,X1)

#define GetApp() Application::GetApp()
#define CX_PANE_BORDER 6    // 3 pixels on each side of each pane
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StatusBar::StatusBar(Wnd* pParentWnd, const UINT* pIDArray, int nIDCount,  UINT nID) : nCount(0), pSbp(NULL)
{
	HookWndCreate(this);
	if(!CreateStatusWindow( WS_CHILD|WS_VISIBLE|SBARS_SIZEGRIP, NULL, *pParentWnd, nID))
		UnhookWndCreate();
	SetIndicators(pIDArray, nIDCount);
	WINDOWPLACEMENT wp;
	GetWindowPlacement(*pParentWnd, &wp); 
	SetWindowPlacement(*pParentWnd, &wp); 
 //	pParentWnd->SendMessage(WM_SIZE, 0, 0);
}

StatusBar::~StatusBar()
{
	STATUSPANE* ps = pSbp;
	while(nCount--)
		delete ps++->strText;
	delete pSbp;
}

void StatusBar::OnSize(UINT state, int cx, int cy)
{
	if(nCount)
		UpdateAllPanes(true, false);
	Default();
}

void StatusBar::OnUpdateUI()
{
	for(int i = 0; i < nCount; i++)
	{
		
		UINT nVK;
		UINT flag = 0x0001;
		
		switch (pSbp[i].nID)
		{
		case ID_INDICATOR_CAPS:
			nVK = VK_CAPITAL;
			break;
			
		case ID_INDICATOR_NUM:
			nVK = VK_NUMLOCK;
			break;
			
		case ID_INDICATOR_SCRL:
			nVK = VK_SCROLL;
			break;
		default:
			nVK = 0;
		}
		if(nVK)
		{
			UINT nNewStyle = pSbp[i].nStyle & ~SBPS_DISABLED;
			if (!::GetKeyState(nVK) & flag)
				nNewStyle |= SBPS_DISABLED;
			pSbp[i].nStyle = nNewStyle;
			pSbp[i].nFlags |= SBPF_UPDATE;
		}
	}
	UpdateAllPanes(false, true);
}

void StatusBar::OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags)
{
	int index;
	if((flags & MF_POPUP) == 0)
		if((index = CommandToIndex(ID_INDICATOR_TEXT)) >= 0)
		{
			SetPaneText(index, item, true);
		}
}

LRESULT StatusBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	DISPATCH
		ON_MSG(WM_SIZE, OnSize)
		ON_MSG(WM_IDLEUPDATECMDUI, OnUpdateUI)
		ON_MSG(WM_MENUSELECT, OnMenuSelect)
	ROUGHT(Wnd::WindowProc);
}

int StatusBar::CommandToIndex(UINT nIDFind) const
{
	if (nCount <= 0)
		return -1;

	STATUSPANE* pSBP = pSbp;
	for (int i = 0; i < nCount; i++, pSBP++)
		if (pSBP->nID == nIDFind)
			return i;

	return -1;
}

bool StatusBar::SetIndicators(const UINT* pIDArray, int nIDCount)
{
	ASSERT(nIDCount >= 1);  // must be at least one of them
	ASSERT(::IsWindow(*this));

	// first allocate array for panes and copy initial data

	STATUSPANE* pSBP;
	if(pSbp)
		delete pSbp;
	pSBP = pSbp = new STATUSPANE[nIDCount];
	nCount = nIDCount;
	pSbp = pSBP;
	memset(pSbp, 0, sizeof(STATUSPANE) * nIDCount);
	// copy initial data from indicator array
	bool bResult = true;
	if (pIDArray != NULL)
	{
		HFONT hFont = (HFONT)SendMessage(WM_GETFONT);
		HDC hDC = GetDC(NULL);
//		CClientDC dcScreen(NULL);
		HGDIOBJ hOldFont = NULL;
		if (hFont != NULL)
			hOldFont = SelectObject(hDC, hFont);
//			hOldFont = dcScreen.SelectObject(hFont);

//		STATUSPANE* pSBP = _GetPanePtr(0);
		for (int i = 0; i < nIDCount; i++)
		{
			char* s1 = NULL;
			pSBP->nID = *pIDArray++;
			pSBP->nFlags |= SBPF_UPDATE;
			if (pSBP->nID != 0 && pSBP->nID < ID_INDICATOR_DYNAMIC)
			{
				pSBP->strText = GetApp()->LoadString(pSBP->nID);
				if (!pSBP->strText)
				{
					TRACE1("Warning: failed to load indicator string 0x%04X.\n",
						pSBP->nID);
					bResult = false;
					break;
				}
				SIZE size;
				::GetTextExtentPoint(hDC, pSBP->strText, strlen(pSBP->strText), &size);
				pSBP->cxText = size.cx;
				ASSERT(pSBP->cxText >= 0);
				SetPaneText(i, pSBP->strText, false);
			}
			else
			{
				// no indicator (must access via index)
				// default to 1/4 the screen width (first pane is stretchy)
				pSBP->cxText = ::GetSystemMetrics(SM_CXSCREEN)/4;
				if (i == 0)
					pSBP->nStyle |= (SBPS_STRETCH | SBPS_NOBORDERS);
			}
			++pSBP;
		}
		if (hOldFont != NULL)
			SelectObject(hDC, hOldFont);
		ReleaseDC(NULL, hDC);
//			dcScreen.SelectObject(hOldFont);

	}
	UpdateAllPanes(TRUE, TRUE);

	return bResult;
}

void StatusBar::SetPaneText(int nIndex, LPCTSTR lpszNewText, bool bUpdate)
{
	ASSERT(::IsWindow(*this));
	
	STATUSPANE* pSBP = pSbp + nIndex;
	
	if (!(pSBP->nFlags & SBPF_UPDATE) &&
		((lpszNewText == NULL && pSBP->strText == NULL) ||
		 (lpszNewText != NULL && pSBP->strText && strcmp(pSBP->strText,lpszNewText) == 0)))
	{
		// nothing to change
		return;
	}
	if (lpszNewText != NULL)
	{
		if(lpszNewText != pSBP->strText)
		{
			if(pSBP->strText)
				delete pSBP->strText;
			pSBP->strText = new char(strlen(lpszNewText)+1);
			strcpy(pSBP->strText, lpszNewText);
		}
	}
	else
	{
		if(pSBP->strText)
			delete pSBP->strText;
		pSBP->strText = NULL;
	}
	if (!bUpdate)
	{
		// can't update now, wait until later
		pSBP->nFlags |= SBPF_UPDATE;
		return;
	}
	
	pSBP->nFlags &= ~SBPF_UPDATE;
	DefWindowProc(SB_SETTEXT, ((WORD)pSBP->nStyle)|nIndex,
		(pSBP->nStyle & SBPS_DISABLED) ? NULL :
	(LPARAM)(LPCTSTR)pSBP->strText);
}

void StatusBar::SetPaneText(int nIndex, UINT nID, bool bUpdate)
{
	ASSERT(::IsWindow(*this));
	char* s1 = GetApp()->LoadString(nID);
	
	STATUSPANE* pSBP = pSbp + nIndex;
	
	if (!(pSBP->nFlags & SBPF_UPDATE) &&
		((s1 == NULL && pSBP->strText == NULL) ||
		 (s1 != NULL && pSBP->strText != NULL && strcmp(pSBP->strText, s1) == 0)))
	{
		if(s1)
			delete s1;
		return;
	}
	if (s1 != NULL)
	{
		if(s1 != pSBP->strText)
		{
			if(pSBP->strText)
				delete pSBP->strText;
			pSBP->strText = s1;
		}
	}else
	{
		if(pSBP->strText)
			delete pSBP->strText;
		pSBP->strText = NULL;
	}
	if (!bUpdate)
	{
		// can't update now, wait until later
		pSBP->nFlags |= SBPF_UPDATE;
		return;
	}
	
	pSBP->nFlags &= ~SBPF_UPDATE;
	DefWindowProc(SB_SETTEXT, ((WORD)pSBP->nStyle)|nIndex,
		(pSBP->nStyle & SBPS_DISABLED) ? NULL :
	(LPARAM)(LPCTSTR)pSBP->strText);
}

void StatusBar::UpdateAllPanes(bool bUpdateRects, bool bUpdateText)
{
	ASSERT(::IsWindow(*this));

	// update the status pane locations
	if (bUpdateRects)
	{
		// get border information and client work area
		RECT rect; GetWindowRect(*this, &rect);
		OffsetRect(&rect, -rect.left, -rect.top);
		CalcInsideRect(rect, TRUE);
		int rgBorders[3];
		DefWindowProc(SB_GETBORDERS, 0, (LPARAM)&rgBorders);

		// determine extra space for stretchy pane
		int cxExtra = rect.right - rect.left + rgBorders[2];
		int nStretchyCount = 0;
		STATUSPANE* pSBP = pSbp;
		for (int i = 0; i < nCount; i++)
		{
			if (pSBP->nStyle & SBPS_STRETCH)
				++nStretchyCount;
			cxExtra -= (pSBP->cxText+CX_PANE_BORDER + rgBorders[2]);
			++pSBP;
		}

		// determine right edge of each pane
		int* rgRights = (int*)_alloca(nCount * sizeof(int));
		int right = rgBorders[0];
		pSBP = pSbp;
		for (i = 0; i < nCount; i++)
		{
			// determine size of the pane
			ASSERT(pSBP->cxText >= 0);
			right += pSBP->cxText+CX_PANE_BORDER;
			if ((pSBP->nStyle & SBPS_STRETCH) && cxExtra > 0)
			{
				ASSERT(nStretchyCount != 0);
				int cxAddExtra = cxExtra / nStretchyCount;
				right += cxAddExtra;
				--nStretchyCount;
				cxExtra -= cxAddExtra;
			}
			rgRights[i] = right;

			// next pane
			++pSBP;
			right += rgBorders[2];
		}

		// set new right edges for all panes
		VERIFY(DefWindowProc(SB_SETPARTS, nCount, (LPARAM)rgRights));
//		DefWindowProc(SB_SETPARTS, nCount, (LPARAM)rgRights);
	}

	// update text in the status panes if specified
	if (bUpdateText)
	{
		STATUSPANE* pSBP = pSbp;
		for (int i = 0; i < nCount; i++)
		{
			if (pSBP->nFlags & SBPF_UPDATE)
				SetPaneText(i, pSBP->strText);
			++pSBP;
		}
	}
}

void StatusBar::CalcInsideRect(RECT& rect, bool bHorz) const
{
	ASSERT(::IsWindow(*(StatusBar*)this));
	ASSERT(bHorz);  // vertical status bar not supported

	// subtract standard CControlBar borders
	//CompatCalcInsideRect(rect, bHorz);

	// subtract size grip if present
	StatusBar* pBar = (StatusBar*)this;
	if ((GetStyle() & SBARS_SIZEGRIP) && !::IsZoomed(::GetParent(*pBar)))
	{
		// get border metrics from common control
		int rgBorders[3];
		pBar->DefWindowProc(SB_GETBORDERS, 0, (LPARAM)&rgBorders);

		// size grip uses a border + size of scrollbar + cx border
		rect.right -= rgBorders[0] + ::GetSystemMetrics(SM_CXVSCROLL) +
			::GetSystemMetrics(SM_CXBORDER) * 2;
	}
}

/*void CompatCalcInsideRect(CRect& rect, BOOL bHorz) const
{
	DWORD dwStyle = m_dwStyle;

	if (dwStyle & CBRS_BORDER_LEFT)
		rect.left += afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_TOP)
		rect.top += afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_RIGHT)
		rect.right -= afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect.bottom -= afxData.cyBorder2;

	// inset the top and bottom.
	if (bHorz)
	{
		rect.left += m_cxLeftBorder;
		rect.top += m_cyTopBorder;
		rect.right -= m_cxRightBorder;
		rect.bottom -= m_cyBottomBorder;
	}
	else
	{
		rect.left += m_cyTopBorder;
		rect.top += m_cxLeftBorder;
		rect.right -= m_cyBottomBorder;
		rect.bottom -= m_cxRightBorder;
	}
}
*/
