#include "StdAfx.h"
#include ".\registry.h"

void Registry::translit(char* s1){
	do{
		if(*s1 & 0x80)
			*s1 = ' ';
	}while(*++s1);
}
string Registry::translit(string& key){
	if(!(flags & RF_TRANSLIT))
		return key;
	char* buf = new char[key.length()*2];
	strcpy(buf,key.c_str());
	translit(buf);
	return buf;
}
void Registry::translit(DWORD t, LPBYTE lpData){
	if(!(flags & RF_TRANSLIT))
		return;
	if(t!=REG_SZ)
		return;
	translit((char*)lpData);
}

inline bool Registry::chk(DWORD ret){
	switch(ret){
		case ERROR_SUCCESS:
			return true;
		case ERROR_FILE_NOT_FOUND:
			if (flags & RF_NOT_THROW_NOT_FOUND) return false;
			throw err(err::not_found);
		default:
			throw fuckyou();
	}
}

DWORD					Registry::Value::map2w[16]={REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE,REG_NONE};
Registry::Value::Type	Registry::Value::map2n[16]={rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none,rt_none};

void Registry::Value::initmaps(){
	map2w[rt_none]=REG_NONE;
	map2w[rt_string]=REG_SZ;
	map2w[rt_int]=REG_DWORD;
	map2w[rt_int64]=REG_QWORD;
	
	map2n[REG_NONE]=rt_none;
	map2n[REG_SZ]=rt_string;
	map2n[REG_DWORD]=rt_int;
	map2n[REG_QWORD]=rt_int64;
}
Registry::Value::Value(DWORD t, LPBYTE lpData, DWORD cbData) : _t(map2n[t]), _i(0), _i64(0){
	switch(_t){
		case rt_string: _s = (char*)lpData; break;
		case rt_int: _i = (uint)*lpData; break;
		case rt_int64: _i64 = (uint64)*lpData; break;
		default:;
	}
}
Registry::Value::operator const BYTE*(){
	switch(_t){
		case rt_string:	return (const BYTE*)_s.c_str();
		case rt_int:	return (const BYTE*)&_i;
		case rt_int64:	return (const BYTE*)&_i64;
		default:		return NULL;
	}
};
string Registry::Value::tostring(){
	char buf[64];
	switch(_t){
		case rt_string:	return _s;
		case rt_int:	return _itoa(_i, buf, 10);
		case rt_int64:	return _i64toa(_i64, buf, 10);
		default:		return string();
	}
};

DWORD Registry::Value::size(){
	switch(_t){
		case rt_string:	return (DWORD)_s.length()+1;
		case rt_int:	return sizeof(_i);
		case rt_int64:	return sizeof(_i64);
		default:		return 0;
	}
};


Registry::Value Registry::get(string valname){
	DWORD cbData = 0;
	DWORD type = 0;
	DWORD ret = RegQueryValueEx(hkey, valname.c_str(),NULL,&type,0,&cbData);
	if(!chk(RegQueryValueEx(hkey, valname.c_str(),NULL,&type,0,&cbData)))
		return Value();
	LPBYTE lpData = new BYTE[cbData];
	chk(RegQueryValueEx(hkey, valname.c_str(),NULL,&type,lpData,&cbData));
	translit(type,lpData);
	return Value(type,lpData,cbData);
}
void Registry::set(string valname, Value val){
	chk(RegSetValueEx( hkey, valname.c_str(), 0, val,val,0));
}
void Registry::select(string& key){
	HKEY hk = NULL;
	chk( RegOpenKeyEx( hkey, key.c_str(), 0, KEY_ALL_ACCESS, &hk ) );
	hkey=hk;
	skey = translit(key);
}
void Registry::enum_subkeys(list<string> &keys){

	char	name[1024];
	DWORD	cname = 1024;
	LONG	ret=ERROR_SUCCESS;

	for (DWORD dwIndex = 0; ret==ERROR_SUCCESS; dwIndex++, cname = 1024){
		ret = RegEnumKeyEx( hkey, dwIndex, name, &cname, 0, NULL, 0, NULL);
		keys.push_back(name);
	}
//ERROR_NO_MORE_ITEMS
}