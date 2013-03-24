#pragma once
#include "vector.h"
class QTable;
class RField;
class Query;
class QField;
class Record;

class RKey
{
	typedef vector<RField*> RFields;
	typedef vector<QField*> QFields;

	QTable*		pQTable;	//Src
	RFields		pRFields;	// Src
	Query*		pQuery;		//Trg
	QFields		pQFields;	//Trg
public:
	RKey(QTable* pt, Record* pr);
	virtual ~RKey();
};
