// RKey.h: interface for the RKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RKEY_H__BC8CAC03_F65C_11D1_A64D_204C4F4F5020__INCLUDED_)
#define AFX_RKEY_H__BC8CAC03_F65C_11D1_A64D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class RField;
class QField;
class Table;
class QTable;
class Query;
class Record;
class Field;
#include "Set.h"
class SqlStmt;
class RKey
{
public:/// For Link
	QTable*		pQTable;	//Src
	RFields		pRFields;	// Src
	Query*		pQuery;		//Trg
	QFields		pQFields;	//Trg

public:
	RKey(QTable* pt, Record* pr);
	virtual ~RKey(){};
	
	operator Query*(){return pQuery;};
	operator QTable*(){return pQTable;};
	operator RFields&(){return pRFields;};
	void Get(char* str);
	bool CanSet(char*& str);
	void Set(char* str);
	void SetIdentity();
	UINT GetCount(){return pRFields.GetCount();};

	bool IsPrimary();
	bool IsNull();
	void SetNull();

	void Select(SqlStmt& str);
	void Delete(SqlStmt& str);
	void SrcSelect(SqlStmt& str);
	void Update(SqlStmt& str);
	void Refresh(SqlStmt& str);

	RKey& operator=(RKey& rk);


/*	RField**	pRtrg;
	Record*		pTRec;
	const char*	rest;
	bool		clone;
	bool		copy;
*/
/*	RKey(RKey* prk);
	RKey(RKey& rk);
*/
/*	Table* GetTable();
	CString GetForm(int num);
	const char* GetRest();
	void SetRest(const char* str);
*/
/*	Record* GetTarget(){return pTRec;};
	Record* GetSource();
	void SetTarget(Record* pr);
	void SaveTarget();
	bool Return();
	bool IfKey();
	void FreeTarget();
	void RequerySrc();
	void RequeryTrg();
*/
/*	Record* GetRecord();
	void EmptyTarget();
	void EmptySrc();
	void NewTarget();
	void SQLRefr(CString& str, QFields& wqf, bool useAlias);
	void SQLStr(CString& str, bool useAlias);
	bool Where(CString& str, bool bWhere);
*/
/*	const char* GetName();
	const char* GetSrcData(int n = 0);
	void SetSrcData(const char* data, int n );//n = 0
	void SetSrcData(int data, int n = 0);
	void GetSrcFields(char* str, RFields& rf);
	const char* GetFun(char* str);
*/	
private:
/*friend class Record;
friend class DataBase;
*/
};

#endif // !defined(AFX_RKEY_H__BC8CAC03_F65C_11D1_A64D_204C4F4F5020__INCLUDED_)
