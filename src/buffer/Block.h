#pragma once
#define BLOCKSIZE 4096

class Block {
public:
	Block();
	char *dataPointer();
	~Block();

	void fillOne(); // for test
	bool equal(Block &);
private:
	unsigned char data[BLOCKSIZE];
};

