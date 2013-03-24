#include "StdAfx.h"
#include "DataContext.h"
#include "MainWnd.h"

#include "FrameThread.h"
#include "Menu.h"
#include "dbclientapp.h"

dbClientApp::~dbClientApp() {
	while(FrameThread::pHead) delete FrameThread::pHead;
//	FreeStyles();
//	delete Record::pDB;
//	delete Record::pDict;
//	delete Action::pAction;
//	return code;
}

dbClientApp::dbClientApp(HINSTANCE hi) : Application(hi) {
	SetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDIGITS, "0");
	SetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SGROUPING, "0;0");
	Wnd::RegisterClass();
	FrameWnd::RegisterClass();

	Menu* pMenu = new Menu();
	Menu* mnFile = pMenu->addMenu("File");
	Menu* mnEdit = pMenu->addMenu("Edit");
	Menu* mnHelp = pMenu->addMenu("Help");
	mnFile->addCmd("New", MainWnd::onFileNew);
	mnFile->addCmd("CloseAll", MainWnd::onCloseAll);
	mnFile->addCmd("New", MainWnd::onFileNew);
	mnFile->addSep();
	mnFile->addCmd("Exit", MainWnd::onExit);
	mnHelp->addCmd("About", MainWnd::onExit);

	pMainWnd = new MainWnd(pMenu);
//	pMainWnd->show();

//	TestDC* p = new TestDC;
//	new FrameThread(p);
//	new FrameThread(new TestDC);
//	FrameWnd* p = new FrameWnd(new TestDC);

//	MainWnd* p = dynamic_cast<MainWnd*>(pMainWnd);
}