#pragma once

#include "props.h"
#include "vector.h"


#define PROP_FIELD_NAME_FAKE		PROP_NAME
#define PROP_FIELD_CAPT				(1|PROP_INT)
#define PROP_FIELD_STR				(2|PROP_INT)
#define PROP_FIELD_SIZE				(3|PROP_INT)
#define PROP_FIELD_DISPLAY_SIZE		(4|PROP_INT)
#define PROP_FIELD_MAX				5

#define PROP_KEY_NAME_FAKE			PROP_NAME
#define PROP_KEY_CAPT				(1|PROP_STR)
#define PROP_KEY_EXT				(2|PROP_STR)
#define PROP_KEY_RST				(3|PROP_STR)
#define PROP_KEY_STYLE				(4|PROP_INT)
#define PROP_KEY_FORM				(5|PROP_INT)
#define PROP_KEY_LIST				(6|PROP_INT)
#define PROP_KEY_MAX				7

#define PROP_TABLE_NAME_FAKE		PROP_NAME
#define PROP_TABLE_MAX				1

class Table;
class Domain;
class Field : public props
{
	friend class Dictionary;
	Table*		_pTable;
	Domain*		_pDomain;

	virtual name_space* get_name_space(){return _name_space;};
	static name_space _name_space[];
public:
	Field( Table* p, string name, Domain* pd ) : props(name, PROP_FIELD_MAX), _pTable(p), _pDomain(pd) {};
	~Field(){};
	operator Domain*(){ return _pDomain; }
};
class PKey : public props
{
	friend class Dictionary;


protected:
	typedef vector<Field*>	Fields;
	Table*		_pTable;
	Fields		_vFields;

public:

//	Field& operator[](int n){ASSERT(n >= 0 && n < vFields.size()); return *vFields[n];};
	PKey(Table*	p, string name) : props(name, PROP_KEY_MAX), _pTable(p){};
	~PKey(){};
	operator Table*(){return _pTable;}
	bool exists(Field* p){ return _vFields.exists(p); }
	Field* operator[](int n){ return _vFields[n]; }
	int field_count(){ return _vFields.count(); }

private:
	virtual name_space* get_name_space(){return _name_space;};
	static name_space _name_space[];
};

class FKey : public PKey
{
	friend class Dictionary;
	PKey*		_pPKey;

public:
	FKey(Table*	pt, string& name) : PKey(pt, name), _pPKey(NULL){};
	~FKey(){};
	Table*	getPTable(){ return *_pPKey;}
	PKey& getPKey(){ return *_pPKey; }
};
class DBObj;
class Table : public props
{
	friend class Dictionary;
	typedef vector<Field*>	Fields;
	typedef vector<FKey*>	FKeys;

	typedef DBObj* (*createObj)();

	PKey*	_pPKey;
	FKeys	_vFKeys;
	Fields	_vFields;
	Field*	_pDField;

	virtual name_space* get_name_space(){return _name_space;};
	static name_space _name_space[];
	void set_default(){
		for( int i = 0; i < _vFields.count(); i++ )
		{
			Field* pf = _vFields[i];
			if(_pPKey->exists(pf))
				continue;
			_pDField = pf;
			return;
		}
		_pDField = _vFields[0];
	}
public:
	Table(string name) : props(name, PROP_TABLE_MAX), _pPKey(NULL), _pDField(NULL) {};
	virtual ~Table(){ delete _pPKey; }
	Field* getField(string& name) {
		if(name.empty()){
			if(!_pDField)
				set_default();
			return _pDField;
		}
		return _vFields[name]; 
	}
	FKey* getFKey(string& name) { return _vFKeys[name]; }
	createObj _createObj;
};

class Dictionary
{
	typedef vector<Table*>	Tables;
	typedef std::ostream	ostream;
	Tables	_vTables;

	void toXML(ostream& s, props* v);
	void toXML(ostream& s, Table* v, bool ref = false);
	void toXML(ostream& s, Field* v, bool ref = false);
	void toXML(ostream& s, PKey* v, bool ref = false);
	void toXML(ostream& s, FKey* v, bool ref = false);
	template <class T>
	void toXML(ostream& s, vector<T*>& v, bool ref = false);

public:
	Dictionary();
	~Dictionary();

	Table*	addTable(const char* name);
	Field*	addField(Table* table, string name, Domain* pd);
	PKey*	addPKey(Table* table, int num, const char** fileds);
	FKey*	addFKey(Table* table, string name, string ftable, int num, const char** fields);

	void toXML(ostream& s);
};
