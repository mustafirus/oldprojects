#pragma once

class Query;
class Table;
class QField;

class View
{
	typedef vector<string>		Fields;
	Fields	_fields;
	Query*	_pQuery;
	string						form;
	string						list;
public:
	View(Table* pt) : _pQuery(new Query(pt)){};
	virtual ~View(void){delete _pQuery;};
	QField* getQField(string& s){return _pQuery->getQField(s);}
	QField* getQField(const char* s){string s1(s); return getQField(s1);}
};
