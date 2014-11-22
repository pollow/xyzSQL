#pragma once
#ifndef __RECORD_MANAGER_H__
#define __RECORD_MANAGER_H__

#ifdef GNNNG_CUSTOM
class catalog_manager;
class IndexManager;

#else
#include "../catalog/catalog.h"
#include "../parser/stmt.h"
#endif

#include <vector>
#include <cstdint>

#include "Record.h"
#include "../buffer/BufferManager.h"
#include "../buffer/Block.h"

class Cursor {
public:
	Cursor();
	~Cursor();
private:

};

class recordBlock : public Block {
public:
	//const static int headWidth = 4;
	recordBlock() {

	}

	recordBlock(Block& b) : Block(b) {
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

	bool insertRecord(Record r) {
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
			headOfRecord += 4;
			for (i = 0; i < r.size(); i++) {
				*reinterpret_cast<unsigned char *>(headOfRecord + i) = r.data[i];
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
	Record getRecord(int size, int offset) {
		char * headOfRecord = this->dataPointer() + offset;
#ifdef CAN_THROW
		if (*reinterpret_cast<std::uint32_t *>(headOfRecord) < 0xffffffff) {
			throw std::exception("Record Manager: the record has been deleted");
		}
#endif
		Record x;
		headOfRecord += 4;
		for (auto i = 0; i < size; i++) {
			x.data[i] = *reinterpret_cast<unsigned char *>(headOfRecord + i);
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

class IndexManager {

};

class RecordManager {
public:
	const static std::string master;
	RecordManager();
	void Init(BufferManager* BM, catalog_manager* CM, IndexManager * IM);
	~RecordManager();

	void createMaster(std::string tableName);
	void insertRecord(std::string tableName, Record newRecord);
	void deleteRecord(std::string tableName, int blocknum, int offset, int size);

	Record getRecord(std::string filename, int blocknum, int offset, int size);
	Cursor* getCursor(std::string filename, int blocknum, int offset);
private:
	BufferManager* bm;
	catalog_manager* cm;
	IndexManager* im;
};

#endif
 