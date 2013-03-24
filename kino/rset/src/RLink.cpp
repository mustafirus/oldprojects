// RLink.cpp: implementation of the RLink class.
//
//////////////////////////////////////////////////////////////////////
#if 0

#include "stdx.h"
#include "Dict.h"
#include "DbCursor.h"
#include "DataBase.h"
#include "Record.h"
#include "SqlStmt.h"
#include "RKey.h"
#include "RLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
QRestr* RLink::CreateQRestr(Query* pQuery, QRestr* pHead)
{
	if(psrckey->IsPrimary())
		return NULL;
	return QRestr::Create(pQuery, psrckey, pHead, psrcrec);
};

void RLink::Select(SqlStmt& str, bool bWhere)
{
	if(ptrgkey->IsPrimary())
		return;
	if(bWhere)
		str << " AND ";
	else
		str << " WHERE ";
	ptrgkey->SrcSelect(str);
}

void RLink::Bind(DbCursor* pCur)
{
	if(!ptrgkey->IsPrimary())
		pCur->BindParameter(psrckey);
}

void RLink::Bind(DataBase* pDB)
{
	if(!ptrgkey->IsPrimary())
		pDB->BindParameter(psrckey);
}

bool RLink::ReturnToSrc()
{
	if(!psrckey->IsPrimary() && ptrgkey->IsPrimary())
	{
		*psrckey = *ptrgkey;
		psrcrec->Refresh(psrckey);
		return true;
	}
	return false;
}

/*QRestr*	RLink::GetSrcQRestr()
{
	return *psrcrec;
}
*/
bool RLink::SetDefaults(Record* pr)
{
	ASSERT(psrcrec);
	QRestr* pqr = *psrcrec;
	return pqr ? pqr->SetDefaults(pr) : false;
}
#endif
