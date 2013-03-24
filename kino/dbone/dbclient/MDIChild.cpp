// MDIChild.cpp: implementation of the MDIChild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Application.h"
#include "MDIMain.h"
#include "MDIChild.h"

MDIChild::MDIChild(const char* classname, const char* szTitle){
	WinThread::HookWndCreate(this);
	CreateMDIWindow(classname ? classname : "WND", szTitle,
		MDIS_ALLCHILDSTYLES /*| WS_CLIPCHILDREN*/, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
		getMainWnd()->getClient(), getApp()->getInstance(), 0);
	if(!*this){
		WinThread::UnhookWndCreate(); 
		throw 1;
	}
	hide();
}

MDIChild::~MDIChild(){
//	::SendMessage(::GetParent(*this), WM_MDIDESTROY, (WPARAM)hWnd, 0);
//	hWnd = NULL;
};

MDIMain* MDIChild::getMainWnd() {
	Wnd* p = Application::getApp()->getMainWnd();
	ASSERT( dynamic_cast<MDIMain*> (p) );
	return static_cast<MDIMain*> (p);
}

