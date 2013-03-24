// Action.cpp: implementation of the Action class.
//
//////////////////////////////////////////////////////////////////////

#include "stdx.h"
#include "dblib.h"
#include "DataContext.h"
#include "FrameThread.h"
#include "FrameWnd.h"
#include "MainWnd.h"
#include "KinoRes.h"
#include "KinoApp.h"
#include "Exception.h"
#include "Wait.h"
#include "Action.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Action* Action::pAction = NULL;
void PrintPedigree(HWND hWnd, RKey* prk);
void PrintPedigreeSetup(HWND hWnd, RKey* prk);

bool Action::Can(FrameWnd* pWnd, Record* pr, RField* prf, int nID)
{
	if(nID == ID_FILE_PRINT)
	{
		if(!pr)
			return false;
		Table* pt = *((QTable*)*((Query*)*pr)); 
		if(strcmp( pt->name,"dog") )
			return false;
		return true;
	}
	return false;
}

void Action::Exec(FrameWnd* pWnd, Record* pr, RField* prf, int nID)
{
	Wait w;
	bool f_thread	= (nID & ACT_FLAG_THREAD) > 0;
	bool f_blank	= (nID & ACT_FLAG_BLANK) > 0;
	nID &= ~(ACT_FLAG_THREAD|ACT_FLAG_BLANK);
	if(nID >= ID_FORM_FIRST && nID <= ID_FORM_LAST)
	{
		try{
			nID -= ID_FORM_FIRST;
			QTable* pqt = prf ? *prf : (QTable*)NULL;
			ASSERT(pr || prf);
			if(!pr)
				pr = *prf;
			RKey* prk = pr->GetRKey(pqt);
			if(f_blank)
				prk->SetNull();
			
			if(!nID)
			{
				if(!pqt)
					pqt = *prk;
				nID = ((Table*)*pqt)->def_edit;
				if(!nID || (nID == 1 && prk->IsNull()))
					nID = 2;
			}
			if(f_thread)
			{
				FrameThread* ptr = FrameThread::GetCurrent()->CreateThread();
				ptr->pRec = new Record(prk);
				prk = ptr->pRec->GetRKey();
				if(f_blank)
					ptr->pRec->New();
				else
					ptr->pRec->Load();
				ptr->dc.Create(prk, NULL, nID);
				ptr->Resume();
			}else
			{
				DataContext* pdc = NULL;
				pdc = &(FrameThread::GetCurrent()->dc);
				pdc->Create(prk, pr->GetLink(prk), nID);
				new FrameWnd(pdc, pWnd);
			}	
			return;
		}catch(Exception* e)
		{
			e->Effect();
			return;
		}catch(...)
		{
			return;
		}
	}else if(nID >= ID_PROC_FIRST && nID <= ID_PROC_LAST)
	{
		try{
			nID -= ID_PROC_FIRST;
			QTable* pqt = prf ? *prf : (QTable*)NULL;
			ASSERT(pr || prf);
			if(!pr)
				pr = *prf;
			RKey* prk = pr->GetRKey(pqt);
			char* szProc = RecordSet::pDict->GetForm(prk, nID, 1);
			char stmt[255];
			ASSERT(prk->GetCount());
			UINT c = prk->GetCount() - 1;
			ASSERT(c < 20);
			char par[50]="?";
			while(c--)
				strcat(par, ",?");
			sprintf(stmt,"{call %s(%s)}", szProc, par);
			Record::pDB->BindParameter(prk);
			Record::pDB->ExecDirect(stmt);
			Record::pDB->Flush();
			delete szProc;
		}catch(Exception* e)
		{
			e->Effect();
			return;
		}
	}else if(nID == ID_FILE_PRINT)
	{
		if(!pr)
			return;
		Table* pt = *((QTable*)*((Query*)*pr)); 
		if(strcmp( pt->name,"dog") )
			return;
		PrintPedigree(*pWnd, pr->GetRKey(NULL));
	}else if(nID == ID_FILE_PRINT_SETUP)
	{
		PrintPedigreeSetup(*pWnd, NULL);
	}
}
