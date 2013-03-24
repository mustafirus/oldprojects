#pragma once
#include "my_vector.h"

class MainWnd;
class DataContext;
class MenuItem {
public:
	enum Type { cmd, sep, mnu };
	MenuItem(Type t) : type(t){}
	virtual ~MenuItem(){}
	Type getType(){ return type; }
private:
	Type type;
};

class MenuCmd : public MenuItem {
public:
	enum{ main, dc }type;
	typedef void (MainWnd::*MainWndFN)(void);

private:
	string	_caption;
	UINT	_id;
	MainWndFN	_pfn;
public:

	MenuCmd( string	caption, MainWndFN pfn ) : MenuItem(cmd), _caption(caption), _id(-1), _pfn(pfn), type(main) {}
	~MenuCmd(){}
	string	caption() { return _caption; }
	UINT	id( UINT i = -1 ) { if( i != -1 ) _id = i; return _id; }
	void action();
	
	static MainWnd* getMainWnd();
	static DataContext* getDC();
};

class MenuSep : public MenuItem {
public:
	MenuSep() : MenuItem(sep) {}
	~MenuSep(){}
};

class Menu : public MenuItem {
	string	_caption;
	HMENU	_hMenu;
	vector<MenuItem*>	item;
	vector<MenuCmd*>	cmd;
//	void enumcmd(int n = 0, vector<MenuCmd*>& c = cmd);
	void	build(vector<MenuCmd*>& c);
public:
	Menu() : MenuItem(mnu), _hMenu(NULL) {};
	Menu(string caption) : MenuItem(mnu), _caption(caption), _hMenu(NULL){};
	~Menu();

	Menu* addMenu(string caption);
	void addSep();
	void addCmd(string s, MenuCmd::MainWndFN pfn);
	
	string	caption() { return _caption; }
	operator HMENU(){ build(cmd); return _hMenu; }
	bool isActive(UINT id){ return id >= minid &&  id < minid + cmd.count(); }
	void action(UINT id);
private:
	static UINT minid;
};

