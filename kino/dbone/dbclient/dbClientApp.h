#pragma once
#include "application.h"

class dbClientApp :
	public Application
{
public:

	bool onIdle(){
		TRACE1("Focus %x\n", ::GetFocus());
		return Application::onIdle();
	}
	dbClientApp(HINSTANCE hi);
	virtual ~dbClientApp();
//	bool InitInstance();
//	int ExitInstance(int code);
};
