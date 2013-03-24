// DbExceptio.h: interface for the DbExceptio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBEXCEPTION_H__9CBB6852_13E2_11D2_A6A6_204C4F4F5020__INCLUDED_)
#define AFX_DBEXCEPTION_H__9CBB6852_13E2_11D2_A6A6_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "ErrorCodes.h"
#include "Log.h"
#include "Exception.h"

class DbException : public Exception  
{
public:
	DbException(){};
	virtual ~DbException(){};
	static DbException dbExcept;
};
#endif // !defined(AFX_DBEXCEPTION_H__9CBB6852_13E2_11D2_A6A6_204C4F4F5020__INCLUDED_)
