/*
 * IndexManager.h
 *
 *  Created on: 2014年11月7日
 *      Author: Admin
 */

#ifndef INDEXMANAGER_H_
#define INDEXMANAGER_H_
/***
* 1.选择操作
* 2.删除操作
* 3.插入数据操作
* 4.建立索引操作
* 5.删除索引操作
*
*
******/

#include <iostream>
#include <sstream>
#include <stdint.h>
#include <bitset>
#include <queue>
#include "../buffer/BufferManager.h"
#include "../buffer/Block.h"

#define BLOCKSIZE 4096
//#define maxBTreeNode 100
//#define max(a,b) {if((a)>(b)) return (a); return (b);}
using namespace std;

class indexIterator;

// class Block
// {
// public:
// 	Block(){}
// 	~Block(){}
// 	unsigned char getByte(int32_t i){return b[i];}
// 	void setByte(unsigned char c, int32_t i){b[i]=c;}
// 	void print(){// for test
// 		for(int i=0;i<4096;i++)
// 		{
// 			cout<<(unsigned int)(b[i])<<" ";
// 			if(i%16==15)
// 				cout<<endl;
// 			if(i%256==255)
// 				cout<<"|||"<<i+1<<endl;
// 		}
// 	}
// private:
// 	unsigned char b[4096];
// };
// 
// 
// class BufferManager
// {
// public:
// 	BufferManager(){}
// 	~BufferManager(){}
// 	Block readBlock(string fileName,int32_t i){return blocks[i];}
// 	void writeBlock(string fileName,int32_t i,Block &b){blocks[i]=b;}
// 	void createFile(string fileName){}
// 	void deleteFile(string fileName){}
// private:
// 	Block blocks[100];
// };


class blockAnalyzer
{
public:
	blockAnalyzer(Block b1,Block b2,BufferManager* myBfMgr){
		b[0]=b1;b[1]=b2;
		a=(b[1].getByte(4088)<<24)+(b[1].getByte(4089)<<16)+(b[1].getByte(4090)<<8)+b[1].getByte(4091);
		myBufferManager=myBfMgr;
	}
	virtual ~blockAnalyzer(){}
	int32_t getNextEmptyBlock()
	{
		int32_t i;
		unsigned char c;
		uint8_t d;
		for(i=a;;i=(i+1)%8180)
		{
			if(i>4095)
				c=b[1].getByte(i-4096);
			else
				c=b[0].getByte(i);
			bitset<8> bt(c);
			if(c!=0xFF)
			{
				if(i!=0){
					if(!bt[7])
						{c+=0x80;d=0; break;}
					else if(!bt[6])
						{c+=0x40;d=1; break;}
				}
				if(!bt[5])
					{c+=0x20;d=2;break;}
				else if(!bt[4])
					{c+=0x10;d=3;break;}
				else if(!bt[3])
					{c+=0x08;d=4;break;}
				else if(!bt[2])
					{c+=0x04;d=5;break;}
				else if(!bt[1])
					{c+=0x02;d=6;break;}
				else if(!bt[0])
					{c+=0x01;d=7;break;}
			}
		}
		if(i>4095)
			b[1].setByte(c,i-4096);
		else b[0].setByte(c,i);
		a=i;
		return (a<<3)+d;
	}
	int32_t getRootPosition()
	{
		int32_t r;
		r=(b[1].getByte(4092)<<24)+(b[1].getByte(4093)<<16)+(b[1].getByte(4094)<<8)+b[1].getByte(4095);
		return r;
	}
	void changeRootPos(int32_t Position)
	{
		b[1].setByte(Position>>24,4092);
		b[1].setByte(Position>>16,4093);
		b[1].setByte(Position>>8,4094);
		b[1].setByte(Position,4095);
	}
	void deleteBlock(int32_t blockPos)
	{
		uint8_t tmp=0;
		uint8_t tmp1=blockPos&0x7;
		unsigned char c;
		if(blockPos>32767)
		{
			blockPos-=32768;
			tmp=1;
		}
		c=b[tmp].getByte(blockPos>>3);
		bitset<8> bt(c);
		bt[7-tmp1]=0;
		c=(unsigned char)bt.to_ulong();
		b[tmp].setByte(c,blockPos>>3);

	}
	void writeBack(string fileName)
	{
		b[1].setByte(a>>24,4088);
		b[1].setByte(a>>16,4089);
		b[1].setByte(a>>8,4090);
		b[1].setByte(a,4091);
		myBufferManager->writeBlock(fileName,0,b[0]);
		myBufferManager->writeBlock(fileName,1,b[1]);
	}
private:
	int32_t a;
	Block b[2];
	BufferManager* myBufferManager;
};


class treeNode
{
public:
	treeNode(BufferManager* bfmgr,string type,int32_t charLen,int32_t blockNum,bool root, bool leaf)
	{
		myBufferManager=bfmgr;
		nodeSize=BLOCKSIZE;
		Root=root;
		Leaf=leaf;
		columnType=type;
		this->charLen=charLen;
		if(type=="int")
		{
			maxNumber=nodeSize/12-5;
			keyInt=new int[maxNumber];
			keyFloat=NULL;
			keyStr=NULL;
		}
		if(type=="float")
		{
			maxNumber=nodeSize/12-5;
			keyFloat=new float[maxNumber];
			keyInt=NULL;
			keyStr=NULL;
		}
		if(type=="char")
		{
			maxNumber=nodeSize/(charLen+8)-5;
			keyStr=new string[maxNumber];
			keyInt=NULL;
			keyFloat=NULL;
		}
		children=new int32_t[maxNumber+1];
		if(Leaf)
			blockOffset=new int32_t[maxNumber+1];
		else blockOffset=NULL;
		blockNumber=blockNum;
		keyNumber=0;
		children[maxNumber]=0;
	}

	treeNode(BufferManager* bfmgr,Block b);

	virtual ~treeNode()
	{
		delete[] children;
		delete[] blockOffset;
		if(!keyInt)
			delete[] keyInt;
		if(!keyFloat)
			delete[] keyFloat;
		if(!keyStr)
			delete[] keyStr;
	}
	bool compare(string condition,int32_t i);
	int deleteElement(blockAnalyzer* analyzer,string fileName,string value,string &newKey);
	void deleteFromLeaf(int32_t i);
	void deleteFromNonLeaf(int32_t i);
	bool exist(string value);
	bool enough();
	bool enoughGive();
	int32_t next();
	int findPos(string condition);
	int32_t getBlock(string condition);
	int32_t getBlockNumber(){return blockNumber;}
	int32_t getBlockOffset(int32_t i){return blockOffset[i];}
	int32_t getCharLen(){return charLen;}
	int32_t getChild(int32_t i){return children[i];}
	string getColumnType(){return columnType;}
	string getKey(int32_t i);
	int32_t getKeyNumber(){return keyNumber;}
	int getLeaf(indexIterator &it,string fileName,string condition,int condType);
	int getLeftestLeaf(indexIterator &it,string fileName);
	int32_t getLinkedLeafNode();
	bool isLeaf(){return Leaf;}
	int insert(blockAnalyzer* analyzer,string fileName,string value,int32_t recordBlockNumber,int32_t recordBlockOffset, int32_t& newBlockNumber,string& newValue);
	void merge(treeNode* t1, treeNode*t2,string keyValue);
	void moveForward();
	void moveBackward();
	void moveKeyBackward(int32_t i);
	void moveKeyForward(int32_t i);
	void print(){//for test
		cout<<"nodeSize"<<nodeSize<<endl;
		cout<<"blockNumber"<<blockNumber<<endl;
		cout<<"maxNumber"<<maxNumber<<endl;
		cout<<"keyNumber"<<keyNumber<<endl;
		cout<<"columnType"<<columnType<<endl;
		cout<<"charLen"<<charLen<<endl;
		cout<<"Leaf"<<Leaf<<endl;
		cout<<"Root"<<Root<<endl;

	}

	void setBlockOffset(int32_t i,int32_t offset);
	void setChild(int32_t i,int32_t blockNum);
	void setKey(int32_t i,string value);
	void setKeyNumber(int32_t i){keyNumber=i;}
	void setLinkedLeafNode(int32_t blockNum);
	void setRoot(bool state){Root=state;}
	Block transform();
	void writeBack(string fileName);
private:
	int32_t nodeSize;
	int32_t blockNumber;

	int32_t maxNumber;
	int32_t keyNumber;
	string columnType;
	int32_t charLen;
	string *keyStr;
	int *keyInt;
	float *keyFloat;

	int32_t *children;
	int32_t *blockOffset;
		
	bool Leaf;
	bool Root;

	BufferManager* myBufferManager;
};


class IndexManager
{
public:
	IndexManager(BufferManager* bfmngr){
		myAnalyzer=NULL;
		myBufferManager=bfmngr;
	}
	virtual ~IndexManager(){delete myAnalyzer;}
	int selectNode(indexIterator &iterator,string fileName, int  condType ,string condition);
	int insertNode(string fileName, string value, int32_t recordBlockNumber, int32_t recordBlockOffset);
	int deleteNode(string fileName, string value);
	void createIndex(string fileName,string colType,int32_t charLen,int32_t number,string value[], int32_t blockNumber[], int32_t blockOffset[]);
	void dropIndex(string fileName);
	int getStarter(indexIterator &it,string fileName){
		string tmp="";
		selectNode(it,fileName,3,tmp);
		return 0;
	}
	void print(string fileName){
		Block tmpBlock,tmpBlock2;
		queue<int32_t> myQueue;
		treeNode *tmpNode;
		if(lastFile!=fileName)
		{
			if(myAnalyzer!=NULL){
				myAnalyzer->writeBack(fileName);
				delete myAnalyzer;
			}
			tmpBlock=myBufferManager->readBlock(fileName,0);
			tmpBlock2=myBufferManager->readBlock(fileName,1);
			myAnalyzer=new blockAnalyzer(tmpBlock,tmpBlock2,myBufferManager);

			cout<<"maybe"<<endl;
			getchar();
		}
		int32_t blockPos=myAnalyzer->getRootPosition();
		myQueue.push(blockPos);
		while(myQueue.size()!=0)
		{
			int32_t tmp1=myQueue.front();
			myQueue.pop();
			tmpBlock=myBufferManager->readBlock(fileName,tmp1);
			tmpNode=new treeNode(myBufferManager,tmpBlock);
			for(int32_t i=0;i<tmpNode->getKeyNumber();i++)
				cout<<tmpNode->getChild(i)<<" "<<tmpNode->getKey(i)<<" ";
			if(tmpNode->isLeaf())
				cout<<"   "<<tmpNode->getLinkedLeafNode();
			else
				cout<<tmpNode->getChild(tmpNode->getKeyNumber());

			cout<<endl;
			if(!tmpNode->isLeaf())
				for(int32_t j=0;j<=tmpNode->getKeyNumber();j++)
					myQueue.push(tmpNode->getChild(j));
			//cout<<"round"<<endl;
			//getchar();

		}

	}
	Block newIndexHead(){
		Block b;
		b.setByte(0x20,0);
		return b;
	}
	Block newIndexHead2(){
		Block b;
		b.setByte(0x02,4095);
		return b;
	}
	//void analyzeIndex(Block* tmpBlock,string& colType, int32_t& charLen,int32_t& blockPos);

private:
	string lastFile;
	blockAnalyzer* myAnalyzer;
	BufferManager *myBufferManager;

};


class indexIterator
{
public:
	indexIterator(){}

	~indexIterator(){}
	int next(int32_t& t1,int32_t& t2)
	{
		if(i<node->getKeyNumber())
		{
			t1=node->getChild(i);
			t2=node->getBlockOffset(i);
			i++;
		}
		else
		{
			int32_t a=node->getLinkedLeafNode();
			if(a!=0)
			{
				delete node;
				node=new treeNode(myBufferManager,myBufferManager->readBlock(fileName,a));
				t1=node->getChild(0);
				t2=node->getBlockOffset(0);
				i=1;
			}
			else
				return -1;
		}
		return 0;
	}

	void set(string f,BufferManager* b,treeNode* t,int32_t it){
		node=t;
		fileName=f;
		myBufferManager=b;
		i=it;
	}


private:
	treeNode* node;
	int32_t i;
	string fileName;
	BufferManager* myBufferManager;
};


#endif /* INDEXMANAGER_H_ */

