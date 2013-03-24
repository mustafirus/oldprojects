//============================================================================
// Name        : sqlgen.cpp
// Author      : mustafirus
// Version     :
// Copyright   : GPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "stdx.h"
#include "DictParser.h"
#include "Table.h"
#include "QTree.h"
using namespace std;



int main() {


	DictParser sParser;
	sParser .Create ();

	FILE *fp = fopen ("src/kino.xml", "r");
	char k[4096];
	int l = fread (k, 1, 4096, fp); // READ
	fclose (fp);
	l=0;

	sParser .Parse (k);
	string s = sParser.pDict->toString();
	printf("%s", s.c_str());
/*
	Table* pt = sParser.pDict->getTable("dog");
	QTree qt(pt);
	QField* pqf = NULL;
	pqf = qt.getQField("name");
	pqf = qt.getQField("father.name");
	pqf = qt.getQField("father.owner.name");
	pqf = qt.getQField("father.owner.city.name");
*/

	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}

