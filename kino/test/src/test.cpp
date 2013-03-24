//============================================================================
// Name        : test.cpp
// Author      : wdfsdf
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "stdx.h"
#include <iostream>
#include <map>
#include "ExpatImpl.h"
#include "Map.h"
#include "Field.h"
#include "Table.h"
using namespace std;



#define EL_DB		1
#define EL_TABLE	2
#define EL_FK		3
#define EL_FIELD	4




class CMyXML: public CExpatImpl<CMyXML> {
public:
	stringMap<int, 10, noDel<int> > mp;
	Table* pTable;
	PKey* pPKey;

	CMyXML() : pTable(NULL) {
		mp.setAt("db",EL_DB);
		mp.setAt("table",EL_TABLE);
		mp.setAt("fkey",EL_FK);
		mp.setAt("field",EL_FIELD);
	}

	// Invoked by CExpatImpl after the parser is created
	void OnPostCreate() {
		// Enable all the event routines we want

		EnableStartElementHandler();
		EnableEndElementHandler();
		// Note: EnableElementHandler will do both start and end

		EnableCharacterDataHandler();
	}

	// Start element handler
	void OnStartElement(const XML_Char *pszName, const XML_Char **papszAttrs) {
		int n = mp[pszName];
		switch (n) {
			case EL_TABLE:{
				Table::props p;
				for(int i=0; papszAttrs[i]; i+=2){
					p[papszAttrs[i]] = papszAttrs[i+1];

				}
				pTable = new Table(p);
			}
				break;
			case EL_FIELD:{
				Field::props p;
				for(int i=0; papszAttrs[i]; i+=2){
					p[papszAttrs[i]] = papszAttrs[i+1];

				}
				Field* ppf = new Field(p);
			}
				break;
			case EL_DB:
				break;
			case EL_FK:
				break;
			default:
				break;
		}
		printf("We got a start element %s - %d\n", pszName, n);
		return;
	}

	// End element handler
	void OnEndElement(const XML_Char *pszName) {
		printf("We got an end element %s\n", pszName);
		return;
	}

	// Character data handler
	void OnCharacterData(const XML_Char *pszData, int nLength) {
		// note, pszData is NOT null terminated

		printf("We got %d bytes of data\n", nLength);
		return;
	}
};
class testel{
	int i;
public:
	testel(){
		i = 1;
		printf("constructor %d\n", this);
	}
	~testel(){
		printf("destructor %d\n", this);
		i = 0;
	}
};

int main() {
	printf("vector\n");
	vector<testel> tt;
	printf("push\n");
	testel tal;
	tt.push_back(tal);
	printf("ref\n");
	testel& tal1 = tt[0];
	printf("resize0\n");
	tt.resize(0);
	printf("return\n");
	return 0;


	/*string k1 = "k1";
	string k2 = "k2";
	string a = "a";
	string b = "b";
	props p;
	p["k1"]=a;
	p["k2"]=b;
	props p1 = p;
	const char* a1 = p1[k1].c_str();
	const char* a2 = p1["k1"].c_str();
	const char* b1 = p1[k2].c_str();
	const char* b2 = p1["k2"].c_str();
	return 0;
*/

	CMyXML sParser;
	sParser .Create ();

	FILE *fp = fopen ("src/kino.xml", "r");
	char k[1024];
	int l = fread (k, 1, 1024, fp); // READ
	fclose (fp);

	return sParser .Parse (k);

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
