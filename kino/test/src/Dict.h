// Dict.h: interface for the Dict class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DICT_H__E4774EE1_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
#define AFX_DICT_H__E4774EE1_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_


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
	stringMap<QT*,  45>	qtMap;
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

private:
	bool LoadTables();
	static int LoadDict(Dict* pDict);

#ifdef _DEBUG
public:
	void Dump();
#endif //_DEBUG
};
#define DbError(N,S)
#define DbError2(N,S1, S2)

#endif // !defined(AFX_DICT_H__E4774EE1_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
