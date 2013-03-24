#if !defined(AFX_TABLE_H__1ACBEBB0_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_)
#define AFX_TABLE_H__1ACBEBB0_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_

using namespace std;

template <class T>
class vectorautodel : public vector<T>
{
public:
	~vectorautodel(){
		int n = vector<T>::size();
		for (int i = 0; i < n; i++) {
			delete (*this)[i];
		}
	}
};


class Props{
	typedef map<string, string> props;
	typedef pair<string, string> pairtype;

	struct tostr: public unary_function<pairtype, void> {
		string str;
		void operator()(pairtype x) { str+= x.first + "=""" + x.second + """"; }
	};
	props	_props;
public:
	explicit Props(const char** sstr){
		for(int i=0; sstr[i]; i+=2){
			_props.insert(pairtype(sstr[i], sstr[i+1]));
		}
	}
	~Props(){

	}
	string getProp(string key){
		return _props[key];
	}

	string name(){return getProp("name");};
	string toString(){
		props::iterator x = _props.begin();
		props::iterator last = _props.end();
		string str;
		for (; x != last ; ++x){
		  str+= (*x).first + "='" + (*x).second + "'";
		}
		return str;
	}
};

class Field : public Props
{
public:
	enum Type{Unknown, Char, TinyInt, SmallInt, Integer, Date, Time, DateTime};

//	Field(){};
	Field(const char** props) : Props(props) {};
	~Field(){};
	string toString(){
		string str;
		str += "<field " + Props::toString() + "/>\n";
		return str;
	}
};

class Table;
class Dict;

class PKey : public Props
{
	typedef vector<Field*> fields;
protected:
	fields		_fields;
public:
	Table*		pTable;

	operator fields&() {return _fields;};
	PKey(Table* pt, const char** props) : Props(props), pTable(pt) {};
	~PKey(){};
	void addField(Field* pf){
		_fields.push_back(pf);
	};
	string toString(){
		string str;
		str += "<id " + Props::toString() + ">\n";
		for(size_t i=0; i < _fields.size(); i++){
			str +=_fields[i]->toString();
		}
		str += "</id>\n";
		return str;
	}

};

class FKey : public PKey
{
public:
	PKey*		pPKey;

	FKey(Table* pt, const char** props) : PKey(pt, props), pPKey(NULL){}
	~FKey(){}
	void bindKeys(Dict* pd);
	string toString(){
		string str;
		str += "<fkey " + Props::toString() + ">\n";
		for( size_t i=0; i < _fields.size(); i++){
			str +=_fields[i]->toString();
		}
		str += "</fkey>\n";
		return str;
	}

};

//class QTable;
class Table : public Props
{
	typedef vectorautodel<Field*> Fields;
	typedef map<string, Field*> FieldM;
	typedef vectorautodel<FKey*> FKeys;
	typedef map<string, FKey*> FKeyM;

public:

private:
	Fields	_fields;
	FieldM	_fieldm;
	PKey*	_pkey;
	FKeys	_fkeys;
	FKeyM	_fkeym;

public:
	Table(const char** props) : Props(props) {};
	~Table(){};

//Operations
	void addField(Field* pf){
		_fields.push_back(pf);
		_fieldm[pf->name()]=pf;
	};
	Field* getField(const string& name){
		return _fieldm[name];
	};

	//////////////////////////////////////////////////////////////////////////
		///    PKeys
	//////////////////////////////////////////////////////////////////////////
	void addPKey(PKey* pk){
		_pkey = pk;
	};
	PKey* getPKey(){
		return _pkey;
	};
//////////////////////////////////////////////////////////////////////////
	///    FKeys
//////////////////////////////////////////////////////////////////////////
	void addFKey(FKey* fk){
		_fkeys.push_back(fk);
		_fkeym[fk->name()] = fk;
	};
	void bindKeys(Dict* pd){ // Bind FKeys to tables after creation all off them
		size_t n = _fkeys.size();
		for (size_t i = 0; i < n; ++i) {
			_fkeys[i]->bindKeys(pd);
		}
	}
	FKey* getFKey(const string& name) throw (out_of_range){
			return _fkeym.at(name);
	}

	FKey* getFKey(PKey* pRef){
		ASSERT(pRef);
		for(size_t i = 0; i < _fkeys.size(); i++)
		{
			if(_fkeys[i]->pPKey == pRef)
				return _fkeys[i];
		}
		return NULL;
	}

	string toString(){
		string str;
		str += "<table " + Props::toString() + ">\n";
		str +=_pkey->toString();
		for( size_t i=0; i < _fields.size(); i++){
			str +=_fields[i]->toString();
		}
		for( size_t i=0; i < _fkeys.size(); i++){
			str +=_fkeys[i]->toString();
		}
		str += "</table>\n";
		return str;
	}



	Field* GetField(size_t n);
	operator PKey*(){ return _pkey;};
	//operator char*(){if(!this) return NULL; return name;};
	string name(){return getProp("name");};
/*

	Field* GetKeyField(int n){return mPKey[n];}
	int    GetKeyFieldCount(){return mPKey.pFields.GetSize();}
*/

};


#endif // !defined(AFX_TABLE_H__1ACBEBB0_DE6E_11D1_A60C_204C4F4F5020__INCLUDED_)
