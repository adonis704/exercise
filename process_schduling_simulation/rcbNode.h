#ifndef rcbNode_h
#define rcbNode_h
#include"pcbList.h"

class RCBNode {
public:
	string name;	//资源名称
	int total, available;	//总资源，剩余资源
	PCBList* wl;	//阻塞队列

	RCBNode(string n, int total);
	~RCBNode();
};

#endif