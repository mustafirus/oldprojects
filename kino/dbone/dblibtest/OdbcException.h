// OdbcException.h: interface for the OdbcException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCEXCEPTION_H__E4774EDE_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
#define AFX_ODBCEXCEPTION_H__E4774EDE_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DbException.h"

class OdbcException : public DbException  
{
public:
	void Report(RETCODE  code, const char* pszCall,	const char* pszFile, int nLine, HENV henv, HDBC hdbc, HSTMT hstmt);
	OdbcException()
#ifdef DEBUGSQL
	 : stmt(NULL)
#endif //DEBUGSQL
	 {};
	virtual ~OdbcException(){};

	static OdbcException ex;
#ifdef DEBUGSQL
	const char* stmt;
	void Stmt(const char* str){stmt = str;}
#endif //DEBUGSQL
};

#endif // !defined(AFX_ODBCEXCEPTION_H__E4774EDE_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
