// RSWhere.h: interface for the RSWhere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RSWHERE_H__04652EC2_3C09_11D2_A715_204C4F4F5020__INCLUDED_)
#define AFX_RSWHERE_H__04652EC2_3C09_11D2_A715_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class SqlStmt;

class RSWhere  
{
friend class RSField;
	int len;
	char* data;
	bool bstr;
	char* op1;
	char* op2;
	RSField* pRField;
	void Grow(int l);
public:
	RSWhere(RSField* prf, bool bs) : len(0), data(NULL), bstr(bs),
		op1(NULL), op2(NULL), pRField(prf) {};
	virtual ~RSWhere();
	void Set(const char* str);

	char* Op1(){return op1;};
	char* Op2(){return op2;};
	operator const char*(){return data;};
//	char* GetBuffer(int l){if(l > len)Grow(l);return data;};
//	char* ReleaseBuffer(int l){if(l > len)Grow(l);return data;};
	void Where(SqlStmt& str);
};

#endif // !defined(AFX_RSWHERE_H__04652EC2_3C09_11D2_A715_204C4F4F5020__INCLUDED_)
