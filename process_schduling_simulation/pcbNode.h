#ifndef pcbNode_h
#define pcbNode_h
#include<string>
#include<vector>
#define NULL 0
using namespace std;
typedef enum {
	NONE,
	RUNNING,
	BLOCKED,
	READY
} PCB_STATE;

typedef enum {
	INIT,
	USER,
	SYSTEM
} PCB_PRIORITY; 

class PCBNode {
public:
	PCBNode* parent;	//父进程
	vector<PCBNode*> children;	//子进程（们）
	PCBNode* succ; PCBNode* pred;	//前继和后继
	PCB_STATE state;	//进程状态
	PCB_PRIORITY priority;	//进程优先级
	string name;	//进程名字
	int r1, r2, r3, r4;	//占有的资源
	int r1r, r2r, r3r, r4r;	//需要请求的资源

	PCBNode(string n = "NULL", PCB_PRIORITY po = INIT, PCBNode* pr = NULL, PCBNode* su = NULL,
		PCBNode* p = NULL, vector<PCBNode*> c = {}, PCB_STATE s = NONE);
	~PCBNode();
	PCBNode* insertAsPred(string n, PCB_PRIORITY po);
	PCBNode* insertAsSucc(string n, PCB_PRIORITY po);
	string rawState();
};

#endif