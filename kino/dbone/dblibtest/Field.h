// Field.h: interface for the Field class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIELD_H__1ACBEBB1_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_)
#define AFX_FIELD_H__1ACBEBB1_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Array.h"

class Field  
{
public:
	enum Type{Unknown, Char, TinyInt, SmallInt, Integer, Date, Time, DateTime};

	const char* name;
	const char* capt;
	bool	ro;
	bool	str;
	int		strlen;
	int		scrlen;
	int		style;
	int		fnum;
	const char* stmdf;
	const char*	deft;
	Type	type;

	Field();
	~Field();
DECLARE_DICT_MEMORY
};
const char* sdup(unsigned char *const str, int n);
#endif // !defined(AFX_FIELD_H__1ACBEBB1_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_)
