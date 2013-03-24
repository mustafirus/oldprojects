// mdUI.h: interface for the CmdUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MDUI_H__F84406A2_3F6D_11D2_A719_204C4F4F5020__INCLUDED_)
#define AFX_MDUI_H__F84406A2_3F6D_11D2_A719_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CmdUI  
{
	HMENU	hMenu;
	UINT	nID;
public:
	CmdUI(HMENU hmenu) : hMenu(hmenu), nID(0){};
	~CmdUI(){};
	CmdUI* operator[](UINT n){nID = GetMenuItemID(hMenu, n); return this;}
	bool operator==(UINT n){return nID == n;}
	operator UINT(){return nID;}
	int GetCount(){return GetMenuItemCount(hMenu);};
	void Enable(bool enable = true)
	{
		EnableMenuItem(hMenu, nID, (enable ? MF_ENABLED : MF_DISABLED|MF_GRAYED));
	};
};

#endif // !defined(AFX_MDUI_H__F84406A2_3F6D_11D2_A719_204C4F4F5020__INCLUDED_)
