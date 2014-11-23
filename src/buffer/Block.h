#pragma once
#ifndef __BLOCK_H__
#define __BLOCK_H__
#include <stdint.h>

#define BLOCKSIZE 4096

class Block {
public:
	Block();
	char *dataPointer();
	~Block();
	unsigned char getByte(int32_t i){return data[i];}
	void setByte(unsigned char c, int32_t i){data[i]=c;}
	void fillOne(); // for test
	void fillZero();
	bool equal(Block &);
private:
	unsigned char data[BLOCKSIZE];
};

#endif
