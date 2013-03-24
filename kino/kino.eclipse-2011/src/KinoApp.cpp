// KinoApp.cpp: implementation of the KinoApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "dblib.h"
#include "Resource.h"
#include "MainWnd.h"
#include "Grid.h"
#include "Column.h"
#include "FrameWnd.h"
#include "Styles.h"
#include "Splash.h"
#include "Action.h"
#include "Log.h"
#include "Wait.h"
#include "DbException.h"
#include "KinoApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define duser	"dict"
#define dpass	"jdkjfewl"
#define dsn		"kino"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Log errlog(Log::MsgBox);
#ifdef _DEBUG
Log warnlog(Log::Dbg);
#else
#define LOGFILE
Log warnlog(Log::File);
#endif //_DEBUG
bool GetUser(char* user, int userlen, char* pass, int passlen, HINSTANCE resourse_hInst, HWND hWndMain);

bool KinoApp::InitInstance()
{
	try{
	Exception::ex.errlog = &errlog;
	Exception::ex.warnlog = &warnlog;

	SetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDIGITS, "0");
	SetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SGROUPING, "0;0");
	SetKeyName("Mustafa Software", "Kino");
	Wnd::RegisterClass();
	pMainWnd = new MainWnd(IDR_MAINFRAME);
	errlog.SetHwnd(*pMainWnd);
	Wait w;
	SplashWnd::ShowSplashScreen(pMainWnd);
	Action::pAction = new Action;

#ifdef DEBUG_NEW
#undef new
#define REDEF_NEW
#endif //DEBUG_NEW
	Record::pDict = new Dict(new OdbcDataBase (dsn, duser, dpass));
#ifdef REDEF_NEW
#define new DEBUG_NEW
#undef REDEF_NEW
#endif //REDEF_NEW

	FrameWnd::RegisterClass();
	Grid::RegisterClass();
	Column::RegisterClass();
	
//	errlog.SetFile(pDB->user+".log");

	InitStyles();
	char user[10];
	char pass[10];
	if(!GetUser(user, 10, pass, 10, GetInstance(), *pMainWnd))
		return false;
#ifdef LOGFILE
	char fn[_MAX_PATH];
	strcpy(fn,user);
	strcat(fn,".Log");
	warnlog.SetFile(Log::File, fn);
#endif //LOGFILE

	SplashWnd::ShowSplashScreen(pMainWnd);
#ifdef DEBUG_NEW
#undef new
#define REDEF_NEW
#endif //DEBUG_NEW
	Record::pDB = new OdbcDataBase("kino", user, pass);
#ifdef REDEF_NEW
#define new DEBUG_NEW
#undef REDEF_NEW
#endif //REDEF_NEW
#undef new
	while(!Record::pDict->Ready())
		Sleep(5000);
	pMainWnd->ShowWindow(SW_SHOW);
	SplashWnd::EnableHide();
	}catch(Exception* e)
	{
		e->Effect();
		return false;
	}catch(...)
	{
		return false;
	}
	return true;
}
int KinoApp::ExitInstance(int code)
{
	FreeStyles();
	delete Record::pDB;
	delete Record::pDict;
	delete Action::pAction;
	return code;
}

/*bool KinoApp::PreTranslateMessage(MSG* pMsg) 
{
	return SplashWnd::PreTranslateAppMessage(pMsg);
}
*/
KinoApp theApp;
