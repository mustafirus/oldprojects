#include "StdAfx.h"

#include "vector.h"

#include "application.h"

Application::Application(void)
{

}

Application::~Application(void)
{

}

void Application::run(){
	Session* ps = NULL;
	while(ps = waitForSession()){
		_vSessions.add(ps);
		createThread(ps);
	}
}

void Session::run(){
	Action* pa=NULL;
	while(waitForAction(pa)){
		pa
	}
}
