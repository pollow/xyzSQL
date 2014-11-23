#include <iostream>
#include <string>
#include "BufferManager.h"
#include <cstdio>
#include <cstdlib>

int main() {
	BufferManager buf;
	//buf.createFile("new");

	//std::ofstream *y = new std::ofstream("testfstream");
	//y->seekp(100);
	//(*y) << "ff";
	//y->close();
	//return 0;
	////x.seekp(100);
	////x << "123";
	////x.close();
	//return 0;

	Block test;
	test.fillOne();

	//buf.createFile("db");
	//buf.createFile("index");
	//buf.createFile("catalog");

	buf.writeBlock("index", 0, test);
	buf.writeBlock("index", 0, test);
	buf.writeBlock("index", 2, test);
	buf.printQ();
	buf.writeBlock("db", 2, test);
	buf.printQ();
	for (auto i = 0; i < 32; i++) {
		buf.writeBlock("db", i, test);
		buf.printQ();
	}
	buf.writeBlock("db", 31, test);
	Block x = buf.readBlock("db", 31);
	std::cout << x.equal(test) << std::endl;
	buf.printQ();

	x = buf.readBlock("index", 0);
	std::cout << x.equal(test) << std::endl;
	buf.printQ();

	for (auto i = 0; i < 32; i++) {
		x = buf.readBlock("db", i);
		buf.printQ();
		std::cout << x.equal(test) << std::endl;
	}
	
	//buf.createFile("go");
	//buf.writeBlock("go", 2, test);
	buf.flushQ();
	system("pause");
	return 0;
}

