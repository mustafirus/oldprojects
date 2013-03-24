// OdbcCursor.cpp: implementation of the OdbcCursor class.
//
//////////////////////////////////////////////////////////////////////
#if 0
#include "stdx.h"
#include "Dict.h"
#include "OdbcDataBase.h"
#include "RSField.h"
#include "RSWhere.h"
#include "RKey.h"
#include "OdbcCursor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif //_DEBUG

#ifdef DEBUGSQL
#define SaveCmd(X) if(lstrlen(X)>=1024){CopyMemory(OdbcCursor::stmt, X, 1024); OdbcCursor::stmt[1023] = 0;}else lstrcpy(OdbcCursor::stmt, X)
#define FlushCmd *stmt = 0;
#else
#define SaveCmd(X)
#define FlushCmd
#endif

#ifdef DEBUGSQL
#define Check(f) X_ChechSqlReturn(f, #f, __FILE__, __LINE__, pDB, *pStmt, stmt)
#else
#define Check(f) X_ChechSqlReturn(f, #f, __FILE__, __LINE__, pDB, *pStmt)
#endif //DEBUGSQL
#define CheckInit(f) X_ChechSqlReturn(f, #f, __FILE__, __LINE__, pDB, hstmt, stmt)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OdbcCursor::OdbcCursor(OdbcDataBase* pdb, const char* stmt, int frows) :
maxrows(frows), pDB(pdb), pStmt(NULL), next_parameter(1)
{
	pstatus = new UWORD[maxrows]; 
	SetStmt(stmt);
}

void OdbcCursor::SetStmt(const char* stmt)
{
	SaveCmd(stmt);
	if(pStmt)
	{
		Check(SQLFreeStmt(*pStmt, SQL_UNBIND));
//		Check(SQLFreeStmt(*pStmt, SQL_RESET_PARAMS));
		Check(SQLFreeStmt(*pStmt, SQL_CLOSE));
		pStmt->Unlock();
	}
	if(!stmt)
		return;
	pStmt = pDB->ssmap[stmt];
	if(!pStmt)
	{
		HSTMT hstmt;
		CheckInit(SQLAllocStmt(pDB->hdbc, &hstmt));
		CheckInit(SQLSetStmtOption(hstmt, SQL_CONCURRENCY, SQL_CONCUR_READ_ONLY)); 
		CheckInit(SQLSetStmtOption(hstmt, SQL_CURSOR_TYPE, SQL_CURSOR_DYNAMIC)); 
//		CheckInit(SQLSetStmtOption(hstmt, SQL_USE_BOOKMARKS, SQL_UB_ON)); 
		CheckInit(SQLPrepare(hstmt, (UCHAR*) stmt, strlen(stmt)));
		pStmt = pDB->ssmap.SetAt(stmt, hstmt);
	}
	pStmt->Lock();
	next_parameter = 1;
	Check(SQLSetStmtOption(*pStmt, SQL_ROWSET_SIZE, maxrows));
}

void OdbcCursor::Execute()
{
	Check(SQLExecute(*pStmt));
}

OdbcCursor::~OdbcCursor()
{
	Check(SQLFreeStmt(*pStmt, SQL_UNBIND));
//	Check(SQLFreeStmt(*pStmt, SQL_RESET_PARAMS));
	Check(SQLFreeStmt(*pStmt, SQL_CLOSE));
//	next_parameter = 0;
	pStmt->Unlock();
	delete[] pstatus;
}
//bool OdbcCursor::Bind(int col, int len, char* buf, SDWORD* pindicator, short int type /*= SQL_C_CHAR*/)
/*{
	Check(SQLBindCol(*pStmt, col, type, buf, len, pindicator));
	return true;
};
*/
bool OdbcCursor::BindEx(int col, int* len, char* buf, SDWORD* pindicator)
{
	SDWORD l;
	Check(SQLColAttributes(*pStmt, col, SQL_COLUMN_DISPLAY_SIZE,
		NULL, 0, NULL, &l));
	*len = ++l;
	buf = new char[l * maxrows];
	Check(SQLBindCol(*pStmt, col, SQL_C_CHAR, buf, l, pindicator));
	return true;
};

bool OdbcCursor::Bind(int col, RSField* prf)
{
	Field* pf = *prf;
	SDWORD* pi = (SDWORD*)prf->GetIndicator();
	if(pf->type == Field::DateTime)
	{
		*pi = sizeof(DATE_STRUCT);
		Check(SQLBindCol(*pStmt, col, SQL_C_DATE,
			prf->GetDBPtr(), 0, pi));
	}else
	{
		*pi = SQL_NTS;
		Check(SQLBindCol(*pStmt, col, SQL_C_CHAR,
			prf->GetDBPtr(), prf->GetLen(), pi));
	}
	return true;
};

void OdbcCursor::BindParameter(RKey* prk)
{
	RFields& rf = *prk;
	for(int i=0; i < rf.GetCount(); i++)
	{
		Field* pf = *(rf[i]);
		SDWORD* pi = (SDWORD*)rf[i]->GetIndicator();
		*pi = SQL_NTS;
		if(rf[i]->state == RField::s_null)
			*pi = SQL_NULL_DATA;
		Check(SQLBindParameter(*pStmt, next_parameter++, SQL_PARAM_INPUT,
			OdbcDataBase::CTypeCrack(rf[i]->type),
			OdbcDataBase::TypeCrack(pf->type == Field::DateTime ? Field::Char : pf->type),
			rf[i]->GetLen(),
			0, (void*)((const char*)*rf[i]), 0, pi));
	}
}

void OdbcCursor::BindParameter(RSField* prf)
{
	if(!prf->IsWhere())
		return;
	RSWhere* pw = *prf;
	Field* pf = *prf;
	SDWORD* pi = (SDWORD*)prf->GetIndicator();
	*pi = SQL_NTS;
	if(prf->state == RField::s_null)
		*pi = SQL_NULL_DATA;
	ASSERT(pw->Op1());
	Check(SQLBindParameter(*pStmt, next_parameter++, SQL_PARAM_INPUT,
		OdbcDataBase::CTypeCrack(prf->type),
		OdbcDataBase::TypeCrack(pf->type == Field::DateTime ? Field::Char : pf->type), prf->GetLen(),
		0, pw->Op1(), 0, pi));
	if(pw->Op2())
	Check(SQLBindParameter(*pStmt, next_parameter++, SQL_PARAM_INPUT,
		OdbcDataBase::CTypeCrack(prf->type),
		OdbcDataBase::TypeCrack(pf->type == Field::DateTime ? Field::Char : pf->type), prf->GetLen(),
		0, pw->Op2(), 0, pi));
}

void OdbcCursor::BindParameter(RField* prf)
{
	Field* pf = *prf;
	SDWORD* pi = (SDWORD*)prf->GetIndicator();
	*pi = SQL_NTS;
	if(prf->state == RField::s_null)
		*pi = SQL_NULL_DATA;
	Check(SQLBindParameter(*pStmt, next_parameter++, SQL_PARAM_INPUT,
		SQL_C_CHAR, OdbcDataBase::TypeCrack(pf->type == Field::DateTime ? Field::Char : pf->type),
		prf->GetLen(), 0, (void*)(const char*)*prf, 0, pi));
}

int OdbcCursor::Fetch(FetchType ft, int irow, int nfetchrows)
{
	int fetchtype = 0;
	if(!nfetchrows)
	{
		ASSERT(FALSE);
		return 0;
	}
	switch(ft)
	{
	case f_first:
		fetchtype = SQL_FETCH_FIRST; break;
	case f_next:
		fetchtype = SQL_FETCH_NEXT; break;
	case f_prev:
		fetchtype = SQL_FETCH_PREV; break;
	case f_random:
		fetchtype = SQL_FETCH_ABSOLUTE; break;
	case f_relative:
		fetchtype = SQL_FETCH_RELATIVE; break;
	case f_last:
		fetchtype = SQL_FETCH_LAST; break;
	}
	if(nfetchrows > maxrows)
		nfetchrows = maxrows;
	Check(SQLSetStmtOption(*pStmt, SQL_ROWSET_SIZE, nfetchrows));
	UDWORD crow=0;
	Check(SQLExtendedFetch(*pStmt, fetchtype, irow, &crow, pstatus));
	return crow;
};
#endif
