#ifndef rcbNode_h
#define rcbNode_h
#include"pcbList.h"

class RCBNode {
public:
	string name;	//��Դ����
	int total, available;	//����Դ��ʣ����Դ
	PCBList* wl;	//��������

	RCBNode(string n, int total);
	~RCBNode();
};

#endif