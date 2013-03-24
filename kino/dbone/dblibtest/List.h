// List.h: interface for the List class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIST_H__57B7C3B2_3DE2_11D2_A718_204C4F4F5020__INCLUDED_)
#define AFX_LIST_H__57B7C3B2_3DE2_11D2_A718_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class BaseList
{
	BaseList**	pprev;
	BaseList*	next;

public:
	BaseList(BaseList*& pHead) :
	  next(pHead), pprev(&pHead)
	{
		if(pHead)
			pHead->pprev = &next;
		pHead = this;
//		*pprev = this;
	};
	virtual ~BaseList(){
		*pprev = next; 
		if(next) next->pprev = pprev;
	};
	operator BaseList*(){return next;};
};

template <class T>
class List : public BaseList
{
	T*		data;
public:
	List(T* pData, List*& pHead) : BaseList((BaseList*&)pHead), data(pData){};
	~List(){delete data;};
	operator List*(){return (List*)operator BaseList*();};
	operator T*(){return data;}
	T* operator->(){return data;};
};
class RLink;
typedef List<RLink> LinkList;


#endif // !defined(AFX_LIST_H__57B7C3B2_3DE2_11D2_A718_204C4F4F5020__INCLUDED_)
