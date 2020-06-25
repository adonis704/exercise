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
	PCBNode* parent;	//������
	vector<PCBNode*> children;	//�ӽ��̣��ǣ�
	PCBNode* succ; PCBNode* pred;	//ǰ�̺ͺ��
	PCB_STATE state;	//����״̬
	PCB_PRIORITY priority;	//�������ȼ�
	string name;	//��������
	int r1, r2, r3, r4;	//ռ�е���Դ
	int r1r, r2r, r3r, r4r;	//��Ҫ�������Դ

	PCBNode(string n = "NULL", PCB_PRIORITY po = INIT, PCBNode* pr = NULL, PCBNode* su = NULL,
		PCBNode* p = NULL, vector<PCBNode*> c = {}, PCB_STATE s = NONE);
	~PCBNode();
	PCBNode* insertAsPred(string n, PCB_PRIORITY po);
	PCBNode* insertAsSucc(string n, PCB_PRIORITY po);
	string rawState();
};

#endif