#pragma once
#include "vector.h"
#include "database.h"

class OdbcCursor;

class OdbcDatabase : public Database
{
	HENV		henv;
	HDBC		hdbc; 
	HSTMT		hstmt;
//	OdbcStmt*	pStmt;
//	HSTMT		basehstmt;
	bool		connected;
	RETCODE		retcode;
	int			next_parameter;
	bool needflash; // for debug

//Helpers
	void Connect(const char* dsn, const char* user, const char* pass);
	void Disconnect();
public:
	operator HDBC() { return hdbc; }

public:
	class Exception : public Database::Exception {
		OdbcCursor*		pc;
		string			error;
	public:
		Exception(string e, OdbcCursor* p);
		virtual ~Exception(){};
		virtual string tostring(){return error;};
	};


	OdbcDatabase(const char* dsn, const char* user, const char* pass);
	virtual ~OdbcDatabase();
	virtual Cursor* getCursor(Query* pq);

	SQLSMALLINT getCType(Domain* pd);
	void setCType(Domain* pd, SQLSMALLINT t);

	SQLSMALLINT getSType(Domain* pd);
	void setSType(Domain* pd, SQLSMALLINT t);

	virtual void	setTypeString(Domain* pd, string s);
	virtual string	getTypeString(Domain* p);

private:
	struct BKType{
		Domain*		pd;
		SQLSMALLINT id;
		bool operator==(Domain* p) { return p == pd; }
		bool operator==(BKType& t) { return t.pd == pd; }
	};

	struct BKTypeS{
		Domain*		pd;
		string		id;
		BKTypeS() : pd(NULL){}
		BKTypeS(Domain* p, string& s) : pd(p), id(s){}
		BKTypeS(BKTypeS& t) : pd(t.pd), id(t.id){}
		bool operator==(Domain* p) { return p == pd; }
		bool operator==(BKTypeS& t) { return t.pd == pd; }
		BKTypeS& operator=(BKTypeS& t) { pd = t.pd; id = t.id; return *this; }
	};

	static vector<BKTypeS>	typestring;
	static vector<BKType>	ctype;
	static vector<BKType>	stype;

	friend inline bool X_ChechSqlReturn(RETCODE  code,
		const char* pszCall, const char* pszFile,
		int nLine, OdbcDatabase* pDB, HSTMT hstmt,
		const char* stmt);
};


#include "Statement.h"
class OdbcCursor : public Cursor{
	HSTMT		_hstmt;
	OdbcDatabase* get_db(){ return dynamic_cast<OdbcDatabase*>(_pdb); }
public:
	OdbcCursor( OdbcDatabase* pdb,  SQLStatement* pst);
	virtual ~OdbcCursor();

	operator HSTMT() { return _hstmt; }
	virtual void prepare();
	virtual void bind(int col, Var prf);
	virtual void parameter(int num, Var& v);
	virtual void exec();
	virtual void fetch(int n, bool absolute = false);
private:
	bool checkSqlReturn( RETCODE  code, const char* pszCall,
		const char* pszFile, int nLine, OdbcCursor* p);
};



inline bool X_ChechSqlReturn(RETCODE  code, const char* pszCall,
	const char* pszFile, int nLine,
	HENV henv, HDBC hdbc, HSTMT hstmt, const char* str = NULL);
inline bool X_ChechSqlReturn(RETCODE  code, const char* pszCall,
	const char* pszFile, int nLine, OdbcDatabase* pDB, HSTMT hstmt, const char* stmt = NULL)
{
	return X_ChechSqlReturn(code, pszCall, pszFile, nLine, (HENV)pDB->henv, pDB->hdbc, hstmt, stmt);
}
