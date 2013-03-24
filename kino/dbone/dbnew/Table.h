#pragma once







/*class Object{};
class Document : public Object{
	typedef int Int;
	typedef float Date;
	Int		number;
	Date	date;
	class Items : public Object{
		Document* pParentObj;
	};
	void OnSave();
	void OnNew();
};
*/

/*
class QField
{
	Field*	pField;
	QTable* pQTable;
	QField* next;
public:
	QField(QTable* pTable, Field* pField);
	virtual ~QField();

	operator Field&(){return pField;};
	operator QTable*(){return pQTable;};
	QFields& operator>>(QFields& qf);
	QField* Add(QField* pqf);
	QField* GetQField(Field* pfld);
	bool ro(){return pField->ro ;};
	void Select(SqlStmt& str);
	void Refresh(SqlStmt& str);
	void Where(SqlStmt& str);
	void DeleteWhere(SqlStmt& str);
	void UpdateWhere(SqlStmt& str);
	void RefreshWhere(SqlStmt& str);
	void Mark(){pQTable->Mark(false);};
};
*/