#include "StdAfx.h"
#include "comp.h"

#define PLUGINNAME "comp"

wstring mtow(string s){
	wchar_t* w = new wchar_t[(s.length()+1)*2];
	mbstowcs( w, s.c_str(), s.length()+1 );
	wstring ww = w;
	delete w;
	return ww;
}

Comp::Comp(XmlRpcServer* s, Config* cfg) : Plugin(s, cfg){
	REGMETHOD(getName);
	REGMETHOD(setName);
	REGMETHOD(joinDomain);
	
}

Comp::~Comp(void)
{
}
string	Comp::getName(){
	DWORD sz = 256;
	TCHAR buf[256];
	if (GetComputerName( buf, &sz ))
		return buf;
	return "";
}

void	Comp::setName(string n){
	BOOL b = SetComputerName(n.c_str());
}

string	Comp::getDescription(){
/*	DWORD sz = 256;
	TCHAR buf[256];
	if(GetComputerObjectName( NameDisplay, buf, &sz ))
		return buf;*/
	return "";
}
void Comp::setDescription(string d){
}
string	Comp::getDomain(){
	return "";
}
void Comp::joinDomain(string d){
//	wstring ws = d;
	DWORD ret = NetJoinDomain( NULL, mtow(d).c_str(), NULL, NULL, NULL, 
		NETSETUP_JOIN_DOMAIN | NETSETUP_ACCT_CREATE);
}
int	Comp::getTZ(){
	return 0;
}
void Comp::setTZ(int id){
}
string	Comp::getSyncServer(){
	return "";
}
void Comp::setSyncServer(string s){
}
string Comp::getProxy(){
	return "";
}
void Comp::setProxy(string d){
}
