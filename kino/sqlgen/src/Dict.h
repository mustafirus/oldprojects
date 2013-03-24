/*
 * Dict.h
 *
 *  Created on: 16 марта 2011
 *      Author: golubev
 */

#ifndef DICT_H_
#define DICT_H_

#include "Table.h"
#include "QTree.h"

class Dict : public Props{
	typedef vectorautodel<Table*> Tables;
	typedef map<string, Table*> TableM;
	typedef vectorautodel<QTree*> QTrees;
	typedef map<string, QTree*> QTreeM;
	typedef vectorautodel<QView*> QViews;
	typedef map<string, QView*> QViewM;

	Tables _tables;
	TableM _tablem;
	QTrees _qtrees;
	QTreeM _qtreem;
	QViews _qviews;
	QViewM _qviewm;
public:
	Dict(const char** props) : Props(props) {}
	void addTable(Table* pt){
		_tables.push_back(pt);
		_tablem[pt->name()] = pt;
	}
	Table* getTable(const string& s){
		return _tablem.at(s);
	}

	QTree* getQTree(const string& s){
		QTree* pq = _qtreem[s];
		if(pq) return pq;
		pq = new QTree(getTable(s));
		_qtrees.push_back(pq);
		_qtreem[pq->name()] = pq;
		return pq;
	}

	void addQView(QView* pq){
		_qviews.push_back(pq);
		_qviewm[pq->name()] = pq;
	}

	string toString(){
		string str;
		str += "<db " + Props::toString() + ">\n";

		for( size_t i=0; i < _tables.size(); i++){
			str +=_tables[i]->toString();
		}
		str += "</db>\n";
		return str;
	}
	void bindKeys(){
		size_t n = _tables.size();
		for (size_t i = 0; i < n; ++i) {
			_tables[i]->bindKeys(this);
		}
	}
};

#endif /* DICT_H_ */
