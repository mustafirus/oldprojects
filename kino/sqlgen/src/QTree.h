/*
 * QTree.h
 *
 *  Created on: 13 марта 2011
 *      Author: golubev
 */

#ifndef QTREE_H_
#define QTREE_H_
using namespace std;

#include "Table.h"

class QTable;

/////////////////////////////////////////////////////////////
class QField
{
	Field*	pField;
	QTable* pQTable;
	QField* next;
public:
	QField(QTable* pTable, Field* pField);
	virtual ~QField();

	QField* add(QField* pqf);
	QField* GetQField(Field* pfld);
};

/////////////////////////////////////////////////////////////
class QTable
{
	Table*  pNTable;
	string	alias;
	QTable* pPTable;
	QField* pQField;
	FKey*	pFKey;
	QTable* next;

	QTable* add(QTable* pqf);
	QTable* find(QTable* pqt, FKey* pfk);
public:
// Construction/Destruction
	explicit QTable(Table* pt) : pNTable(pt), alias("t"), pPTable(NULL),	pQField(NULL), pFKey(NULL), next(NULL)	{
		alias+=aNum();
		printf("alias %s\n", alias.c_str());
	}

	QTable(QTable* pt, FKey* pfk) : pNTable(pfk->pPKey->pTable), alias("t"), pPTable(pt), pQField(NULL), pFKey(pfk), next(NULL)	{
		alias+=aNum();
		printf("alias %s\n", alias.c_str());
	}
	~QTable(){
		if(next) delete next;
	};
	string name(){return pNTable->name();};
////////////////////////////////////////////////////
	QField* getQField(const string& name);


	QTable* getLink(QTable* pqt);

	QTable* join(const string& fkey){
		return join(pNTable->getFKey(fkey));
	}
private:
	QTable* join(FKey* pfk){
		ASSERT(pfk);
		QTable* pqt = find(this, pfk);
		if(pqt)
			return pqt;
		pqt = add(new QTable(this, pfk));
		return pqt;
	}
	static int aliasNum;
	static string aNum(){
		char str[32];
		sprintf(str,"%d", ++aliasNum);
		return str;
	}
};



class QTree {
	typedef list<string> FieldParts;

	QTable* pQTable;

	template<typename Iter>
	QTable* for_each(Iter f, Iter l, QTable* ptab)	{
		for (; f != l; ++f)
			ptab = ptab->join(*f);
		return ptab;
	};
public:
	QTree(Table* pt): pQTable(new QTable(pt)) {}
	~QTree() { if(pQTable) delete pQTable;}
	string name(){return pQTable->name();};

	QField* getQField(const string& str) throw(out_of_range) {
		FieldParts fp =   parse(str.c_str(), '.');
		QTable* last = pQTable;
		string fn = fp.back();fp.pop_back();
		return for_each(fp.begin(),fp.end(),last)->getQField(fn);
	}

	static FieldParts parse(const char* s, char delimeter);
};

class QView : public Props{
	typedef vector<QField*> Fields;
	typedef map<string, QField*> FieldM;

	Fields _fields;
	FieldM _fieldm;

	QTree* pQTree;
public:
	QView(Dict* pd, const char** props);
	void addQField(Props props){
		QField* pq = pQTree->getQField(props.getProp("ref"));
		_fields.push_back(pq);
		_fieldm[props.getProp("name")] = pq;
	}

};

#endif /* QTREE_H_ */
