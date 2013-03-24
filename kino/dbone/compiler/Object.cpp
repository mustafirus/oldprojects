#include "StdAfx.h"
#include "object.h"


class TestInit{
public:
	TestInit(){
		Object dom(Object::domain, "int", string());
		dom.addParam("pr1", "val1");
		dom.addParam("pr2", "val2");
		dom.addComponent( new Method("test1", "\tint a = 1;\n\t\tif(a)\n\t\t\treturn 1;\n\t\treturn -1;") );
		dom.addComponent( new Method("test2", "\tint a = 1;") );
		string str;
		dom.tostring(str);
		FILE* f = fopen("test.tst","w");
		fprintf(f, str);
		fclose(f);
	};
};

TestInit testinit;