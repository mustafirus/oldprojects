// dbnew.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Dictionary.h"
#include "Statement.h"

#include "Query.h"
#include "Domain.h"

#include "rkey.h"
#include "rfield.h"
#include "database.h"
#include "record.h"

#include "view.h"

#include "odbcdatabase.h"

template<class T>
class smartptr{
	T*	_pT;
public:
	virtual T* operator->(){_pT->Load();return _pT; }
};


class City{
public:
	void Load(){};
	Var		name;
};
class Customer{
	typedef smartptr<City> CityPtr;
public:
	Customer(){};
	~Customer(){};
//	Var			name;
	CityPtr		city;
};

OdbcDatabase database("newdb", "mysql", "cdvfbg1q");
Cursor* Record::getCursor(Query* pq){return database.getCursor(pq);};

DomainInt		di; 
DomainString	ds20(20);

void test(){

	database.setCType(&di, SQL_C_LONG);
	database.setCType(&ds20, SQL_C_CHAR);
	database.setSType(&di, SQL_INTEGER);
	database.setSType(&ds20, SQL_CHAR);
	database.setTypeString(&di, "int");
	database.setTypeString(&ds20, "char(20)");


	Dictionary d;
	Table* tab;
	Field* fld;
	PKey*  pk;
	FKey*  fk;

	tab = d.addTable("City");
	fld = d.addField(tab, "id", &di);
	fld = d.addField(tab, "name", &ds20);
	const char* s1[] = {"id"};
	pk	= d.addPKey(tab, 1, s1);

	tab = d.addTable("Customer");
	fld = d.addField(tab, "id", &di);
	fld = d.addField(tab, "name", &ds20);
	fld = d.addField(tab, "city_id", &di);
	const char* s2[] = {"id"};
	pk	= d.addPKey(tab, 1, s2);
	
	const char* s3[] = {"city_id"};
	fk	= d.addFKey(tab, "city", "City", 1, s3);
	
	Query q(tab);
#if 0
	;
	QField* pqf = q.getQField("name");
	Statement::QFields ql(pqf);
	pqf = q.getQField("city.fuck");
	pqf && ql.add(pqf);
	pqf = q.getQField("city.name");
	pqf && ql.add(pqf);

	SQLStatement sql(&q);
	sql.selectFields(&ql);
	sql.buildSelect();
#endif

	try{
	Recordset r(&q);
	r.getRField("name");
	r.getRField("city.name");
	r.load();
	r.first();
	r.reload();
	r.next();
	r.next();
	r.next();
	r.next();
	r.prev();
	r.last();
	r.prev();
	r.prev();
	}catch(Database::Exception& e){
		string str = e.tostring();
		printf("%s", (const char*)str);
	}
return;
	Customer* cu = new Customer;
	string city;
	city = cu->city->name;
	
	
	

//	DBObj* pDBObj = tab->_createObj();
	
//	std::ofstream of("C:\\fuck");
//	d.toXML(of);


}
#if 0
#include "vector.h"
#include "string.h"
#include "dictionary.h"

void test1(){
	vector<string> sv;
	vector<Field*> sf;
	sv.add("aaa");
	Field* p;
	p = sf.add(p);
	p = sf["sdf"];
	sf.delete_all();
}
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	test();
	return 0;
}

