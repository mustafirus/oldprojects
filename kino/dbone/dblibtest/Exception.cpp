// Exception.cpp: implementation of the Exception class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Log.h"
#include "Exception.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Exception Exception::ex;
Log*	Exception::errlog = NULL;
Log*	Exception::warnlog = NULL;
//errlog(Log::MsgBox), warnlog(Log::File) 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void __cdecl Exception::printf(DWORD fmt, ...)
{
	va_list argList;
	va_start(argList, fmt);
	int code = fmt >> 30;
	switch(code)
	{
	case 2:
//		warnlog->printf(fmt, argList);
		break;
	case 3:
//		errlog->printf(fmt, argList);
		break;
	}
	va_end(argList);
}

void Exception::Effect()
{
	if(memptr)
	{
		if(pfnfree)
			pfnfree(memptr);
		memptr = NULL;
		pfnfree = NULL;
	}
//	errlog->Dump();
//	warnlog->Dump();
};

void __cdecl Exception::printf(char* fmt, char code, ...)
{
	va_list argList;
	va_start(argList, code);
	switch(code)
	{
	case 'w':
//		warnlog->printf(fmt, argList);
//		warnlog->Dump();
		break;
	case 'e':
//		errlog->printf(fmt, argList);
		break;
	}
	va_end(argList);
}
