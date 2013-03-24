// OdbcDataBase.cpp: implementation of the OdbcDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dict.h"
#include "OdbcException.h"
#include "RSWhere.h"
#include "RSField.h"
#include "RSField.h"
#include "RKey.h"
#include "OdbcCursor.h"
#include "OdbcDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ex	OdbcException::ex
#ifdef DEBUGSQL
#define SaveCmd(X) if(lstrlen(X)>=1024){CopyMemory(stmt, X, 1024); stmt[1023] = 0;}else lstrcpy(stmt, X)
#define FlushCmd *stmt = 0;
#else
#define SaveCmd(X)
#define FlushCmd
#endif

inline bool X_ChechSqlReturn(RETCODE  code, const char* pszCall,
	const char* pszFile, int nLine, HENV henv, HDBC hdbc, HSTMT hstmt, const char* str)
{
#ifdef DEBUGSQL
	ex.stmt = str;
#endif //DEBUGSQL
 	switch(code)
	{
	case SQL_SUCCESS:
		return true;
	case SQL_SUCCESS_WITH_INFO:
		ex.Report(code, pszCall, pszFile, nLine, henv, hdbc, hstmt);
#ifdef DEBUGSQL
		ex.stmt = NULL;
#endif //DEBUGSQL
		return true;
	case SQL_NO_DATA_FOUND:
		ex.Report(code, pszCall, pszFile, nLine, henv, hdbc, hstmt);
#ifdef DEBUGSQL
		ex.stmt = NULL;
#endif //DEBUGSQL
		return false;
	default:

		ex.Report(code, pszCall, pszFile, nLine, henv, hdbc, hstmt);
#ifdef DEBUGSQL
		ex.stmt = NULL;
#endif //DEBUGSQL
		throw &ex;
		return false;
	}
}

#ifdef DEBUGSQL
#define Check(f) X_ChechSqlReturn(retcode = f, #f, __FILE__, __LINE__, henv, hdbc, hstmt, stmt)
#else
#define Check(f) X_ChechSqlReturn(retcode = f, #f, __FILE__, __LINE__, henv, hdbc, hstmt)
#endif //DEBUGSQL

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OdbcDataBase::OdbcDataBase(const char* dsn, const char* user, const char* pass) :
henv(SQL_NULL_HENV), hdbc(SQL_NULL_HDBC), hstmt(SQL_NULL_HSTMT), pStmt(NULL), basehstmt(NULL),
connected(false), retcode(0), needflash(false),	next_parameter(1)
{
	FlushCmd;
	Init();
	try{
	Connect(dsn, user, pass);
	}catch(Exception* e)
	{
		e->SetDelMem(this, Dict::free);
		Disconnect();
		Init(false);
		throw e;
	}
}

OdbcDataBase::~OdbcDataBase()
{
	Disconnect();
	Init(false);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void OdbcDataBase::Init(bool init /*= true*/)
{
	if(init)
	{//SQL_NULL_HDBC, SQL_NULL_HSTMT SQL_NULL_HENV
		Check(SQLAllocEnv(&henv)); /* Environment handle */ 
		Check(SQLAllocConnect(henv, &hdbc)); /* Connection handle */ 
		Check(SQLSetConnectOption(hdbc, SQL_LOGIN_TIMEOUT, 5));	/* Set login timeout to 5 seconds. */ 
//		Check(SQLSetConnectOption(hdbc, SQL_ODBC_CURSORS, SQL_CUR_USE_ODBC));	/* Set login timeout to 5 seconds. */ 
	}
	else
	{
		Check(SQLFreeConnect(hdbc)); hdbc = SQL_NULL_HDBC;
		Check(SQLFreeEnv(henv)); henv = SQL_NULL_HENV;
	}
}

void OdbcDataBase::Connect(const char* dsn, const char* user, const char* pass)
{
	Check(
		SQLConnect(hdbc, (UCHAR*) dsn, dsn ? strlen(dsn) : 0, (UCHAR*) user,
		user ? strlen(user) : 0, (UCHAR*) pass, pass ? strlen(pass):0)
		);
	Check(SQLAllocStmt(hdbc, &hstmt));
	basehstmt = hstmt;
	connected = true;
}
void OdbcDataBase::Disconnect()
{
	if(hstmt != SQL_NULL_HSTMT)
		Check(SQLFreeStmt(hstmt, SQL_DROP)); hstmt = SQL_NULL_HSTMT;
	if(connected)
		Check(SQLDisconnect(hdbc));
	connected = false;
}

void OdbcDataBase::ExecDirect(const char* str)
{
	ASSERT(needflash == false);
	needflash = true;
	SaveCmd(str);
	Check(SQLExecDirect(hstmt, (UCHAR*) str, strlen(str)));
}

DbStmt* OdbcDataBase::Prepare(const char* str)
{
	pStmt = ssmap[str];
	if(!pStmt)
	{
		HSTMT hs;
		Check(SQLAllocStmt(hdbc, &hs));
		Check(SQLPrepare(hs, (UCHAR*) str, strlen(str)));
//		pst = new OdbcStmt(str, hs);
		pStmt = ssmap.SetAt(str, hs);
	}
	pStmt->Lock();
	hstmt = *pStmt;
	return pStmt;
}
void OdbcDataBase::Set(DbStmt* pst)
{
	pStmt = (OdbcStmt*)pst;
	pStmt->Lock();
	hstmt = *pStmt;
}

void OdbcDataBase::Flush()
{
	needflash = false;
	next_parameter = 1;
	Check(SQLFreeStmt(hstmt, SQL_UNBIND));
	Check(SQLFreeStmt(hstmt, SQL_RESET_PARAMS));
	Check(SQLFreeStmt(hstmt, SQL_CLOSE));
	hstmt = basehstmt;
	if(pStmt)
		pStmt->Unlock();
	pStmt = NULL;

	FlushCmd;
}

void OdbcDataBase::FlushEx()
{
	needflash = false;
	Check(SQLFreeStmt(hstmt, SQL_UNBIND));
	Check(SQLFreeStmt(hstmt, SQL_CLOSE));
	if(pStmt)
		pStmt->Unlock();
	pStmt = NULL;
	hstmt = basehstmt;
	next_parameter = 1;
}

void OdbcDataBase::Exec()
{
	Check(SQLExecute(hstmt));
}

void __cdecl OdbcDataBase::ExecFDirect(const char* str, ...)
{
	char buf[256];
	va_list va;
	va_start(va,str);
	_vsnprintf( buf, 256, str, va);
	va_end(va);
	ExecDirect(buf);
}

void OdbcDataBase::Bind(int col, RField* prf)
{
	Field* pf = *prf;
	SDWORD* pi = (SDWORD*)prf->GetIndicator();
	if(pf->type == Field::DateTime)
	{
		*pi = 0;
		Check(SQLBindCol(hstmt, col, SQL_C_DATE,
			prf->GetBuf(), 0, pi));
		prf->type = Field::DateTime;
	}else
	{
		*pi = SQL_NTS;
		Check(SQLBindCol(hstmt, col, SQL_C_CHAR,
			prf->GetBuf(), prf->GetLen(), pi));
	}
}

void OdbcDataBase::Bind(int col, char* buf, int len, int* pi, Field::Type type /*= Field::Char*/)
{
		Check(SQLBindCol(hstmt, col, CTypeCrack(type),
			buf, len, (long*)pi));
}

void OdbcDataBase::BindParameter(/*int num, */int len, char* buf, SDWORD* pindicator)
{
	Check(SQLBindParameter(hstmt, next_parameter++, SQL_PARAM_INPUT,
			SQL_C_CHAR, SQL_CHAR, len,
			0, buf, 0, pindicator));
};

void OdbcDataBase::BindParameter(/*int num, */int* par)
{
	Check(SQLBindParameter(hstmt, next_parameter++, SQL_PARAM_INPUT,
			SQL_C_SLONG, SQL_INTEGER, 0,
			0, par, 0, NULL));
};

void OdbcDataBase::BindParameter(RKey* prk)
{
	RFields& rf = *prk;
	for(int i=0; i < rf.GetCount(); i++)
	{
		Field* pf = *(rf[i]);
		SDWORD* pi = (SDWORD*)rf[i]->GetIndicator();
		*pi = SQL_NTS;
		if(rf[i]->state == RField::s_null)
			*pi = SQL_NULL_DATA;
		Check(SQLBindParameter(hstmt, next_parameter++, SQL_PARAM_INPUT,
			OdbcDataBase::CTypeCrack(rf[i]->type),
			OdbcDataBase::TypeCrack(pf->type == Field::DateTime ? Field::Char : pf->type),
			rf[i]->GetLen(),
			0, (void*)((const char*)*rf[i]), 0, pi));
	}
}

void OdbcDataBase::BindParameter(RSField* prf)
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
	Check(SQLBindParameter(hstmt, next_parameter++, SQL_PARAM_INPUT,
		OdbcDataBase::CTypeCrack(prf->type),
		OdbcDataBase::TypeCrack(pf->type == Field::DateTime ? Field::Char : pf->type), prf->GetLen(),
		0, pw->Op1(), 0, pi));
	if(pw->Op2())
	Check(SQLBindParameter(hstmt, next_parameter++, SQL_PARAM_INPUT,
		OdbcDataBase::CTypeCrack(prf->type),
		OdbcDataBase::TypeCrack(pf->type == Field::DateTime ? Field::Char : pf->type), prf->GetLen(),
		0, pw->Op2(), 0, pi));
}

void OdbcDataBase::BindParameter(RField* prf)
{
	Field* pf = *prf;
	SDWORD* pi = (SDWORD*)prf->GetIndicator();
	*pi = SQL_NTS;
	if(prf->state == RField::s_null)
		*pi = SQL_NULL_DATA;
	Check(SQLBindParameter(hstmt, next_parameter++, SQL_PARAM_INPUT,
		SQL_C_CHAR, OdbcDataBase::TypeCrack(pf->type == Field::DateTime ? Field::Char : pf->type),
		prf->GetLen(), 0, (void*)(const char*)*prf, 0, pi));
}


void OdbcDataBase::GetData(int icol, int& num)
{
	SDWORD cbValue = 0;
 	Check(SQLGetData(hstmt, icol, SQL_C_SLONG, &num, 0, &cbValue));
}

void OdbcDataBase::GetData(int icol, char& data)
{
	SDWORD cbValue = 0;
 	Check(SQLGetData(hstmt, icol, SQL_C_TINYINT, &data, 0, &cbValue));
}

void OdbcDataBase::GetData(int icol, bool& b)
{
	int num = 0;
 	Check(SQLGetData(hstmt, icol, SQL_C_SLONG, &num, 0, NULL/*&cbValue*/));
	b = num ? true : false;
}

void OdbcDataBase::GetData(int icol, char*& data)
{
	int buflen;
	Check(SQLColAttributes(hstmt, icol, SQL_COLUMN_LENGTH,
		NULL, NULL, NULL, (long*)&buflen));
	ASSERT(buflen>0);
	if(buflen > 255)
		buflen = 255;
	char* buf = new char[++buflen];
	SDWORD cbValue;
  	Check(SQLGetData(hstmt, icol, SQL_C_CHAR, buf, buflen, &cbValue));
	if(data)
		delete data;
	if(cbValue == SQL_NULL_DATA)
		data = NULL;
	else if(cbValue < buflen )
	{
		data = new char[cbValue+1];
		strcpy(data, buf);
	}else if(cbValue == SQL_NO_TOTAL)
	{
		data = buf;
		return;
	}else
	{
		data = new char[cbValue+1];
		strcpy(data, buf);
		GetData(icol, data + buflen - 1, cbValue + 1 - (buflen - 1));
	}
	delete buf;
}

void OdbcDataBase::GetData(int icol, char* data, int num)
{
	SDWORD cbValue;
  	Check(SQLGetData(hstmt, icol, SQL_C_CHAR, data, num, &cbValue));
	if(cbValue == SQL_NULL_DATA)
		*data = 0;
}

bool OdbcDataBase::Read()
{
	if(Check(SQLFetch(hstmt)) )
		return true;
	if(retcode != SQL_NO_DATA_FOUND)
		ASSERT(FALSE);
	Flush();
	return false;
}

int  OdbcDataBase::GetIdentity()
{
	int* pi=0;
	int iret=0;
	ExecFDirect("select convert(int,@@identity)");
	Read();
	GetData(1, iret);
	Flush();
	return iret;
}

DbCursor* OdbcDataBase::GetCursor(const char* stmt, int maxfr)
{
	return new OdbcCursor(this, stmt, maxfr);
}	



bool OdbcDataBase::ExecProc(const char * name, int id, int& newid)
{
	SDWORD cbValue;
 	int pret=0;
	Check(SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER,
		0, 0, &pret, 0, NULL));
	Check(SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT_OUTPUT, SQL_C_SLONG, SQL_INTEGER,
		0, 0, &id, 0, &cbValue));
	ExecFDirect("{? = call %s(?)}",name);
	if(!pret)
		newid = id;
	Check(SQLFreeStmt(hstmt, SQL_UNBIND));
	Check(SQLFreeStmt(hstmt, SQL_RESET_PARAMS));
	Check(SQLFreeStmt(hstmt, SQL_CLOSE));
	needflash = false;
	return pret == 0;
}

bool OdbcDataBase::ExecProc(const char * name, int id, char* str, int slen)
{
	SDWORD cbValue;
 	int pret=0;
	Check(SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER,
		0, 0, &pret, 0, NULL));
	Check(SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,
		0, 0, &id, 0, NULL));
	Check(SQLBindParameter(hstmt, 3, SQL_PARAM_OUTPUT, SQL_C_CHAR, SQL_CHAR,
		slen, 0, str, slen, &cbValue));
	ExecFDirect("{? = call %s(?,?)}",name);
	Check(SQLFreeStmt(hstmt, SQL_UNBIND));
	Check(SQLFreeStmt(hstmt, SQL_RESET_PARAMS));
	Check(SQLFreeStmt(hstmt, SQL_CLOSE));
	needflash = false;
	return pret == 0;
}

Field::Type OdbcDataBase::TypeCrack(unsigned char dbtype)
{
	switch(dbtype)
	{ 
	case SQL_VARCHAR:
	case SQL_CHAR:
		return Field::Char;
	case SQL_INTEGER:
		return Field::Integer;
	case SQL_SMALLINT:
		return Field::SmallInt;
	case SQL_DATETIME:
	case SQL_TYPE_DATE:
	case SQL_TYPE_TIME:
		return Field::DateTime;
	case SQL_FLOAT:
	case SQL_REAL:
	case SQL_DOUBLE:
	case SQL_NUMERIC:
	case SQL_DECIMAL:
	case SQL_TYPE_TIMESTAMP:
	case SQL_UNKNOWN_TYPE:
	default:
		return Field::Unknown;
	}
}

unsigned char OdbcDataBase::TypeCrack(Field::Type dicttype)
{
	switch(dicttype)
	{ 
	case Field::Char:
		return SQL_VARCHAR;
	case Field::Integer:
		return SQL_INTEGER;
	case Field::TinyInt:
	case Field::SmallInt:
		return SQL_SMALLINT;
//	case Field::TinyInt:
//		return SQL_TINYINT;
	case Field::DateTime:
		return SQL_DATETIME;
	default:
		return SQL_UNKNOWN_TYPE;
	}
}

unsigned char OdbcDataBase::CTypeCrack(Field::Type dicttype)
{
	switch(dicttype)
	{ 
	case Field::Char:
		return SQL_C_CHAR;
	case Field::Integer:
		return SQL_C_SLONG;
	case Field::SmallInt:
		return SQL_C_SSHORT;
	case Field::DateTime:
		return SQL_C_DATE;
	default:
		return SQL_UNKNOWN_TYPE;
	}
}

int OdbcDataBase::Export(const char * tablename, const char * filename)
{
	SDWORD cbValue = SQL_NTS;
 	int pret=0;
	Check(SQLBindParameter(hstmt, 1, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER,
		0, 0, &pret, 0, NULL));
	Check(SQLBindParameter(hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
		0, 0, (char*)tablename, 0, &cbValue));
	Check(SQLBindParameter(hstmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
		0, 0, (char*)filename, 0, &cbValue));
	ExecDirect("{? = call master..xp_export(?, ?)}");
	long rows = 0;
	Check(SQLRowCount(hstmt, &rows));
	Check(SQLFreeStmt(hstmt, SQL_UNBIND));
	Check(SQLFreeStmt(hstmt, SQL_RESET_PARAMS));
	Check(SQLFreeStmt(hstmt, SQL_CLOSE));
	needflash = false;
	if(pret != 0)
		return -1;
	return rows;
}

Field::Type OdbcDataBase::Convert(const char* frombuf, char* tobuf, Field::Type from, Field::Type to)
{
	if(from == Field::DateTime && to == Field::Char)
	{
		DATE_STRUCT d = *((DATE_STRUCT*)frombuf);
		sprintf(tobuf, "%d/%d/%d", d.day, d.month, d.year);
		return Field::Char;
	}
	return from;
}

void OdbcDataBase::CheckData(RField* prf)
{
	SDWORD* pi = (SDWORD*)prf->GetIndicator();
	ASSERT(pi);
/*	if(*pi == SQL_NULL_DATA)
		prf->state = RField::s_null;
	else
		prf->state /= RField::s_null;
*/	prf->SetData(*pi == SQL_NULL_DATA);
}

void OdbcDataBase::GetInfo(TABLEINFO* pti)
{
	memset(pti, 0, sizeof(pti));
	Check(SQLNumResultCols(hstmt, (SQLSMALLINT*)&pti->cols));
	ASSERT(pti->cols > 0 && pti->cols < 256);
	PCOLINFO& pci = pti->pColInfo;
	pci = new COLINFO[pti->cols];
	memset(pci,0,sizeof(COLINFO) * pti->cols);
	SQLSMALLINT nl;
	int off = 0;
	for(int i = 0; i < pti->cols; i++)
	{
		Check(SQLDescribeCol(hstmt, i+1, (UCHAR*)pci[i].name, sizeof(pci[i].name), &nl, (short*)&pci[i].type,
			(ULONG*)&pci[i].len, NULL, NULL));
		pci[i].name[nl] = 0;
		pci[i].len++;
		pci[i].offset = off;
		off += pci[i].len;

	}
	pti->size = off;
}
