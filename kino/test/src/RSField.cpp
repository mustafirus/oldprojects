// RSField.cpp: implementation of the RSField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "Dict.h"
#include "RecordSet.h"
#include "QField.h"
#include "DataBase.h"
//#include "StMdfr.h"
//#include "Frame.h"
#include "RSWhere.h"
#include "SqlStmt.h"
#include "RSField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
RSField::RSField(): dbbuf(NULL), pRSWhere(NULL){}

/*RSBookmark::RSBookmark(RecordSet* prs)
{
	pRec	= prs;
	buf		= (char*) new BOOKMARK[prs->GetSize()];
	buf_attrib = b_native;
	dbbuf	= (char*) new BOOKMARK[prs->dbsize];
	pind	= new SDWORD[prs->dbsize];
};
*/
RSField::RSField(QField* pqf, RecordSet* pr, RField* prf): dbbuf(NULL),
pRSWhere(NULL)
{
	pRec	= pr;
	pQField = pqf;
//	pSt		= new RStyle(pQField->GetStyle());
	pPRField= prf;
	buf		= new char[GetLen() * pr->GetSize()];
	buf_attrib = b_native;
	dbbuf	= new char[GetLen() * pr->dbsize];
	pind	= Record::pDB->GetIndicator(pr->dbsize);
//	modified= false;
//	loaded	= false;

/*	if(pQField->GetField()->stmdf)
	{
		char f[30]; int n,m;
		VERIFY(sscanf(pQField->GetField()->stmdf,
			" %[^, ] , %d , %d",f,&m,&n) == 3);
		RFields rf;
		pRec->GetRFields(f, rf, true, pQField->GetQTable());
		ASSERT(rf.GetCount() == 1);
		pStMdfr = new StMdfr(rf[0], m, n);
	}
*/
}

RSField::~RSField()
{
	ifdel(dbbuf);
//	ifdel(pind);
}

RSField::operator const char*()
{
	if(state != s_data)
		if(!pRec->Load())
			return NULL;
	int r = ((RecordSet*)pRec)->GetCurRow();
	if(r<0)
		return NULL;
	return buf + GetLen()*r;
	
}

char* RSField::GetBuf()
{
	state /= s_null;
	int r = ((RecordSet*)pRec)->GetCurRow();
	if(r<0)
		r = 0;
	return buf + GetLen()*r;
};

char* RSField::GetRow(int num)
{
	if(state != s_data)
		if(!pRec->Load())
			return NULL;
	int row = ((RecordSet*)pRec)->GetPageRow(num);
	if(row<0)
		return NULL;
	else
		return buf + GetLen() * row;
}

/*void RSField::Requery()
{
	((RecordSet*)pRec)->Requery();
};
*/

/*bool RSField::IfKey()
{
	if(pPRField)
		return !strcmp(pPRField->GetPtr(),GetPtr());
	else return false;
}
*/


bool RSField::Where(SqlStmt& str, bool bWhere)
{
	if(!pRSWhere)
		return bWhere;
	if(!bWhere)
		str << "\nWHERE ";
	else
		str << " AND ";
	pQField->Select(str);
	pQField->Mark();
	pRSWhere->Where(str);
	return true;
}
/*void RSField::DrawRow(FDC * pDC, int row, CRect r, bool focused)
{
	focused = focused && (row == ((RecordSet*)pRec)->GetPageCur());
	if(pStMdfr)
	{
		pStMdfr->focused = focused;
		pStMdfr->SetData(((RSField*)pStMdfr->GetRField())->GetRow(row));
	}
	pDC->SetStyle(pSt->GetStyle(!focused ? 2 : 3), pStMdfr);
	pDC->Border(r);
	r.DeflateRect(2,1);
	pDC->DrawText(r, GetRow(row));
//	pDC->SetStyle(pSt->GetStyle(focused ? 1 : 0), focused ? NULL : pStMdfr);
}
*/
/*void RSField::Draw(FDC * pDC, CRect r, bool focused)
{

}
*/
/*
void RSField::Copy(int start, int bstart, int count)
{
	int len = GetLen();
	if(((Field*)*pQField)->type == Field::DateTime)
	{
		char* st;
		char* bst;
		DATE_STRUCT d;
		SDWORD* pi	= ((SDWORD*)pind) + bstart;
		for(int i = 0; i < count; i++)
		{
			st	= buf + len * (start+i);
			if(*pi++ == SQL_NULL_DATA)
			{
				*st = 0;
				continue;
			}
			bst	= dbbuf + sizeof(DATE_STRUCT) * (bstart+i);
			d = *((DATE_STRUCT*)bst);
			sprintf(st, "%02d/%02d/%d", d.day, d.month, d.year);
		}
//		type = Field::Char;
		state = s_data;
		return;
	}
	char* st	= buf + len * start;
	char* bst	= dbbuf + len * bstart;
	SDWORD* pi	= ((SDWORD*)pind) + bstart;
	memcpy(st, bst, len * count);
	while(count--)
	{
		if(*pi++ == SQL_NULL_DATA)
			*st=0;
		st+=len;
	}
	state = s_data;
}

void RSField::Move(int start, int count)
{
	memmove(buf, buf + GetLen() * start, GetLen() * count);
}
*/
RSField::operator RSWhere*()
{
	if(!pRSWhere)
	{
	bool bs = ((Field*)*pQField)->type == Field::DateTime ? false : ((Field*)*pQField)->str;
		pRSWhere = new RSWhere(this, bs);
	}
	return pRSWhere;
};

bool RSField::IsWhere()
{
	return pRSWhere && pRSWhere->data;
}
