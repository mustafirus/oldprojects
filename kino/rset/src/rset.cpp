//============================================================================
// Name        : rset.cpp
// Author      : mustafirus
// Version     :
// Copyright   : GPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "stdx.h"

using namespace std;

class Record{
public:
	Data& operator[](int col);
};
class Recordset : public Record{
	bool selectRow(int num);
};

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
