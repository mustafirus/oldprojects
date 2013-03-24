// OdbcDataBase.h: interface for the OdbcDataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODBCDATABASE_H__E4774EDF_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
#define AFX_ODBCDATABASE_H__E4774EDF_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "DataBase.h"
#include "Map.h"


class OdbcStmt : public DbStmt
{
	OdbcStmt*	next;
	HSTMT hst;
	
	bool locked;
public:
	OdbcStmt(const char* stmt, HSTMT hstmt, OdbcStmt* pHead) :
	  DbStmt(stmt), next(pHead), hst(hstmt), locked(false){};
	operator HSTMT(){if(!this) return NULL; return hst;}
	void Lock(){locked = true;};
	void Unlock(){locked = false;};
	OdbcStmt * Find(const char* stmt)
	{
		if(!locked && !lstrcmp(stmt, *this))
			return this;
		if(next)
			return next->Find(stmt);
		else
			return NULL;
	};
};
#define HASH_SIZE 50
class OdbcStmtMap
{
	OdbcStmt* pData[HASH_SIZE];
	unsigned char GetHash(const char* key) const
	{
		UINT nHash = 0;
		UINT n = 0;
		while (*key)
			nHash += (nHash<<14) + *key++ ;
		nHash  = nHash % HASH_SIZE;
		return nHash;
	};
public:
	OdbcStmtMap(){ASSERT(HASH_SIZE < 256);memset(pData, 0, sizeof(pData));};
	~OdbcStmtMap()
	{
		for(int i = 0; i < HASH_SIZE; i++)
			if(pData[i])
				delete pData[i];
	};
	OdbcStmt* OdbcStmtMap::SetAt(const char* stmt, HSTMT hstmt)
	{
		unsigned char i = GetHash(stmt);
		ASSERT(operator[](stmt) == NULL);
		return pData[i] = new OdbcStmt(stmt, hstmt, pData[i]);
	}
		
	OdbcStmt* operator[](const char* stmt) const
	{
		unsigned char i = GetHash(stmt);
		if(pData[i])
			return pData[i]->Find(stmt);
		else
			return NULL;
	}
};


//typedef StringMap<OdbcStmt, 50> OdbcStmtMap;

class OdbcDataBase : public DataBase
{
	HENV		henv;
	HDBC		hdbc; 
	HSTMT		hstmt;
	OdbcStmt*	pStmt;
	HSTMT		basehstmt;
	bool		connected;
	RETCODE		retcode;
	int			next_parameter;
	bool needflash; // for debug
	OdbcStmtMap	ssmap;

//Helpers
	void Connect(const char* dsn, const char* user, const char* pass);
	void Disconnect();
	void Init(bool init = true);
public:

public:
	OdbcDataBase(const char* dsn, const char* user, const char* pass);
	virtual ~OdbcDataBase();
	DbCursor* GetCursor(const char* stmt, int maxfr);

	void ExecDirect(const char* str);
	DbStmt* Prepare(const char* str);
	void Set(DbStmt* pst);
	void Exec();
	void __cdecl ExecFDirect(const char* str, ...);
	void Bind(int col, RField* prf);
	void Bind(int col, char* buf, int len, int* pi, Field::Type type = Field::Char);
//	void Bind(RFields& rf);
	void BindParameter(/*int col, */int len, char* buf, SDWORD* pindicator);
	void BindParameter(/*int num, */int* par);
	void BindParameter(RKey* prk);
	void BindParameter(RSField* prf);
	void BindParameter(RField* prf);
	void GetData(int icol, int& data);
	void GetData(int icol, char*& data);
	void GetData(int icol, bool& b);
	void GetData(int icol, char& data);
	void GetData(int icol, char* data, int num);
	void CheckData(RField* prf);
	void* GetIndicator(int rows){return new SDWORD[rows];};
	Field::Type Convert(const char* frombuf, char* tobuf, Field::Type from, Field::Type to);
	void GetInfo(TABLEINFO* pti);
	
	int Export(const char * tablename, const char * filename);
	
//	void Bind(int num, RField** pr);
	bool Read();
	void Flush();
	void FlushEx();
	int  GetIdentity();
	bool ExecProc(const char* name, int id, int& newid);
	bool ExecProc(const char * name, int id, char* str, int slen);
	bool Ready(){return connected && hstmt != SQL_NULL_HSTMT;};

//Static
	static unsigned char TypeCrack(Field::Type dicttype);
	static unsigned char CTypeCrack(Field::Type dicttype);
	static Field::Type TypeCrack(unsigned char dbtype);

private:
friend inline bool X_ChechSqlReturn(RETCODE  code,
		const char* pszCall, const char* pszFile,
		int nLine, OdbcDataBase* pDB, HSTMT hstmt,
		const char* stmt);
friend class OdbcCursor;
#ifdef DEBUGSQL
	char		stmt[1024];
#endif //DEBUGSQL
};

inline bool X_ChechSqlReturn(RETCODE  code, const char* pszCall,
	const char* pszFile, int nLine,
	HENV henv, HDBC hdbc, HSTMT hstmt, const char* str = NULL);
inline bool X_ChechSqlReturn(RETCODE  code, const char* pszCall,
	const char* pszFile, int nLine, OdbcDataBase* pDB, HSTMT hstmt, const char* stmt = NULL)
{
	return X_ChechSqlReturn(code, pszCall, pszFile, nLine, (HENV)pDB->henv, pDB->hdbc, hstmt, stmt);
}

#endif // !defined(AFX_ODBCDATABASE_H__E4774EDF_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
