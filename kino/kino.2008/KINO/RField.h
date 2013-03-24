// RField.h: interface for the RField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RFIELD_H__BC8CAC02_F65C_11D1_A64D_204C4F4F5020__INCLUDED_)
#define AFX_RFIELD_H__BC8CAC02_F65C_11D1_A64D_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "QField.h"
#include "Record.h"
class Field;
class RKey;
class StMdfr;
class RStyle;
class FDC;
class SqlStmt;
//DATE_STRUCT

class RField
{
friend class Record;
friend class OdbcCursor;
friend class OdbcDataBase;
friend class RecordSet;
friend class RKey;
//friend class DataBase;
public:
enum Flag{s_data = 1, s_null = 2, s_new = 3, s_modified = 4, s_hidden = 8};

class STATE
{
	char data;
public:
	STATE(){data = s_hidden;};
	bool operator==(Flag f){return (data&f) > 0;}
	bool operator!=(Flag f){return !(data&f);}
	void operator=(Flag f){data |=f;}
	void operator/=(Flag f){data &= ~f;}
};	

protected:
	char*		buf;
	QField*		pQField;
	RField*		pPRField;
	Record*		pRec;
	void*		pind;
//	char		attr;
	char		v;
	STATE		state;
//	enum STATE{s_dummy, s_d	atanull, s_data, s_modified, s_modifiednull} state;
	enum {b_none, b_native, b_foreign} buf_attrib;

	RField();

public:
	Field::Type type;

	RField(QField* pqf, Record* pr, RField* prf = NULL);
	virtual ~RField();

//	enum Attribute{Hidden = 1, ReadOnly = 2};

	virtual char*	GetBuf(){state /= s_null; return buf;};
			int		GetLen();
	virtual	operator const char*();
			operator Record*(){return pRec;}
			operator QField*(){return pQField;};
			operator Field*(){return *pQField;};
			operator QTable*(){return *pQField;};
			operator RKey*();
	QField*	operator->(){return pQField;}

/*	bool	IsData(){return state == s_data;};
	void	SetNull(){v++; state = s_null;};
	bool	IsNull(){return state == s_null;};
	bool	IsModified(){return state == s_modified;};
	void	show(bool s = true){if(s) state /= s_hidden; else state = s_hidden;};
*/
	void	SetDefault();
	void	SetData(bool null = false);
	void	SetModified(){v++; if(*buf == 0) state = s_null; state = s_modified; pRec->SetModified();};
	bool	ro();
	bool	hidden(){return state == s_hidden;};
	char	ver(){return v;};
	void*	GetIndicator(){return pind;};

// SQL strings
	void Refresh(SqlStmt& str){pQField->Refresh(str);};

	void Delete();
//Clipboard support
	virtual	void OnCopy(HWND hWnd);
	virtual	void OnPaste(HWND hWnd);
	virtual	bool CanCopy();
	virtual	bool CanPaste(HWND hWnd);
};
#endif // !defined(AFX_RFIELD_H__BC8CAC02_F65C_11D1_A64D_204C4F4F5020__INCLUDED_)
