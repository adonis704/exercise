#include"pcbList.h"

//队列初始化
PCBList::PCBList() {
	//初始化头尾结点
	header = new PCBNode();
	trailer = new PCBNode();
	header->succ = trailer; header->pred = NULL;
	trailer->pred = header; trailer->succ = NULL;
	size = 0;	//大小为0
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

//创建进程并插入队列首部
PCBNode* PCBList::insertAsFirst(string n, PCB_PRIORITY po) {
	size++;
	return header->insertAsSucc(n, po);
}
//创建进程并插入队列尾部
PCBNode* PCBList::insertAsLast(string n, PCB_PRIORITY po) {
	size++;
	return trailer->insertAsPred(n, po);
}

//删除队列中的进程
void PCBList::remove(PCBNode* x) {
	x->succ->pred = x->pred;
	x->pred->succ = x->succ;
	size--;
	delete x;
}
//出队
PCBNode* PCBList::dequeue() {
	if (size == 0) return 0;
	PCBNode* temp = getFirst();
	header->succ->succ->pred = header->succ->pred;
	header->succ->pred->succ = header->succ->succ;
	size--;
	return temp;
}
//入队
PCBNode* PCBList::enqueue(PCBNode* x) {
	PCBNode* temp = x;
	size++;
	trailer->pred->succ = x;
	x->pred = trailer->pred;
	trailer->pred = x;
	x->succ = trailer;
	return temp;
}
