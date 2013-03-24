#pragma once

struct FrameData{
	short int next_brother;
	short int first_child;
	short int type;
	short int fieldnum;
};

class DevFrame{
	short int type;
	vector<DevFrame*> child;
	string		name;
public:
	class FrameTyper{
	public:
		virtual short int gettype(char t) = 0;
		virtual char	  gettype(short int  t) = 0;
	};
	
	DevFrame(FrameData* buf, short int num) : type(buf[num].type){
		for( int n = buf[num].first_child; n != 0; n = buf[n].next_brother ){
			child.add( new DevFrame( buf, n ) );
		}
	};
	DevFrame( char t ) {
		type = typer->gettype( t );
	};
	DevFrame( string sname ) : name(sname) {
		type = typer->gettype( (char)0 );
	};
	~DevFrame(){};
	int size(){
		for ( int i = 0, n = 1; i < child.count(); i++ ){
			n += child[i]->size();
		}
		return n;
	}
	int dump( FrameData* buf, int start = 0 ){
		buf[start].type = type;
		buf[start].first_child = buf[start].next_brother = 0;

		for ( int i = 0, cur = start, next = start + 1; i < child.count(); i++ ){
			if (i == 0){
				buf[cur].first_child = next;
			}else{
				buf[cur].next_brother = next;
			}
			cur = next;
			next = child[i]->dump(buf, cur);
		}
		return next;
	}

	char* dump( char* buf ){

		if( type == 0){
			*buf = 0;
			strcpy(buf, name);
			return buf + strlen(buf);
		}


		*buf++ = '<';
		char t = typer->gettype( type );
		*buf++ = t ? t : '?';
		*buf++ = ' ';

		for ( int i = 0; i < child.count(); i++ ){
			buf = child[i]->dump(buf);
		}
		*buf++ = ' ';
		*buf++ = '>';
		*buf=0;
		return buf;
	}

	static void begin_add_frame(string type);
	static void end_add_frame();
	static void add_field( string name );
	static stack<DevFrame*> fstack;
	static FrameTyper* typer;
	static DevFrame*	_root;
};

