/*
 * mainTest.cpp
 *
 *  Created on: 2014Äê11ÔÂ7ÈÕ
 *      Author: Admin
 */

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "IndexManager.h"
#define MAX 200
using namespace std;

void f(string a[], int num)
{
	int n=num;
	for(int j=0;j<n-1;j++){
		int i=rand()%num;
		string tmp;
		tmp=a[num-1];
		a[num-1]=a[i];
		a[i]=tmp;
		num--;
	}

}



int main()
{
	srand(time(NULL));
	int a,b,c;
	int32_t block[MAX]={0};
	int32_t offset[MAX]={0};
	string value[MAX];
	string dvalue[MAX/2];
	BufferManager myBufferManager;
	IndexManager i(&myBufferManager);
	//myBufferManager.createFile("abc.db");
	//cout<<"ok"<<endl;
	for(int i=0;i<MAX;i++)
	{
		stringstream ss;
		ss.clear();
		ss<<i;
		ss>>value[i];
	}
	for(int i=0;i<MAX/2;i++)
	{
		stringstream ss;
		ss.clear();
		ss<<i;
		ss>>dvalue[i];
	}
	f(value,MAX);
	f(dvalue,MAX/2);
//	for(int i=0;i<MAX;i++)
//		cout<<value[i]<<endl;
//	cout<<endl;
//	for(int i=0;i<MAX/2;i++)
//			cout<<dvalue[i]<<endl;
	i.createIndex("abc.db","char",200,MAX,value,block,offset);
//	for(int j=0;j<MAX/2;j++){
//		cout<<"delete:"<<dvalue[j]<<endl;
//		i.deleteNode("abc.db",dvalue[j]);
	//i.print("abc.db");
//	while(true){
//		string str;
//		cout<<"please input:";
//		cin>>str;
//
//		if(str!="delete"&&str!="insert")
//			continue;
//		cin>>c;
//		string tmp11;
//		stringstream sss;
//		sss.clear();
//		sss<<c;
//		sss>>tmp11;
//		if(str=="insert")
//			i.insertNode("a",tmp11,123,456);
//		else
//			i.deleteNode("a",tmp11);
//		i.print("aa");
//	}
	return 0;
}

