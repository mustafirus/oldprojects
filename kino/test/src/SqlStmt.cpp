// SqlStmt.cpp: implementation of the SqlStmt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Dict.h"
#include "SqlStmt.h"

#define GROWBY 256
#ifdef _DEBUG
#define new DEBUG_NEW
#endif //_DEBUG
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SqlStmt& SqlStmt::operator<<(const char* s1)
{
	int len = strlen(s1);
	if(end + len + 1 > stmt + buflen)
	{
		int l = end - stmt;
		char * s1 = new char[buflen += GROWBY];
		strcpy(s1, stmt);
		delete stmt;
		stmt = s1;
		end = stmt + l;
	}
	strcpy(end, s1); end += len;
	return *this;
};
