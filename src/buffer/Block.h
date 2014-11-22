#pragma once
#ifndef __BLOCK_H__
#define __BLOCK_H__

#define BLOCKSIZE 4096

class Block {
public:
	Block();
	char *dataPointer();
	~Block();

	void fillOne(); // for test
	void fillZero();
	bool equal(Block &);
private:
	unsigned char data[BLOCKSIZE];
};

#endif