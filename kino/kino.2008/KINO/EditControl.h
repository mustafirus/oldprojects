// EditControl.h: interface for the EditControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDITCONTROL_H__03771A23_43CA_11D2_A71D_204C4F4F5020__INCLUDED_)
#define AFX_EDITCONTROL_H__03771A23_43CA_11D2_A71D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Edit.h"
#include "Frame.h"

class EditControl : public Edit, public Frame
{
	bool	focused;
	bool	hilighted;
protected:
	bool	edit;
public:
	EditControl(Wnd* pWndParent, DWORD dwStyle, UINT nID);
	virtual ~EditControl();

	void SetEditMode(bool m);
	void HiliteWindow(bool hi);
	void Invalidate();

	virtual void SetTabs(TabMap* pTab, TabPos first, TabPos& last);
	virtual void SetPos(POINT pt, SIZE sz, HDWP hdwp = NULL);

	virtual void OnChar(TCHAR ch, int cRepeat);
	UINT OnNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp);
	void OnKillFocus(HWND hwndNewFocus);
	void OnSetFocus(HWND hwndOldFocus);
	void OnLButtonDown(bool fDoubleClick, int x, int y, UINT keyFlags);
	virtual void OnKey(UINT vk, bool fDown, int cRepeat, UINT flags);
	void OnMouseMove(int x, int y, UINT keyFlags);
	void OnPaint();
	void OnNCPaint(HRGN hrgn);

	virtual HBRUSH OnCtlColorReflect(HDC hdc, int type);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	static RECT border;
	DECLARE_COMMAND_MAP()
};

#endif // !defined(AFX_EDITCONTROL_H__03771A23_43CA_11D2_A71D_204C4F4F5020__INCLUDED_)
