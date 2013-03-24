// Table.h: interface for the Table class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLE_H__1ACBEBB0_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_)
#define AFX_TABLE_H__1ACBEBB0_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <map>
#include <vector>
#include <string>
#include "Array.h"
#include "Field.h"
#include "FixSet.h"

using namespace std;

class Table;

typedef char NAME[30];
typedef char* PNAME;
typedef struct NAMEINFO_tag
{
	int		fnum;
	int		pnum;
	PNAME*	names;
}NAMEINFO, *PNAMEINFO;

class PKey
{
public:
	Table*		pTable;
	const char*	name;
	const char*	capt;
	const char*	ext;
	const char*	rst;
	int			style;
	int			form;
	int			list;
	int			fnum;
	int			pnum;
	FieldSet	pFields;
	NAME*		forms;
	NAME*		procs;

	Field* operator[](int n){if(n >= 0 && n < pFields.GetSize())return pFields[n]; else return NULL;};
	PKey() : pTable(NULL), name(NULL), capt(NULL), ext(NULL), rst(NULL), style(0), form(0), list(0), 
	fnum(0), pnum(0), forms(NULL), procs(NULL){};
	~PKey(){ifdel((char*)name); ifdel((char*)capt); ifdel((char*)ext); ifdel((char*)rst);
	if (forms) delete[] forms; if(procs) delete[] procs;};

#ifdef _DEBUG
	void Dump();
#endif //_DEBUG

};

class FKey : public PKey
{
public:
	PKey*		pPKey;

	FKey() : pPKey(NULL){};
	~FKey(){};

#ifdef _DEBUG
	void Dump();
#endif //_DEBUG
};

//class QTable;
class Table
{
	typedef map<string, Field*> FieldM;
	typedef vectorautodel<Field*> Fields;
	typedef vectorautodel<FKey*> FKeys;

public:
	typedef map<string, string> props;

private:
	props	_props;
	Fields	_fields;
	FieldM	_fieldm;

public:/// For Link
friend class Dict;
/*
	char*	name;
	char*	capt;
	int		def_edit;
*/
	FKeys	pFKeys;
	PKey	mPKey;

public:
//	Table(): def_edit(0), name(NULL), capt(NULL) {};
	explicit Table(props& p): _props(p) {};
	~Table(){};

//Operations
	bool Find(Field* pf);
	void addField(Field* pf){
		_fields.push_back(pf);
		_fieldm[pf->name()]=pf;
	};
	Field* GetField(int n);
	Field* GetField(const char* name);
	FKey* GetFKey(const char* name);
	FKey* GetFKey(PKey* pRef);
	PKey* GetPKey(){return &mPKey;};
	operator PKey*(){if(!this) return NULL; return &mPKey;};
	//operator char*(){if(!this) return NULL; return name;};
	string& name(){return _props["name"];};

	Field* GetKeyField(int n){return mPKey[n];}
	int    GetKeyFieldCount(){return mPKey.pFields.GetSize();}


#ifdef _DEBUG
	void Dump();
#endif //_DEBUG
};


#endif // !defined(AFX_TABLE_H__1ACBEBB0_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_)
