#pragma once

#include "string.h"
#include "Query.h"

class Statement
{
public:
	typedef list<QField*> QFields;
	typedef list<QTable*> QTables;
	
	Statement(Query* pq) : _pQuery(pq), _selectQFields(NULL), _whereQFields(NULL) {};
	virtual ~Statement(void);
	void selectFields(QFields* p){
		if(!_selectQFields)
			_selectQFields = p;
		else
			_selectQFields->add(p);
	}
	void whereFields(QFields* p){
		if(!_whereQFields)
			_whereQFields = p;
		else
			_whereQFields->add(p);
	}

protected:
	Query* _pQuery;
	QFields* _selectQFields;
	QFields* _whereQFields;
};

class SQLStatement : public Statement{

public:
	SQLStatement(Query* pq) : Statement(pq){};
	virtual ~SQLStatement(void){};

	string buildSelect(){
		string sel, from, sql;
		QTables* plqt = NULL;
		QFields* next = _selectQFields;
		(*next)->getQTables(plqt);
		sel << ((*next))->name();
		while (next = next->next()){
			(*next)->getQTables(plqt);
			sel << ", " << ((*next))->name();
		}
		QTables* next_t = plqt;
		from << ((*next_t))->name();

		while (next_t = next_t->next()){
			QTable* pqt = *next_t;
			from << " JOIN " << pqt->name();
			for(int i = 0; i < pqt->getkeyfield_count(); i++)
			{
				from << (i ? " AND " : " ON ") << pqt->getNkey_name(i) <<
					" = " << pqt->getPkey_name(i);
			}
		}
		sql << "SELECT " << sel << " FROM " << from;
		if (_whereQFields)
			sql << " WHERE " << "";
		printf("%s\n", (const char*)sql);
		return sql;
	}
};