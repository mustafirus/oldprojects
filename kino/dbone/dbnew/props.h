#pragma once
#define PROP_STR		0x8000
#define PROP_INT		0x4000
#define PROP_IDMASK		0x00FF
#define PROP_TPMASK		0xFF00
#define PROP_NAME					(0|PROP_STR)
#include "string.h"
class props{
	friend class Dictionary;
	union prop{
		int		n;
		const char*	s;
	};
	int		_size;
	prop*	_props;
	int getid(int id, int type){
		ASSERT(id&type);
		return id &= PROP_IDMASK;
	}
	prop operator[](int id){
		ASSERT((id & PROP_IDMASK) < _size);
		return _props[id];
	};
protected:
	string		_name;
	struct name_space{
		const int			id;
		const char*const	name;
	};
	virtual name_space* get_name_space() = 0;
public:
	props(string name, int max) : _name(name){
		_props = new prop[_size=max];
		memset(_props, 0, sizeof(prop)*_size);
	};
	~props(){
		delete[] _props;
	};

	string& name(){ return _name;};
//	operator string(){ return name(); }

	void set(int id, int val){
		_props[getid(id, PROP_INT)].n = val;
	};
	void set(int id, const char* val){
		int n = getid(id, PROP_STR);
		if (_props[n].s)
			free((void*)_props[n].s);
		_props[n].s= strdup(val);
	};

	void get(int id, int* val){
		*val = _props[getid(id, PROP_INT)].n;
	}
	void get(int id, const char** val){
		*val = _props[getid(id, PROP_STR)].s;
	}

};
