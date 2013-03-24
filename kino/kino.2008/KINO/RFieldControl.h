// RFieldControl.h: interface for the RFieldControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFIELDCONTROL_H__B5CF9D32_4265_11D2_A71C_204C4F4F5020__INCLUDED_)
#define AFX_RFIELDCONTROL_H__B5CF9D32_4265_11D2_A71C_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WndLib.h"
#include "EditControl.h"
#include "Defines.h"
class RField;

class RFieldControl : public EditControl
{
	int		style;
	RField*	pRField;
	char	ver;
//	Static	Caption;
public:
//	RFieldControl(RField* prf, RECT* pr, Wnd* pWndParent, UINT nID);
	RFieldControl(RField* prf, Wnd* pWndParent, UINT nID);
	virtual ~RFieldControl();
//	virtual void PostNcDestroy(){};

//Operations
	const char* GetCaption();
	void GetCaptionSize(SIZE& sz);
	void Activate(UINT syskey);
//Frame Operations
	virtual void GetMinMax(PMINMAX pmm);
//	void GetDims(RDIMS* pdims);
//	void SetPos(RECT* r);

// Overidables 
	virtual HBRUSH OnCtlColorReflect(HDC hdc, int type);

// Message Handlers
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void OnPaint();
	bool OnCmdUI(CmdUI* pCmdUI);
	void OnContextMenu(HWND hwndContext, UINT xPos, UINT yPos);
	void OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);
	bool OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg);
	void OnLButtonDblClk(bool fDoubleClick, int x, int y, UINT keyFlags);
	void OnKillFocus(HWND hwndNewFocus);
	void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);
	void OnChar(TCHAR ch, int cRepeat) ;

	void OnEdit(UINT nID);
	DECLARE_COMMAND_MAP()
};

#endif // !defined(AFX_RFIELDCONTROL_H__B5CF9D32_4265_11D2_A71C_204C4F4F5020__INCLUDED_)
