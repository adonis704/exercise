#ifndef pcbList_h
#define pcbList_h
#include"pcbNode.h"

class PCBList {
private:
	PCBNode* header;	//头结点
	PCBNode* trailer;	//尾节点
	int size;	//队列大小

public:
	PCBList();
	~PCBList();
	PCBNode* getFirst();
	PCBNode* getLast();
	int getSize();
	PCBNode* insertAsFirst(string n, PCB_PRIORITY po);
	PCBNode* insertAsLast(string n, PCB_PRIORITY po);
	void remove(PCBNode* x);
	PCBNode* dequeue();
	PCBNode* enqueue(PCBNode* x);
};


#endif