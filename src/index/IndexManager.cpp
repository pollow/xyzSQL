/*
* IndexManager.cpp
*
*  Created on: 2014年11月7日
*      Author: Admin
*/

#include "IndexManager.h"
#include <sstream>
#include <stdint.h>
#include <cmath>
using namespace std;

int it=0;
treeNode::treeNode(BufferManager* bfmgr,Block b)
{
	myBufferManager=bfmgr;
	nodeSize=BLOCKSIZE;
	blockNumber=(b.getByte(0)<<24)+(b.getByte(1)<<16)+(b.getByte(2)<<8)+b.getByte(3);
	maxNumber=(b.getByte(4)<<24)+(b.getByte(5)<<16)+(b.getByte(6)<<8)+b.getByte(7);
	keyNumber=(b.getByte(8)<<24)+(b.getByte(9)<<16)+(b.getByte(10)<<8)+b.getByte(11);

	unsigned char c=b.getByte(12);
	if(c=='f')
		columnType="float";
	else if(c=='i')
		columnType="int";
	else if(c=='c')
		columnType="char";

	Root=(bool)b.getByte(13);
	Leaf=(bool)b.getByte(14);
	charLen=(b.getByte(16)<<24)+(b.getByte(17)<<16)+(b.getByte(18)<<8)+b.getByte(19);

	if(columnType=="char")
	{
		keyStr=new string[maxNumber];
		keyInt=NULL;
		keyFloat=NULL;
	}
	else if(columnType=="int")
	{
		keyStr=NULL;
		keyInt=new int[maxNumber];
		keyFloat=NULL;
	}
	else if(columnType=="float")
	{
		keyStr=NULL;
		keyInt=NULL;
		keyFloat=new float[maxNumber];
	}
	if(Leaf)
		blockOffset=new int32_t[maxNumber+1];
	else blockOffset=NULL;
	children=new int32_t[maxNumber+1];

	int32_t j=20;
	for(int32_t i=0;i<keyNumber;i++)
	{
		if(columnType=="char")
		{
			for(int32_t k=0;;k++)
			{
				unsigned char c=b.getByte(j+i*(charLen+1)+k);
				if(c==0) break;
				else keyStr[i].append(1,c);
			}
		}
		if(columnType=="int")
			keyInt[i]=(b.getByte(j+i*4)<<24)+(b.getByte(j+4*i+1)<<16)+(b.getByte(j+4*i+2)<<8)+b.getByte(j+4*i+3);

		if(columnType=="float")
		{
			int a=(b.getByte(j+4*i)<<24)+(b.getByte(j+4*i+1)<<16)+(b.getByte(j+4*i+2)<<8)+b.getByte(j+4*i+3);
			keyFloat[i]=*(float *)&a;
		}
	}
	if(columnType=="char")
		j=j+maxNumber*(charLen+1);
	else
		j=j+maxNumber*4;

	for(int32_t i=0;i<keyNumber+1;i++)
		children[i]=(b.getByte(j+4*i)<<24)+(b.getByte(j+4*i+1)<<16)+(b.getByte(j+4*i+2)<<8)+b.getByte(j+4*i+3);
	if(Leaf)
		children[maxNumber]=(b.getByte(j+4*maxNumber)<<24)+(b.getByte(j+4*maxNumber+1)<<16)+(b.getByte(j+4*maxNumber+2)<<8)+b.getByte(j+4*maxNumber+3);

	j=j+(maxNumber+1)*4;
	if(Leaf)
		for(int32_t i=0;i<keyNumber+1;i++)
			blockOffset[i]=(b.getByte(j+4*i)<<24)+(b.getByte(j+4*i+1)<<16)+(b.getByte(j+4*i+2)<<8)+b.getByte(j+4*i+3);

}

//return true, when condition>=key[i]
bool treeNode::compare(string condition,int32_t i)
{
	stringstream ss;
	int n;
	float f;
	if(columnType=="char")
		return condition>=keyStr[i];
	if(columnType=="int")
	{
		ss<<condition;
		ss>>n;
		return n>=keyInt[i];
	}
	if(columnType=="float")
	{
		ss<<condition;
		ss>>f;
		return f>=keyFloat[i];
	}
}


//find the proper position in treeNode structure to insert condition
int treeNode::findPos(string condition)
{
	int i;
	for(i=0;i<keyNumber;i++)
	{
		if(compare(condition,i))
			continue;
		break;
	}
	return i;
}


//if the value exist in the array of keys in treeNode, return true
bool treeNode::exist(string value)
{
	stringstream ss;
	ss<<value;
	int n;
	float f;
	ss>>n;
	ss.clear();
	ss<<value;
	ss>>f;
	for(int32_t i=0;i<keyNumber;i++)
	{
		if(columnType=="char"&&keyStr[i]==value){//cout<<value<<endl;
			return true;}
		if(columnType=="int"&&keyInt[i]==n){
			return true;}
		if(columnType=="float"&&keyFloat[i]==f){
			return true;}
	}
	return false;
}


// if the key is enough, return true
bool treeNode::enough()
{
	if(Root&&Leaf)
		return true;
	if(Root)
		return keyNumber>=1;
	if(Leaf)
	{
		if(keyNumber>=ceil((double)maxNumber/2))
			return true;
	}
	else if(keyNumber>=ceil((double)(maxNumber+1)/2)-1)
		return true;
	return false;
}


//if the node is enough to give a value or child
bool treeNode::enoughGive()
{
	if(Root&&Leaf)
		return true;
	if(Root)
		return keyNumber>=2;
	if(Leaf)
	{
		if(keyNumber>=1+ceil((double)maxNumber/2))
			return true;
	}
	else if(keyNumber>=ceil((double)(maxNumber+1)/2))
		return true;
	return false;
}


//move i'th key back 0->1
void treeNode::moveKeyBackward(int32_t i)
{
	if(columnType=="char")
		keyStr[i+1]=keyStr[i];
	if(columnType=="int")
		keyInt[i+1]=keyInt[i];
	if(columnType=="float")
		keyFloat[i+1]=keyFloat[i];
}


//move i'th key forward 0<-1
void treeNode::moveKeyForward(int32_t i)
{
	if(columnType=="char")
		keyStr[i-1]=keyStr[i];
	if(columnType=="int")
		keyInt[i-1]=keyInt[i];
	if(columnType=="float")
		keyFloat[i-1]=keyFloat[i];
}


//get i'th key, output as string
string treeNode::getKey(int32_t i)
{
	stringstream ss;
	string result;
	if(columnType=="char")
		return keyStr[i];
	if(columnType=="int")
	{
		ss<<keyInt[i]; ss>>result; return result;
	}
	if(columnType=="float")
	{
		ss<<keyFloat[i]; ss>>result; return result;
	}
}


//set the value to i'th key position
void treeNode::setKey(int32_t i,string value)
{
	stringstream ss;
	int n;
	float f;
	if(columnType=="char")
	{
		keyStr[i]= value;
	}
	if(columnType=="int")
	{
		ss<<value; ss>>n; keyInt[i]=n;
	}
	if(columnType=="float")
	{
		ss<<value; ss>>f; keyFloat[i]=f;
	}
}


//set the value to i'th child position
void treeNode::setChild(int32_t i,int32_t blockNum)
{
	children[i]=blockNum;
}


//delete key[i], children[i], blockOffset[i] from leaf node
void treeNode::deleteFromLeaf(int32_t i)
{
	for(int32_t tmp=i;tmp<keyNumber-1;tmp++)
	{
		moveKeyForward(tmp+1);
		children[tmp]=children[tmp+1];
		blockOffset[tmp]=blockOffset[tmp+1];
	}
	keyNumber--;
}


//delete key[i],children[i+1] from nonleaf node
void treeNode::deleteFromNonLeaf(int32_t i)
{
	for(int32_t tmp=i+1; tmp<=keyNumber-1;tmp++)
	{
		moveKeyForward(tmp);
		children[tmp]=children[tmp+1];
	}
	keyNumber--;
}


//move everything forward 0<-1
void treeNode::moveForward()
{
	if(Leaf)
		for(int32_t tmp=0;tmp<keyNumber-1;tmp++)
		{
			moveKeyForward(tmp+1);
			children[tmp]=children[tmp+1];
			blockOffset[tmp]=blockOffset[tmp+1];
		}
	else
	{
		for(int32_t tmp=0;tmp<keyNumber-1;tmp++)
		{
			moveKeyForward(tmp+1);
			children[tmp]=children[tmp+1];
		}
		children[keyNumber-1]=children[keyNumber];
	}
	keyNumber--;
}


//move everything backward 1<-0
void treeNode::moveBackward()
{
	if(Leaf)
	{
		for(int32_t i=keyNumber-1;i>=0;i--)
		{
			moveKeyBackward(i);
			children[i+1]=children[i];
			blockOffset[i+1]=blockOffset[i];
		}
	}
	else
	{
		children[keyNumber+1]=children[keyNumber];
		for(int32_t i=keyNumber-1;i>=0;i--)
		{
			moveKeyBackward(i);
			children[i+1]=children[i];
		}
	}
	keyNumber++;
}


//set the value to i'th blockOffset position
void treeNode::setBlockOffset(int32_t i,int32_t offset)
{
	blockOffset[i]=offset;
}


//get linked leafNode
int32_t treeNode::getLinkedLeafNode()
{
	return children[maxNumber];
}


//set linked leafNode
void treeNode::setLinkedLeafNode(int32_t blockNumber)
{
	children[maxNumber]=blockNumber;
}


//in index file, get the block number of the leaf node which may contain an element whose value is equal to condition
int treeNode::getLeaf(indexIterator &it,string fileName,string condition)
{	//得到condition条件所对应的叶子节点的block号
	int32_t i=findPos(condition);
	if(Leaf&&!exist(condition))
		return -1;
	if(Leaf)
	{
		it.set(fileName,myBufferManager,this,i-1);
		return 0;
	}
	Block tmpBlock=myBufferManager->readBlock(fileName, children[i]);
	treeNode* tmpNode=new treeNode(myBufferManager,tmpBlock);
	int result=tmpNode->getLeaf(it,fileName,condition);
	if(!tmpNode->Leaf||result==-1)
		delete tmpNode;
	return result;
}


//in index file, get the leftest leaf node's block number
int treeNode::getLeftestLeaf(indexIterator &it,string fileName)
{
	if(Leaf)
	{
		it.set(fileName,myBufferManager,this,0);
		return 0;
	}
	Block tmpBlock=myBufferManager->readBlock(fileName, children[0]);
	treeNode* tmpNode=new treeNode(myBufferManager,tmpBlock);
	int result=tmpNode->getLeftestLeaf(it,fileName);
	if(!tmpNode->Leaf||result==-1)
		delete tmpNode;
	return result;
}


//merge
void treeNode::merge(treeNode* t1, treeNode*t2,string keyValue)
{
	int32_t i,j;
	if(t1->Leaf)
	{
		t1->setLinkedLeafNode(t2->getLinkedLeafNode());
		for(i=t1->keyNumber,j=0;;i++,j++)
		{
			if(j<=t2->keyNumber-1)
			{
				t1->setKey(i,t2->getKey(j));
				t1->setChild(i,t2->getChild(j));
				t1->setBlockOffset(i,t2->getBlockOffset(j));
				t1->keyNumber++;
			}
			else break;
		}
	}
	else
	{
		t1->setKey(t1->keyNumber,keyValue);
		t1->keyNumber++;
		for(i=t1->keyNumber,j=0;;j++,i++)
		{
			if(j<=t2->keyNumber-1)
			{
				t1->setKey(i,t2->getKey(j));
				t1->setChild(i,t2->getChild(j));
				t1->keyNumber++;
			}
			else break;
		}
		t1->setChild(i,t2->getChild(t2->keyNumber));
	}
}


//transform to block
Block treeNode::transform()
{
	Block b;

	b.setByte(blockNumber>>24,0);
	b.setByte(blockNumber>>16,1);
	b.setByte(blockNumber>>8,2);
	b.setByte(blockNumber,3);

	b.setByte(maxNumber>>24,4);
	b.setByte(maxNumber>>16,5);
	b.setByte(maxNumber>>8,6);
	b.setByte(maxNumber,7);

	b.setByte(keyNumber>>24,8);
	b.setByte(keyNumber>>16,9);
	b.setByte(keyNumber>>8,10);
	b.setByte(keyNumber,11);

	b.setByte(columnType[0],12); // 'c','i','f'
	b.setByte(Root,13);
	b.setByte(Leaf,14);

	b.setByte(charLen>>24,16);
	b.setByte(charLen>>16,17);
	b.setByte(charLen>>8,18);
	b.setByte(charLen,19);

	int32_t i=0;
	int32_t j=20;

	//cout<<"in transform1"<<endl;

	for(i=0;i<keyNumber;i++)
	{
		if(columnType=="char")
		{
			for(int32_t k=0;;k++)
			{
				if(keyStr[i].length()>=k)
					b.setByte((char)(keyStr[i][k]),j+i*(charLen+1)+k);
				else {
					b.setByte(0,j+i*(charLen+1)+k);
					break;
				}
			}
		}
		if(columnType=="int")
		{
			stringstream ss;
			ss<<keyInt[i];
			int32_t a;
			ss>>a;
			b.setByte(a>>24,j+4*i);
			b.setByte(a>>16,j+4*i+1);
			b.setByte(a>>8,j+4*i+2);
			b.setByte(a,j+4*i+3);
		}
		if(columnType=="float")
		{
			stringstream ss;
			float ff;
			ss<<keyFloat[i];
			ss>>ff;
			b.setByte(((*(int *)&ff)>>24)&0xFF,j+4*i);
			b.setByte(((*(int *)&ff)>>16)&0xFF,j+4*i+1);
			b.setByte(((*(int *)&ff)>>8)&0xFF,j+4*i+2);
			b.setByte((*(int *)&ff)&0xFF,j+4*i+3);
		}
	}
	//cout<<"in transform2"<<endl;

	if(columnType=="char")
		j=j+maxNumber*(charLen+1);
	else
		j=j+maxNumber*4;

	for(i=0;i<keyNumber+1;i++)
	{
		b.setByte((children[i]>>24)&0xFF,j+i*4);
		b.setByte((children[i]>>16)&0xFF,j+i*4+1);
		b.setByte((children[i]>>8)&0xFF,j+i*4+2);
		b.setByte(children[i]&0xFF,j+i*4+3);
	}

	if(Leaf)
	{
		i=maxNumber;
		b.setByte((children[i]>>24)&0xFF,j+i*4);
		b.setByte((children[i]>>16)&0xFF,j+i*4+1);
		b.setByte((children[i]>>8)&0xFF,j+i*4+2);
		b.setByte(children[i]&0xFF,j+i*4+3);
		//cout<<children[i]<<endl;////////////////////////////
	}

	j=j+(maxNumber+1)*4;
	//cout<<"in transform3"<<endl;

	if(Leaf)
	{
		int32_t i=0;
		for(i=0;i<keyNumber+1;i++)
		{
			b.setByte((blockOffset[i]>>24)&0xFF,j+4*i);
			b.setByte((blockOffset[i]>>16)&0xFF,j+4*i+1);
			b.setByte((blockOffset[i]>>8)&0xFF,j+4*i+2);
			b.setByte(blockOffset[i]&0xFF,j+4*i+3);
		}

	}
	//cout<<"in transform"<<endl;

	return b;
}


//write the treeNode back to file
void treeNode::writeBack(string fileName)
{
	Block t=transform();
	//cout<<"in writeBack"<<endl;
	//t.print();
	myBufferManager->writeBlock(fileName,blockNumber,t);
}


int treeNode::insert(blockAnalyzer* analyzer ,string fileName,string value, int32_t recordBlockNumber,int32_t recordBlockOffset,int32_t& newBlockNumber,string& newValue)
{
	Block tmpBlock;
	treeNode* tmpNode;
	int p=findPos(value);
	int i,j;
	if(Leaf)	// this node is leaf node
	{
		//cout<<"in node::insert1"<<endl;/////////////////
		//cout<<"p"<<" "<<p<<endl;///////////////
		//cout<<"keyNumber"<<keyNumber<<endl;/////////////////
		if(exist(value))
		{
			cerr<<"duplicate"<<endl;
			return -1;
		}
		else if(keyNumber<maxNumber) //do not need to split
		{
			//cout<<"in node::insert2"<<endl;/////////////////////////
			if(keyNumber!=0)
				for(i=keyNumber-1;i>=0;i--)
				{
					if(i>=p)
					{
						children[i+1]=children[i];
						blockOffset[i+1]=blockOffset[i];
						moveKeyBackward(i);
					}
					else break;
				}
			keyNumber++;
			//cout<<"in node::insert3"<<endl;/////////////////
			setKey(p,value);
			children[p]=recordBlockNumber;
			blockOffset[p]=recordBlockOffset;
			writeBack(fileName);
			return 0;
		}
		else // need to split
		{
			newBlockNumber=analyzer->getNextEmptyBlock();
			tmpNode=new treeNode(myBufferManager,columnType,charLen,newBlockNumber,false,true);
			bool tmpState=false;
//			if(it==122)
//			{
//				cout<<"leaf"<<blockNumber<<endl;
//				cout<<"leaf"<<newBlockNumber<<endl;
//				getchar();
//			}
			//cout<<"in node::insert4"<<endl;

			for(i=maxNumber-1,j=floor((double)(maxNumber+1)/2)-1; j>=0 ; j--)
			{
				if(!tmpState&&p>i)
				{
					tmpNode->setKey(j,value);
					tmpNode->setChild(j,recordBlockNumber);
					tmpNode->setBlockOffset(j,recordBlockOffset);
					tmpState=true;
					continue;
				}
				tmpNode->setKey(j,getKey(keyNumber-1));
				tmpNode->setChild(j,getChild(keyNumber-1));
				tmpNode->setBlockOffset(j,getBlockOffset(keyNumber-1));
				keyNumber--;
				i--;
			}
//			if(it==122)
//			{
//
//				getchar();
//			}
			//cout<<"in node::insert5"<<endl;
			tmpNode->keyNumber=floor((double)(maxNumber+1)/2);
			//cout<<"tmpNode->keyNumber"<<tmpNode->keyNumber<<endl;///////////////////////////
			if(!tmpState)
			{
				for(i=keyNumber-1;;i--)
				{
					if(p>i)
					{
						setKey(i+1,value);
						setChild(i+1,recordBlockNumber);
						setBlockOffset(i+1,recordBlockOffset);
						keyNumber++;
						break;
					}
					moveKeyBackward(i);
					children[i+1]=children[i];
					blockOffset[i+1]=blockOffset[i];
				}
			}
			tmpNode->setLinkedLeafNode(getLinkedLeafNode());
			setLinkedLeafNode(newBlockNumber);
			newValue=tmpNode->getKey(0);
			//cout<<"newValue"<<newValue<<endl;////////////////////////////
//			if(it==122)
//			{
//				int i;
//				for(i=0;i<tmpNode->keyNumber;i++)
//				{
//					cout<<tmpNode->children[i]<<" "<<tmpNode->keyStr[i]<<" ";
//				}
//				cout<<endl;
//				for(i=0;i<keyNumber;i++)
//					cout<<children[i]<<" "<<keyStr[i]<<" ";
//				cout<<endl;
//				getchar();
//			}
			if(Root)
				setRoot(false);
			writeBack(fileName);
			tmpNode->writeBack(fileName);
			delete tmpNode;
			return 1;
		}
	}
	else//this node is not leaf node
	{
		int32_t blockPos;
		string newKey;
//		if(it==122)
//			{cout<<p<<endl;
//			cout<<children[p]<<endl;
//		getchar();}
		tmpBlock=myBufferManager->readBlock(fileName,children[p]);
		tmpNode=new treeNode(myBufferManager,tmpBlock);
		//cout<<"in node::insert nonleaf node1"<<" "<<tmpNode->isLeaf()<<" "<<Root<<" "<<p<<" "<<children[p]<<endl;

		int result = tmpNode->insert(analyzer,fileName,value,recordBlockNumber,recordBlockOffset,blockPos,newKey);
		delete tmpNode;

		if(result == -1)//duplicate
			return -1;
		if(result == 0) //insert complete
			return 0;
		if(result == 1)// sub tree is split
		{

//			if(it==122)
//					{
//							cout<<"root split1"<<endl;
//							cout<<blockPos<<endl;
//							cout<<result<<endl;
//										getchar();
//									}
			if(keyNumber<maxNumber)// the current Node do not need to split
			{
//				if(it==122)
//				{
//						cout<<"root split2"<<endl;
//						cout<<blockPos<<endl;
//						cout<<result<<endl;
//						getchar();
//				}
				//cout<<"I am right"<<endl;////////////////////////////
				for(int32_t k=keyNumber-1;k>=p;k--)
				{
					moveKeyBackward(k);
					children[k+2]=children[k+1];
				}
				keyNumber++;
				//cout<<"p"<<p<<endl;/////////////////////////
				//cout<<"newKey"<<newKey<<endl;/////////////////
				//cout<<"blockPos"<<blockPos<<endl;//////////////
				setKey(p,newKey);
				setChild(p+1,blockPos);
//				if(it==122)
//								{
//										cout<<"root split2"<<endl;
//										cout<<blockPos<<endl;
//										cout<<result<<endl;
//										getchar();
//								}
				writeBack(fileName);
				return 0;
			}
			else //the current Node need to split too
			{
//				if(it==122)
//				{
//					cout<<"root split"<<endl;
//					cout<<blockPos<<endl;
//					getchar();
//				}
				if(Root)
					setRoot(false);
				newBlockNumber = analyzer->getNextEmptyBlock();
				tmpNode=new treeNode(myBufferManager,columnType,charLen,newBlockNumber,false,false);
				bool tmpState=false;
				//cout<<"in nonleaf node split ok0"<<endl;
				for(j=maxNumber-1,i=floor((double)maxNumber/2)-1;i>=0;i--)
				{
					if(!tmpState&&p>j)
					{
						tmpNode->setChild(i+1,blockPos);
						tmpNode->setKey(i,newKey);
						tmpState=true;
						continue;
					}
					tmpNode->setChild(i+1,getChild(keyNumber));
					tmpNode->setKey(i,getKey(keyNumber-1));
					keyNumber--;
					j--;
				}
				tmpNode->keyNumber=floor((double)maxNumber/2);
				//cout<<"in nonleaf node split ok1"<<endl;
				if(!tmpState&&p>j)
				{
					newValue=newKey;
					tmpNode->setChild(0,blockPos);
				}
				else
				{
					newValue=getKey(keyNumber-1);
					tmpNode->setChild(0,getChild(keyNumber));
					keyNumber--;
					if(!tmpState)
					{
						for(i=keyNumber-1;;i--)
						{
							if(p>i)
								break;
							moveKeyBackward(i);
							children[i+2]=children[i+1];
						}
						setKey(p,newKey);
						setChild(p+1,blockPos);
						keyNumber++;
					}
				}
				//cout<<"in nonleaf node split ok2"<<endl;
				//cout<<"tmpNode"<<tmpNode->keyNumber<<endl;
				//cout<<keyNumber<<endl;
				writeBack(fileName);
				tmpNode->writeBack(fileName);
				delete tmpNode;
				return 1;
			}
		}
	}
}


int treeNode::deleteElement(blockAnalyzer* analyzer,string fileName,string value,string &newKey)
{
	int32_t p=findPos(value);
	if(Leaf)//this node is leaf
	{
		if(!exist(value))//value not exist
		{
			cerr<<"value not exist"<<endl;
			return -1;
		}
		else	// value is exist in the leaf node
		{
			if(enoughGive()) // enough element to delete
			{
				deleteFromLeaf(p-1);
				if(p==1)
				{
					newKey=getKey(0);
					writeBack(fileName);
					return 1;
				}
				writeBack(fileName);
				return 0;
			}
			else//not enough after delete, has to merge or borrow from sibling
			{
				deleteFromLeaf(p-1);
				if(p==1)
				{
					newKey=getKey(0);
					return 3;
				}
				return 2;
			}
		}
	}
	else// this node is nonleaf
	{
		Block tmpBlock=myBufferManager->readBlock(fileName,children[p]);
		treeNode *tmpNode=new treeNode(myBufferManager,tmpBlock);
		string newValue;
		int result = tmpNode->deleteElement(analyzer,fileName,value,newValue);
		if(result==0||result==-1)
		{
			delete tmpNode;
			writeBack(fileName);
			return result;
		}
		if(result==1)
		{
			delete tmpNode;
			if(p!=0)
			{
				setKey(p-1,newValue);
				writeBack(fileName);
				return 0;
			}
			else
			{
				if(Root)
					return 0;
				newKey=newValue;
				writeBack(fileName);
				return 1;
			}
		}
		if(result==2||result==3)
		{
			//cout<<"ok??"<<endl;
			Block tmpBlock1;
			treeNode* tmpNode1;
			if(p!=keyNumber) //merge or borrow from back
			{
				tmpBlock1=myBufferManager->readBlock(fileName,children[p+1]);
				tmpNode1=new treeNode(myBufferManager,tmpBlock1);
				if(!tmpNode1->enoughGive())//merge
				{
					merge(tmpNode,tmpNode1,getKey(p));
					//cout<<"p"<<p<<endl;
					deleteFromNonLeaf(p);
					analyzer->deleteBlock(tmpNode1->getBlockNumber());
					delete tmpNode1;
					if(Root&&!enough())
					{
						tmpNode->setRoot(true);
						tmpNode->writeBack(fileName);
						analyzer->changeRootPos(tmpNode->getBlockNumber());
						analyzer->deleteBlock(getBlockNumber());
						delete tmpNode;
						return 4;
					}
					tmpNode->writeBack(fileName);
					delete tmpNode;
				}
				else //borrow
				{
					//cout<<"ok???"<<endl;
					if(tmpNode->Leaf)	// subNode is leaf
					{
						tmpNode->setKey(tmpNode->keyNumber,tmpNode1->getKey(0));
						tmpNode->setChild(tmpNode->keyNumber,tmpNode1->getChild(0));
						tmpNode->setBlockOffset(tmpNode->keyNumber,tmpNode1->getBlockOffset(0));
						tmpNode->keyNumber++;
						tmpNode1->moveForward();
						setKey(p,tmpNode1->getKey(0));
					}
					else	//subNode is not leaf
					{
						tmpNode->setKey(tmpNode->keyNumber,getKey(p));
						tmpNode->setChild(tmpNode->keyNumber+1,tmpNode1->getChild(0));
						tmpNode->keyNumber++;
						setKey(p,tmpNode1->getKey(0));
						tmpNode1->moveForward();
					}
					tmpNode->writeBack(fileName);
					tmpNode1->writeBack(fileName);
					delete tmpNode;
					delete tmpNode1;
				}
				if(enough())
				{
					if(result==3)
					{
						if(p!=0)
							setKey(p-1,newValue);
						else
						{
							newKey=newValue;
							writeBack(fileName);
							return 1;
						}
					}
					writeBack(fileName);
					return 0;
				}
				else
				{
					if(result==3)
					{
						if(p!=0)
							setKey(p-1,newValue);
						else
						{
							newKey=newValue;
							return 3;
						}
					}
					return 2;
				}
			}
			else //merge or borrow from forward
			{
				tmpBlock1=myBufferManager->readBlock(fileName,children[p-1]);
				tmpNode1=new treeNode(myBufferManager,tmpBlock1);
				if(!tmpNode1->enoughGive())//merge
				{

					//cout<<"ok?????"<<endl;
					merge(tmpNode1,tmpNode,getKey(p-1));
					deleteFromNonLeaf(p-1);
					analyzer->deleteBlock(tmpNode->getBlockNumber());
					delete tmpNode;
					if(Root&&!enough())
					{
						tmpNode1->setRoot(true);
						analyzer->changeRootPos(tmpNode1->getBlockNumber());
//						cout<<"rootPos"<<tmpNode1->getBlockNumber()<<endl;
//
//						cout<<"currootPos"<<analyzer->getRootPosition()<<endl;
//						getchar();
						analyzer->deleteBlock(getBlockNumber());
						tmpNode1->writeBack(fileName);
						delete tmpNode1;
						return 4;
					}
					tmpNode1->writeBack(fileName);
					delete tmpNode1;

				}
				else //borrow
				{

					if(tmpNode->Leaf)
					{
						tmpNode->moveBackward();
						tmpNode->setKey(0,tmpNode1->getKey(tmpNode1->keyNumber-1));
						tmpNode->setChild(0,tmpNode1->getChild(tmpNode1->keyNumber-1));
						tmpNode->setBlockOffset(0,tmpNode1->getBlockOffset(tmpNode1->keyNumber-1));
						tmpNode1->keyNumber--;
						setKey(p-1,tmpNode->getKey(0));
					}
					else
					{
						tmpNode->moveBackward();
						if(result==2)
							tmpNode->setKey(0,getKey(p-1));
						else
							tmpNode->setKey(0,newValue);
						tmpNode->setChild(0,tmpNode1->getChild(tmpNode1->keyNumber));
						setKey(p-1,tmpNode1->getKey(tmpNode1->keyNumber-1));
						tmpNode1->keyNumber--;
					}
					tmpNode1->writeBack(fileName);
					tmpNode->writeBack(fileName);
					delete tmpNode1;
					delete tmpNode;
				}
				if(enough())
				{
					writeBack(fileName);
					return 0;
				}
				else
				{
					return 2;
				}

			}

		}

	}

}


int IndexManager::selectNode(indexIterator &iterator,string fileName, int condType ,string condition)
{
	int result;
	Block tmpBlock;
	Block tmpBlock2;
	treeNode *currentNode;
	int32_t blockPos1;
	int32_t blockOffset1;

	if(lastFile!=fileName)
	{
		if(myAnalyzer!=NULL){
			myAnalyzer->writeBack(fileName);
			delete myAnalyzer;
		}
		tmpBlock=myBufferManager->readBlock(fileName,0);
		tmpBlock2=myBufferManager->readBlock(fileName,1);
		myAnalyzer=new blockAnalyzer(tmpBlock,tmpBlock2,myBufferManager);
	}

	blockPos1=myAnalyzer->getRootPosition();
	tmpBlock=myBufferManager->readBlock(fileName,blockPos1);//得到root所在的Block
	currentNode=new treeNode(myBufferManager,tmpBlock);//用Block还原出root

	if(condType==3||condType==5)//where的条件为小于或小于等于
		result=currentNode->getLeftestLeaf(iterator,fileName);
	else
		result=currentNode->getLeaf(iterator,fileName,condition);
	if(!currentNode->isLeaf()||result==-1)
		delete currentNode;
	return result;
}


int IndexManager::insertNode(string fileName, string value, int32_t recordBlockNumber, int32_t recordBlockOffset)
{
	Block tmpBlock;
	Block tmpBlock2;
	treeNode* tmpNode;
	int32_t blockPos;
	string newKey;
	int32_t newBlock;

	if(lastFile!=fileName)
	{
		if(myAnalyzer!=NULL){
			myAnalyzer->writeBack(fileName);
			delete myAnalyzer;
		}
		tmpBlock=myBufferManager->readBlock(fileName,0);
		tmpBlock2=myBufferManager->readBlock(fileName,1);
		myAnalyzer=new blockAnalyzer(tmpBlock,tmpBlock2,myBufferManager);
	}
	//cout<<"in manager::insertNode1"<<endl;//////////////////////////////

	blockPos=myAnalyzer->getRootPosition();
	//analyzeIndex(tmpBlock,colType,charLen,blockPos);
	tmpBlock=myBufferManager->readBlock(fileName,blockPos);
	tmpNode=new treeNode(myBufferManager,tmpBlock);//得到根节点

	//cout<<"in manager::insertNode2"<<endl;////////////////////////////////////////////

	int result = tmpNode->insert(myAnalyzer,fileName,value,recordBlockNumber,recordBlockOffset,newBlock,newKey);

	//cout<<"in manager::insertNode3"<<endl;//////////////////////////////////

	if(result==-1)//insert complete
	{
		delete tmpNode;
		return -1;
	}
	else if(result==0)
	{
		//do nothing
	}
	else if(result==1)//create a new Root
	{
		int32_t newRoot;
		newRoot=myAnalyzer->getNextEmptyBlock();
		treeNode *newNode=new treeNode(myBufferManager,tmpNode->getColumnType(),tmpNode->getCharLen(),newRoot,true,false);
		newNode->setChild(0,blockPos);
		newNode->setKey(0,newKey);
		newNode->setChild(1,newBlock);
		newNode->setKeyNumber(1);
		newNode->writeBack(fileName);
		delete newNode;
		myAnalyzer->changeRootPos(newRoot);
	}
	//cout<<"in manager::insertNode4"<<endl;///////////////////////////////////

	delete tmpNode;
	return 0;
}


int IndexManager::deleteNode(string fileName, string value)
{
	Block tmpBlock1,tmpBlock2;
	int32_t blockPos;
	treeNode *tmpNode;
	string newKey;

	//cout<<"ok1"<<endl;
	if(lastFile!=fileName)
	{
		if(myAnalyzer!=NULL)
		{
			myAnalyzer->writeBack(fileName);
			delete myAnalyzer;
		}
		tmpBlock1=myBufferManager->readBlock(fileName,0);
		tmpBlock2=myBufferManager->readBlock(fileName,1);
		myAnalyzer=new blockAnalyzer(tmpBlock1,tmpBlock2,myBufferManager);
	}

	blockPos=myAnalyzer->getRootPosition();
	tmpBlock1=myBufferManager->readBlock(fileName,blockPos);
	tmpNode=new treeNode(myBufferManager,tmpBlock1);
	//cout<<"ok2"<<endl;
	int result=tmpNode->deleteElement(myAnalyzer,fileName,value,newKey);
	//cout<<"ok?"<<endl;
	if(result == -1)
	{
		delete tmpNode;
		return -1;
	}
	if(result == 0 || result == 1)
		tmpNode->writeBack(fileName);
	delete tmpNode;

	return 0;
}


void IndexManager::createIndex(string fileName,string colType,int32_t charLen,int32_t number,string value[],int32_t blockNumber[],int32_t blockOffset[])
{
	Block block1=newIndexHead();
	Block block2=newIndexHead2();
	treeNode* tmpNode=new treeNode(myBufferManager,colType,charLen,2,true,true);
	//tmpNode->print();
	myBufferManager->createFile(fileName);
	myBufferManager->writeBlock(fileName,0,block1);
	myBufferManager->writeBlock(fileName,1,block2);
	myAnalyzer=new blockAnalyzer(block1,block2,myBufferManager);
//	cout<<myAnalyzer->getNextEmptyBlock()<<endl;
//	cout<<myAnalyzer->getRootPosition()<<endl;
	tmpNode->writeBack(fileName);
	//cout<<"ok1"<<endl;
	delete tmpNode;
	lastFile=fileName;
	//cout<<"ok3"<<endl;
	for(int32_t i=0;i<number;i++)
	{	insertNode(fileName,value[i],blockNumber[i],blockOffset[i]);
		it++;
	}
	//print(fileName);
//	Block block3=myBufferManager->readBlock(fileName,2);
//	block3.print();

}


void IndexManager::dropIndex(string fileName)
{
	//myBufferManager->deleteFile(fileName);
	if(lastFile==fileName && myAnalyzer!=NULL)
	{
		delete myAnalyzer;
		myAnalyzer=NULL;
	}
}

