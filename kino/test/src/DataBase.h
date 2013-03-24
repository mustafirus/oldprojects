// DataBase.h: interface for the DataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__670D9FD3_DFFF_11D1_A611_204C4F4F5020__INCLUDED_)
#define AFX_DATABASE_H__670D9FD3_DFFF_11D1_A611_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Field.h"
#include "Set.h"
//#include "Defines.h"

class DbException;
extern DbException dbExcep;
class DbCursor;
class RSField;
class RKey;

class DbStmt
{
	char* buf;
public:
	DbStmt(const char* stmt){buf = new char[strlen(stmt)+1]; strcpy(buf, stmt);};
	virtual ~DbStmt(){delete buf;};
	operator const char*(){return buf;};
//DECLARE_DICT_MEMORY
};

class DataBase  
{
public:
	virtual int Export(const char* tablename, const char * filename) = 0;
	DataBase(){};
	virtual ~DataBase(){};
	virtual DbCursor* GetCursor(const char* stmt, int maxfr) = 0;
	virtual void ExecDirect(const char* str) = 0;
	virtual DbStmt* Prepare(const char* str) = 0;
	virtual void Set(DbStmt* pst) = 0;
	virtual void Exec() = 0;
	//virtual void __cdecl ExecFDirect(const char* str, ...) = 0;
	virtual void GetData(int icol, int& data) = 0;
	virtual void GetData(int icol, char*& data) = 0;
	virtual void GetData(int icol, char& data) = 0;
	virtual void GetData(int icol, bool& b) = 0;
	virtual void GetData(int icol, char* data, int num) = 0;
	virtual void CheckData(RField* prf) = 0;
	virtual void* GetIndicator(int rows) = 0;
	virtual Field::Type Convert(const char* frombuf, char* tobuf, Field::Type from, Field::Type to) = 0;
//	virtual void Bind(RFields& rf) = 0;
	//virtual void GetInfo(TABLEINFO* pti) = 0;

	virtual void Bind(int col, RField* prf) = 0;
	virtual void Bind(int col, char* buf, int len, int* pi, Field::Type type = Field::Char) = 0;

//	virtual void Bind(RKey* prk) = 0;
	//virtual void BindParameter(/*int num, */int len, char* buf, SDWORD* pindicator) = 0;
	virtual void BindParameter(/*int num, */int* par) = 0;
	virtual void BindParameter(RKey* prk) = 0;
	virtual void BindParameter(RSField* prf) = 0;
	virtual void BindParameter(RField* prf) = 0;
	
	virtual bool Read() = 0;
	virtual void Flush() = 0;
	virtual void FlushEx() = 0;
	virtual int  GetIdentity() = 0;
	virtual bool ExecProc(const char* name, int id, int& newid) = 0;
	virtual bool ExecProc(const char * name, int id, char* str, int slen) = 0;

};

#endif // !defined(AFX_DATABASE_H__670D9FD3_DFFF_11D1_A611_204C4F4F5020__INCLUDED_)
