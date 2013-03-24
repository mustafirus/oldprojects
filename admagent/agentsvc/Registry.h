#pragma once

#ifdef AGENTLIB_IMP
#define DLEXP __declspec( dllexport ) 
#else
#define DLEXP __declspec( dllimport ) 
#endif

typedef unsigned int uint;
typedef unsigned __int64 uint64;
#define RF_NOT_THROW_NOT_FOUND	0x00000001
#define RF_TRANSLIT				0x00000010
class DLEXP Registry
{
	HKEY	hkey;
	string	skey;
	inline bool chk(DWORD ret);
	void select(string& key);
	void translit(char* s1);
	string translit(string& key);
	void translit(DWORD t, LPBYTE lpData);
public:
	typedef list<string> Keys;
	typedef Keys::iterator Iter;
	Registry(HKEY hk, string s = 0, uint f = 0)	: hkey(hk), flags(f)		{select(s);};
//	Registry(Registry& reg)			: hkey(reg.hkey), flags(0)	{};
	~Registry()													{RegCloseKey(hkey);};

	class DLEXP Value{
		enum Type { rt_none=0, rt_string=1, rt_int=2, rt_int64=3 };
		Type	_t;
		string	_s;
		uint	_i;
		uint64	_i64;
	public:
		Value(DWORD t, LPBYTE lpData, DWORD cbData);
		Value()				:_t(rt_none),	_i(0), _i64(0)			{};
		Value(string s)		:_t(rt_string), _s(s), _i(0), _i64(0)	{};
		Value(uint i)		:_t(rt_int),	_i(i), _i64(0)			{};
		Value(uint64 i64)	:_t(rt_int64),	_i(0), _i64(i64)		{};
		operator DWORD()			{return map2w[_t];};
		operator string()			{return _s;};
		operator const BYTE*();
		string tostring();
		DWORD size();

		static void initmaps();
	private:
		static DWORD		Registry::map2w[16];
		static Value::Type	Registry::map2n[16];
	};

	class err{
	public:
		enum _type{not_found};
		typedef _type type;
		err(type e):_e(e){};
		bool operator==(type e){return _e == e;};
		bool operator!=(type e){return _e != e;};
	private:
		_type _e;
	};

	uint	flags;
	Value get(string valname);
	void set(string valname, Value val);
	void enum_subkeys(list<string> &keys);
	operator string(){return skey;}
	operator HKEY(){return hkey;}
};
