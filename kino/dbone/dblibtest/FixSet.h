#if !defined(FIXSET_H__INCLUDED_)
#define FIXSET_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

template <class T>
class FixSet
{
	T**	pData;
	int	fields;
	int	max_fields;

public:
	FixSet() : pData(0), fields(0), max_fields(0){};
	~FixSet(){if(pData)delete pData;};
	void SetSize(int n)
	{
		if(n<0)return;
		if(n==0)
		{
			fields = max_fields = 0;
			if(pData)
				delete pData;
			pData = NULL;
			return;
		}
		if(n<fields)
			fields = n;
		T** p=new T*[max_fields = n];
		memset(p, 0, sizeof(T*)*max_fields);
		if(pData)
		{
			memcpy(p,pData,sizeof(T*)*fields);
			delete pData;
		}
		pData = p;
	};
	int GetSize(){return max_fields;};
	int GetCount(){return fields;};
	T* Add(T* pt)
	{
		if(!pt)
			return NULL;
		if(fields == max_fields)
		{
			ASSERT(FALSE);
			return NULL;
		}
		return pData[fields++] = pt;
	};
	T* operator[](int n){return (n>=0 && n<fields)?pData[n] : NULL;};
};
class Field;
typedef FixSet<Field> FieldSet;
#endif // !defined(FIXSET_H__INCLUDED_)
