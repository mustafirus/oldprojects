#pragma once

class User{
	string			username;
	string			password;
	string			homedir;
};
class Record;
class RKey;
class Session{
	typedef vector<Record*>	Records;
	User*			_pUser;
	Records			_vRecords;
	void			send(int size, void*);
	void			recv(int& size, void*);
public:
	Session(){};
	Record* getRecord(RKey* prk){
		View* pv = _pApplication->getView(prk);
		if(!pv)
			return NULL;
		if(list())
			pr = new Recordset(pv, prk);
		else
			pr = new Records(pv, prk);
		return pr;
	};
	void run();
};

class Application
{
	vector<Session*> _vSessions;
public:
	Application(void);
	virtual ~Application(void);
	void run();
};
