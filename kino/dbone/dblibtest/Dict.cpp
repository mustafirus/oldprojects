// Dict.cpp: implementation of the Dict class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Log.h"
//#include "Clock.h"
//#include "ErrorCodes.h"
#include "Dict.h"
#include "DataBase.h"
#include "DbException.h"
#include "Table.h"
#include "FCapts.h"
#include "RKey.h"
#include "QField.h"
#include "Query.h"
//#include "Record.h"

HANDLE Dict::hLoadDict = NULL;
static Field::Type TypeCrack(unsigned char dbtype);

#define duser "dict"
#define dpass "jdkjfewl"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HANDLE Dict::hDictHeap = HeapCreate( HEAP_GENERATE_EXCEPTIONS,  32768, 0);

void* Dict::malloc(unsigned int size)
{
	return HeapAlloc(hDictHeap, HEAP_GENERATE_EXCEPTIONS, size);
}

void Dict::free(void* p)
{
	HeapFree(hDictHeap, 0, p);
}

Dict::Dict(DataBase* pdb) : pDB(pdb), pFCapts(NULL), ready(false), pStmtForm(NULL), pStmtProc(NULL)
{
	DWORD dw;
	ASSERT(hLoadDict == NULL);
	if(!  (hLoadDict = CreateThread(NULL, 0, 
		(PTHREAD_START_ROUTINE)LoadDict, this, 0, &dw))
		)
	{
		delete pDB;
		DbError(MSG_ERROR_CREATE_THREAD,"load db dictionary")
	}
}

Dict::~Dict()
{
	if(hLoadDict)
	{
		TerminateThread(hLoadDict, -1);
		CloseHandle(hLoadDict);
	}
	ifdel(pFCapts);
	ifdel(pDB);
	pDB = NULL;
}

bool Dict::Ready()
{
	if(ready)
		return true;
	if(WaitForSingleObject(hLoadDict, 5000) == WAIT_TIMEOUT)
		return false;
	DWORD dw;
	if(!GetExitCodeThread(hLoadDict, &dw)||dw != 0)
	{
		delete pDB;
		pDB = NULL;
		DbError(MSG_ERROR_LOAD_DICT,"");
		return false;
	}
	ready = true;
	CloseHandle(hLoadDict); hLoadDict = NULL;
	return true;
}

Query* Dict::GetQuery(const char* name)
{
	return *qtMap[name];
}

Query* Dict::GetQuery(Table* pt)
{
	return *qtMap[pt->mPKey.name];
}

Table* Dict::GetTableByName(const char* name)
{
	for(int i = 0; i < pTables.GetSize(); i++)
	{
		if(!lstrcmp(pTables[i]->name, name))
			return pTables[i];
	}
	DbError(MSG_ERROR_GET_TABLE, name);
	return NULL;
}

PKey* Dict::GetPKey(const char* name)
{
	return *GetTable(name);
}

Table* Dict::GetTable(const char* keyname)
{
	return *qtMap[keyname];
}

const char* Dict::GetCapt(QField* pqf)
{
	const char* s1 =
	pFCapts->GetCapt( ((FKey*)*((QTable*)*pqf)), *pqf);
	ASSERT(((Field*)*pqf)->capt);
	return s1 ? s1 : ((Field*)*pqf)->capt;
}

DWORD WINAPI Dict::LoadDict(Dict* pDict)
{
	return pDict->LoadTables() ? 0 : -1;
};


#define GetData		pDB->GetData
#define ExecDirect	pDB->ExecDirect
#define ExecFDirect	pDB->ExecFDirect
#define Read		pDB->Read
#define Flush		pDB->Flush
#define FlushEx		pDB->FlushEx
#define Prepare		pDB->Prepare
#define Set			pDB->Set
#define Bind		pDB->BindParameter
#define Exec		pDB->Exec

bool Dict::LoadTables()
{
	Field* pf;
	Table* pt;
	char s1[30];
	char s2[30];
	int field_num;
	int table_num;
	int prim;
	int num;
	int type;
	try{
		ExecDirect("SELECT count(*) FROM SD_Tables");
		if(!Read())
			return false;
		GetData(1, num);
		pTables.SetSize(num);
		Flush();
		
		ExecDirect("SELECT name, capt, fcount, def_edit FROM SD_Tables ORDER BY id");
		for(int i=0; i < pTables.GetSize(); i++)
		{
			if(!Read())
				return false;
			GetData(1, pTables[i]->name);
			GetData(2, pTables[i]->capt);
			GetData(3, num);
			pTables[i]->pFields.SetSize(num);
			GetData(4, pTables[i]->def_edit);
		}
		Flush();
		table_num = pTables.GetSize();
		SDWORD indicator = SQL_NTS;
		DbStmt* pStmt =	Prepare("select f.name, f.capt, f.ro, f.strlen, f.scrlen, f.type, f.style, f.stmdf, f.deft from SD_Fields f join SD_Tables t on t.id=f.table_id where t.name = ? order by fnum");
		for(i=0; i < table_num; i++)
		{
			pt = pTables[i];
			Bind(0, pTables[i]->name,
				&indicator);
			Exec();
			field_num = pt->pFields.GetSize();
			char ft;
			for(int j = 0; j < field_num; j++)
			{
				if(!Read())
					return false;
				pf = pTables[i]->pFields[j];
				GetData(1, (char*&)pf->name);
				GetData(2, (char*&)pf->capt);
				GetData(3, pf->ro);
				GetData(4, pf->strlen);
				GetData(5, pf->scrlen);
				GetData(6, ft);
				pf->type = TypeCrack(ft);
				GetData(7, pf->style);
				GetData(8, (char*&)pf->stmdf);
				GetData(9, (char*&)pf->deft);
				pf->fnum = j;
				if(pf->type == Field::Char || pf->type == Field::DateTime)
					pf->str=true;
				else
					pf->str=false;
			}
			FlushEx();
			Set(pStmt);
		}
		Flush();
		///Keys////////////////////////////////////////////////
		///Primary////////////////////////////////////////////////
		ExecDirect("select t.name, h.name, h.capt, h.ext, h.rst, h.fcount, h.style, max(ISNULL(f.num, 0)), max(ISNULL(p.num, 0))"
			"\nfrom SD_Key_H h"
			"\njoin SD_Tables t on t.id=h.table_id"
			"\nleft join SD_Forms f on h.id = f.key_id and f.type = 0"
			"\nleft join SD_Forms p on h.id = p.key_id and p.type = 1"
			"\nwhere h.prim = 1"
			"\ngroup by t.name, h.name, h.capt, h.ext, h.rst, h.fcount, h.style");
		while(Read())
		{
			GetData(1, s1, 30);
			pt = GetTableByName(s1);
			pt->mPKey.pTable = pt;
			GetData(2,(char*&)pt->mPKey.name);
			GetData(3,(char*&)pt->mPKey.capt);
			GetData(4,(char*&)pt->mPKey.ext);
			GetData(5,(char*&)pt->mPKey.rst);
			GetData(6,num);
			pt->mPKey.pFields.SetSize(num);
			GetData(7,pt->mPKey.style);
			GetData(8,pt->mPKey.fnum);
			GetData(9,pt->mPKey.pnum);
			if(pt->mPKey.fnum)
			{
				pt->mPKey.forms = new NAME[pt->mPKey.fnum];
				memset(pt->mPKey.forms, 0, sizeof(NAME) * pt->mPKey.fnum);
			}
			if(pt->mPKey.pnum)
			{
				pt->mPKey.procs= new NAME[pt->mPKey.pnum];
				memset(pt->mPKey.procs, 0, sizeof(NAME) * pt->mPKey.pnum);
			}
		}
		Flush();
		for(i = 0; i < pTables.GetSize(); i++)
		{
			pt = pTables[i];
			if(pt->mPKey.name)
				qtMap.SetAt(pt->mPKey.name, new QT(pt));
		}
		ExecDirect("select t.name, f.type, f.num, f.name"
			"\nfrom SD_Key_H h"
			"\njoin SD_Tables t on t.id=h.table_id"
			"\njoin SD_Forms f on h.id = f.key_id"
			"\nwhere h.prim = 1");
		while(Read())
		{
			GetData(1, s1, 30);
			pt = GetTableByName(s1);
			pt->mPKey.pTable = pt;
			GetData(2,type);
			GetData(3,num);
			num--;
			switch(type)
			{
			case 0:
				GetData(4,pt->mPKey.forms[num], sizeof(NAME));
				break;
			case 1:
				GetData(4,pt->mPKey.procs[num], sizeof(NAME));
				break;
			}
		}
		///Foreign////////////////////////////////////////////////
//ExecDirect("select t.name, f.name, f.capt, f.ext, f.rst, f.fcount, f.style, r.name from SD_Key_H f join SD_Tables t on t.id=f.table_id left join SD_Key_H r on r.id=f.ref_id where f.prim = 3");
		ExecDirect("select t.name, h.name, h.capt, h.ext, h.rst, h.fcount, h.style, r.name, max(ISNULL(f.num, 0)), max(ISNULL(p.num, 0))"
			"\nfrom SD_Key_H h"
			"\njoin SD_Tables t on t.id=h.table_id"
			"\njoin SD_Key_H r on r.id = h.ref_id"
			"\nleft join SD_Forms f on h.id = f.key_id and f.type = 0"
			"\nleft join SD_Forms p on h.id = p.key_id and p.type = 1"
			"\nwhere h.prim = 3"
			"\ngroup by t.name, h.name, h.capt, h.ext, h.rst, h.fcount, h.style, r.name");

		FKey* pfk;
		while(Read())
		{
			GetData(1, s1, 30);
			pt = GetTableByName(s1);
			pfk = pt->pFKeys[pt->pFKeys.GetSize()];
			pfk->pTable = pt;
			
			GetData(2,(char*&)pfk->name);
			GetData(3,(char*&)pfk->capt);
			GetData(4,(char*&)pfk->ext);
			GetData(5,(char*&)pfk->rst);
			GetData(6,num);
			pfk->pFields.SetSize(num);
			GetData(7,pfk->style);
			GetData(8, s1, 30);
			pfk->pPKey = GetPKey(s1);
			if(!pfk->pPKey)
				DbError(MSG_PK_NOT_FOUND,s1);
			GetData(9,pfk->fnum);
			GetData(10,pfk->pnum);
			if(pfk->fnum)
			{
				pfk->forms = new NAME[pfk->fnum];
				memset(pfk->forms, 0, sizeof(NAME) * pfk->fnum);
			}
			if(pfk->pnum)
			{
				pfk->procs= new NAME[pfk->pnum];
				memset(pfk->procs, 0, sizeof(NAME) * pfk->pnum);
			}
		}
		Flush();
		ExecDirect("select h.prim, t.name, h.name, f.num, f.fnum\n\
			from SD_Key_F f\n\
			join SD_Key_H h on h.id=f.key_id\n\
			join SD_Tables t on t.id=h.table_id\n\
			order by t.name, h.name");
		while(Read())
		{
			GetData(1, prim);
			switch(prim)
			{
			case 1:
				GetData(2, s1, 30);
				if( !pt || (pt && lstrcmpi(s1, pt->name)) )
					pt = GetTableByName(s1);
				ASSERT(pt);
				GetData(4, num);
				pt->mPKey.pFields.Add(pt->pFields[num - 1]);
				break;
			case 2:
				continue;
			case 3:
				GetData(2, s1, 30);
				GetData(3, s2, 30);
				if( !pt || (pt && lstrcmpi(s1, pt->name)) )
					pt = GetTableByName(s1);
				pfk = pt->GetFKey(s2);
				ASSERT(pt && pfk);
				GetData(5, num);
				pfk->pFields.Add(pt->pFields[num - 1]);
			}
		}
		Flush();
		ExecDirect("select h.prim, t.name, h.name, f.num, f.capt "
							"from SD_Key_Capt f "
							"join SD_Key_H h on h.id=f.key_id "
							"join SD_Tables t on t.id=h.table_id "
							"where f.capt is not null "
							"order by t.name, h.name, f.num");
		while(Read())
		{
			GetData(1, prim);
			ASSERT(prim = 3);
			GetData(2, s1, 30);
			GetData(3, s2, 30);
			if( !pt || (pt && lstrcmpi(s1, pt->name)) )
				pt = GetTableByName(s1);
			pfk = pt->GetFKey(s2);
			ASSERT(pt && pfk);
			GetData(4, num);
			GetData(5, s1, 32);
			pFCapts = new FCapts(pfk, pt->pFields[num], s1, pFCapts);
		}
		Flush();
	}catch(DbException* e)
	{
		e->Effect();
		return false;
	}
	return true;
}

char* Dict::GetForm(Table* pt, int n, int type /*= 0*/)
{
	return GetForm(pt->name, pt->mPKey.name, n, type);
}

char* Dict::GetForm(Query* pq, int n, int type /*= 0*/)
{
	return GetForm(*(QTable*)*pq, n, type);
}

char* Dict::GetForm(const char* key, int n, int type /*= 0*/)
{
	Table* pt = GetTable(key);
	char* str = GetForm(pt->name, pt->mPKey.name, n, type);
	ASSERT(str);
	return str;
}

char* Dict::GetForm(RKey* prk, int n, int type /*= 0*/)
{
	FKey* pfk = *((QTable*)*prk);
	char* str = NULL;
	if(pfk)
		str = GetForm(pfk->pTable->name, pfk->name, n, type);
	if(!str)
	{
		Table* pt = *((QTable*)*prk);
		str = GetForm(pt->name, pt->mPKey.name, n, type);
	}
	return str;
}

char* Dict::GetForm(const char* table, const char* key, int n, int type /*= 0*/)
{
	char* str = NULL;
	SDWORD ind = SQL_NTS;
	if(!pStmtForm)
	{
		pStmtForm = Prepare("SELECT data from SD_Forms f\n\
			join SD_Key_H k on k.id = f.key_id\n\
			join SD_Tables t on t.id = k.table_id\n\
			WHERE t.name = ? AND k.name = ? AND num = ? AND type = ?");
	}else
		Set(pStmtForm);
	Bind(strlen(table), (char*)table, &ind);
	Bind(strlen(key), (char*)key, &ind);
	Bind(&n);
	Bind(&type);
	Exec();
	if(Read())
		GetData(1, str);
	FlushEx();
	return str;
}

/*char* Dict::GetProc(RKey* prk, int n)
{
	FKey* pfk = *((QTable*)*prk);
	char* str = NULL;
	if(pfk)
		str = GetProc(pfk->pTable->name, pfk->name, n);
	if(!str)
	{
		Table* pt = *((QTable*)*prk);
		str = GetProc(pt->name, pt->mPKey.name, n);
	}
	ASSERT(str);
	return str;
}
*/
/*char* Dict::GetProc(const char* table, const char* key, int n)
{
	char* str = NULL;
	SDWORD ind = SQL_NTS;
	if(!pStmtProc)
	{
		pStmtProc = Prepare("SELECT data from SD_Procs f\n\
			join SD_Key_H k on k.id = f.key_id\n\
			join SD_Tables t on t.id = k.table_id\n\
			WHERE t.name = ? AND k.name = ? AND num = ?");
	}else
		Set(pStmtProc);
	Bind(strlen(table), (char*)table, &ind);
	Bind(strlen(key), (char*)key, &ind);
	Bind(&n);
	Exec();
	if(Read())
		GetData(1, str);
	else DbError2(MSG_ERROR_FORM_NOTFOUND, table, key);
	FlushEx();
	return str;
}
*/
////////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////////
#define SQLVOID        0x1f
#define SQLTEXT        0x23
#define SQLVARBINARY   0x25
#define SQLINTN        0x26
#define SQLVARCHAR     0x27
#define SQLBINARY      0x2d
#define SQLIMAGE       0x22
#define SQLCHAR        0x2f
#define SQLINT1        0x30
#define SQLBIT         0x32
#define SQLINT2        0x34
#define SQLINT4        0x38
#define SQLMONEY       0x3c
#define SQLDATETIME    0x3d
#define SQLFLT8        0x3e
#define SQLFLTN        0x6d
#define SQLMONEYN      0x6e
#define SQLDATETIMN    0x6f
#define SQLFLT4        0x3b
#define SQLMONEY4      0x7a
#define SQLDATETIM4    0x3a
#define SQLDECIMAL     0x6a
#define SQLNUMERIC     0x6c

Field::Type TypeCrack(unsigned char dbtype)
{
	switch(dbtype)
	{ 
	case SQLVARCHAR:
	case SQLCHAR:
	case SQLTEXT:
		return Field::Char;
	case SQLINT1:
		return Field::TinyInt;
	case SQLINT2:
		return Field::SmallInt;
	case SQLINT4:
	case SQLINTN:
		return Field::Integer;
	case SQLDATETIME:
	case SQLDATETIM4:
	case SQLDATETIMN:
		return Field::DateTime;
	default:
		return Field::Unknown;
	}
}



/*
case SQLBIT:
case SQLVOID:
case SQLVARBINARY:
case SQLBINARY:
case SQLIMAGE:
case SQLMONEY:
case SQLFLT8:
case SQLFLTN:
case SQLMONEYN:
case SQLFLT4:
case SQLMONEY4:
case SQLDECIMAL:
case SQLNUMERIC:
*/

#ifdef _DEBUG
void Dict::Dump()
{
	for(int i = 0; i < pTables.GetSize(); i++)
	{
		pTables[i]->Dump();
	}
}
#endif //_DEBUG
