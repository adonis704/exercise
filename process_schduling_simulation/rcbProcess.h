#ifndef rcbProcess_h
#define rcbProcess_h
#include"rcbNode.h"
typedef enum {
	R1,
	R2,
	R3,
	R4
} RESOURCE_TYPE;

class RCBProcess {
public:
	RCBNode* r1; RCBNode* r2; RCBNode* r3; RCBNode* r4;	//ืสิด

	RCBProcess();
	~RCBProcess();
	bool request(RESOURCE_TYPE r ,int num, PCBNode* p);
	void release(RESOURCE_TYPE r, int num);
};

#endif