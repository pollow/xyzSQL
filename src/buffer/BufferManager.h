#pragma once
#ifndef __BUFFER_MANAGER_H__
#define __BUFFER_MANAGER_H__
#include <string>
#include <vector>
#include <map>
#include "Block.h"
#include "File.h"
#include <iostream>

#define BUFSIZE 32
#define	MAXTIME 0xffffffff
//#define MAXTIME 9 // for test only

class BufferRecord {
public:
	BufferRecord() {
		blocknum = -1;
		accessTime = 0;
		block = nullptr;
		filename = "";
		dirty = false;
	}

	~BufferRecord() {
		//if (block)
		//	delete block;
	}

	inline bool equal(std::string filename, int num) {
		return this->filename == filename && this->blocknum == num;
	}

	int blocknum;
	std::string filename;
	unsigned int accessTime;
	Block * block;
	bool dirty;
};

class BufferManager {
public:
	static const std::string dataFileDir;
	BufferManager();
	void load();
	~BufferManager();
	Block readBlock(std::string, int);
	void writeBlock(std::string, int, Block&);
	void createFile(std::string);
	BufferRecord* findQ(std::string, int);
	File* findFile(std::string);
	BufferRecord* insertQ(std::string, int, Block*);
	void flushQ();
	void retimeQ();
	// for test only
	void printQ() {
		std::cout << "=======================begin buffer =================\n";
		for (auto i = bufQueue.begin(); i != bufQueue.end(); i++) {
			std::cout << i->dirty << " " << i->accessTime << " " << i->filename << " " << i->blocknum << std::endl;
		}
		std::cout << "=======================end buffer =================\n";
	}

	void newTrashCan();
	void appendTrashCan(int blocknum, int offset);
	void emptyTrashCan();

private:
	std::map<std::string, File *> files;
	std::vector<BufferRecord> bufQueue;
	/*Block bufData[BUFSIZE];*/
	unsigned int time;
};

#endif
