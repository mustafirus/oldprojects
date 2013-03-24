#pragma once
#include "plugin.h"



class Comp :
	public  Plugin
{
	string	getName();
	void	setName(string n);
	string	getDescription();
	void	setDescription(string d);
	string	getDomain();
	void	joinDomain(string d);
	int		getTZ();
	void	setTZ(int id);
	string	getSyncServer();
	void	setSyncServer(string s);
	string	getProxy();
	void	setProxy(string d);
	string	get();
	void	set(string d);
	DECMETHOD0_1(getName);
	DECMETHOD1_0(setName);
	DECMETHOD1_0(joinDomain);
public:
	Comp(XmlRpcServer* s, Config* cfg);
	virtual ~Comp();
};
