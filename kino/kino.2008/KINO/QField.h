// QField.h: interface for the QField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QFIELD_H__2A22DB53_DE78_11D1_A60C_204C4F4F5020__INCLUDED_)
#define AFX_QFIELD_H__2A22DB53_DE78_11D1_A60C_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Field.h"
#include "QTable.h"

class QField
{
	Field*	pField;
	QTable* pQTable;
	QField* next;
public:
	QField(QTable* pTable, Field* pField);
	virtual ~QField();

	operator Field*(){return pField;};
	operator QTable*(){return pQTable;};
	QFields& operator>>(QFields& qf);
	QField* Add(QField* pqf);
/*	QField* Find(QTable* pTable, const char* str);
	QField* Find(QTable* pTable, Field* pfld);
*/	QField* GetQField(Field* pfld);
	bool ro(){return pField->ro /*|| (pQTable->pPTable && pQTable->pNTable->def_edit)*/;};
	void Select(SqlStmt& str);
	void Refresh(SqlStmt& str);
	void Where(SqlStmt& str);
	void DeleteWhere(SqlStmt& str);
	void UpdateWhere(SqlStmt& str);
	void RefreshWhere(SqlStmt& str);
	void Mark(){pQTable->Mark(false);};
/*	void Find(QTable* pTable, QFields& qf);
	Field* GetField(){return pField;};
	QTable* GetQTable(){return pQTable;};
	void SQLStr(CString& str, bool mark, bool useAlias);
	int GetStyle();
	const char* GetCaption();
*/protected:
DECLARE_DICT_MEMORY
};

#endif // !defined(AFX_QFIELD_H__2A22DB53_DE78_11D1_A60C_204C4F4F5020__INCLUDED_)
