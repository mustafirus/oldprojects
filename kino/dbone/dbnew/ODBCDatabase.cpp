#include "StdAfx.h"
#include "odbcdatabase.h"
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
//		ex.Report(code, pszCall, pszFile, nLine, henv, hdbc, hstmt);
#ifdef DEBUGSQL
		ex.stmt = NULL;
#endif //DEBUGSQL
		return true;
	case SQL_NO_DATA_FOUND:
//		ex.Report(code, pszCall, pszFile, nLine, henv, hdbc, hstmt);
#ifdef DEBUGSQL
		ex.stmt = NULL;
#endif //DEBUGSQL
		return false;
	default:

//		ex.Report(code, pszCall, pszFile, nLine, henv, hdbc, hstmt);
#ifdef DEBUGSQL
		ex.stmt = NULL;
#endif //DEBUGSQL
//		throw &ex;
		return false;
	}
}

#ifdef DEBUGSQL
#define Check(f) X_ChechSqlReturn( f, #f, __FILE__, __LINE__, henv, hdbc, hstmt, stmt)
#else
#define Check(f) X_ChechSqlReturn( f, #f, __FILE__, __LINE__, henv, hdbc, hstmt)
#define CheckC(f) checkSqlReturn( f, #f, __FILE__, __LINE__, this)
#endif //DEBUGSQL

class Exception{};

vector<OdbcDatabase::BKType>	OdbcDatabase::ctype;
vector<OdbcDatabase::BKType>	OdbcDatabase::stype;
vector<OdbcDatabase::BKTypeS>	OdbcDatabase::typestring;


OdbcDatabase::OdbcDatabase(const char* dsn, const char* user, const char* pass) :
henv(SQL_NULL_HENV), hdbc(SQL_NULL_HDBC), hstmt(SQL_NULL_HSTMT),
connected(false), retcode(0), needflash(false),	next_parameter(1)
{
	FlushCmd;
	//SQL_NULL_HDBC, SQL_NULL_HSTMT SQL_NULL_HENV
	Check(SQLAllocEnv(&henv)); /* Environment handle */ 
	Check(SQLAllocConnect(henv, &hdbc)); /* Connection handle */ 
	Check(SQLSetConnectOption(hdbc, SQL_LOGIN_TIMEOUT, 5));	/* Set login timeout to 5 seconds. */ 
//	Check(SQLSetConnectOption(hdbc, SQL_ODBC_CURSORS, SQL_CUR_USE_ODBC));	/* Set login timeout to 5 seconds. */ 

	try{
	Connect(dsn, user, pass);
	}catch(Exception* e)
	{
//		e->SetDelMem(this, Dict::free);
		Disconnect();
		throw e;
	}
}

OdbcDatabase::~OdbcDatabase()
{
	Disconnect();
	Check(SQLFreeConnect(hdbc)); hdbc = SQL_NULL_HDBC;
	Check(SQLFreeEnv(henv)); henv = SQL_NULL_HENV;
}

#pragma warning( push )
#pragma warning( disable : 4267 )
void OdbcDatabase::Connect(const char* dsn, const char* user, const char* pass)
{
	Check(
		SQLConnect(hdbc, (UCHAR*) dsn, (SQLSMALLINT)dsn ? strlen(dsn) : 0, (UCHAR*) user,
		user ? strlen(user) : 0, (UCHAR*) pass, pass ? strlen(pass):0)
		);
	connected = true;
}
#pragma warning( pop )
void OdbcDatabase::Disconnect()
{
	if(hstmt != SQL_NULL_HSTMT)
		Check(SQLFreeStmt(hstmt, SQL_DROP)); hstmt = SQL_NULL_HSTMT;
	if(connected)
		Check(SQLDisconnect(hdbc));
	connected = false;
}

//***********************************************************
Cursor* OdbcDatabase::getCursor(Query* pq){
	return new OdbcCursor(this, new SQLStatement(pq));
}





void OdbcDatabase::setTypeString(Domain* pd, string s){
	BKTypeS bt(pd, s);
	ASSERT(!typestring.exists(bt));
	typestring.add(bt);
}

string OdbcDatabase::getTypeString(Domain* pd){
	BKTypeS t = typestring[pd];
	ASSERT(t.pd == pd);
	return t.id;
}

SQLSMALLINT OdbcDatabase::getCType(Domain* pd){
	BKType t = ctype[pd];
	ASSERT(t.pd == pd);
	return t.id;
}

void OdbcDatabase::setCType(Domain* pd, SQLSMALLINT t){
	BKType bt = {pd, t};
	ASSERT(!ctype.exists(bt));
	ctype.add(bt);
}

SQLSMALLINT OdbcDatabase::getSType(Domain* pd){
	BKType t = stype[pd];
	ASSERT(t.pd == pd);
	return t.id;
}

void OdbcDatabase::setSType(Domain* pd, SQLSMALLINT t){
	BKType bt = {pd, t};
	ASSERT(!stype.exists(bt));
	stype.add(bt);
}

//******************************************************

OdbcCursor::OdbcCursor( OdbcDatabase* pdb,  SQLStatement* pst) : Cursor(pdb, pst){
	CheckC(SQLAllocStmt(*pdb, &_hstmt));
	SQLUINTEGER Value = SQL_SCROLLABLE;
	CheckC(SQLSetStmtAttr( _hstmt, SQL_ATTR_CURSOR_SCROLLABLE, reinterpret_cast<SQLPOINTER>(Value), 0));
};

OdbcCursor::~OdbcCursor(){
	if(_hstmt != SQL_NULL_HSTMT)
		CheckC(SQLFreeStmt(_hstmt, SQL_DROP));
	_hstmt = SQL_NULL_HSTMT;
};
void OdbcCursor::prepare(){
	SQLStatement* pst = dynamic_cast<SQLStatement*>(_pst);
	string sql = pst->buildSelect();
	CheckC( SQLPrepare(_hstmt, (SQLCHAR*)(const char*)sql, sql.length()) );
}

/*
void OdbcDatabase::Exec()
{
//	Check(SQLExecute(hstmt));
}
*/

void OdbcCursor::bind(int col, Var v)
{
	SQLINTEGER i = 0;// (SDWORD*)prf->GetIndicator();
	SQLSMALLINT ctype = get_db()->getCType(v);
	CheckC(SQLBindCol(_hstmt, col, ctype,
		v->data(), v->size(), &i));
}

void OdbcCursor::parameter(int num, Var& v)
{
	SQLINTEGER i = 0;// (SDWORD*)prf->GetIndicator();
	SQLSMALLINT ctype = get_db()->getCType(v);
	CheckC(SQLBindParameter(_hstmt, num, SQL_PARAM_INPUT,
			ctype, SQL_CHAR, v->size(),
			0, v->data(), v->size(), &i));
}

void OdbcCursor::exec(){
	CheckC(SQLExecute(_hstmt));
}

void OdbcCursor::fetch(int n, bool absolute/* = false*/){
	
	SQLUSMALLINT fetchtype;
	switch (n){
		case Cursor::first:
			fetchtype = SQL_FETCH_FIRST;
			break;
		case Cursor::last:
			fetchtype = SQL_FETCH_LAST;
			break;
		default:
			fetchtype = absolute ? SQL_FETCH_ABSOLUTE : SQL_FETCH_RELATIVE;
			break;
	}
	if ( CheckC(SQLFetchScroll(_hstmt, fetchtype, n)) ){
		_pos = _none;
		return;
	}
	_pos = n > 0 ? _eof : _bof;
}

bool OdbcCursor::checkSqlReturn( RETCODE  code, const char* pszCall, const char* pszFile, int nLine, OdbcCursor* p){
	string str, sLine;
 	switch(code)
	{
	case SQL_SUCCESS:
		return true;
	case SQL_NO_DATA_FOUND:
		return false;
	case SQL_SUCCESS_WITH_INFO:
	default:
		sLine = nLine;
		str << pszCall << "/" << pszFile << "(" << sLine << ")";
		throw OdbcDatabase::Exception(str, p);
	}
	return false;
}

OdbcDatabase::Exception::Exception(string e, OdbcCursor* p) : error(e){
/*	SQL_HANDLE_ENV
	SQL_HANDLE_DBC
	SQL_HANDLE_STMT
	SQL_HANDLE_DESC 
*/
	SQLSMALLINT		HandleType;
	SQLHANDLE		Handle;
	SQLCHAR			Sqlstate[6]={0,0,0,0,0,0};
	SQLINTEGER		NativeErrorPtr;
	SQLCHAR			MessageText[1024];
	SQLSMALLINT     BufferLength = 1024;
	SQLSMALLINT		TextLength = 0;
	memset(MessageText, 0, 1024);
	if(p){
		HandleType = SQL_HANDLE_STMT;
		Handle = *p;
	}else{
		ASSERT(0);
	}

	SQLGetDiagRec( HandleType, Handle, 1, Sqlstate, &NativeErrorPtr, MessageText, BufferLength,
		 &TextLength);
	MessageText[TextLength] = 0;
	error << (const char*)Sqlstate << " " << (const char*)MessageText;
}
