#pragma once
#include "Domain.h"

class Cursor;
class Query;
class Database
{
protected:
	Database(){}
	virtual ~Database(){}

public:
	class Exception{
	public:
		Exception(){};
		virtual ~Exception(){};
		virtual string tostring() = 0;
	};
	virtual Cursor* getCursor(Query* pq)=0;
	virtual string getTypeString(Domain* p) = 0;
};

class Statement;
class QField;
class Cursor{
protected:
	Database*	_pdb;
	Statement*	_pst;
	enum { _none, _eof, _bof } _pos;
public:
	enum {first = 2000000000, last = -2000000000};
	typedef list<QField*> QFields;

	Cursor(Database* pdb, Statement* pst) : _pdb(pdb), _pst(pst), _pos(_bof){};
	virtual ~Cursor(){};
	void selectFields(QFields* p);
	void whereFields(QFields* p);

	virtual void prepare() = 0;
	virtual void bind(int col, Var prf) = 0;
	virtual void parameter(int num, Var& v) = 0;
	virtual void exec() = 0;
	virtual void fetch(int n, bool absolute = false) = 0;

	bool bof(){ return _pos == _bof; }
	bool eof(){ return _pos == _eof; }
};
