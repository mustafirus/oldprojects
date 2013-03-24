#pragma once
#include "my_vector.h"
class Frame;
class Wnd;
class DataContext
{
	Wnd*		_pWnd;
public:
	DataContext(){};
	~DataContext(){};
	virtual const char* name() = 0;

	virtual void setWnd(Wnd* pw){ _pWnd = pw; }
	virtual Wnd* getWnd(){ return _pWnd; }

	virtual Frame* getFrame() = 0;
	virtual bool isModified() = 0;
	virtual void save() = 0;
	virtual void load() = 0;
	virtual void action() = 0;
};

#include "Frame.h"
class TestDC : public DataContext
{
public:
	TestDC(){};
	~TestDC(){};
	const char* name()	{ return "test";	}
	Frame* getFrame();
	bool isModified()	{ return false;		}
	void save()			{					};
	void load()			{					};
	void action()		{					};
};

