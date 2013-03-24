// Application.h: interface for the Application class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPLICATION_H__E7D87218_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
#define AFX_APPLICATION_H__E7D87218_272F_11D2_A6D9_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "WinThread.h"
class Wnd;

class Application : public WinThread  
{
	friend int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );

	HANDLE		hProcess;
	DWORD		nProcessID;

	HINSTANCE	hInstance;
	const char* pszRegistryKey;
	const char* pszProfileName;
	const char* pCmdLine;
	int			nCmdShow;

protected:
	Wnd*		pMainWnd;
	static Application* pApp;
public:
	char* LoadString(UINT sID);
	Application(): hInstance(NULL), pszRegistryKey(NULL), pszProfileName(NULL),
		pCmdLine(NULL), nCmdShow(SW_SHOWNA), pMainWnd(NULL)
	{ASSERT(pApp == NULL);pApp = this;};
	virtual ~Application()
	{
		ifdel((char*)pszRegistryKey);
		ifdel((char*)pszProfileName);
	};

	virtual void Attach();
	virtual void Detach();
	virtual bool OnIdle();
	virtual bool PreTranslateMessage(MSG* pMsg);

	Wnd*		GetMainWnd(){return pMainWnd;};
	HINSTANCE	GetInstance(){return hInstance;};
	HMENU		LoadMenu(UINT MenuID){return ::LoadMenu(hInstance, MAKEINTRESOURCE(MenuID));};
	HACCEL		LoadAccelerators(UINT asselID){return ::LoadAccelerators(hInstance, MAKEINTRESOURCE(asselID) );};
	HACCEL		LoadAccelerators(const char* szAccel){return ::LoadAccelerators(hInstance, szAccel );};
	HCURSOR		LoadCursor(UINT curID){return ::LoadCursor(hInstance, MAKEINTRESOURCE(curID) );};
	const char*	GetProfileString( const char* szSection, const char* szEntry, const char* szDefault = NULL );
	bool		WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue);
	HKEY		GetAppRegistryKey();
	HKEY		GetSectionKey(LPCTSTR lpszSection);
	void		SetKeyName(const char* company, const char* profile);

 

	static Application* GetApp(){ASSERT(pApp);return pApp;};
};

#endif // !defined(AFX_APPLICATION_H__E7D87218_272F_11D2_A6D9_204C4F4F5020__INCLUDED_)
