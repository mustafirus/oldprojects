// DataBase.h: interface for the DataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASE_H__670D9FD3_DFFF_11D1_A611_204C4F4F5020__INCLUDED_)
#define AFX_DATABASE_H__670D9FD3_DFFF_11D1_A611_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class RData{
	enum { _text, _int, _double, _blob, _null } _type;
	union{
		char*	s;
		long long	i;
		double	d;
		void*	b;
	};
	int		l;
};

class RField;
class DataBase  
{

public:
	DataBase(){};
	virtual ~DataBase(){};

	virtual void Prepare(const char* str) = 0;
	virtual void Exec() = 0;
	virtual void Flush() = 0;
	virtual void GetData(int icol, int& data) = 0;
	virtual void GetData(int icol, char& data) = 0;
	virtual void GetData(int icol, bool& b) = 0;
	virtual void GetData(int icol, char* data, int num) = 0;
	virtual void GetData(RFields rf){

	};

	virtual void BindParameter(RKey* prk) = 0;
	virtual void BindParameter(RSField* prf) = 0;
	virtual void BindParameter(RField* prf) = 0;
};

#endif // !defined(AFX_DATABASE_H__670D9FD3_DFFF_11D1_A611_204C4F4F5020__INCLUDED_)
