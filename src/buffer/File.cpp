#define _CRT_SECURE_NO_DEPRECATE
#include "File.h"
#include <cstdio>
#include <iostream>
#include <fstream>

File::File() {
	alive = false;
	stream = nullptr;
}

File::File(std::string filename) : name(filename) {
	File();
	std::FILE *file;
	file = std::fopen(filename.c_str(), "r");
	if (!file){
		file = std::fopen(filename.c_str(), "w");
		if (!file) {
			std::string s = "Can not create '" + filename + "'";
			//throw std::exception(s.c_str());
		}
	} else {
		std::string s = "Can not create file '" + filename + "': file already exists";
		//throw std::exception(s.c_str());
	}
	if (file) {
		alive = true;
		std::fclose(file);
	}
}

bool File::isAlive() {
	return alive;
}

File::~File() {
}

Block* File::read(int num) {
	// for test 
	open();
	//if (num >= size || num < 0) {
	//	throw std::exception("Block number overflow");
	//}
	Block* newOne = new Block;
	stream->seekg(BLOCKSIZE * num);
	stream->read(newOne->dataPointer(), BLOCKSIZE);
	close();
	return newOne;
}

void File::write(int num, Block* update) {
	open();
	if (num >= size) {
		size = num;
	}
	stream->seekp(BLOCKSIZE * num);
	//std::cout << "write at ";
	//std::cout << stream->tellp() << std::endl;
	stream->write(update->dataPointer(), BLOCKSIZE);
	stream->flush();
	//stream->seekp(0, std::ios::end);
	//std::cout << stream->tellp() << std::endl;
	close();
	// for test
	//std::cout << blocks.size() << std::endl;
	//if (num >= static_cast<int>(blocks.size()))
	//	blocks.resize(num + 1);
	//blocks[num] = update;
	//std::cout << blocks.size() << std::endl;
}

void File::open() {
	if (!alive) {
		//throw std::exception(std::string("Error in opening file: '" + name + "'").c_str());
	}
		
	stream = new std::fstream(name, std::ios::in | std::ios::out | std::ios::binary);
	//std::cout << "Open file "<< std::endl;
}

void File::close() {
	if (!alive || stream == nullptr){
		//throw std::exception(std::string("Error in closing file: '" + name + "'").c_str());
	}
	stream->close();
	delete stream;
}

void File::load(std::string filename) {
	std::FILE *file;
	file = std::fopen(filename.c_str(), "r");
	if (!file) {
		//throw std::exception(std::string("Can not load file '" + filename + "'").c_str());
	}
	
	alive = true;
	name = filename;
}