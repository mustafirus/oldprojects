// DBCursor.h: interface for the DBCursor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBCURSOR_H__821BF332_F861_11D1_A654_204C4F4F5020__INCLUDED_)
#define AFX_DBCURSOR_H__821BF332_F861_11D1_A654_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class DataBase;
class RKey;
class RSField;

class DbCursor  
{
public:
	enum FetchType{f_first, f_next, f_prev, f_random, f_relative, f_last};

	DbCursor(){};
	virtual void SetStmt(const char* stmt) = 0;
	virtual ~DbCursor(){};
//	virtual void* GetBookmark() = 0;


//	virtual bool Bind(int col, int len, char* buf, SDWORD* pindicator, short int type = SQL_C_CHAR) = 0;
//	virtual void Bind(RKey* prk) = 0;
	virtual bool Bind(int col, RSField* prf) = 0;
	virtual void BindParameter(RField* prf) = 0;
	virtual void BindParameter(RSField* prf) = 0;
	virtual void BindParameter(RKey* prk) = 0;

	virtual void Execute() = 0;
	virtual bool BindEx(int col, int* len, char* buf, SDWORD* pindicator) = 0;
	virtual int Fetch(FetchType fetchtype, int rownum, int nfetchrows) = 0;
//DECLARE_DICT_MEMORY
};

#endif // !defined(AFX_DBCURSOR_H__821BF332_F861_11D1_A654_204C4F4F5020__INCLUDED_)
