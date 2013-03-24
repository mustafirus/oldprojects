// Wait.cpp: implementation of the Wait class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "MainWnd.h"
#include "Wait.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int Wait::count = NULL;

Wait::Wait()
{
	count++;
	((MainWnd*)Application::GetApp()->GetMainWnd())->Wait();
}

Wait::~Wait()
{
	if(!--count)
		((MainWnd*)Application::GetApp()->GetMainWnd())->Wait(false);
}
