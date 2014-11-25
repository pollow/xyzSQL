#include "RecordManager.h"

const std::string RecordManager::master = "master.db";
const std::string RecordManager::trash = "trash.tmp";

RecordManager::RecordManager(){
    bm = nullptr;
    cm = nullptr;
    im = nullptr;
    trashFile = nullptr;
    cursor = nullptr;
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
	recordBlock r( bm->readBlock(filename, blocknum) );
	r.deleteRecord(size, offset);
	bm->writeBlock(filename, blocknum, r);
}

void RecordManager::insertRecord(std::string tableName, Record newRecord, int& blockNum, int& offset) {
	std::string filename = tableName + "/" + master;
	// insert into file - block using free list
    // 
	bool finish = false;
	recordBlock b = bm->readBlock(filename, 0);
	bm->printQ();
	recordBlock d;
	int i;
	for (i = 0; i < b.getBlockCount(); i++) {
		d = bm->readBlock(filename, i + 1);
		finish = d.insertRecord(newRecord, offset);
		if (finish) {
			bm->writeBlock(filename, i + 1, d);
            blockNum = i+1;
			break;
		}
	}
	if (!finish) {
		recordBlock newBlock;
		newBlock.Init(newRecord.size());
		newBlock.insertRecord(newRecord, offset);
		bm->writeBlock(filename, b.getBlockCount() + 1, newBlock);
        blockNum = b.getBlockCount() + 1;
		b.setBlockCount(b.getBlockCount() + 1);
		bm->writeBlock(filename, 0, b);
	}

	// update index
}

Record RecordManager::getRecord(std::string tableName, int blocknum, int offset, int size) {
	std::string filename = tableName + "/" + master;
	auto cat = cm->exist_relation(tableName);
	recordBlock r = bm->readBlock(filename, blocknum);
	return Record(r.getRecord(size, offset), cat->cols);
}

Cursor* RecordManager::getCursor(std::string tableName, int size) {
    std::string filename = tableName + "/" + master;
    recordBlock start = bm->readBlock(filename, 0);
    if (cursor != nullptr) {
        delete cursor; 
        cursor = nullptr;
    }
    cursor = new Cursor(bm, cm, tableName, 1, 0, size, start.getBlockCount());
    return cursor;
}

RecordManager::~RecordManager() {}
