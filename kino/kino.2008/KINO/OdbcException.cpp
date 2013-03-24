// OdbcException.cpp: implementation of the OdbcException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Log.h"
#include "OdbcException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
OdbcException OdbcException::ex;//5701 Change context to kino; 5703 Change lang to russian

void OdbcException::Report(RETCODE  code, const char* pszCall,	const char* pszFile, int nLine, HENV henv, HDBC hdbc, HSTMT hstmt)
{
	bool prn;
	bool rpt = false;
	SDWORD fNativeError;
	UCHAR szSqlState[10];
	UCHAR szErrorMsg[SQL_MAX_MESSAGE_LENGTH];
	SWORD cbErrorMsg;
	while(SQLError(henv, hdbc, hstmt, szSqlState, &fNativeError,
		szErrorMsg, SQL_MAX_MESSAGE_LENGTH - 1, &cbErrorMsg) == SQL_SUCCESS)
	{
		switch(fNativeError)
		{
		case 5701:
		case 5703:
			prn = false;
			break;
		default:
			rpt = prn = true;
		}
		if(prn)
			OdbcException::ex.printf("Error - %s : %s\n",
			(code == SQL_SUCCESS_WITH_INFO) ? 'w': 'e', szSqlState, szErrorMsg);
	}
	#ifdef DEBUGSQL
	if(rpt && stmt && *stmt)
	{
		OdbcException::ex.printf("\tSQL Command buffer: '%s'\n", 'w', stmt);
	}
	#endif //DEBUGSQL
#ifdef _DEBUG
	if(rpt)
 	switch(code)
	{
	case SQL_SUCCESS_WITH_INFO:
		OdbcException::ex.printf("\t{ SQL_SUCCESS_WITH_INFO\n\tFile: %s \n\tLine: %d, \n\tFunction: %s\n\t",
			'w',pszFile, nLine, pszCall);
		break;
	case SQL_NEED_DATA:
		OdbcException::ex.printf("\t{ SQL_NEED_DATA\n\tFile: %s \n\tLine: %d, \n\tFunction: %s\n\t",
			'e',pszFile, nLine, pszCall);
		break;
	case SQL_STILL_EXECUTING:
		OdbcException::ex.printf("\t{ SQL_STILL_EXECUTING\n\tFile: %s \n\tLine: %d, \n\tFunction: %s\n\t",
			'e', pszFile, nLine, pszCall);
		break;
	case SQL_ERROR:
		OdbcException::ex.printf("\t{ SQL_ERROR\n\tFile: %s \n\tLine: %d, \n\tFunction: %s\n\t",
			'e', pszFile, nLine, pszCall);
		break;
	case SQL_INVALID_HANDLE:
		OdbcException::ex.printf("\t{ SQL_INVALID_HANDLE\n\tFile: %s \n\tLine: %d, \n\tFunction: %s\n\t",
			'e', pszFile, nLine, pszCall);
		break;
	case SQL_NO_DATA_FOUND:
		OdbcException::ex.printf("\t{ SQL_NO_DATA_FOUND\n\tFile: %s \n\tLine: %d, \n\tFunction: %s\n\t",
			'w', pszFile, nLine, pszCall);
		break;
	}
#endif //_DEBUG
}
