#include"rcbNode.h"

//��ʼ��
RCBNode::RCBNode(string n, int t) {
	name = n;	//��Դ����
	total = t;	//��Դ����
	available = t;	//������Դ
	wl = new PCBList();	//��������
}
RCBNode::~RCBNode() {

}