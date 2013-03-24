#pragma once
class Wnd;
typedef void (Wnd::*X_PCMDFN)(void);
typedef void (Wnd::*X_PCMDFNEX)(UINT);

struct X_CMDMAP_ENTRY
{
	UINT		nID;        // control ID
	UINT		nLastID;    // used for entries specifying a range of control id's
	UINT		nCode;		// control code or WM_NOTIFY code
	X_PCMDFN	pfn;		// routine to call (or special value)
};

struct X_CMDMAP
{
	const X_CMDMAP* pBaseMap;
	const X_CMDMAP_ENTRY* lpEntries;
};
#define X_CMDMAP_CODE_RANGE		0x8000000

#define DECLARE_COMMAND_MAP() \
private: \
	static const X_CMDMAP_ENTRY _cmdEntries[]; \
protected: \
	static const X_CMDMAP cmdMap; \
	virtual const X_CMDMAP* GetCmdMap() const;

#define BEGIN_COMMAND_MAP(theClass, baseClass) \
	const X_CMDMAP* theClass::GetCmdMap() const \
		{ return &theClass::cmdMap; } \
			const X_CMDMAP theClass::cmdMap = \
	{ &baseClass::cmdMap, &theClass::_cmdEntries[0] }; \
	const X_CMDMAP_ENTRY theClass::_cmdEntries[] = \
	{ 


#define END_COMMAND_MAP			\
		{0, 0, 0, (X_PCMDFN)0 } \
	}; 

#define ON_COMMAND(id, memberFxn) \
	{(WORD)id, (WORD)id, 0, (X_PCMDFN)&memberFxn },
		// ON_COMMAND(id, OnFoo) is the same as
		//   ON_CONTROL(0, id, OnFoo) or ON_BN_CLICKED(0, id, OnFoo)

#define ON_COMMAND_EX(id, memberFxn) \
	{(WORD)id, (WORD)id, X_CMDMAP_CODE_RANGE, (X_PCMDFN)(X_PCMDFNEX)&memberFxn },

#define ON_COMMAND_RANGE(id, idLast, memberFxn) \
	{(WORD)id, (WORD)idLast, X_CMDMAP_CODE_RANGE, (X_PCMDFN)(X_PCMDFNEX)&memberFxn },

