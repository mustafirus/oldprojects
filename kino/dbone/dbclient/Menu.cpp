#include "StdAfx.h"
#include "Application.h"
#include "DataContext.h"
#include "MainWnd.h"
#include "menu.h"

//	hMenu  = CreateMenu();
//	AppendMenu( hMenu, MF_STRING, 1, "item" );
//	AppendMenu( hMenu, MF_STRING, 1, "item" );
//	AppendMenu( hMenu, MF_STRING, 1, "item" );

MainWnd* MenuCmd::getMainWnd() {
	MainWnd* p = dynamic_cast<MainWnd*> ( Application::getApp()->getMainWnd() );
	ASSERT(p);
	return 	p;
}

DataContext* MenuCmd::getDC() {
	FrameWnd* pfw = getMainWnd()->getActiveChild();
	ASSERT(pfw);
	DataContext* pdc = pfw->context();
	ASSERT(pdc);
	return 	pdc;
}

void MenuCmd::action(){
	switch(type){
		case main: 
			(getMainWnd()->*(_pfn))();
			break;
		case dc: 
			getDC()->action();
			break;
	}
}

UINT Menu::minid = 0;

Menu::~Menu(){
	item.delete_all();
}

Menu* Menu::addMenu(string name){
	Menu* m = new Menu(name);
	item.add(m);
	return m;
}
void Menu::addSep(){
	item.add(new MenuSep);
}
void Menu::addCmd(string s, MenuCmd::MainWndFN pfn){
	item.add(new MenuCmd(s, pfn));
}

void Menu::build(vector<MenuCmd*>& vc) {
	if (!_hMenu)
		_hMenu  = CreateMenu();
	for (int i = 0; i < item.count(); i++){
		MenuItem* it = item[i];
		switch(it->getType()){
			case MenuItem::cmd: {
				MenuCmd* c = dynamic_cast<MenuCmd*>(it);
				c->id(minid + vc.count());
				vc.add(c);
				AppendMenu( _hMenu, MF_STRING, c->id(), c->caption() );
								}
				break;
			case MenuItem::sep:
				AppendMenu( _hMenu, MF_SEPARATOR, 0, "" );
				break;
			case MenuItem::mnu: {
				Menu* m = dynamic_cast<Menu*>(it);
				m->build(vc);
				AppendMenu( _hMenu, MF_POPUP, (UINT_PTR) m->_hMenu, m->caption() );
								}
				break;
			default:
				ASSERT(0);
		}
	}
}

void Menu::action(UINT id){
	cmd[(int)id]->action(); //MenuCmd::getMainWnd()->(
}
