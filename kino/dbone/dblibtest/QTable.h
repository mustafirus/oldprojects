// QTable.h: interface for the QTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QTABLE_H__2A22DB52_DE78_11D1_A60C_204C4F4F5020__INCLUDED_)
#define AFX_QTABLE_H__2A22DB52_DE78_11D1_A60C_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Set.h"
#include "Table.h"
//typedef CArray<int,int> IntArray;
/*
class DataBase;
class QJoin
{
friend class QTable;
public:
	QJoin(FKey* pFKey = NULL);
	~QJoin();
	int PKey(int n){return m_nPKey[n];};
	int NKey(int n){return m_nNKey[n];};
	int Count(){return fields;};
	CString name;

protected:
	int  fields;
	int* m_nPKey;
	int* m_nNKey;
	const char* rst;
	const char** fcapt;
};

*/
class QField;
class SqlStmt;
class QTable
{
public:/// For Link
	Table*  pNTable;
	char	Alias[16];
	QTable* pPTable;
	QField* pQField;
	FKey*	pFKey;
	QTable* next;
	bool	mark;
	int		style;
	QTable* Add(QTable* pqf);
	QTable* Find(QTable* pqt, FKey* pfk);
public:
// Construction/Destruction
	QTable(Table* pTable);
	QTable(QTable* pPTable, FKey* pKey);
	virtual ~QTable();

// Construction helpers
	QTable* Join(const char* fkeyname);
	QTable* Join(FKey* pfk);
	QTable* GetLink(QTable* pqt);
//	QTable* GetLink(QTable* pqtlink, QTable* pqtthis = NULL);
	QField* GetQField(const char* name);
	void	GetQFields(FieldSet& f, QFields& qf);
	const char* GetExtent(const char* str);
	operator Table*(){return pNTable;};
	operator Table&(){return *pNTable;};
	operator QTable*(){return next;};
	operator FKey*(){return pFKey;};
	bool	IsMaster(){return pPTable == NULL;};
	QFields& operator>>(QFields& qf);

// RKey creation

	void GetKeyFields(QFields& qtrg);
	QTable* GetFirstChild()
	{
		if(!pPTable)
			return this;
		if(!pPTable->pPTable)
			return this;
		else 
			return pPTable->GetFirstChild();
	};
	void		Select(SqlStmt& str, bool first = true);
	void		Delete(SqlStmt& str);
	const char*	AliaS(){return Alias;};
	void		Mark(bool this_only);
	const char* GetRest();
	const char* GetName(int num, int type = 0);
	void		GetNames(PNAMEINFO pn);
//	QField* CreateField(const char* name);
/*	Table*	GetLink(const char* fkey);
	Field*	PKey(int n){return pPTable->pNTable->GetField(join.PKey(n));};
	Field*	NKey(int n){return pNTable->GetField(join.NKey(n));};
	int		KeyCount(){return join.Count();};

	void GetNKeyFields(QField* pqfroot, QFields& qtrg);

	QTable* FindChild(QTable* pqt);
	void Mark(bool this_only);
	QTable* Next(){return next;};
	const char* GetFCapt(int n);
	int GetStyle();
*/
//	static Table* GetTable(CString pkey_name);
//	static DataBase* pDB;
private:
/*	CString GetForm(int n);
*/
DECLARE_DICT_MEMORY
};

#endif // !defined(AFX_QTABLE_H__2A22DB52_DE78_11D1_A60C_204C4F4F5020__INCLUDED_)
