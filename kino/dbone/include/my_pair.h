template < class T1, class T2 >
class pair{
	T1 _name;
	T2 _val;
public:
	pair(){};
	pair( T1 t1, T2 t2 ) : _name(t1), _val(t2){};
	pair& operator=( pair& p ){ _name = p._name; _val = p._val; return *this; }
	bool operator==( string n ){ return _name == n; }
	T1 name(){return _name;}
	T2 val(){return _val;}
};