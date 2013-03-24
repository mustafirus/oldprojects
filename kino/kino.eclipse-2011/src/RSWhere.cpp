// RSWhere.cpp: implementation of the RSWhere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Dict.h"
#include "SqlStmt.h"
#include "RSField.h"
#include "RSWhere.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif //_DEBUG

#ifdef ifdel
#undef ifdel
#endif
#define ifdel(D)	if(D) {delete D; D=NULL;}

#define sdup(D,S)	D = new char[strlen(S)+1];\
					strcpy(D, S);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//IMPLEMENT_DICT_MEMORY(RSWhere)
RSWhere::~RSWhere()
{
	pRField->pRSWhere = NULL;
	delete data;
	delete op1;
	if(op2) delete op2;
};

void RSWhere::Set(const char* str)
{
	if(!str || !*str)
	{
		pRField->SetDummy();
		delete this;
		return;
	}
	if(data && !strcmp(str, data))
		return;
	int l = strlen(str)+1;
	if(l > len)
		Grow(l);
	strcpy(data, str);
	if(!pRField->Load())
		delete this;
}

void RSWhere::Grow(int l)
{
	if(data)
		delete data;
	data = new char[len = l];
}
void RSWhere::Where(SqlStmt& str)
{
	if(!data)
		return;
	ifdel(op1);
	ifdel(op2);
	char* ss = data;
	if(bstr)
	{
		if(*ss == '!')
		{
			str << " NOT ";
			ss++;
		}
		if(strchr(ss, '~'))
		{
			op1 = new char[strlen(ss)+1];
			op2 = new char[strlen(ss)+1];
			sscanf(ss, "%[^~]~%s", op1, op2);
			str << " BETWEEN ? AND ? ";
		}else if(strcspn(ss, "%_[^]/") < strlen(ss))
		{
			str << " LIKE ?";
			sdup(op1, ss);
		}else
		{
			str << " LIKE ?";
			int len = strlen(ss);
			op1 = new char[len+2];
			strcpy(op1, ss);
			strcat(op1, "%");
		}
	}else
	{
		if(strchr(ss, '~'))
		{
			if(*ss == '!')
			{
				str << " NOT ";
				ss++;
			}
			op1 = new char[strlen(ss)+1];
			op2 = new char[strlen(ss)+1];
			sscanf(ss, "%[^~]~%s", op1, op2);
			str << " BETWEEN ? AND ? ";
		}else
		{
			switch(*ss)
			{
			case '>':
				str << " > ?";
				ss++;
				break;
			case '<':
				str << " < ?";
				ss++;
				break;
			case '=':
				str << " = ?";
				ss++;
				break;
			case '!':
				str << " <> ?";
				ss++;
				break;
			default:
				str << " = ?";
			}
			sdup(op1, ss);
		}
	}
}
