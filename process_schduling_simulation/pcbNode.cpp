#include "pcbNode.h"

//��ʼ�����̿��ƿ�
PCBNode::PCBNode(string n, PCB_PRIORITY po, PCBNode* pr, PCBNode* su, PCBNode* p, vector<PCBNode*> c, PCB_STATE s) {
	parent = p;children = c;	//�����̣��ӽ���
	state = s;	//����״̬
	priority = po;	//�������ȼ�
	name = n;	//��������
	pred = pr;succ = su;	//ǰ�̺��
	r1 = r2 = r3 = r4 = 0;	//ռ�е���Դ
	r1r = r2r = r3r = r4r = 0;	//�������Դ
}
PCBNode::~PCBNode() {
	
}

//������㲢�ѽ�㵱��ǰ�̽�����
PCBNode* PCBNode::insertAsPred(string n, PCB_PRIORITY po) {
	PCBNode* x = new PCBNode(n, po, this->pred, this);
	this->pred->succ = x;
	this->pred = x;
	return x;
}
//������㲢�ѽ�㵱����̽�����
PCBNode* PCBNode::insertAsSucc(string n, PCB_PRIORITY po) {
	PCBNode* x = new PCBNode(n, po, this, this->succ);
	this->succ->pred = x;
	this->succ = x;
	return x;
}

//������״̬ת��Ϊ��Ӧ���ַ���
string PCBNode::rawState(){
	if (state == RUNNING) return "running";
	else if (state == BLOCKED) return "blocked";
	else if (state == READY) return "ready";
	else return "none";
}


