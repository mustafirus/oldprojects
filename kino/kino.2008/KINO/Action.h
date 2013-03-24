// Action.h: interface for the Action class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTION_H__D7E1C0A3_4CB9_11D2_A738_204C4F4F5020__INCLUDED_)
#define AFX_ACTION_H__D7E1C0A3_4CB9_11D2_A738_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class FrameWnd;
class Record;
class RField;
#define		ACT_DEFAULT				  0
#define		ACT_ALT_DEFAULT			 -1
#define		ACT_DEFAULT_EX			-11
#define		ACT_FIELD_EDIT		
#define		ACT_FIELD_EDIT_EX
#define		ACT_FIELD_VIEW
#define		ACT_FIELD_VIEW_EX
#define		ACT_FIELD_SAVE
#define		ACT_RECORD_NEW
#define		ACT_RECORD_LOAD
#define		ACT_RECORD_SAVE
#define		ACT_RECORD_REFRESH

class Action  
{
public:
	Action(){};
	virtual ~Action(){};
	
	void Exec(FrameWnd* pWnd, Record* pr, RField* prf, int nID);
	bool Can(FrameWnd* pWnd, Record* pr, RField* prf, int nID);

	static Action* GetAction(){ASSERT(pAction);return pAction;};
	static Action* pAction;
};

#endif // !defined(AFX_ACTION_H__D7E1C0A3_4CB9_11D2_A738_204C4F4F5020__INCLUDED_)
