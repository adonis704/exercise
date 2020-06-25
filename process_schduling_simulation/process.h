#ifndef process_h
#define process_h
#include<iostream>
#include"pcbList.h"
#include"rcbProcess.h"

class Process {
private:
	PCBList* rl1; PCBList* rl2; PCBList* rl3;	//三个就绪队列
	vector<PCBNode*> nodeVec;	//用于记录所有进程信息
	PCBNode* current;	//running的进程
	RCBProcess* rcb;	//资源块

	void destroy(PCBNode* x);
	PCBNode* enqueue(PCBNode* x);
	void dequeue();
	void schedule();
	void awakeCheck(RESOURCE_TYPE r);
	void release(RESOURCE_TYPE r, int num, PCBNode* p);
	void print();
	vector<PCBNode*>::iterator find(string n);
	
public:
	Process();
	~Process();
	PCBNode* create(string n, PCB_PRIORITY p);
	void destroy(string n);
	void request(RESOURCE_TYPE r, int num);
	void release(RESOURCE_TYPE r, int num);
	void timeOut();
	void provide(string n);
	void init();
	void listReady();
	void listBlocked();
	void listAvailable();
};


#endif