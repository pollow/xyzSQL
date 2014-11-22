#include "Block.h"


Block::Block() {
}

void Block::fillOne() {
	for (auto i = 0; i < BLOCKSIZE; i++) {
		data[i] = 0xff;
	}
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