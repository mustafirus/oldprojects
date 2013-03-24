#include "stdx.h"
#include "Defines.h"
#include "dblib.h"
#include "DataContext.h"
#include "FrameWnd.h"
#include "Frame.h"
#include "Wait.h"
#include "Grid.h"
#include "FieldFrame.h"
#include "RFieldControl.h"
#include "Errorcodes.h"
#include "DbException.h"
#include "Pedigree.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#include "VecFrame.h"
//#include "TextFrame.h"
//#include "TitulFrame.h"
//#include "SelectFrame.h"
//#include "RFrame.h"

/*Frame* Frame::CreateFrame(char*&str, CreateContext* pContext)
{
	if(*str == '<')str++;
	switch(*str)
	{
//	case 'V':	return new VecFrame(str, pContext); break;
//	case 'H':	return new VecFrame(str, pContext); break;
	case 'T':	return new TextFrame(++str, pContext); break;
	case 'F':	return new FieldFrame(++str, pContext); break;
//	case 'L':	return new ListFrame(++str, pContext); break;
//	case 'R':	return new RFrame(++str, pContext); break;
//	case 'S':	return new SelectFrame(++str, pContext); break;
//	case 'U':	return new TitulFrame(++str, pContext); break;
	default:	_ASSERT(0);return NULL; break;
	}
	return NULL;
}
*/
/*class Breaker
{
	char* pos;
	char  ch;
public:
	Breaker(char* str, char brk) : ch(brk)
	{
		*(pos = strchr(str, brk)) = 0;
	};
	~Breaker()
	{
		*pos = ch;
	};
};
*/
Frame* Frame::CreateFrame(char*&str, DataContext* pdc)
{
	Wait w;
	ASSERT(pdc->pWnd);
	if(*str == '<')str++;
	switch(*str)
	{
	case 'V':
	case 'H':
		{
			VecFrame* pvf = new VecFrame; 
			pvf->horz = (*str == 'H') ? true : false;
			while(*str != '>')
			{
				if(*str == '<')
				{
					pvf->Add(CreateFrame(str, pdc));
					if(*str != '>')
					{
						Exception::ex.printf(MSG_ERROR_FRAME_SYNTAX_ERROR);
						delete pvf;
						throw &Exception::ex;
					}
				}
				str++;
			}
			return pvf;
		}
//	case 'T':	return new TextFrame(++str, pWnd); break;
	case 'F':
		{
			Breaker brk(str, '>');
			Breaker br1(str, '(');
			if(br1)
			{
				Breaker br2(br1, ')');
				if(br2 != true)
				{
					Exception::ex.printf(MSG_ERROR_FRAME_SYNTAX_ERROR);
					throw &Exception::ex;
				}
				str = br2;
			}
			FieldFrame* pFrame = new FieldFrame;
			RFields rf;
			try{
				str = pdc->GetRecord()->GetRFields(++str, rf);
			}catch(DbException* e){
				e->Effect();
				str = brk;
				if(str)
					str--;
				return NULL;
			}
			for(int i = 0; i < rf.GetCount(); i++)
			{
				pFrame->vCtrls.Add(new RFieldControl(rf[i], pdc->pWnd, 0));
			}
			return pFrame;
		}
	case 'L':
		{
			bool ok = true;
			str++;
			str += strspn(str, " ");
			RecordSet* pr; 
			if(strchr(str,':'))
			{
				Breaker ss(str, ' ');
				try{
					Record* prrrr = pdc->GetDefRecord();
					if(!prrrr)
					{
						prrrr = pdc->GetRecord();
						pdc->bAutoSave = true;
					}
					pr = (RecordSet*) prrrr->GetLinkedRecord(str);
				}catch(Exception* e){e->Effect(); ok = false;}
				str = ss;
			}else
				pr = pdc->GetRecordSet();
			Breaker ss(str, '>');
			char* s1 = str;
			str = ss;
			if(str)
				str--;
			if(ok)
				return new RGrid(s1, pr, 10, pdc->pWnd, 0);
			else
				return NULL;
		}
	case 'P':
		{
			Breaker brk(str, '>');
			Pedigree* pFrame = NULL;
			try{
				pFrame = new Pedigree(pdc->pWnd, pdc->GetRKey());
			}catch(DbException* e)
			{
				e->Effect();
			}
			str = brk;
			if(str)
				str--;
			return pFrame;
		}
//	case 'R':	return new RFrame(++str, pContext); break;
//	case 'S':	return new SelectFrame(++str, pContext); break;
//	case 'U':	return new TitulFrame(++str, pContext); break;
	default:
		{
//			_ASSERT(0);
			Breaker brk(str, '>');
			char t[2]={*str,'\0'};
			Exception::ex.printf(MSG_ERROR_FRAMETYPE_NOTFOUND, t);
			Exception::ex.Effect();
			str = brk;
			if(str)
				str--;
			return NULL;
		}
	}
	return NULL;
}
