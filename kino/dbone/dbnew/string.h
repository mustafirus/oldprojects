#pragma once

class string
{
	struct data
	{
		data(int n, char* s) : cnt(n), str(s){};
		int		cnt;
		char*	str;
	};
	data*	_buf;
	void set(char* s){
		if(_buf && --_buf->cnt) _buf = NULL;
		if(!_buf){

			_buf = new data(1, s);
			return;
		}
		if(_buf->str){
			free(_buf->str);
		};
		_buf->str = s;
		_buf->cnt++;
	}
public:
	string(){ _buf = NULL; };
	string(string& s){ (_buf = s._buf) && _buf->cnt++; };
	string(const char* s){
		_buf = new data(1, strdup(s));
	}
	~string(void){
		if(_buf && !--_buf->cnt)	{
			free(_buf->str); delete _buf;
		}
	};

	int length(){ return (_buf && _buf->str) ? (int)strlen(_buf->str) : 0; }
	
	string& operator=(string s){
		if(_buf && !--_buf->cnt)	{
			free(_buf->str); delete _buf;
		}
		(_buf = s._buf) && _buf->cnt++;
		return *this;
	};

	string& operator=(const char* s){
		set(strdup(s));
		return *this;
	}

	string& setint(__int64 n, int b){
		char * s1 = (char*) malloc(b);
		sprintf(s1, "t%I64d", n);
		set(s1);
		return *this;
	}

	string& operator=(int n){
		return setint(n, 16);
	}

	string& operator=(__int64 n){
		return setint(n, 32);
	}

	string& cat(const char* s){
		size_t newlen = ( (_buf && _buf->str) ? strlen(_buf->str) : 0) + strlen(s) + 1;
		char * s1 = (char*) malloc(newlen);
		if (_buf && _buf->str)
			strcpy(s1, _buf->str);
		else
			*s1 = 0;
		strcat(s1, s);
		set(s1);
		return *this;
	}
	string& operator<<(const char* s){
		return cat(s);
	}
	string& operator<<(string& s){
		return cat(s);
	}
/*	string& operator+=(string& s){
		return cat(s);
	}*/
	bool operator==(string& s){
		return _buf == s._buf || _buf->str == s._buf->str || (_buf->str && s._buf->str && !strcmp(_buf->str, s._buf->str));
	}
	operator const char* (){return _buf->str;}
	string left(char sep = '.'){
		char* s1 = strchr(_buf->str, sep);
		if(s1)	*s1++=0;
		string left = _buf->str;
		operator=(s1);
		return left;
	}
	bool empty(){
		return !(_buf && _buf->str && *_buf->str);
	}
};

template < class T >
class list{
	list*	_next;
	T		_data;
public:
	list(T d) : _next(NULL), _data(d){};
	list(T d, char sep) : _next(NULL), _data(d.left(sep)){ if(d.empty())return; _next = new list(d, sep);};
	~list() { if (_next) delete _next;};
	T& add(T d){
		if ( _data == d )
			return _data;
		if(_next) _next->add(d);
		else _next = new list(d);
		return _next->_data;
	};
	void add(list<T>* p){
		if(_next) _next->add(p);
		else _next = p;
	};
	list* next(){ return _next; }
	operator T&(){ return _data;};
	T& operator->(){ return _data;};
};

/*inline strlist* split(strlist& l, string& s, char sep){
	s1.split(s)
	strlist l = new strlist()
	return new strlist(s);
}
*/