// Installer.cpp: implementation of the Installer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Installer.h"
#include "registry.h"

Installer::Installer(XmlRpcServer* s, Config* cfg) : Plugin(s, cfg){
	REGMETHOD(list);
	REGMETHOD(remove);
	REGMETHOD(install);
}

void Installer::list_rpc(XmlRpcValue& params, XmlRpcValue& result)
{
	Registry reg(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	Registry::Keys keys;
	reg.enum_subkeys(keys);
	string out;
	int n = 0;
	for ( Registry::Iter it = keys.begin(); it != keys.end( ); it++, n++ ){
		Registry r(reg, *it, RF_NOT_THROW_NOT_FOUND | RF_TRANSLIT);
		result[n]["Key"] = (string)r;
		result[n]["DisplayName"] = (string) r.get("DisplayName");
		result[n]["ProductID"] = (string) r.get("ProductID");
		result[n]["WindowsInstaller"] = (int)(uint) r.get("WindowsInstaller");
	}
	return;
}
void aaa(){
#if 0
	char code[64];
	char name[256];
	DWORD name_len = sizeof(name);
	UINT ret=0;
	DWORD i;
	string s1;
	for(i = 0, ret = 0; ret == ERROR_SUCCESS; i++){
		ret = MsiEnumProducts( i, code );
		*name = 0;
		
		MsiGetProductInfo( code, "ProductName", //INSTALLPROPERTY_INSTALLEDPRODUCTNAME
			name, &name_len);
		s1 = s1 + code + "\t" + name + "\n";
	}
//	return s1;
#endif
}
void Installer::remove(string code)
{
	DWORD mode = 
		REINSTALLMODE_PACKAGE
		| REINSTALLMODE_FILEOLDERVERSION
		| REINSTALLMODE_USERDATA
		| REINSTALLMODE_MACHINEDATA
		| REINSTALLMODE_SHORTCUT;
	MsiConfigureProduct( code.c_str(), 0, INSTALLSTATE_ABSENT );
}
void Installer::install(string path, string props)
{
	MsiInstallProduct( path.c_str(), props.c_str() );
}
#if 0

const int Installer::cmdsz = 1024;
#define PROPSLEN 2048
bool checklen(char* s1, char* s2, int len)
{
	int l1 = s1 == NULL ? 0 : strlen(s1);
	int l2 = s2 == NULL ? 0 : strlen(s2);
	return l1+l2<len;
}

bool Installer::getprod(char* buf, char* prod, char* props, char* code, DWORD code_size)
{
	char* prodname = strtok(buf, " ");
	char* addprops = strtok(NULL, "");
	
	if (!prodname ) return false;
	if(!checklen(srcdir, prodname, _MAX_PATH-strlen(".msi")))return false;
	if(!checklen(defprops, addprops, PROPSLEN))return false;

	sprintf(prod, "%s%s.msi", srcdir, prodname);
	if (props){
		strcpy(props, defprops);
		if (addprops)
			strcat(props, addprops);
	}
	if(code){
		MSIHANDLE hProduct = NULL;
		UINT ret = ERROR_SUCCESS;
		ret = MsiOpenPackage( prod, &hProduct );
		if(ret != ERROR_SUCCESS) return false;

		ret = MsiGetProductProperty( hProduct,
			"ProductCode", code, &code_size);
		if(ret != ERROR_SUCCESS) return false;
		ret = MsiCloseHandle( hProduct );
		if(ret != ERROR_SUCCESS) return false;
	}

	return true;
}

void Installer::check_msi_ret(UINT ret) {
	if(ret == ERROR_SUCCESS) {
		sock->send("Ok\n");
	}else {
		sock->sendf("...error 0x%x\n", ret);
	}
}

void Installer::run()
{
	while(!!*sock)
	{
		*cmd=0;
		sock->getline(cmd, cmdsz);
		switch(*cmd)
		{
		case 'i':
			{
				char prod[_MAX_PATH];
				char props[PROPSLEN];
				if (!getprod(cmd+1,prod, props)) break;
				sock->sendf("installing %s %s\n", prod, props);
				check_msi_ret(MsiInstallProduct( prod, props ));
			}
			break;
		case 'u':
			{
				char prod[_MAX_PATH];
				char code[64];
				if (!getprod(cmd+1,prod, NULL, code, sizeof(code))) break;
				sock->sendf("updating %s\n", prod);
				DWORD mode = 
					REINSTALLMODE_PACKAGE
					| REINSTALLMODE_FILEOLDERVERSION
					| REINSTALLMODE_USERDATA
					| REINSTALLMODE_MACHINEDATA
					| REINSTALLMODE_SHORTCUT;
				check_msi_ret(MsiReinstallProduct( code, mode ));
			}
			break;
		case 'r':
			{
				char prod[_MAX_PATH];
				char code[64];
				if (!getprod(cmd+1,prod, NULL, code, sizeof(code))) break;
				sock->sendf("removing %s\n", prod);
				DWORD mode = 
					REINSTALLMODE_PACKAGE
					| REINSTALLMODE_FILEOLDERVERSION
					| REINSTALLMODE_USERDATA
					| REINSTALLMODE_MACHINEDATA
					| REINSTALLMODE_SHORTCUT;
				check_msi_ret( MsiConfigureProduct( code, 0, INSTALLSTATE_ABSENT ) );
			}
			break;
		case 'l':
			{
				char code[64];
				char name[64];
				DWORD name_len = sizeof(name);
				UINT ret=0;
				DWORD i;
				for(i = 0, ret = 0; ret == ERROR_SUCCESS; i++){
					ret = MsiEnumProducts( i, code );
					*name = 0;
					
//					MsiGetProductInfo( code, INSTALLPROPERTY_LOCALPACKAGE,
//						name, &name_len);
					MsiGetProductInfo( code, INSTALLPROPERTY_INSTALLEDPRODUCTNAME,
						name, &name_len);
					sock->sendf("%d\t%s\t%s\n", i, code, name);
				}
			}
			break;
		case 'q':
			return;
		case '?':
		case 'h':
			sock->send(
				"i <product> [props] - install product\n"
				"u <product> - update product\n"
				"r <product> - remove product(uninstall)\n"
				"l - list of installed products\n"
				"p - list of default properties for all installations(not implemented yet)\n"
				"p <prop list> - set default properties for all installations(not implemented yet)\n"
				"q - quit\n"
				"h | ? - help\n"
				);
			break;
		default:
			sock->send("Unknown command. Press h | ? for help\n");
		}
	}
}
#endif
