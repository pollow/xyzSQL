#include "Block.h"
#include <cstring>

Block::Block() {
}

void Block::fillOne() {
	for (auto i = 0; i < BLOCKSIZE; i++) {
		data[i] = 0xff;
	}
}

void Block::fillZero() {
	std::memset(data, 0, BLOCKSIZE);
}

char * Block::dataPointer() {
	return reinterpret_cast<char *>(&data);
}

Block::~Block() {
}

bool Block::equal(Block &b) {
	bool f = true;
	for (auto i = 0; i < BLOCKSIZE; i++) {
		if (this->data[i] != b.data[i]) {
			f = false;
			break;
		}
	}
	return f;
}
