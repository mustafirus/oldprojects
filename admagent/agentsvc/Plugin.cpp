#include "stdafx.h"
#include "plugin.h"


Plugin::Plugin(XmlRpcServer* s, Config* cfg): _server(s), _cfg(cfg), methods_cnt(0){
};
Plugin::~Plugin(){
	for(int i = 0; i<methods_cnt; i++)
		delete methods[i];
};


