#include "StdAfx.h"

#include "Query.h"
#include "Domain.h"
#include "database.h"
#include "rkey.h"
#include "rfield.h"
#include "record.h"

#if 0
Record::Record(void)
{
}

Record::~Record(void)
{
}
#endif// 0

#if 0
Cursor* Record::getCursor(Query* pq){return d.getCursor(pq);};
#endif// 0

void Recordset::load(){
	Cursor* pc = _pCursor; 
	typedef list<QField*> QFieldList;

	list<QField*>* pqfl = NULL;
	_vRFields.tolist(pqfl);

	pc->selectFields(pqfl);
	pc->whereFields(NULL);
	pc->prepare();
	for( int i = 0; i < _vRFields.count(); i++){
		RField* prf = _vRFields[i];
		pc->bind(i+1, *prf);
	}
	pc->exec();
	//pc->BindParameter();
}

RField* Recordset::getRField(string n){
	QField* pqf = _pQuery->getQField(n);
	ASSERT(pqf);
	RField* prf = _vRFields[pqf];
	if(!prf)
		_vRFields.add(prf = new RField( pqf, this ));
	return prf;
}
