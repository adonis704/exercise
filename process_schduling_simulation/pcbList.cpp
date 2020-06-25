#include"pcbList.h"

//���г�ʼ��
PCBList::PCBList() {
	//��ʼ��ͷβ���
	header = new PCBNode();
	trailer = new PCBNode();
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
	size = 0;	//��СΪ0
}
PCBList::~PCBList() {

}

int PCBList::getSize() {
	return size;
}
PCBNode* PCBList::getFirst() {
	return header->succ;
}

PCBNode* PCBList::getLast() {
	return trailer->pred;
}

//�������̲���������ײ�
PCBNode* PCBList::insertAsFirst(string n, PCB_PRIORITY po) {
	size++;
	return header->insertAsSucc(n, po);
}
//�������̲��������β��
PCBNode* PCBList::insertAsLast(string n, PCB_PRIORITY po) {
	size++;
	return trailer->insertAsPred(n, po);
}

//ɾ�������еĽ���
void PCBList::remove(PCBNode* x) {
	x->succ->pred = x->pred;
	x->pred->succ = x->succ;
	size--;
	delete x;
}
//����
PCBNode* PCBList::dequeue() {
	if (size == 0) return 0;
	PCBNode* temp = getFirst();
	header->succ->succ->pred = header->succ->pred;
	header->succ->pred->succ = header->succ->succ;
	size--;
	return temp;
}
//���
PCBNode* PCBList::enqueue(PCBNode* x) {
	PCBNode* temp = x;
	size++;
	trailer->pred->succ = x;
	x->pred = trailer->pred;
	trailer->pred = x;
	x->succ = trailer;
	return temp;
}
