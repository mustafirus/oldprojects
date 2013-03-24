#pragma once

#include "my_vector.h"
#include "my_pair.h"

/*! \brief All Component of db objects 
*	
*	All objects of db dictionary - fiels, functions, methods(function with void return)
*/
class Component {
protected:
	string _domain;
	string _name;
	string _body;
	Component(string name) : _name(name){};
	~Component(){};
public:
	virtual void tostring(string& str) = 0;
};

class Field : public Component{
	string _domain;
public:
	Field(string domain, string name) : _domain(domain), Component(_name){};
	~Field(){};
	void tostring(string& str){
		str << "\t" << _domain << " " << _name << ";\n";
	};
};

class Method : public Component{
	string _body;
public:
	Method(string name, string body) : Component(name), _body(body) {};
	~Method();
	void tostring(string& str){
		str << "\t" << _name << "(){{c++\n" << _body << "\n\t}};\n";
	};
};

class Function : public Component{
	string _domain;
	string _body;
public:
	Function(string domain, string name, string body) : _domain(domain), Component(_name), _body(body){};
	~Function(){};
	void tostring(string& str){
		str << "\t" << _domain << " " << _name << "(){{c++\n" << _body << "\n\t}};\n";
	};
};

/*! \brief All objects of db dictionary
*	
*	All objects of db dictionary - domains, tables, views
*/
class Object {
	typedef pair<string, string> Param;
	typedef vector< Param > Params;
	string		_name;
	string		_parent;	
	Object*		_parent_obj;	
	Params		_params;
	typedef vector< Component* > Components;
	Components	_components;



public:
	enum Type { domain = 1, table = 2, view = 3} _type;
	
	Object( Type type, string name, string parent ) : _type(type),  _name(name), _parent(parent), _parent_obj(NULL) {};
	~Object(){};
	string name()	{ return _name; }
	string parent()	{ return _parent; }
	string typestr(){
		switch(_type){
			case domain:
				return "domain";
			case table:
				return "table";
			case view:
				return "view";
			default:
				ASSERT(0);
				return "";
		}
	};

	void addParam(string name, string val){
		_params.add( Param( name, val ) );
	}

	void addComponent(Component* pcom){
		_components.add( pcom );
	}

	void tostring( string& str ){
		str << typestr() << " " << name();
		if (_parent_obj){
			str << " : " << _parent_obj->name();
		}
		if (_params.count()){
			str << "[\n\t" << _params[0].name() << " => " << _params[0].val() ;
			for ( int i = 1; i < _params.count(); i++ ){
				str << ",\n\t" << _params[i].name() << " => " << _params[i].val() ;
			}
			str << "\n]";
		}
		str << "{\n";
		for ( int i = 0; i < _components.count(); i++ ){
			_components[i]->tostring(str);
		}
		str << "\n}";
	}
};


class Dict{
	typedef vector<Object*> Objects;

	Objects		_objects;
	Object*		_pObj;

	void beginObject(Object::Type type, string name, string parent){
		_pObj = new Object( type, name, parent);
	}
	void endObject(){
		_objects.add(_pObj);
	}
	Object* object(){ return _pObj; }
	Object* operator->(){ return _pObj; }
};
