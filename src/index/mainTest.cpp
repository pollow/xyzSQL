/*
 * mainTest.cpp
 *
 *  Created on: 2014��11��7��
 *      Author: Admin
 */

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "IndexManager.h"
#define MAX 80
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
	string dvalue[190];
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
	for(int i=0;i<190;i++)
	{
		stringstream ss;
		ss.clear();
		ss<<i;
		ss>>dvalue[i];
	}
	f(value,MAX);
	f(dvalue,190);
//	for(int i=0;i<MAX;i++)
//		cout<<value[i]<<endl;
//	cout<<endl;
//	for(int i=0;i<190;i++)
//			cout<<dvalue[i]<<endl;


	i.createIndex("abc.db","char",200,MAX,value,block,offset);
//	int j;
//	for(j=0;j<190;j++){
//		//cout<<"delete:"<<dvalue[j]<<endl;
//		i.deleteNode("abc.db",dvalue[j]);
//	}
//
		i.print("abc.db");

//	while(true)
//	{
//		//getchar();
//		cout<<"delete:"<<dvalue[j]<<endl;
//		//getchar();
//		i.deleteNode("abc.db",dvalue[j]);
//		cout<<"delete finish"<<endl;
//	//	getchar();
//
//		i.print("abc.db");
//		j++;
//	}
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
//	}afasd
	return 0;
}

