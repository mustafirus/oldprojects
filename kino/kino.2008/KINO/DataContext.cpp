// DataContext.cpp: implementation of the DataContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "dblib.h"
#include "Frame.h"
#include "ErrorCodes.h"
#include "Exception.h"
#include "Application.h"
#include "Kinores.h"
#include "DataContext.h"

#define GetApp() Application::GetApp()
#define MAXACCELS	10
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void DataContext::Create(const char* table)
{
	ASSERT(_isFree());

	pQuery = RecordSet::pDict->GetQuery(table);
	ASSERT(pQuery);

	((char*)szForm) = RecordSet::pDict->GetForm(pQuery, 2);
	if(!szForm)
	{
		Exception::ex.printf(MSG_ERROR_FORM_NOTFOUND, ((Table*)*(QTable*)*pQuery)->name,
			((Table*)*(QTable*)*pQuery)->mPKey.name);
		throw &Exception::ex;
	}

	Fun(*pQuery, 1);
//	((char*)szFrameName) = (char*)((QTable*)*pQuery)->GetName(1);
}

void DataContext::Create(RKey* prk, RLink* prl, UINT nID)
{
	ASSERT(_isFree());
	pRKey = prk;
	pRLink = prl;
	((char*)szForm) = RecordSet::pDict->GetForm(prk, nID);
	Fun(*prk, nID);
//	((char*)szFrameName) = (char*)((QTable*)*prk)->GetName(nID-1);
}

void DataContext::FreeContext()
{
	pQuery = NULL;
	if(pRLink)
	{
		ASSERT(pRec);
		delete pRLink;
		pRLink = NULL;
	}
	pRec = NULL;
	pRKey = NULL;
	rt = none;
	ifdel(szForm);
	(char*)szFrameName = NULL;
	(char*)szForm = NULL;
	pWnd = NULL;
	bAutoSave = false;
}

Record*	DataContext::GetRecord(bool create /*= true*/)
{
	if(!create)
		if(rt == rec)
			return pRec;
		else
			return NULL;
	ASSERT(rt != rset && pRKey);
	rt = rec;
	if(!pRec)
	{
		pRec = new Record(pRKey);
		if(pRLink)
		{
			pRec->SetLink(pRLink, pRKey);
			pRLink = NULL;
		}
	}
	return pRec;
}

RecordSet* DataContext::GetRecordSet()
{
	ASSERT(rt != rec);
	rt = rset;
	if(pRec)
		return (RecordSet*)pRec;
	if(pQuery)
		pRec = new RecordSet(20, pQuery);
	else
		pRec = new RecordSet(20, *pRKey);
	if(pRLink)
	{
		pRec->SetLink(pRLink, NULL);
		pRLink = NULL;
	}
	return (RecordSet*)pRec;
}

static void CrackAcc(char* acstr, UINT wID, ACCEL* pac, int cac);
void DataContext::Fun(QTable * pqt, UINT nID)
{
	ASSERT(nID > 0);
	nID--;

	NAMEINFO ni;
	MENUITEMINFO mi;
	memset(&mi, 0, sizeof(mi));
	mi.cbSize = sizeof(mi);
	pqt->GetNames(&ni);
	((char*)szFrameName) = ni.names[nID];
	hAccel = GetApp()->LoadAccelerators(((Table*)*pqt)->name);
	ACCEL ac[MAXACCELS];
	int cac = CopyAcceleratorTable(hAccel, ac, MAXACCELS);

	char str[50];
	if(ni.fnum>2 || ni.pnum>0)
		hMenu = CreateMenu();
	mi.fMask = MIIM_TYPE|MIIM_ID;
	mi.fType = MFT_STRING;
	while(ni.pnum)
	{
		mi.wID = ID_PROC_FIRST + ni.pnum;
		ni.pnum--;
		strcpy(str, ni.names[ni.fnum + ni.pnum]);
		CrackAcc(str + strlen(str), mi.wID, ac, cac);
		mi.dwTypeData = str;
		if(mi.dwTypeData)
			InsertMenuItem(hMenu, 0, TRUE, &mi);
	}
	if(ni.fnum>2 && ni.pnum>0)
	{
		mi.fType = MFT_SEPARATOR;
		mi.wID = 0;
		InsertMenuItem(hMenu, 0, TRUE, &mi);
	}
	mi.fType = MFT_STRING;
	while(ni.fnum>2)
	{
		mi.wID = ID_FORM_FIRST + ni.fnum;
		ni.fnum--;
		strcpy(str, ni.names[ni.fnum]);
		CrackAcc(str + strlen(str), mi.wID, ac, cac);
		mi.dwTypeData = str;
		if(mi.dwTypeData)
			InsertMenuItem(hMenu, 0, TRUE, &mi);
	}
	delete ni.names;
}

static void CrackAcc(char* acstr, UINT wID, ACCEL* pac, int cac)
{
	while(cac)
	{
		cac--;
		if(pac->cmd == wID)
		{
			const char* szalt = (pac->fVirt&FALT) ? "Alt+" : "";
			const char* szctrl = (pac->fVirt&FCONTROL) ? "Ctrl+" : "";
			const char* szshift = (pac->fVirt&FSHIFT) ? "Shift+" : "";
			sprintf(acstr, "\t%s%s%s%c", szalt, szctrl, szshift, pac->key);
		}
		pac++;
	}
}
