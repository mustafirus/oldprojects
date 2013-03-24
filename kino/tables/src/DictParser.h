/*
 * DictParser.h
 *
 *  Created on: 13 марта 2011
 *      Author: golubev
 */

#ifndef DICTPARSER_H_
#define DICTPARSER_H_

#include "ExpatImpl.h"
#include "Dict.h"

#define EL_DB		1
#define EL_TABLES	2
#define EL_TABLE	3
#define EL_FIELD	4
#define EL_PK		5
#define EL_FK		6
#define EL_VIEWS	7
#define EL_VIEW		8
#define EL_QFIELD	9



class DictParser: public CExpatImpl<DictParser> {
	struct elmap{
		const char* s;
		int			i;
	};

	static elmap _elmap[];
public:
	map<string,int> mp;
	Table* pTable;
	PKey* pPKey;
	FKey* pFKey;
	Dict* pDict;
	QView* pQView;
	DictParser();
	virtual ~DictParser(){

	}
	// Invoked by CExpatImpl after the parser is created
	void OnPostCreate() {
		EnableStartElementHandler();
		EnableEndElementHandler();
	}

	void OnStartElement(const XML_Char *pszName, const XML_Char **papszAttrs) {
		int n = mp[pszName];
		switch (n) {
			case EL_DB:
				pDict = new Dict(papszAttrs);
				break;
			case EL_TABLE:
				pTable = new Table(papszAttrs);
				assert(pDict);
				pDict->addTable(pTable);
				break;
			case EL_PK:
				pPKey = new PKey(pTable, papszAttrs);
				assert(pTable);
				pTable->addPKey(pPKey);
				break;
			case EL_FK:
				pFKey = new FKey(pTable, papszAttrs);
				assert(pTable);
				pTable->addFKey(pFKey);
				break;
			case EL_FIELD:{
				assert(pTable);
				Field* pField = new Field(papszAttrs);
				pTable->addField(pField);
				if (pPKey)
					pPKey->addField(pField);
				if (pFKey)
					pFKey->addField(pField);
			}
				break;
			case EL_VIEW:
				assert(pDict);
				pQView = new QView(pDict, papszAttrs);
				break;
			case EL_QFIELD:{
				assert(pQView);
//				Props p(papszAttrs);
				pQView->addQField(Props(papszAttrs));
			}
				break;
			default:
				break;
		}
		printf("We got a start element %s - %d\n", pszName, n);
		return;
	}

	void OnEndElement(const XML_Char *pszName) {
		int n = mp[pszName];
		switch (n) {
			case EL_TABLES:
				pDict->bindKeys();
				break;
			case EL_TABLE:
				pTable = NULL;
				break;
			case EL_PK:
				pPKey = NULL;
				break;
			case EL_FK:
				pFKey = NULL;
				break;
			default:
				break;
		}
		printf("We got an end element %s\n", pszName);
		return;
	}

};

#endif /* DICTPARSER_H_ */
