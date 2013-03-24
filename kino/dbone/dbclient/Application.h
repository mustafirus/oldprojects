// Application.h: interface for the Application class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPLICATION_H__E7D87218_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
#define AFX_APPLICATION_H__E7D87218_272F_11D2_A6D9_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WinThread.h"
#include "string.h"
class Wnd;

class Application : public WinThread  
{
	HANDLE		hProcess;
	DWORD		nProcessID;

	HINSTANCE	hInstance;
//	string		pszRegistryKey;
//	string		pszProfileName;
//	string		pCmdLine;
//	int			nCmdShow;

protected:
	Wnd*		pMainWnd;
	static Application* pApp;
public:
	char* LoadString(UINT sID);
	Application(HINSTANCE	hi);
	virtual ~Application();

//	virtual void Attach();
//	virtual void Detach();
	virtual bool onIdle();
	virtual bool preTranslateMessage(MSG* pMsg);

	Wnd*		getMainWnd()		{ return pMainWnd;	}
	void		clearMainWnd()		{ pMainWnd = NULL;	}
	HINSTANCE	getInstance()		{ return hInstance;	};
	HMENU		LoadMenu(UINT MenuID){return ::LoadMenu(hInstance, MAKEINTRESOURCE(MenuID));};
	HACCEL		LoadAccelerators(UINT asselID){return ::LoadAccelerators(hInstance, MAKEINTRESOURCE(asselID) );};
	HACCEL		LoadAccelerators(const char* szAccel){return ::LoadAccelerators(hInstance, szAccel );};
	HCURSOR		LoadCursor(UINT curID){return ::LoadCursor(hInstance, MAKEINTRESOURCE(curID) );};
//	const char*	GetProfileString( const char* szSection, const char* szEntry, const char* szDefault = NULL );
//	bool		WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
//	HKEY		GetAppRegistryKey();
//	HKEY		GetSectionKey(LPCTSTR lpszSection);
//	void		SetKeyName(const char* company, const char* profile);

 

	static Application* getApp(){ ASSERT(pApp);return pApp; };
};

#endif // !defined(AFX_APPLICATION_H__E7D87218_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
