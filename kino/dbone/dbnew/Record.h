#pragma once

class Record
{
protected:
	typedef vector<RField*>	RFields;

	Query*		_pQuery;
	RFields		_vRFields;
	RKey*		_pPRKey;
	Cursor*		_pCursor;
#if 0
	RObject*	_pRObject;
#endif

protected:
	Record(Query* p) : _pQuery(p), _pCursor(getCursor(_pQuery)){}
public:
	Record(RKey* rkey, Query* p);
	virtual ~Record(){};
	virtual void load(){};
	virtual void save(){};
	static Cursor* getCursor(Query* pq);
};

class Recordset : public Record
{
public:
	Recordset(Query* pq) : Record(pq){};
	virtual void load();
	virtual void save(){};
//	virtual void next(){pc->next();};
	RField* getRField(string name);

	void first()			{ _pCursor->fetch(Cursor::first);}
	void next(int n = 1)	{ _pCursor->fetch(n);}
	void prev(int n = 1)	{ _pCursor->fetch(-n);}
	void last()				{ _pCursor->fetch(Cursor::last);}
	void move(int n)		{ _pCursor->fetch(n, true);}
	void reload()			{ _pCursor->fetch(0);}

	bool bof(){ return _pCursor->bof(); }
	bool eof(){ return _pCursor->eof(); }
};



