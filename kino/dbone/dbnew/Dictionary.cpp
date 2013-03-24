#include "StdAfx.h"
#include ".\dictionary.h"

/*Field* Table::getField(string& name)
{
	for(int i=0; i < _vFields.count(); i++)
	{
		Field* pf = _vFields[i];

		if( pf->name() == name )
			return pf;
	}
	return NULL;
};
*/

Table::name_space Table::_name_space[]={
	{0, NULL}
};

Field::name_space Field::_name_space[]={
	{PROP_FIELD_CAPT, "Caption"},
	{PROP_FIELD_STR, "String"},
	{PROP_FIELD_SIZE, "Size"},
	{PROP_FIELD_DISPLAY_SIZE, "DisplaySize"},
	{0, NULL}
};
PKey::name_space PKey::_name_space[]={
	{PROP_KEY_CAPT, "Caption"},
	{PROP_KEY_EXT, "Ext"},
	{PROP_KEY_RST, "Rst"},
	{PROP_KEY_STYLE, "Style"},
	{PROP_KEY_FORM, "Form"},
	{PROP_KEY_LIST, "List"},
	{0, NULL}
};




Dictionary::Dictionary(void){
}

Dictionary::~Dictionary(void){
}

#if 0
static bool eq( int id, props* pr, const char* s1 )
{
	const char* s2;
	pr->get(PROP_FIELD_NAME, &s2);
	return strcmp(s1,s2) ? false : true;
}

template <class T, class V>
static T* find_prop(vector<T*>& v, int id, V val){
	for(int n = 0; n < v.count(); n++){
		T* pf = v[n];
		if (eq(id, pf, val)){
			return pf;
		}
	}
	return NULL;
}
#endif

template <class T>
static T* find_by_name(vector<T*>& v, string val){
	for(int n = 0; n < v.count(); n++){
		T* pf = v[n];
		if ( pf->name() == val ){
			return pf;
		}
	}
	return NULL;
}

Table* Dictionary::addTable(const char* name){
	Table* pTable = _vTables.add(new Table(name));
	return pTable;
}

Field* Dictionary::addField(Table* table, string name, Domain* pd){
	return table->_vFields.add(new Field(table, name, pd));
}

PKey*  Dictionary::addPKey(Table* table, int num, const char** fields){
	PKey* pk = new PKey(table, table->name());
	for ( int i = 0; i < num; i++)
	{
		Field* pf = find_by_name(table->_vFields, fields[i]);
		pk->_vFields.add(pf);
	}
	return table->_pPKey = pk;
}

FKey*  Dictionary::addFKey(Table* table, string name, string ftable, int num, const char** fields){
	FKey* fk = new FKey(table, name);
	Table* t = find_by_name(_vTables, ftable);
	fk->_pPKey = t ? t->_pPKey : NULL;
	for ( int i = 0; i < num; i++)
	{
		Field* pf = find_by_name(table->_vFields, fields[i]);
		fk->_vFields.add(pf);
	}
	return table->_vFKeys.add(fk);
}

void Dictionary::toXML(ostream& s, props* v){
	props::name_space* pn = v->get_name_space();
	int ival;
	const char* sval;
	for( int i = 0; i < v->_size; i++){
		int id = pn[i].id;
		const char* name = pn[i].name;
		
		switch ( id & PROP_TPMASK ){
			case PROP_STR:
				v->get(id, &sval);
				if(!sval) sval = "";
				s << " " << name << "=" << "\"" << sval<< "\"";
				break;
			case PROP_INT:
				v->get(id, &ival);
				s << " " << name << "=" << "\"" << ival<< "\"";
				break;
			default:
				ASSERT(0);
		}
	}
}
void Dictionary::toXML(ostream& s, Table* v, bool ref/* = false*/){
	if(ref){
		const char* name = v->name();
		s << "\t\t<TableRef>" << name << "</TableRef>\n";
	}else{
		s << "<Table";
		toXML(s, static_cast<props*>( v ) );
		s << ">\n";
		toXML(s, v->_vFields);
		toXML(s, v->_pPKey);
		toXML(s, v->_vFKeys);
		s	<< "</Table>\n";
	}
}

void Dictionary::toXML(ostream& s, Field* v, bool ref/* = false*/){
	if(ref){
		const char* name = v->name();
		s << "\t\t<FieldRef>" << name << "</FieldRef>\n";
	}else{
		s << "\t<Field";
		toXML(s, static_cast<props*>( v ) );
		s << "></Field>\n";
	}
}

void Dictionary::toXML(ostream& s, PKey* v, bool ref/* = false*/){
	if(ref){ASSERT(0); return;}
	s << "\t<PKey";
	toXML(s, static_cast<props*>( v ) );
	s << " >\n";
	toXML(s, v->_vFields, true);
	s	<< "\t</PKey>\n";
}
void Dictionary::toXML(ostream& s, FKey* v, bool ref/* = false*/){
	if(ref){ASSERT(0); return;}
	s << "\t<FKey";
	toXML(s, static_cast<props*>( v ) );
	s << ">\n";
	toXML(s, v->_pPKey->_pTable, true);
	toXML(s, v->_vFields, true);
	s	<< "\t</FKey>\n";
}
template <class T>
void Dictionary::toXML(ostream& s, vector<T*>& v, bool ref/* = false*/){
	for( int i = 0; i < v.count(); i++){
		toXML(s, v[i], ref);
	}
}

void Dictionary::toXML(ostream& s){
	toXML(s, _vTables);
}
