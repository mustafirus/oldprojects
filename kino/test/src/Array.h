// Array.h: interface for the Array class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARRAY_H__E4774EE2_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
#define AFX_ARRAY_H__E4774EE2_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

template <class T>
class Array
{
	T**	pData;
	int	elems;
	int	max_elems;
	void SetMaxSize(int n);
public:
	Array():pData(0), elems(0), max_elems(0){};
	~Array(){SetMaxSize(0);};
	int GetSize(){return elems;};
	void SetSize(int n){SetMaxSize(n); elems = n;};
	T* operator[](int n)
	{
		if(n < 0)
			return NULL;
		else if(n >= elems)
		{
			if(n >= max_elems)
				SetMaxSize(n + GrowBy);
			elems = n + 1;
		}
		if(pData[n])
			return pData[n];
		else
			return NULL;
	};
	
private:
	static int GrowBy;
};

template <class T>
int Array<T>::GrowBy = 4;

template <class T>
void Array<T>::SetMaxSize(int n)
{
	if(n<0)
		return;
	if(n < elems)
	{
		for(int i = n; i < elems; i++)
		{
			delete pData[i];
		}
		elems = n;
	}
	if(n == 0)
	{
		delete pData;
		pData = NULL;
		return;
	}
	max_elems = n;
	T** p=new T*[max_elems];
	memset(p, 0, sizeof(T*)*max_elems);
	if(pData)
	{
		memcpy(p,pData,sizeof(T*)*elems);
		delete pData;
	}
	pData = p;
};

class Table;
class Field;
class FKey;
typedef Array<Table> Tables;
//typedef Array<Field> Fields;

typedef Array<FKey> FKeys;


#include <vector>
using namespace std;

template <class T>
class vectorautodel : public vector<T>
{
public:
	~vectorautodel(){
		int n = vector<T>::size();
		for (int i = 0; i < n; i++) {
			delete (*this)[i];
		}
	}
};


#endif // !defined(AFX_ARRAY_H__E4774EE2_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
