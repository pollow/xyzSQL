#pragma once
#ifndef __RECORD_MANAGER_H__
#define __RECORD_MANAGER_H__


#include "record.h"
#include "../catalog/catalog.h"
#include "../parser/stmt.h"

#include <vector>
#include <cstdint>
#include <fstream>
#include "../buffer/BufferManager.h"
#include "../buffer/Block.h"
#include "../index/IndexManager.h"
//#include "../catalog/catalog.h"

class Cursor;

class recordBlock : public Block {
public:
	//const static int headWidth = 4;
	recordBlock() {

	}

	recordBlock(const Block& b) : Block(b) {
	};

	void Init(int size) {
		char * headOfRecord;
		size += 4; // first int used to save pointer or flag
		uint32_t j = size;
		for (auto i = 0; i < BLOCKSIZE / size - 1; i++) {
			headOfRecord = this->dataPointer() + i * size;
			*reinterpret_cast<std::uint32_t *>(headOfRecord) = j;
			j += size;
		}
		headOfRecord = this->dataPointer() + (BLOCKSIZE / size - 1) * size;
		*reinterpret_cast<std::uint32_t *>(headOfRecord) = 0;
	}

	//inline std::uint32_t getRecordSize() {
	//	return *reinterpret_cast<std::uint32_t *>(this->dataPointer());
	//}
	//inline std::uint32_t getRecordCount() {
	//	return *reinterpret_cast<std::uint32_t *>(this->dataPointer() + 4);
	//}

	bool insertRecord(Record r, int &offset) {
		char * headOfRecord = nullptr;
		int size = r.size() + 4; // first int used to save pointer or flag
		uint32_t j = size;
		bool ret = false;

		int i = *reinterpret_cast<std::uint32_t *>(this->dataPointer());
		if (i == 0) { // empty free list, full block
			ret = false;
		} else {
			headOfRecord = this->dataPointer() + i;
			uint32_t next = *reinterpret_cast<std::uint32_t *>(headOfRecord);
			*reinterpret_cast<std::uint32_t *>(this->dataPointer()) = next;
			*reinterpret_cast<std::uint32_t *>(headOfRecord) = 0xffffffff;
			offset = i;
			headOfRecord += 4;
			for (i = 0; i < r.size(); i++) {
				*reinterpret_cast<unsigned char *>(headOfRecord + i) = r.buf[i];
			}
			ret = true;
		}
		return ret;
	}

	void deleteRecord(int size, int offset) {
		char * headOfRecord = nullptr;
		std::uint32_t size_r = size + 1;
		uint32_t next = *reinterpret_cast<std::uint32_t *>(this->dataPointer());
		*reinterpret_cast<std::uint32_t *>(this->dataPointer()) = offset;
		headOfRecord = this->dataPointer() + offset;
		*reinterpret_cast<std::uint32_t *>(headOfRecord) = next;
	}
	
    bool isValid(int offset) {
        char * headOfRecord = this->dataPointer() + offset;
        return (*reinterpret_cast<std::uint32_t *>(headOfRecord) == 0xffffffff);
    }    

	std::vector<unsigned char> getRecord(int size, int offset) {
		char * headOfRecord = this->dataPointer() + offset;
#ifdef CAN_THROW
		if (*reinterpret_cast<std::uint32_t *>(headOfRecord) < 0xffffffff) {
			throw std::exception("Record Manager: the record has been deleted");
		}
#endif
		std::vector<unsigned char> x;
		headOfRecord += 4;

		for (auto i = 0; i < size; i++) {
			x.push_back(*reinterpret_cast<unsigned char *>(headOfRecord + i));
		}

		return x;
	}

	std::uint32_t getFreelist() {
		return getBlockCount();
	}

	void setFreelist(std::uint32_t c) {
		setBlockCount(c);
	}

	void setBlockCount(std::uint32_t c) {
		*reinterpret_cast<std::uint32_t *>(this->dataPointer()) = c;
	}

	std::uint32_t getBlockCount() {
		return *reinterpret_cast<std::uint32_t *>(this->dataPointer());
	}

private:

};

//class headerBlock : public Block {
//public:
//	headerBlock () {
//	}
//	
//	headerBlock(Block &block):Block(block) {
//		
//	}
//};

class RecordManager {
public:
	const static std::string master;
	const static std::string trash;

	RecordManager();
	void Init(BufferManager* BM, catalog_manager* CM, IndexManager * IM);
	~RecordManager();

	void createMaster(std::string tableName);
	void insertRecord(std::string tableName, Record newRecord, int&, int&);
	void deleteRecord(std::string tableName, int blocknum, int offset, int size);

	Record getRecord(std::string tableName, int blocknum, int offset, int size);
	Cursor* getCursor(std::string tableName, int size);
	
private:
	BufferManager* bm;
	catalog_manager* cm;
	IndexManager* im;
	std::fstream *trashFile;
    Cursor *cursor;
};

class Cursor {
public:
	Cursor() {
        filename = "";
        blockNum = 0;
        offset = 0;
        size = 0;
        maxBlockCount = 0;
        bm = nullptr;
        cm = nullptr;
    };

    Cursor(BufferManager *bm,  catalog_manager *cm, std::string tableName, int blockNum, int offset, int size, int maxBlockCount){
        this->tableName = tableName;
        this->filename = tableName + "/" + RecordManager::master;
        this->blockNum = blockNum;
        this->offset = offset;
        this->size = size;
        this->maxBlockCount = maxBlockCount;
        this->bm = bm;
        this->cm = cm;
        this->endFlag = false;
	block = bm->readBlock(filename, blockNum);	
    }; 

	~Cursor() {

    };


    bool next() {
        int i = blockNum;
        int j = offset;
        int maxRecordCount = BLOCKSIZE / (size + 4)- 1;
        int maxOffset = maxRecordCount * (size + 4);
 //       bool finish = false;
        while(i <= maxBlockCount && j <= maxOffset) {
            j += (size + 4);
            if (block.isValid(j)) {
//                finish = true;
                break;
            }
            if (j > maxOffset) {
                j = 0;
                i = i + 1;
                if (i <= maxBlockCount) {
                    block = bm->readBlock(filename, i);
                }
            }
        }
        if (i > maxBlockCount) {
            return false;
        }
        blockNum = i;
        offset = j;
        return true;
    }

    Record getRecord() {
        auto catm = cm->exist_relation(tableName);
        return Record(block.getRecord(size, offset), catm->cols);
    }
    
private:
    BufferManager* bm;
    catalog_manager* cm;
    std::string tableName;
    std::string filename;
    int blockNum;
    int offset;
    int size;
    int maxBlockCount;
    recordBlock block;
    bool endFlag;
};


#endif
 
