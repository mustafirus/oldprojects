// MDIChild.cpp: implementation of the MDIChild class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Application.h"
#include "MDIChild.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool MDIChild::DestroyWindow()
{
	::SendMessage(::GetParent(*this), WM_MDIDESTROY, (WPARAM)(HWND)(*this), 0);
	return true;
};

