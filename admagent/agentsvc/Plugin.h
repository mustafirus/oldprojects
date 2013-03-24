#pragma once

#define MAX_METHODS 10
class Config{
	virtual string get(string name)=0;
};
class Plugin
{
	friend class PluginMethod;
	friend class PluginSystem;

	XmlRpcServer*	_server;

	PluginMethod* methods[MAX_METHODS];
	int		methods_cnt;

protected:
	Config*			_cfg;
public:
	Plugin(XmlRpcServer* s, Config* cfg);
	virtual ~Plugin(void);
	operator XmlRpcServer*(){return _server;}
};

typedef void (Plugin::*Method)(XmlRpcValue& params, XmlRpcValue& result);

class PluginMethod : public XmlRpcServerMethod
{

	Plugin*		plugin;
	Method		method;
public:
	PluginMethod(Plugin* p, std::string const& n, Method m) : XmlRpcServerMethod(n, *p), plugin(p) ,method(m){
		plugin->methods[plugin->methods_cnt++]=this;
	}
	void execute(XmlRpcValue& params, XmlRpcValue& result){
		(plugin->*method)(params, result);
	}

};

#define REGMETHOD(X)	new PluginMethod(this, PLUGINNAME"."#X, (Method)X##_rpc)
#define DECMETHOD(X)	\
	void X##_rpc(XmlRpcValue& params, XmlRpcValue& result){ \
		X(params, result);\
	}

#define DECMETHOD0_1(X)	\
	void X##_rpc(XmlRpcValue& params, XmlRpcValue& result){ \
		result = X();\
	}
#define DECMETHOD1_0(X)	\
	void X##_rpc(XmlRpcValue& params, XmlRpcValue& result){ \
		X(params[0]);\
	}
#define DECMETHOD2_0(X)	\
	void X##_rpc(XmlRpcValue& params, XmlRpcValue& result){ \
		X(params[0], params[1]);\
	}

#define GETPLUGIN_FUNC "GetPlugin"
typedef Plugin* (*pfGetPlugin)(XmlRpcServer* s, Config* cfg);
