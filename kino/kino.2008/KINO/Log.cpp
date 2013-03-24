// Log.cpp: implementation of the Log class.
//
//////////////////////////////////////////////////////////////////////
#include "stdx.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
#endif

#define MINBUFSIZE	255

XDump xDump;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Log::Log(DestType type, const char* filename ) : hFile(NULL), lerr(0)
{
	SetFile(type, filename);
}

void Log::SetFile(DestType type, const char* filename)
{
	dtype = type;
	char* fn=(char* ) filename;
	bool fnew=false;
	if(!fn)
	{
		fnew=true;
		fn = new char[_MAX_PATH];
		GetModuleFileName(NULL, fn ,_MAX_PATH);
		char* s1=strrchr(fn,'.');
		if(!s1)
			throw -1;
		strcpy(++s1,"log");

	}
	if(hFile)
		Close();
	switch(dtype)
	{
	case File:
		hFile = CreateFile(fn, GENERIC_WRITE, FILE_SHARE_READ, NULL,
			OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		SetFilePointer(hFile, 0, 0, FILE_END);
		break;
	case EvtLog:
		hFile = RegisterEventSource(NULL, fn);
		break;
	case Console:
		{
		hFile = CreateFile("CONOUT$", GENERIC_WRITE, FILE_SHARE_READ, NULL,
			OPEN_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		DWORD ret = GetLastError();
		SetStdHandle(STD_OUTPUT_HANDLE, hFile);
		break;
		}
	default:
		hFile=NULL;
	}
	if(fnew)
		delete fn;
//	buf=NULL;
}

void Log::Close()
{
	if(hFile)
	switch(dtype)
	{
	case File:
		CloseHandle(hFile);
		break;
	case EvtLog:
		DeregisterEventSource(hFile);
		break;
	}
/*	if(buf)
		delete buf;
	buf = NULL;
*/
}

Log::~Log()
{
	Close();
}

void Log::last_error()
{
/*	char buf[255];
	switch(dtype)
	{
	case File:
		AddToFile(GetLastErrorText(buf,255));
		break;
	case EvtLog:
		AddToMessageLog(GetLastErrorText(buf,255));
	case Console:
		AddToConsole(GetLastErrorText(buf,255));
		break;
	}
*/
}
void Log::Dump()
{
	switch(dtype)
	{
	case File:
		{
			DWORD NumberOfBytesWritten;
			WriteFile(hFile, buf.str(), buf.pcount(), &NumberOfBytesWritten, NULL);
		}
		break;
	case Console:
		{
			DWORD NumberOfBytesWritten;
			WriteConsole(hFile, buf.str(), buf.pcount(), &NumberOfBytesWritten, NULL);
		}
		break;
	case EvtLog:
		{
			char   szMsg[256];
			const char*  lpszStrings[2];
			sprintf(szMsg, TEXT("Error: %d"), lerr);
			lpszStrings[0] = szMsg;
			lpszStrings[1] = buf.str();
			
			if (dtype == EvtLog)
				ReportEvent(hFile, // handle of event source
				EVENTLOG_ERROR_TYPE,  // event type
				0,                    // event category
				0,                    // event ID
				NULL,                 // current user's SID
				2,                    // strings in lpszStrings
				0,                    // no bytes of raw data
				lpszStrings,          // array of error strings
				NULL);                // no raw data
			
		}
		break;
	case MsgBox:
		buf << ends;
		MessageBox(GetForegroundWindow(), buf.str(), "Error!", MB_TOPMOST|
			MB_SETFOREGROUND|MB_TASKMODAL|MB_OK);
		break;
	case Dbg:
		buf << ends;
		xDump << buf.str();
		break;
	}
	buf.rdbuf()->freeze(0);
	buf.seekp(0);
}
























/*void Log::AddString(char* str)
{
	switch(dtype)
	{
	case File:
		AddToFile(str);
		break;
	case Console:
		AddToConsole(str);
		break;
	case EvtLog:
		AddToMessageLog(str);
		break;
#ifdef _DEBUG
	case Dbg:
		xDump << str;
#endif //_DEBUG
	}
}
*/
/*void Log::AddToMessageLog(char* lpszMsg)
{
    char   szMsg[256];
    const char*  lpszStrings[2];
	
	
	DWORD dwErr = GetLastError();
	
	// Use event logging to log the error.
	//
	
	sprintf(szMsg, TEXT("Error: %d"), dwErr);
	lpszStrings[0] = szMsg;
	lpszStrings[1] = lpszMsg;
	
	if (dtype == EvtLog)
		ReportEvent(hFile, // handle of event source
			EVENTLOG_ERROR_TYPE,  // event type
			0,                    // event category
			0,                    // event ID
			NULL,                 // current user's SID
			2,                    // strings in lpszStrings
			0,                    // no bytes of raw data
			lpszStrings,          // array of error strings
			NULL);                // no raw data
		
}
void Log::AddToFile(char* lpszMsg)
{
}
void Log::AddToConsole(char* lpszMsg)
{
}
*/

//Helpers
//
//  FUNCTION: GetLastErrorText
//
//  PURPOSE: copies error message text to string
//
//  PARAMETERS:
//    lpszBuf - destination buffer
//    dwSize - size of buffer
//
//  RETURN VALUE:
//    destination buffer
//
//  COMMENTS:
//
static LPTSTR GetLastErrorText( LPTSTR lpszBuf, DWORD dwSize )
{
    DWORD dwRet;
    LPTSTR lpszTemp = NULL;
	
    dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
		NULL,
		GetLastError(),
		LANG_NEUTRAL,
		(LPTSTR)&lpszTemp,
		0,
		NULL );
	
    // supplied buffer is not long enough
    if ( !dwRet || ( (long)dwSize < (long)dwRet+14 ) )
        lpszBuf[0] = TEXT('\0');
    else
    {
        lpszTemp[lstrlen(lpszTemp)-2] = TEXT('\0');  //remove cr and newline character
        sprintf( lpszBuf, TEXT("%s (0x%x)"), lpszTemp, GetLastError() );
    }
	
    if ( lpszTemp )
        LocalFree((HLOCAL) lpszTemp );
	
    return lpszBuf;
}

void __cdecl Log::printf(DWORD fmt, ...)
{
    DWORD dwRet;
    DWORD dwFlags;
    LPTSTR lpszTemp = NULL;
	if(HIWORD(fmt) & 0xFFF > 0xFF)
		dwFlags = FORMAT_MESSAGE_FROM_HMODULE;
	else
		dwFlags = FORMAT_MESSAGE_FROM_SYSTEM;
	
	va_list argList;
	va_start(argList, fmt);
    dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | dwFlags ,
		NULL,
		fmt,
		NULL,
		(LPTSTR)&lpszTemp,
		0,
		&argList);
	buf << lpszTemp;
	va_end(argList);
    if ( lpszTemp )
        LocalFree((HLOCAL) lpszTemp );
}

void __cdecl Log::printf(char * fmt, ...)
{
	char* szText = new char[MINBUFSIZE];
	int	szTextlen = MINBUFSIZE;
	va_list argList;
	va_start(argList, fmt);
	while(_vsnprintf(szText, szTextlen, fmt, argList) < 0)
	{
		delete szText;
		szText = new char[szTextlen += MINBUFSIZE];
	}
	va_end(argList);
	buf << szText;
}

void Log::printf(DWORD fmt, va_list argList)
{
    DWORD dwRet;
    DWORD dwFlags;
    LPTSTR lpszTemp = NULL;
	if((HIWORD(fmt) & 0xFFF) > 0xFF)
		dwFlags = FORMAT_MESSAGE_FROM_HMODULE;
	else
		dwFlags = FORMAT_MESSAGE_FROM_SYSTEM;
	
    dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | dwFlags ,
		NULL,
		fmt,
		NULL,
		(LPTSTR)&lpszTemp,
		0,
		&argList);
	ASSERT(lpszTemp);
    if ( lpszTemp )
	{
		buf << lpszTemp;
        LocalFree((HLOCAL) lpszTemp );
	}
}

void Log::printf(char * fmt, va_list argList)
{
	char* szText = new char[MINBUFSIZE];
	int	szTextlen = MINBUFSIZE;
	while(_vsnprintf(szText, szTextlen, fmt, argList) < 0)
	{
		delete szText;
		szText = new char[szTextlen += MINBUFSIZE];
	}
	buf << szText;
	delete szText;
}

#ifdef _DEBUG
XDump& XDump::operator<<(const char* str)
{
	if(str)
		OutputDebugString(str); 
	return *this;
};
#endif
