// OdbcCursor.h: interface for the OdbcCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCCURSOR_H__C613DE35_39FA_11D2_A711_204C4F4F5020__INCLUDED_)
#define AFX_ODBCCURSOR_H__C613DE35_39FA_11D2_A711_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DBCursor.h"
#include "OdbcDataBase.h"

class OdbcDataBase;
class OdbcStmt;

class OdbcCursor : public DbCursor
{
	UWORD*		pstatus;
	OdbcStmt*	pStmt;
	OdbcDataBase*	pDB;
	int			maxrows;
	RETCODE		retcode;
	char		stmt[1024];
//	OdbcStmtMap	ssmap;
	int			next_parameter;
public:
	OdbcCursor(OdbcDataBase* pdb, const char* stmt, int frows);
	void SetStmt(const char* stmt);
	virtual ~OdbcCursor();
//	void*	GetBookmark();

//	bool Bind(int col, int len, char* buf, SDWORD* pindicator, short int type = SQL_C_CHAR);
	bool Bind(int col, RSField* prf);
	void BindParameter(RField* prf);
	void BindParameter(RSField* prf);
	void BindParameter(RKey* prk);

	void Execute();
	bool BindEx(int col, int* len, char* buf, SDWORD* pindicator);
	int Fetch(FetchType fetchtype, int rownum, int nfetchrows);

};

#endif // !defined(AFX_ODBCCURSOR_H__C613DE35_39FA_11D2_A711_204C4F4F5020__INCLUDED_)
