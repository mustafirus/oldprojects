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
	typedef Edit BASECLASS;
public:
	EditControl(Wnd* pWndParent, DWORD dwStyle, Menu*	pm);
	virtual ~EditControl();

	virtual void GetMinMax(PMINMAX pmm){
		pmm->szMin.cx = 50;
		pmm->szMin.cy = 20;
		pmm->szDes.cx = 100;
		pmm->szDes.cy = 25;
		pmm->szMax.cx = 400;
		pmm->szMax.cy = 30;
	}

	void HiliteWindow(bool hi);

	virtual void SetTabs(TabMap* pTab, TabPos first, TabPos& last);
	virtual void SetPos(POINT pt, SIZE sz, HDWP hdwp = NULL);

	void onSetFocus(HWND hwndOldFocus);
	UINT onNCCalcSize(BOOL fCalcValidRects, NCCALCSIZE_PARAMS * lpcsp);

	virtual HBRUSH onCtlColorReflect(HDC hdc, int type);

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	static RECT border;
};

#endif // !defined(AFX_EDITCONTROL_H__03771A23_43CA_11D2_A71D_204C4F4F5020__INCLUDED_)
