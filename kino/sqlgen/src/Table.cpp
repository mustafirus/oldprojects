/*
 * Tables.cpp
 *
 *  Created on: 13 марта 2011
 *      Author: golubev
 */
#include "stdx.h"
#include "Dict.h"
void FKey::bindKeys(Dict* pd){
	pPKey = pd->getTable(getProp("table"))->getPKey();
}
