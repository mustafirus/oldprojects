#pragma once
#include "string.h"

template<class T> class list;

template<class T>
class vector {

protected:
	T*	_pData;
	int	_count;
	int	_size;
	
public:
	vector() : _pData(new T[growby]), _count(0), _size(growby){};
	~vector(){ delete[] _pData; };
	T operator[](int n){ASSERT( n < _count ); return _pData[n];}
	bool exists(T);
	int	count(){ return _count; }
	T add(T);

	template<class Par>
	T operator[](Par s);

	template<class To>
	void tolist( list<To>*& pl );
private:
	void grow();
	static const growby = 8;
//	void remove_all();
};
//************************************************
template<class T>
class vector<T*> : public vector<void*>{
	typedef vector<void*> Base;
public:
	T* add(T* p){return reinterpret_cast<T*>(Base::add(p)); }
	T* operator[](int n){ return reinterpret_cast<T*>(Base::operator[](n)); }

	template<class Par>
	T* operator[](Par s);
	template<class To>
	void tolist( list<To>*& pl );

	void delete_all();
};
//************************************************


template<class T>
template<class To>
void vector<T>::tolist( list<To>*& pl ){
	for( int i = 0; i < _count; i++){
		if(!pl){
			pl = new list<To>(_pData[i]);
		}else{
			pl->add(_pData[i]);
		}
	}
}

template<class T>
template<class To>
void vector<T*>::tolist( list<To>*& pl ){
	for( int i = 0; i < _count; i++){
		T* p = reinterpret_cast<T*>(_pData[i]);
		if(!pl){
			pl = new list<To>(*p);
		}else{
			pl->add(*p);
		}
	}
}

template<class T>
bool vector<T>::exists(T p){
	for( int i = 0; i < _count; i++ )
	{
		if( p == _pData[i] )
			return true;
	}
	return false;
};


template<class T>
inline T vector<T>::add(T p){
	ASSERT(_count <= _size);
	if( _count == _size) grow();
	return _pData[_count++] = p;
};


template<class T>
void vector<T>::grow(){
	T* p=new T[_size+=growby];
//	memset(p, 0, sizeof(T*)*_size);
	if(_pData)
	{
		for (int i = 0; i < _count; i++){
			p[i]=_pData[i];
		}
//		memcpy(p,_pData,sizeof(T*)*_count);
		delete[] _pData;
	}
	_pData = p;
}
template<class T>
void vector<T*>::delete_all(){
	for (int i = 0; i < count(); i++){
		T* p = reinterpret_cast<T*>(Base::operator[](i));
		if(p) delete p;
	}
}
template<class T>
template<class Par>
T vector<T>::operator[](Par s){
	for( int i = 0; i < _count; i++ )
	{
		if( _pData[i] == s )
			return _pData[i];
	}
	return T();
};

template<class T>
template<class Par>
T* vector<T*>::operator[](Par s){
	for( int i = 0; i < _count; i++ )
	{
		T* p = reinterpret_cast<T*>(_pData[i]);

		if( p->name() == s )
			return p;
	}
	return NULL;
};

