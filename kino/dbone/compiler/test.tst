domain int[
	pr1 => val1,
	pr2 => val2
]{
	test1(){{c++
	int a = 1;
		if(a)
			return 1;
		return -1;
	}};
	test2(){{c++
	int a = 1;
	}};

}