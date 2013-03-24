// Map.h: interface for the Map class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAP_H__CCEE1E12_363D_11D2_A701_204C4F4F5020__INCLUDED_)
#define AFX_MAP_H__CCEE1E12_363D_11D2_A701_204C4F4F5020__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

template <class KEY, class VALUE, int HASH_SIZE>
class Map
{
	class Assoc
	{
		Assoc*	next;
		KEY*	key;
	public:
		Assoc(KEY* k, VALUE* v) : next(0), key(k), value(v){};
		~Assoc(){if(value)delete value;if(next) delete next;};
		VALUE*	value;
		Assoc* Find(KEY* k){if(k==key)return this;if(next)return next->Find(k);else return NULL;};
		Assoc* Add(Assoc* a)
		{
			if(next)
				return next->Add(a);
			next = a;
			return a;
		};
	};
	Assoc* pData[HASH_SIZE];
	unsigned char GetHash(void* key) const;
public:
	Map(){memset(pData, 0, sizeof(pData));};
	~Map()
	{
		for(int i = 0; i < HASH_SIZE; i++)
			if(pData[i])
				delete pData[i];
	};
	void SetAt(KEY* key, VALUE* value);
	bool Lookup(KEY* key, VALUE*& rValue ) const;
};

template <class KEY, class VALUE, int HASH_SIZE>
unsigned char Map<KEY, VALUE, HASH_SIZE>::GetHash(void* key) const
{
		int k = (int) key;
		unsigned char c = 0;
		c += (char)k;
		k = k>>8;
		c += (char)k;
		k = k>>8;
		c += (char)k;
		k = k>>8;
		c += (char)k;
		c = c - c / HASH_SIZE * HASH_SIZE;
		return c;
}
template <class KEY, class VALUE, int HASH_SIZE>
void Map<KEY, VALUE, HASH_SIZE>::SetAt(KEY* key, VALUE* value)
{
	unsigned char i = GetHash(key);
	Assoc* pa;
	if(pData[i])
		pa = pData[i]->Find(key);
	else
	{
		pData[i] = new Assoc(key, value);
		return;
	}
	if(pa)
		pa->value = value;
	else
		pData[i]->Add(new Assoc(key, value));
}

template <class KEY, class VALUE, int HASH_SIZE>
bool Map<KEY, VALUE, HASH_SIZE>::Lookup(KEY* key, VALUE*& rValue ) const
{
	unsigned char i = GetHash(key);
	Assoc* pa;
	if(pData[i])
	{
		pa = pData[i]->Find(key);
		rValue = pa->value;
		return true;
	}
	else
		return false;
}
/////////////////////////////////////////////////////////////////
template <class V>
class delPtr{
public:
	void operator()(V val) { delete val; };
};
template <class V>
class noDel{
public:
	void operator()(V val) {};
};

template <class VALUE, int HASH_SIZE, class delptr = delPtr<VALUE> >
class stringMap
{
	class Assoc
	{
		Assoc*	next;
		const char*	key;
	public:
		Assoc(const char* k, VALUE v) : next(0), key(k), value(v){};
		~Assoc(){ if( value )delptr(value);if(next) delete next;};
		VALUE	value;
		Assoc* Find(const char* k){if(!strcmp(k,key))return this;if(next)return next->Find(k);else return NULL;};
		Assoc* Add(Assoc* a)
		{
			if(next)
				return next->Add(a);
			next = a;
			return a;
		};
	};
	Assoc* pData[HASH_SIZE];
/*
	template <class VALUE, int HASH_SIZE, bool autodel>
	unsigned char stringMap<VALUE, HASH_SIZE, autodel>::GetHash(const char* key) const
*/
	unsigned char GetHash(const char* key) const
	{
		unsigned int nHash = 0;
		unsigned int n = 0;
		while (*key)
			nHash += (nHash<<14) + *key++ ;
		nHash  = nHash % HASH_SIZE;
		return nHash;
	}
public:
	stringMap(){ASSERT(HASH_SIZE < 256);memset(pData, 0, sizeof(pData));};
	~stringMap()
	{
		for(int i = 0; i < HASH_SIZE; i++)
			if(pData[i])
				delete pData[i];
	};
/*
	template <class VALUE, int HASH_SIZE, bool autodel>
	void stringMap<VALUE, HASH_SIZE, autodel>::setAt(const char* key, VALUE value)
*/
//	template <class VALUE, int HASH_SIZE, bool autodel>
	void setAt(const char* key, VALUE value)
	{
		unsigned char i = GetHash(key);
		Assoc* pa;
		if(pData[i])
			pa = pData[i]->Find(key);
		else
		{
			pData[i] = new Assoc(key, value);
			return;
		}
		if(pa)
			pa->value = value;
		else
			pData[i]->Add(new Assoc(key, value));
	}
/*
	template <class VALUE, int HASH_SIZE, bool autodel>
	VALUE stringMap<VALUE, HASH_SIZE, autodel>::operator[](const char* key) const
*/
	VALUE operator[](const char* key) const
	{
		unsigned char i = GetHash(key);
		Assoc* pa;
		if(pData[i])
		{
			pa = pData[i]->Find(key);
			return pa ? pa->value : NULL;
		}
		else
			return NULL;
	}
};




#endif // !defined(AFX_MAP_H__CCEE1E12_363D_11D2_A701_204C4F4F5020__INCLUDED_)
