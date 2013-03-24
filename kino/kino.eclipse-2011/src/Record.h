// Record.h: interface for the Record class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORD_H__2A28BD62_F622_11D1_A64B_204C4F4F5020__INCLUDED_)
#define AFX_RECORD_H__2A28BD62_F622_11D1_A64B_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class Field;
class RKey;
class Query;
class QTable;
class QField;
class DataBase;
class Dict;
class QRestr;
class DbStmt;
class RLink;

#include "Set.h"
#include "List.h"

class Breaker
{
	char* pos;
	char  ch;
public:
	Breaker(char* str, char brk) : pos(NULL), ch(brk)
	{
		if(str && (pos = strchr(str, brk)))
			*pos = 0;
	};
	operator bool(){return (pos && *(pos + 1));};
	operator char*()
	{
		return (pos && *(pos + 1)) ? pos + 1 : NULL;
	}; 
	~Breaker()
	{
		if(pos)
			*pos = ch;
	};
};

class Record  
{
public:
enum Flag{s_dummy = 1, s_blank = 2, s_modified = 4, s_loading = 8, s_prepare = 16};
class STATE
{
	char data;
public:
	STATE(){data = s_dummy|s_prepare;};
	bool operator==(Flag f){return (data&f) > 0;}
	bool operator!=(Flag f){return !(data&f);}
	void operator=(Flag f){data |=f;}
	void operator/=(Flag f){data &= ~f;}
};	
protected:

	Query*		pQuery;
	RFields		pRFields;
	RKey*		pPRKey;
	DbStmt*		pDbStmt;
	RKeys		pRKeys;
	QRestr*		pQRestr;
	LinkList*	pSrcLink;
	RLink*		pTrgLink;
	STATE		state;
//	enum{s_dummy, s_new, s_data, s_modified, s_modified_dummy} state;
//	QRestr*		pQRestr;
//	Records		pChild;
//	bool Save(QTable* pqt);
	virtual RField* CreateRField(QField* pqf, RField* parent = NULL);
	virtual bool Insert();
	virtual bool Update();
protected:
	Record();
	virtual void AssistLoad();
	bool		LoadDefaults();
public:
	Record(RKey* rkey);
	virtual ~Record();

	operator Query*(){return pQuery;};
	operator QRestr*(){return pQRestr;};

	virtual void New();
	virtual bool Load();
	virtual bool Save();
	virtual bool Refresh(RKey* prk);
	
	char*	GetRFields(char* str, RFields& rf, QTable* pqt = NULL);
	void	GetRFields(QFields& qf, RFields& rf);
	RField*	GetRField(QField* pqf);
	RKey*	GetRKey(QTable* pqt = NULL);
	RLink*	GetLink(RKey* prk);
	RLink*	GetLink(char* str);
	virtual void SetLink(RLink* prl, RKey* prk);
	Record* GetLinkedRecord(char* str);
	operator RFields&(){return pRFields;};
			void SetModified();
			bool IsModified(){return state == s_modified;};
			void SetDummy(){state = s_dummy;};
			bool IsDummy(){return state == s_dummy;};
//Clipboard support
	virtual	bool CanCopy();
	virtual	bool CanPaste(HWND hWnd);
	virtual	void Copy(HWND hWnd);
	virtual	void Paste(HWND hWnd);
	virtual	void Undo();

	static DataBase* pDB;
	static Dict* pDict;

private:
	RField*	Find(QField* pqf);
	void Find(QFields& qf, RFields& rf);
public:
	static UINT	CF_RKEY;
//	void FreeLink(RLink* prl){pTrgLink = NULL;}
//	RField& operator[](int n){return *(pRFields[n]);};
//	Record(char* pkey);

/*	void ChildRecord(Record* pr, bool set = true);
	bool		modified;
*/
//	void GetKeyFields(QTable* pqt, RFields& rf);

//	RField*	GetRField(Field* pf, RField* parent = NULL);
//	RField*	GetRField(QField* pqf, RField* parent = NULL);

//	QTable* GetLinkTable(Query* pq);

//	virtual bool Load(int max_fields = 14);
//			bool LoadEx(RField** prf, int fn);
//	virtual bool Save();
//			bool Insert();
//	virtual void Delete();
//			void Empty();
//			void New();
//			void GetDefaults();

//	virtual bool Refresh(RKey* prk);
//	virtual void Requery();
};

#endif // !defined(AFX_RECORD_H__2A28BD62_F622_11D1_A64B_204C4F4F5020__INCLUDED_)
