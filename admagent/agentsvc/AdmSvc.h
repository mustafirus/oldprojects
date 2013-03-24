#pragma once
#include "service.h"
#include "Plugin.h"

#define MAX_PLUGINS 10
#define ADMSVC_VERSION	"1.0"

typedef map<string,string> ConfigHash;
class UpdateMethod : public XmlRpcServerMethod
{
	const string&		_basepath;
public:
	UpdateMethod(XmlRpcServer* s, string& basepath) : XmlRpcServerMethod("update", s), _basepath(basepath){}
	void execute(XmlRpcValue& params, XmlRpcValue& result);

};
class ConfigureMethod : public XmlRpcServerMethod
{
	ConfigHash&		_cfg;
public:
	class XmlRpcValueStruct : public XmlRpcValue {
	public:
		typedef XmlRpcValue::ValueStruct::iterator iter;
		XmlRpcValueStruct(XmlRpcValue& val):XmlRpcValue(val){};
		iter begin()	{assertStruct();	return (*_value.asStruct).begin();}
		iter end()		{assertStruct();	return (*_value.asStruct).end();}
	};
	ConfigureMethod(XmlRpcServer* s, ConfigHash& cfg) : XmlRpcServerMethod("configure", s), _cfg(cfg){}
	void execute(XmlRpcValue& params, XmlRpcValue& result);
};
class VersionMethod : public XmlRpcServerMethod
{
public:
	VersionMethod(XmlRpcServer* s) : XmlRpcServerMethod("version", s){}
	void execute(XmlRpcValue& params, XmlRpcValue& result){
		result = ADMSVC_VERSION;
	};
};

class AdmSvc : protected  Service
{
	class Config : public ::Config
	{
		ConfigHash	cfg;
	public:
		string get(string name){
			return cfg[name];
		};
		operator ConfigHash&(){return cfg;}
	};
	string			_basepath;
	XmlRpcServer	_server;
	Plugin*			_plugins[MAX_PLUGINS];
	HMODULE			_libs[MAX_PLUGINS];
	Config			_cfg;
	void			load_plugins();
	void			unload_plugins();

	ConfigureMethod	_mcfg;
	UpdateMethod	_mupd;
	VersionMethod	_mver;

public:
	AdmSvc(string basepath);
	~AdmSvc(void);
	virtual void run();
/*	virtual void shutdown(){
		delete this;
//		_server.exit();
	};*/
	static AdmSvc* This;
};
