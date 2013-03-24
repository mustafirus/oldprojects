// MainWnd.h: interface for the MainWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINWND_H__A3AAAC47_4634_11D2_A720_204C4F4F5020__INCLUDED_)
#define AFX_MAINWND_H__A3AAAC47_4634_11D2_A720_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class FrameThread;
#include "WndLib.h"
//#define CF_RECORD	CF_PRIVATEFIRST
//#define CF_RKEY		(CF_PRIVATEFIRST + 1)
class MainWnd : public MDIMain  
{
	StatusBar* psb;
	FrameThread* pThread;
public:
	MainWnd(UINT resID);
	virtual ~MainWnd();

	bool brk;
	void Wait(bool w = true);
//	void CreateThread(RKey* prk);
protected:
	virtual void PostNcDestroy();
	virtual bool OnCmdUI(CmdUI* pCmdUI);
// Message handlers
	virtual LRESULT WindowProc(UINT Msg, WPARAM wParam, LPARAM lParam);
	void OnMenuSelect(HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);
	void OnClose();
	bool OnSetCursor(HWND hwndCursor, UINT codeHitTest, UINT msg);

	void OnInitMenuPopup( HMENU hmenuPopup, UINT nIndex, BOOL bSysMenu );

DECLARE_COMMAND_MAP()
	void OnCat(UINT nID);
};

#endif // !defined(AFX_MAINWND_H__A3AAAC47_4634_11D2_A720_204C4F4F5020__INCLUDED_)
