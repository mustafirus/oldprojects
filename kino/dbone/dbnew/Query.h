#pragma once

#include "string.h"
#include "dictionary.h"

class QTable;
class QField
{
	Field*	_pField;
	QTable* _pQTable;
	QField* _next;

	QField* add(QField* pqf);

public:
	typedef list<QTable*> QTables;
	QField(QTable* pqt, Field* pf) :
	  _pQTable(pqt), _pField(pf), _next(NULL){}
	  virtual ~QField(){ delete _next;}
	
	QField* getQField(Field* pf);
	void getQTables(QTables*& lqt);
	string name();
	operator QTable&(){return *_pQTable;};
	operator Field&(){return *_pField;};
	operator Domain*(){ return *_pField; }
};
class QTable
{
	Table*  _pNTable;
	QTable* _pPTable;

	QField* _pQField;
	FKey*	_pFKey;

	QTable* _next;
//	bool	mark;
//	int		style;
	string	_alias;

	QTable* add(QTable* pqf);
	QTable* find(QTable* pqt, FKey* pfk);

public:
	typedef list<QTable*> QTables;
	QTable(Table* pt)
		:_pNTable(pt),
		_pPTable(NULL),
		_pQField(NULL),
		_pFKey(NULL),
		_next(NULL),
		_alias("master")/*,
		mark(false),
		style(0)*/
	{}

	QTable(QTable* parent, FKey* pfk)
		: _pNTable(pfk->getPTable()),
		_pPTable(parent),
		_pQField(NULL),
		_pFKey(pfk),
		_next(NULL)/*,
		mark(false),
		style(pfk->style)*/
	{
		_alias = reinterpret_cast<__int64>(this);
		ASSERT(sizeof(int) == sizeof(this));
		ASSERT(pfk && parent);
	}

	QTable*	join(string& fkey);
	QTable*	join(FKey* pfk);
	QField*	getQField(string& name);
	string	alias(){return _alias;}
	string	name(){return  string () << _pNTable->name() << " as " << alias();}
	string	getNkey_name(int n){return alias() << "." << _pFKey->getPKey()[n]->name();}
	string	getPkey_name(int n){return _pPTable->alias() << "." << (*_pFKey)[n]->name();}
	int		getkeyfield_count(){return _pFKey->field_count();}
					
	void getQTables(QTables*& lqt){
		if(_pPTable) _pPTable->getQTables(lqt);
		QTable* This = this;
		if( !lqt )
			lqt = new QTables(This);
		else
			lqt->add(This);
	}
};

class Query
{
	QTable*	_pQTable;
public:
	Query(Table* pt) : _pQTable(new QTable(pt)) {};
//	Query(void);
	~Query(void){delete _pQTable;};

	operator QTable*&(){ return _pQTable; }
	QField* getQField(string s);
};

inline string QField::name(){ return string() << _pQTable->alias() << "." << _pField->name(); }
inline void QField::getQTables(QTables*& lqt){
	_pQTable->getQTables(lqt);
};
