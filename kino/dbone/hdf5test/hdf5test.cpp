// hdf5test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <H5cpp.h>
#include <string.h>
using namespace H5;

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		H5File hf("test.h5", H5F_ACC_EXCL);//H5F_ACC_EXCL H5F_ACC_TRUNC
		Group gr = hf.openGroup("grp1");
	}catch(Exception e){
		/*		string s = e.getMajorString (H5E_major_t major_num);
		s = e.getMinorString (H5E_minor_t minor_num);
		*/
		std::string ss;
		ss = e.getDetailMsg ();
		const char* s1= e.getCDetailMsg();
		ss = e.getFuncName ();
		s1 = e.getCFuncName ();
		H5File hf("test.h5", H5F_ACC_TRUNC);//H5F_ACC_EXCL H5F_ACC_TRUNC
	}
/*	H5File hf("test.h5", H5F_ACC_TRUNC);//H5F_ACC_EXCL H5F_ACC_TRUNC
	Group gr = hf.createGroup("grp1");
*/
//	Group gr = hf.createGroup("grp1");
	return 0;
}

