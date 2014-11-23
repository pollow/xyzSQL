#include "BufferManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

const std::string BufferManager::dataFileDir = "data/";
const std::string BufferManager::trash = "trash.tmp";

BufferManager::BufferManager() {
	//load();
}

void BufferManager::load() {
	
}

void BufferManager::createFile(std::string filename) {
	//TODO not check exists file
	files.insert(std::pair<std::string, File*>(filename, new File(dataFileDir + filename)));
}

BufferManager::~BufferManager() {

}

File* BufferManager::findFile(std::string filename) {
	std::string path = dataFileDir + filename;
	auto f = files.find(filename);
	File *file = nullptr;
	if (f != files.end()) {
		file = f->second;
	} else {
		File *n = new File();
		n->load(path);
		files.insert(std::pair<std::string, File*>(filename, n));
		file = n;
	}
	return file;
}

BufferRecord *BufferManager::findQ(std::string filename, int index) {
	time++;
	if (time > MAXTIME)
		retimeQ();
	BufferRecord *br = nullptr;
	for (auto i = bufQueue.begin(); i != bufQueue.end(); i++) {
		if (i->equal(filename, index)) {
			br = &(*i);
			//i->accessTime++; wrong
			i->accessTime = time;
		//	std::cout << filename << " " << index << " " << "hit" << std::endl;
			break;
		}
	}
	return br;
}

BufferRecord *BufferManager::insertQ(std::string filename, int num, Block* b) {
	unsigned int minTime = MAXTIME;
	auto old = bufQueue.begin();
	BufferRecord *ret = nullptr;
	if (bufQueue.size() < BUFSIZE) { // not full
		BufferRecord newOne;
		newOne.filename = filename;
		newOne.blocknum = num;
		newOne.accessTime = time;
		newOne.block = b;
		bufQueue.push_back(newOne);
		ret = &bufQueue[bufQueue.size() - 1];
	} else { // full
		for (auto i = bufQueue.begin(); i != bufQueue.end(); i++) {
			if (i->accessTime < minTime) {
				minTime = i->accessTime;
				old = i;
			}
		}
		File *f = nullptr;
		if (old->dirty) {
			f = findFile(old->filename);
			f->write(old->blocknum, old->block);			
		}
		f = findFile(filename);
		delete old->block;
		old->filename = filename;
		old->blocknum = num;
		old->block = f->read(num);
		old->dirty = false;
		old->accessTime = time;
		ret = &(*old);
	}
	return ret;
}

Block BufferManager::readBlock(std::string filename, int index) {
	std::string path = dataFileDir + filename;
	BufferRecord *record = findQ(filename, index);
	if (!record) {
		File* f = findFile(filename);
		record = insertQ(filename, index, f->read(index));
	}
	return *(record->block);

	// end
	//auto f = files.find(filename);
	//if (f != files.end()) {
	//	// find in files
	//	return f->second->read(index);
	//} else {
	//	// check disk
	//	File *n = new File();
	//	n->load(filename);
	//	if (n->isAlive()) {
	//		files.insert(std::pair<std::string, File*>(filename, n));
	//		return n->read(index);
	//	}
	//}
	
}


void BufferManager::writeBlock(std::string filename, int index, Block& block) {
	std::string path = dataFileDir + filename;
	BufferRecord *record = findQ(filename, index);
	if (!record) {
		File* f = findFile(filename);
		//TODO no need to read f, can use a blank block
		Block *blank = new Block;
		record = insertQ(filename, index, blank);
	}
	*(record->block) = block;
	record->dirty = true;

	//std::string fname = dataFileDir + filename;
	//auto f = files.find(fname);
	//if (f != files.end()) {
	//	// find in files
	//	f->second->write(index, block);
	//	return;
	//} else {
	//	// check disk
	//	File *n = new File();
	//	n->load(fname);
	//	if (n->isAlive()) {
	//		files.insert(std::pair<std::string, File*>(filename, n));
	//		n->write(index, block);
	//		return;
	//	}
	//}
	//std::cout << "Not write anything" << std::endl;
	///*files[filename]->write(index, block);*/
}

void BufferManager::flushQ() {
	File *f = nullptr;
	for (auto i = bufQueue.begin(); i != bufQueue.end(); i++) {
		f = findFile(i->filename);
		f->write(i->blocknum, i->block);
	}
	bufQueue.clear();
}

bool bufCompare(const BufferRecord& a, const BufferRecord& b) {
	return a.accessTime < b.accessTime;
}

void BufferManager::retimeQ() {
	time = 0;
	std::sort(bufQueue.begin(), bufQueue.end(), bufCompare);
	for (auto i = bufQueue.begin(); i != bufQueue.end(); i++) {
		i->accessTime = time++;
	}
}

void BufferManager::newTrashCan() {
	trashFile = new std::fstream(trash, ios::in | ios::out | ios::trunc);
}

void BufferManager::appendTrashCan(int blocknum, int offset) {
	(*trashFile) << blocknum << "  " << offset << "\n";
}

void BufferManager::Init(IndexManager *im) {
	this->im = im;
}
void BufferManager::emptyTrashCan() {
	int end = trashFile->tellp();
	trashFile->seekp(0);
	int b, c;
	while (trashFile->tellp() < end) {
		// TODO
	}
}
