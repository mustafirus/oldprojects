#if !defined(SET_H__INCLUDED_)
#define SET_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

template <class T>
class Set
{
	T**	pData;
	int	fields;
	int	max_fields;

public:
	Set():pData(0), fields(0), max_fields(0){};
	~Set(){if(pData)delete pData;};
	T* operator[](int n){return (n>=0 && n<fields)?pData[n] : NULL;};
	int GetCount(){return fields;};
	T* Add(T* pt);
	void SetAt(int n, T* pt){if(n>=0 && n<fields)pData[n]=pt;};
	operator T**(){return pData;}
	void Remove(T* pt);

private:
static int GrowBy;
};
template <class T>
int Set<T>::GrowBy = 4;

template <class T>
T* Set<T>::Add(T* prf)
{
	if(!prf)
		return NULL;
	if(fields == max_fields)
	{
		T** p=new T*[max_fields+=GrowBy];
		memset(p, 0, sizeof(T*)*max_fields);
		if(pData)
		{
			memcpy(p,pData,sizeof(T*)*fields);
			delete pData;
		}
		pData = p;
	}
	return pData[fields++] = prf;
}
template <class T>
void Set<T>::Remove(T* pt)
{
	if(!pt)
		return;
	for(int i = 0; i<fields;i++)
	{
		if(pData[i] == pt)
		{
			if(fields != i+1)
				memmove(pData[i], pData[i+1], sizeof(T*)*(fields-i));
			fields--;
			break;
		}
	}
}
class RField;
class QField;
class RKey;
class Record;
typedef Set<RField> RFields;
typedef Set<QField> QFields;
typedef Set<RKey>	RKeys;
typedef Set<Record> Records;
typedef Set<char> Strs;
#endif // !defined(SET_H__INCLUDED_)
