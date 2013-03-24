// RSField.h: interface for the RSField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSFIELD_H__7DD67CB2_F7FF_11D1_A653_204C4F4F5020__INCLUDED_)
#define AFX_RSFIELD_H__7DD67CB2_F7FF_11D1_A653_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "RField.h"
class RecordSet;
class SqlStmt;
class RSWhere;

class RSField : public RField  
{
friend class RSWhere;
	RSWhere*	pRSWhere;
protected:
	char*		dbbuf;
	RSField();
public:
	RSField(QField* pqf, RecordSet* prs, RField* prf);
	~RSField();

//	void Move(int start, int size);
//	void Copy(int start, int bstart, int count);

	operator const char*();
	virtual char*	GetBuf();
	char* GetDBPtr(){return dbbuf;};
//	char* GetBuffer();
	char* GetRow(int num);
	operator RSWhere*();
	bool IsWhere();
	void SetDummy(){state /= s_data; pRec->SetDummy();};
	bool IsDummy(){return state != s_data;};
	bool Load(){state /= s_data; return pRec->Load();};
	void operator=(RSWhere* prsw){pRSWhere = prsw;};
	const char* operator[](int num){return GetRow(num);};
	bool Where(SqlStmt& str, bool bWhere);
};

/*class RSBookmark : protected RSField
{
public:
	RSBookmark(RecordSet* prs);
	~RSBookmark(){};
	int GetLen(){return sizeof(BOOKMARK);};
	char* GetDBPtr(){return RSField::GetDBPtr();};
	SDWORD* GetIndicator(){return RSField::GetIndicator();}
};
*/
#endif // !defined(AFX_RSFIELD_H__7DD67CB2_F7FF_11D1_A653_204C4F4F5020__INCLUDED_)
