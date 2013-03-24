#include "StdAfx.h"
#include "admsvc.h"

#include "Installer.h"
#include "Comp.h"

AdmSvc* AdmSvc::This = NULL;

AdmSvc::AdmSvc(string basepath): Service("AdmAgent"), _basepath(basepath), _mcfg(&_server, _cfg), _mupd(&_server, _basepath), _mver(&_server)
{
	if(This) throw ServiceException();
	This = this;
	memset(_plugins, MAX_PLUGINS, sizeof(Plugin*));
	load_plugins();
}

AdmSvc::~AdmSvc(void)
{
	unload_plugins();
}

void AdmSvc::run(void)
{
  Service::run();

  int port = 2121;

  XmlRpc::setVerbosity(5);

  // Create the server socket on the specified port
  _server.bindAndListen(port);

  // Enable introspection
  _server.enableIntrospection(true);

  // Wait for requests indefinitely
  while(!must_exit())
	_server.work(5.0);
}

void AdmSvc::load_plugins()
{
//	_plugins[0] = new Installer(&_server, &_cfg);
//	_plugins[1] = new Comp(&_server, &_cfg);
/*	HMODULE hLib = LoadLibrary( "Fuck" );
	pfGetPlugin GetPlugin = (pfGetPlugin)GetProcAddress( hLib,  GETPLUGIN_FUNC );
	fopen
	findfile
	string curdir;
	curdir += "\\plugins\\*.dll"
	intptr_t _findfirst64( curdir.c_str()  ,   struct __finddata64_t *fileinfo );
	_finddata_t
*/

}
void AdmSvc::unload_plugins(){
	delete _plugins[0];
	delete _plugins[1];
}

void ConfigureMethod::execute(XmlRpcValue& params, XmlRpcValue& result){
	XmlRpcValueStruct map(params[0]);
	for(XmlRpcValueStruct::iter it = map.begin(); it != map.end(); it++){
		_cfg[(*it).first] = (string)(*it).second;
	}
	result = "ok";
}
#define UPDATER "uphlp.exe"
void UpdateMethod::execute(XmlRpcValue& params, XmlRpcValue& result){
	bool updater = false;
	for(int i = 0, count = params.size(); i < count; i++ ){
		string name = params[i]["name"];
		if(name == UPDATER){
			name = _basepath + name;
		}else{
			name = _basepath +"update\\"+ name;
		}
		int size = params[i]["size"];
		{
			ofstream os(name.c_str(), ios_base::out | ios_base::trunc | ios_base::binary);
			if(!os.is_open())
				continue;
			XmlRpcValue val = params[i]["data"];
			XmlRpcValue::BinaryData& data = val;
			for(XmlRpcValue::BinaryData::const_iterator it = data.begin(); it != data.end(); it++){
				os << *it;
				size--;
			}
			os.close();
		}
		if(size)
			remove(name.c_str());
	}

	string uphlp = _basepath + "uphlp.exe";
	_spawnl(_P_DETACH, uphlp.c_str(), uphlp.c_str(), NULL);
}
