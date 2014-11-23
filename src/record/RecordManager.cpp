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

    string primary_index = tableName + "/index_" + cm->get_primary(tableName) + ".db";
    im->createIndex("master.index", "int", 4, 0, {}, {}, {});
}

void RecordManager::deleteRecord(std::string tableName, int blocknum, int offset, int size) {
	std::string filename = tableName + "/" + master;
	recordBlock r( bm->readBlock(filename, blocknum) );
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
    int blockNum, offset;
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
    im->insertNode("master.index", newRecord.get_value(cm->get_primary(tableName)).to_str(table_column::INTTYPE) , blockNum, offset);
}

Record RecordManager::getRecord(std::string tableName, int blocknum, int offset, int size) {
	std::string filename = master + "/" + tableName;
	auto cat = cm->exist_relation(tableName);
	recordBlock r = bm->readBlock(filename, blocknum);
	return Record(r.getRecord(size, offset), cat->cols);
}

RecordManager::~RecordManager() {}
