// SqlStmt.h: interface for the SqlStmt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLSTMT_H__C613DE36_39FA_11D2_A711_204C4F4F5020__INCLUDED_)
#define AFX_SQLSTMT_H__C613DE36_39FA_11D2_A711_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RField.h"
class SqlStmt  
{
	char* stmt;
	char* end;
	int buflen;
	enum Part{Sel, From, Where}part;
public:
	enum Type{Select, Update, Insert} type;
	SqlStmt(){buflen = 256; end = stmt = new char[buflen];};
	virtual ~SqlStmt(){delete stmt;};
	SqlStmt& operator<<(const char* s1);
	operator const char*(){return stmt;}
	SqlStmt& operator<<(Type t){type = t;*this << "SELECT "; return *this;};
	SqlStmt& operator<<(RFields rf)
	{
		if(type == Select && part == Sel)
		for(int i = 0; i<rf.GetCount(); i++)
		{
			if(i>0)
				*this<<", ";
			(*rf[i])->Select(*this);
			(*rf[i])->Mark();
		}
		return *this;
	};
};

#endif // !defined(AFX_SQLSTMT_H__C613DE36_39FA_11D2_A711_204C4F4F5020__INCLUDED_)
