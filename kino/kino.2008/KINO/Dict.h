// Dict.h: interface for the Dict class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICT_H__E4774EE1_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
#define AFX_DICT_H__E4774EE1_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef SET_DEBUG_NEW
#ifdef _DEBUG

#define DECLARE_DICT_MEMORY		public:\
void* operator new(unsigned int size);\
void* operator new(unsigned int size, int, const char *, int);\
void operator delete(void* p);

#define IMPLEMENT_DICT_MEMORY(X)		\
void* X::operator new(unsigned int size){return Dict::malloc(size);};\
void* X::operator new(unsigned int size, int, const char *, int){return Dict::malloc(size);};\
void X::operator delete(void* p){Dict::free(p);};

#else

#define DECLARE_DICT_MEMORY		public:\
void* operator new(unsigned int size);\
void operator delete(void* p);

#define IMPLEMENT_DICT_MEMORY(X)		\
void* X::operator new(unsigned int size){return Dict::malloc(size);};\
void X::operator delete(void* p){Dict::free(p);};

#endif //_DEBUG

#else

#define DECLARE_DICT_MEMORY
#define IMPLEMENT_DICT_MEMORY(X)

#endif //SET_DEBUG_NEW

#include "Array.h"
#include "Map.h"
#include "Query.h"
#include "Table.h"
class DataBase;
class Table;
class PKey;
class Query;
class RKey;
class FCapts;
class DbStmt;

class Dict  
{
	struct QT
	{
		Query* pQuery;
		Table* pTable;
		QT(Table* pt) : pTable(pt), pQuery(NULL){};
		~QT(){if(pQuery) delete pQuery;};
		operator Query*(){if(!this) return NULL; if(pQuery)return pQuery; else return pQuery = new Query(pTable);};
		operator Table*(){if(!this) return NULL; return pTable;};
	};

	DataBase*	pDB;
	Tables		pTables;
	FCapts*		pFCapts;
	DbStmt*		pStmtForm;
	DbStmt*		pStmtProc;
	StringMap<QT,  45>	qtMap;
	bool		ready;

	Table* GetTableByName(const char* name);
public:
	bool Ready();
	Dict(DataBase* pdb);
	virtual ~Dict();

	PKey*  GetPKey(const char* name);
	Table* GetTable(const char* keyname);
	Query* GetQuery(const char* name);
	Query* GetQuery(Table* pt);
	const char* GetCapt(QField* pqf);
	char* GetForm(Table* pt, int n, int type = 0);
	char* GetForm(Query* pq, int n, int type = 0);
	char* GetForm(const char* key, int n, int type = 0);
	char* GetForm(const char* table, const char* key, int n, int type = 0);
	char* GetForm(RKey* prk, int n, int type = 0);
//	char* GetProc(const char* table, const char* key, int n);
//	char* GetProc(RKey* prk, int n);

	operator DataBase*(){return pDB;};

	static HANDLE hDictHeap;
	static void* malloc(unsigned int size);
	static void free(void* p);
#ifndef SET_DEBUG_NEW
	void* operator new(unsigned int size){return malloc(size);};
#ifdef _DEBUG
	void* operator new(unsigned int size, int, const char *, int){return malloc(size);};
#endif //_DEBUG
	void operator delete(void* p){free(p);};
#endif //SET_DEBUG_NEW
private:
	bool LoadTables();
	static HANDLE hLoadDict;
	static DWORD WINAPI LoadDict(Dict* pDict);

#ifdef _DEBUG
public:
	void Dump();
#endif //_DEBUG
};
#define DbError(N,S) {DbException::dbExcept.printf(N,S); throw &DbException::dbExcept;}
#define DbError2(N,S1, S2) {DbException::dbExcept.printf(N,S1,S2); throw &DbException::dbExcept;}

#endif // !defined(AFX_DICT_H__E4774EE1_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
