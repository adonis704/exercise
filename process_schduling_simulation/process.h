#ifndef process_h
#define process_h
#include<iostream>
#include"pcbList.h"
#include"rcbProcess.h"

class Process {
private:
	PCBList* rl1; PCBList* rl2; PCBList* rl3;	//������������
	vector<PCBNode*> nodeVec;	//���ڼ�¼���н�����Ϣ
	PCBNode* current;	//running�Ľ���
	RCBProcess* rcb;	//��Դ��

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