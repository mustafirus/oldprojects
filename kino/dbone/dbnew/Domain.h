#pragma once

#include "string.h"
class DataBuffer{
	friend class Domain;
	typedef char rawtype;
	typedef int state;
	int			_size;
	state		_state;
//	rawtype		_data[];

	DataBuffer(){};
	DataBuffer(DataBuffer& buf){if(_size < buf._size) throw 1;};
	~DataBuffer(){};

	void* operator new( size_t base, int extra ) {
		size_t sz = base+extra;
		DataBuffer* p = reinterpret_cast<DataBuffer*>(::new rawtype[sz]);
		memset(p,0,sz);
		p->_size = extra;
		return p; 
	}
	void operator delete(void* p, int extra) {
		::delete[] p; 
	};

public:
	int		size(){ return _size; }
	void*	data(){ return reinterpret_cast<rawtype*>(this)+sizeof(this); }
};

class Domain
{
public:
	typedef DataBuffer	buffer;
	typedef void (*Convert)(buffer* to, buffer* from);
	typedef int		bktype;
	typedef string	fttype;

	virtual bool isValid(buffer* buf){ return buf->size() != _size*_count;};

	virtual string name() = 0;

	virtual int size(){return _size;}
	virtual int count(){return _count;};
	virtual void size(int s){_size = s;};
	virtual void count(int c){_count = c;};

	virtual buffer* new_buffer(){ return new (_size*_count) buffer;};
	virtual void delete_buffer(buffer* p){delete p;};

	virtual void assign(buffer* dest, Domain* sdom, buffer* src){
		if(sdom == this){
			memcpy(dest->data(), src->data(), dest->size());
		}else{
			Convert conv = map(this, sdom);
			conv(dest, src);
		}
	}
	virtual void add(buffer* dest, buffer* src){
	}
	virtual void add(buffer* dest, Domain* sdom, buffer* src){
		buffer* buf = new_buffer();
		assign(buf, sdom, src);
		add(dest, buf);
		delete_buffer(buf);
	}
protected:
	int		_size;
	int		_count;
	bktype	_bktype;
	fttype	_fttype;

	Domain();
	virtual ~Domain();
	static Convert map(Domain* to, Domain* from){return NULL;};
};

class Var{
	Domain*		_pDomain;
	DataBuffer* _pDataBuffer;
public:
	Var(Domain* pd, DataBuffer* pb) : _pDomain(pd), _pDataBuffer(pb){}
	Var(Var& v){ _pDomain = v._pDomain, _pDataBuffer = v._pDataBuffer; }
	virtual ~Var(){};

	DataBuffer* operator->(){
		return _pDataBuffer;
	};
//	void* data(){ return _pDataBuffer->data(); }

	Var& operator=(Var v){
		_pDomain->assign(_pDataBuffer, v._pDomain, v._pDataBuffer);
		return *this;
	};

	Var& operator+=(Var v){
		_pDomain->add(_pDataBuffer, v._pDomain, v._pDataBuffer);
		return *this;
	};

	Var operator+(Var v){
		Var t = *this;
		return t += v;
	};
	Var& operator-=(Var v);
	Var operator-(Var v);

	Var& operator*=(Var v);
	Var operator*(Var v);

	Var& operator/=(Var v);
	Var operator/(Var v);

	operator string(){return string();}
	operator Domain*(){return _pDomain;}
};

class DomainInt : public Domain
{
public:
	DomainInt() { _size = 4; _count = 1;};
	virtual ~DomainInt(){};
	virtual string name(){return "Int";};
	virtual bool isValid(buffer* buf){return Domain::isValid;};
};

class DomainString : public Domain
{
public:
	DomainString(int sz) { _size = 1; _count = sz;};
	virtual ~DomainString(){};
	virtual string name(){return "Int";};
	virtual bool isValid(buffer* buf){return Domain::isValid;};
};

