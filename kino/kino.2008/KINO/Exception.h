// Exception.h: interface for the Exception class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCEPTION_H__A0975BE2_10A8_11D2_A69D_204C4F4F5020__INCLUDED_)
#define AFX_EXCEPTION_H__A0975BE2_10A8_11D2_A69D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class Log;
extern Log errlog;

typedef void (*FNFREE)(void*);

class Exception  
{
	void*	memptr;
	FNFREE	pfnfree;
public:
	Exception():memptr(NULL), pfnfree(NULL) {};
	virtual ~Exception(){};

	void SetDelMem(void *p, FNFREE pfn = NULL){memptr = p; pfnfree = pfn;};
	virtual void Effect();
	void __cdecl printf(DWORD fmt, ...);
	void __cdecl printf(char* fmt, char code, ...);

	static	Exception ex;
	static	Log*	errlog;
	static	Log*	warnlog;
};

#endif // !defined(AFX_EXCEPTION_H__A0975BE2_10A8_11D2_A69D_204C4F4F5020__INCLUDED_)
