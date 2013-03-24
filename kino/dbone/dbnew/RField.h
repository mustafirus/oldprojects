#pragma once

#if 0
class Cursor;
class RKey;
class QField;
class DataBuffer;
#endif

class Record;
class RField
{
	QField*		_pQField;
	Record*		_pRec;
	DataBuffer*	_pDataBuffer;

	RField();

public:

	RField(QField* pqf, Record* pr, RField* prf = NULL);
	virtual		~RField();
	QField*		name() { return _pQField;}
	
	operator Var(){ return Var(*_pQField, _pDataBuffer);};
	operator QField*(){return _pQField;};
#if 0

	operator Record&(){return *_pRec;}
	operator QField&(){return *_pQField;};
	operator Field&(){return *_pQField;};
	operator QTable&(){return *_pQField;};
	operator RKey*();
	operator Domain*(){ return *_pQField; }
#endif
};
