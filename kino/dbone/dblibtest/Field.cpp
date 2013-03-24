// Field.cpp: implementation of the Field class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Dict.h"
#include "Field.h"

IMPLEMENT_DICT_MEMORY(Field)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Field::Field() : name(NULL), capt(NULL), ro(false), str(0), strlen(0),
scrlen(0), type(Unknown), style(0), stmdf(NULL), deft(NULL){}

Field::~Field()
{
	ifdel((char*)name);
	ifdel((char*)capt);
	ifdel((char*)stmdf);
	ifdel((char*)deft);
};

const char* sdup(unsigned char *const str, int n)
{
	if(!str)
		return NULL;
	char* s1 = new char[n+1];
	strncpy(s1, (char*)str, n);
	s1[n]=0;
	return s1;
}
