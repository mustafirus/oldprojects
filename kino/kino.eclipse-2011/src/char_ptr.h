// char_ptr.h: interface for the char_ptr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAR_PTR_H__E4774EE0_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
#define AFX_CHAR_PTR_H__E4774EE0_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class char_ptr  
{
	char* data;
	char** pdata;
public:
	char_ptr() : data(NULL), pdata(NULL){};
	char_ptr(char_ptr& p) : data(p.data), pdata(&p.data){};
	virtual ~char_ptr(){if(pdata)*pdata = data; else if(data){delete data;}};
	char* operator=(char* str){if(data)delete data; return data = str;};
	operator char*(){return data;}
};

#endif // !defined(AFX_CHAR_PTR_H__E4774EE0_34D4_11D2_A6FF_204C4F4F5020__INCLUDED_)
