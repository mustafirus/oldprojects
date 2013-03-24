// Log.h: interface for the Log class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__B61582A2_EB79_11D1_A62B_204C4F4F5020__INCLUDED_)
#define AFX_LOG_H__B61582A2_EB79_11D1_A62B_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <strstrea.h>

class XDump
{
public:
#ifdef _DEBUG
	XDump& operator<<(const char* str);
#else
	XDump& operator<<(const char* str){return *this;};
#endif
};
extern XDump xDump;

class Log  
{
	HANDLE		hFile;
	HWND		hWnd;
	DWORD		lerr;
	strstream	buf;
public:
	enum DestType{File, EvtLog, Console, MsgBox, Dbg};
	DestType	dtype;
public:
	Log(DestType type, const char* filename = NULL);
	virtual ~Log();

	void SetFile(DestType type, const char* fn = NULL);
	void SetHwnd(HWND hwnd){hWnd = hwnd;};
	void Close();

	void last_error();
	void __cdecl printf(DWORD fmt, ...);
	void __cdecl printf(char* fmt, ...);
	void printf(DWORD fmt, va_list argList);
	void printf(char* fmt, va_list argList);
	void Dump();
	void Reset();

private:

/*	void AddString(char* str);
	void AddToMessageLog(char* lpszMsg);
	void AddToFile(char* lpszMsg);
	void AddToConsole(char* lpszMsg);
*/	
};
static LPTSTR GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize );

#endif // !defined(AFX_LOG_H__B61582A2_EB79_11D1_A62B_204C4F4F5020__INCLUDED_)
