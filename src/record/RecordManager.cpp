#include "RecordManager.h"


const std::string RecordManager::master = "master.db";
const std::string RecordManager::trash = "trash.tmp";
RecordManager::RecordManager(){

}

void RecordManager::Init(BufferManager* BM, catalog_manager* CM, IndexManager * IM) {
	bm = BM;
	cm = CM;
	im = IM;
}

void RecordManager::createMaster(std::string tableName) {
	bm->createFile(tableName + "/" + master);
	recordBlock b;
	b.fillZero();
	b.setBlockCount(0);
	bm->writeBlock(tableName + "/" + master, 0, b);
	bm->printQ();
}

void RecordManager::deleteRecord(std::string tableName, int blocknum, int offset, int size) {
	std::string filename = tableName + "/" + master;
	recordBlock r = bm->readBlock(filename, blocknum);
	r.deleteRecord(size, offset);
	bm->writeBlock(filename, blocknum, r);
}

void RecordManager::insertRecord(std::string tableName, Record newRecord) {
	std::string filename = tableName + "/" + master;
	/*catalog* cat = cm->exist_relation(tableName);
	auto cols = cat->cols;*/

	// construct a new record
	//Record newRecord;
	//unsigned char *s = nullptr;
	//for (auto i = 0; i < cols->size(); i++) {
	//	s = reinterpret_cast<unsigned char *>(&vals[i].value);
	//	switch (cols->at(i)->data_type) {
	//	case attrClass::INTTYPE:
	//		newRecord.data.push_back(s[0]);
	//		newRecord.data.push_back(s[1]);
	//		newRecord.data.push_back(s[2]);
	//		newRecord.data.push_back(s[3]);
	//		break;
	//	case attrClass::FLOATTYPE:
	//		newRecord.data.push_back(s[0]);
	//		newRecord.data.push_back(s[1]);
	//		newRecord.data.push_back(s[2]);
	//		newRecord.data.push_back(s[3]);
	//		break;
	//	case attrClass::CHARTYPE:
	//		while (*s) {
	//			newRecord.data.push_back(*s);
	//			s++;
	//		}
	//		newRecord.data.push_back(0);
	//		break;
	//	}
	//}

	// insert into file - block using free list
	bool finish = false;
	recordBlock b = bm->readBlock(filename, 0);
	bm->printQ();
	recordBlock d;
	int i;
	for (i = 0; i < b.getBlockCount(); i++) {
		d = bm->readBlock(filename, i + 1);
		finish = d.insertRecord(newRecord);
		if (finish) {
			bm->writeBlock(filename, i + 1, d);
			break;
		}
	}
	if (!finish) {
		recordBlock newBlock;
		newBlock.Init(newRecord.size());
		newBlock.insertRecord(newRecord);
		bm->writeBlock(filename, b.getBlockCount() + 1, newBlock);
		b.setBlockCount(b.getBlockCount() + 1);
		bm->writeBlock(filename, 0, b);
	}

	// update index
}

Record RecordManager::getRecord(std::string tableName, int blocknum, int offset, int size) {
	std::string filename = master + "/" + tableName;
	recordBlock r = bm->readBlock(filename, blocknum);
	return r.getRecord(size, offset);
}

void RecordManager::newTrashCan() {
	trashFile = new std::fstream(trash);
}

void RecordManager::appendTrashCan(int blocknum, int offset) {
	(*trashFile) << blocknum << "  " << offset << "\n";
}

void RecordManager::emptyTrashCan() {
	int end = trashFile->tellp();
	trashFile->seekp(0);
	while (trashFile->tellp() < end) {
		//im->del....;
	}
}

RecordManager::~RecordManager() {}
