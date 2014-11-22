#pragma once
#include "Block.h"
#include <vector>
#include <fstream>

class File {
public:
	File();
	File(std::string filename);
	~File();
	Block* read(int);
	void write(int, Block*);
	bool isAlive();
	void open();
	void close();
	void load(std::string);
private:
	std::vector<Block> blocks; // for test only
	std::string name;
	bool alive;
	std::fstream* stream;
	int size;
};

