#pragma once
#include "plugin.h"

#ifdef PLUGINNAME
#undef PLUGINNAME
#endif
#define PLUGINNAME "system"

class PluginSystem :
	public Plugin
{
public:

	PluginSystem(XmlRpcServer* s, Config* cfg) : Plugin(s,cfg){
		REGMETHOD(update);
		REGMETHOD(configure);
	}
	DECMETHOD1_0(update);
	DECMETHOD(configure);
	virtual ~PluginSystem(void){}
	void update(string s){
		
	}
	void configure(XmlRpcValue& params, XmlRpcValue& result){
	}
};
