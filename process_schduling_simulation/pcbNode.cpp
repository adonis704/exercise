#include "pcbNode.h"

//初始化进程控制块
PCBNode::PCBNode(string n, PCB_PRIORITY po, PCBNode* pr, PCBNode* su, PCBNode* p, vector<PCBNode*> c, PCB_STATE s) {
	parent = p;children = c;	//父进程，子进程
	state = s;	//进程状态
	priority = po;	//进程优先级
	name = n;	//进程名字
	pred = pr;succ = su;	//前继后继
	r1 = r2 = r3 = r4 = 0;	//占有的资源
	r1r = r2r = r3r = r4r = 0;	//请求的资源
}
PCBNode::~PCBNode() {
	
}

//创建结点并把结点当作前继结点插入
PCBNode* PCBNode::insertAsPred(string n, PCB_PRIORITY po) {
	PCBNode* x = new PCBNode(n, po, this->pred, this);
	this->pred->succ = x;
	this->pred = x;
	return x;
}
//创建结点并把结点当作后继结点插入
PCBNode* PCBNode::insertAsSucc(string n, PCB_PRIORITY po) {
	PCBNode* x = new PCBNode(n, po, this, this->succ);
	this->succ->pred = x;
	this->succ = x;
	return x;
}

//将进程状态转换为相应的字符串
string PCBNode::rawState(){
	if (state == RUNNING) return "running";
	else if (state == BLOCKED) return "blocked";
	else if (state == READY) return "ready";
	else return "none";
}


