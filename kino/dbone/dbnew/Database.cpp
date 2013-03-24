#include "StdAfx.h"
#include "Statement.h"
#include "database.h"

void Cursor::selectFields(QFields* p){
	_pst->selectFields(p);
}
void Cursor::whereFields(QFields* p){
	_pst->whereFields(p);
}
