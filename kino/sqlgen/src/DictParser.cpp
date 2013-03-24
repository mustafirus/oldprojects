/*
 * DictParser.cpp
 *
 *  Created on: 13 марта 2011
 *      Author: golubev
 */

#include "stdx.h"
#include "DictParser.h"

DictParser::elmap DictParser::_elmap[] = {
		{"db", EL_DB},
		{"tables",EL_TABLES},
		{"table",EL_TABLE},
		{"field",EL_FIELD},
		{"pkey",EL_PK},
		{"id",EL_PK},
		{"fkey",EL_FK},
		{"views",EL_VIEWS},
		{"view",EL_VIEW},
		{"qfield",EL_QFIELD}
};

DictParser::DictParser() : pTable(NULL), pPKey(NULL), pFKey(NULL) {
		int n = sizeof(_elmap)/sizeof(_elmap[0]);
		for (int i = 0; i < n; ++i) {
			mp[_elmap[i].s] = _elmap[i].i;
		}

}
