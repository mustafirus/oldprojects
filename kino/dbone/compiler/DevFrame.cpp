#include "StdAfx.h"
#include "devframe.h"

class FrameTyper : public DevFrame::FrameTyper{
public:
	short int gettype(char t){
		switch(t){
			case 0:
				return 0;
			case 'V':
				return 1;
			case 'H':
				return 2;
			case 'F':
				return 3;
			case 'L':
				return 4;
			default:
				return -1;
		}
	}
	char	  gettype(short int  t){
		switch(t){
			case 0:
				return 0;
			case 1:
				return 'V';
			case 2:
				return 'H';
			case 3:
				return 'F';
			case 4:
				return 'L';
			default:
				return -1;
		}
	}
}testtyper;

stack<DevFrame*> DevFrame::fstack;

DevFrame::FrameTyper* DevFrame::typer = &testtyper;

void DevFrame::begin_add_frame(string type){
	TRACE1("Begin '%s' frame\n",(const char*) type);
	DevFrame* p = new DevFrame(type[0]);
	if(!_root)
		_root = p;
	else
		fstack.get()->child.add(p);
	fstack.push(p); 
}
void DevFrame::end_add_frame(){
	TRACE1("End '%s' frame\n",(const char*) "");
	fstack.pop(); 
}
void DevFrame::add_field( string name ){
	TRACE1("Add field '%s'\n",(const char*) name);
	if(!_root)
		_root = new DevFrame('F');

	DevFrame* p = new DevFrame(name);
	fstack.get()->child.add(p);
}
DevFrame*	DevFrame::_root = NULL;


