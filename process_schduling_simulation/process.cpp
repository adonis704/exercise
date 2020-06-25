#include"process.h"

//��ʼ��
Process::Process() {
	//��ʼ������������������
	rl1 = new PCBList();
	rl2 = new PCBList();
	rl3 = new PCBList();
	rcb = new RCBProcess();	//��ʼ����Դ��
	current = NULL;	//��ǰ���н���Ϊ��
	PCBNode* null = new PCBNode();
	nodeVec.push_back(null);	//��һ������յĽ��ָ������ܽ��̣����ں���Ĳ��ҵȲ���
}
Process::~Process() {

}

//init����
void Process::init() {
	create("init", INIT);	//����һ��init����
	cout << "init process is running." << endl;	//�����ʾ��Ϣ
}
//�����½���
PCBNode* Process::create(string n, PCB_PRIORITY p) {
	PCBNode* temp = 0;
	//�������ȼ��������½��̲����䵽��Ӧ�ľ�������
	switch (p) {
	case INIT:
		temp = rl1->insertAsLast(n, p);
		break;
	case USER:
		temp = rl2->insertAsLast(n, p);
		break;
	case SYSTEM:
		temp = rl3->insertAsLast(n, p);
		break;
	}
	if(temp != 0) temp->state = READY;

	//�����ǰ�н��������Ҳ��ǳ�ʼ�����̣����½�����Ϊ��ǰ���̵��ӽ���
	if (current && current->name != "init") {
		current->children.push_back(temp);
		temp->parent = current;
	}
	//����½������ȼ��ȵ�ǰ���̸ߣ���ռ
	if (current && temp->priority > current->priority)	schedule();
	//�����ǰû�н��̣����ӣ����ڴ���init���̣�
	if (!current) dequeue();
	//�����̿��ƿ�ָ������¼���н�����Ϣ��������
	if (temp != 0) nodeVec.push_back(temp);
	//�����ǰ���̲���init���̣��������ǰ�������֣�init���������һ����
	if(current->name != "init") print();
	return temp;
}

//�������̣������ǽ��̵�����
void Process::destroy(string n) {
	vector<PCBNode*>::iterator iter = find(n);	//�ҵ��������ֶ�Ӧ�ĵ�����
	PCBNode* temp = *iter;	//��Ӧ���̿��ƿ��ָ��
	
	//�Ҳ������̣�ֱ�ӷ���
	if (temp->name == "NULL") {
		cout << "no process named " << n << endl;
		return;
	}

	for (int i = 0; i < temp->children.size(); i++) destroy(temp->children[i]);	//������ӽ��̣������ӽ���
	nodeVec.erase(iter);	//��������ɾ����Ӧ����
	destroy(temp);	//���ص�destroy��������ʼʵ�ֳ�������
}
void Process::destroy(PCBNode* x) {
	//�ͷ���Դ�����Ի����������̣����Ѳ�����release������
	if (x->r1 != 0) { cout << "release R1. "; release(R1, x->r1, x);}
	if (x->r2 != 0) { cout << "release R2. "; release(R2, x->r2, x);}
	if (x->r3 != 0) { cout << "release R3. "; release(R3, x->r3, x);}
	if (x->r4 != 0) { cout << "release R4. "; release(R4, x->r4, x);}

	//�ҵ��������ڵĶ��У�ɾ������
	if (x->state == READY) {
		switch (x->priority) {
		case INIT:
			rl1->remove(x);
			break;
		case USER:
			rl2->remove(x);
			break;
		case SYSTEM:
			rl3->remove(x);
			break;
		}
	}
	else if (x->state == BLOCKED) {
		if (x->r1r != 0) 
			rcb->r1->wl->remove(x);
		else if (x->r2r != 0)
			rcb->r2->wl->remove(x);
		else if (x->r3r != 0) 
			rcb->r3->wl->remove(x);
		else if (x->r4r != 0) 
			rcb->r4->wl->remove(x);
	}
	else if (x->state == RUNNING) {
		delete current;
		dequeue();	//�����Ҫ�����Ľ������������еĽ��̣���ô��һ�����̱��������
	}
}
//�ڼ�¼���н�����Ϣ���������ҵ���Ϊn�Ľ��̣����ص�����
vector<PCBNode*>::iterator Process::find(string n) {
	//ʹ�õ���������
	for (vector<PCBNode*>::iterator iter = nodeVec.begin(); iter != nodeVec.end(); iter++)
		if ((*iter)->name == n) return iter;
	//û���ҵ��򷵻ش���յĵ�����
	vector<PCBNode*>::iterator iter = nodeVec.begin();
	return iter;
}

//����
void Process::schedule() {
	enqueue(current);	//����ǰ���н��̷�����Ӧ����
	dequeue();	//��һ�����еĽ��̳���
}
PCBNode* Process::enqueue(PCBNode* x) {
	PCBNode* temp = 0;
	//���ݽ������ȼ�ѡ����Ӧ�ľ����������
	switch (x->priority) {
	case INIT:
		temp = rl1->enqueue(x);
		break;
	case USER:
		temp = rl2->enqueue(x);
		break;
	case SYSTEM:
		temp = rl3->enqueue(x);
		break;
	}
	if (temp != 0) temp->state = READY;
	return temp;
}
void Process::dequeue() {
	//���ȼ��ߵĶ����ȵõ����ȣ�Ϊ�ղŵ���һ���ȼ�����
	int rank = 0;
	if (rl3->getSize() != 0) rank = 3;
	else if (rl2->getSize() != 0) rank = 2;
	else if (rl1->getSize() != 0) rank = 1;

	//������Ӧ���е�һ������
	switch (rank) {
	case 3:
		current = rl3->dequeue();
		break;
	case 2:
		current = rl2->dequeue();
		break;
	case 1:
		current = rl1->dequeue();
		break;
	case 0:
		current = 0;
		break;
	}
	if (current->name != "NULL") current->state = RUNNING;
}

//������Դ
void Process::request(RESOURCE_TYPE r, int num) {
	//������Դ������request������������Դ�Ľ���ǵ�ǰ�������еĽ���
	if (!rcb->request(r, num, current)) {	//���������Դ���㣬��ǰ���̽�����������
		PCBNode* temp = current;
		dequeue();
		cout << "process " << current->name << " is running. ";
		cout << "process " << temp->name << " is blocked. " << endl;
	}
	else {	//��Դ�������������ʾ��Ϣ
		switch (r) {
		case R1:
			cout << "process " << current->name << " requests " << num << " R1." << endl;
			break;
		case R2:
			cout << "process " << current->name << " requests " << num << " R2." << endl;
			break;
		case R3:
			cout << "process " << current->name << " requests " << num << " R3." << endl;
			break;
		case R4:
			cout << "process " << current->name << " requests " << num << " R4." << endl;
			break;
		}
	}
}

//�ͷ���Դ
void Process::release(RESOURCE_TYPE r, int num) {
	if(current) release(r, num, current);	//����ǰ��㴫�����ص�release����
}
void Process::release(RESOURCE_TYPE r, int num, PCBNode* p) {
	rcb->release(r, num);	//��Դ���ͷ���Ӧ��Դ

	//����ռ�е���Ӧ��Դ����
	switch (r) {
	case R1:
		p->r1 -= num;
		break;
	case R2:
		p->r2 -= num;
		break;
	case R3:
		p->r3 -= num;
		break;
	case R4:
		p->r4 -= num;
		break;
	}

	//���Ի�����Ӧ����
	awakeCheck(r);
}

//���Ի��ѽ��̣�����Ϊ��Դ����
void Process::awakeCheck(RESOURCE_TYPE r) {
	PCBNode* temp;

	//����Ӧ��Դ�����������г��Ի��ѽ���
	switch (r) {
	case R1:
		temp = rcb->r1->wl->getFirst();
		while (temp->name != "NULL") {	//���������в���
			if (temp->r1r > rcb->r1->available) break;	//������һ�������ϻ����������˳�

			enqueue(rcb->r1->wl->dequeue());	//��������������Դ���㣬�����������
			rcb->r1->available -= temp->r1r;	//������Ӧ��Դ
			temp->r1 += temp->r1r;	//ռ����Դ
			temp->r1r = 0;	//��������ԴΪ0
			cout << "wake up process " << temp->name << "." << endl;
			temp = temp->succ;	//���������һ����������
		}
		break;
	case R2:
		temp = rcb->r2->wl->getFirst();
		while (temp->name != "NULL") {
			if (temp->r2r > rcb->r2->available) break;
			enqueue(rcb->r2->wl->dequeue());
			rcb->r2->available -= temp->r2r;
			temp->r2 += temp->r2r;
			temp->r2r = 0;
			cout << "wake up process " << temp->name << "." << endl;
			temp = temp->succ;
		}
		break;
	case R3:
		temp = rcb->r3->wl->getFirst();
		while (temp->name != "NULL") {
			if (temp->r3r > rcb->r3->available) break;
			enqueue(rcb->r3->wl->dequeue());
			rcb->r3->available -= temp->r3r;
			temp->r3 += temp->r3r;
			temp->r3r = 0;
			cout << "wake up process " << temp->name << "." << endl;
			temp = temp->succ;
		}
		break;
	case R4:
		temp = rcb->r4->wl->getFirst();
		while (temp->name != "NULL") {
			if (temp->r4r > rcb->r4->available) break;
			enqueue(rcb->r4->wl->dequeue());
			rcb->r4->available -= temp->r4r;
			temp->r4 += temp->r4r;
			temp->r4r = 0;
			cout << "wake up process " << temp->name << "." << endl;
			temp = temp->succ;
		}
		break;
	}
}

//��ʱ
void Process::timeOut() {
	if (!current) return;	//��ǰ�޽������У�����
	PCBNode* temp = current;
	schedule();	//���е���
	cout << "process " << current->name << " is running. ";
	cout << "process " << temp->name << " is ready. " << endl ;
}

//���������Ϣ
void Process::print() {
	cout << "process " << current->name << " is running." << endl;
}
//�����������
void Process::listReady() {
	cout << "2:";
	if (current->priority == SYSTEM) cout << current->name << "-";
	PCBNode* cur = rl3->getFirst();
	//������������е����н���
	for (int i = 0; i < rl3->getSize(); i++) {
		if (i == rl3->getSize() - 1) cout << cur->name;
		else cout << cur->name << "-";
		cur = cur->succ;
	}
	cout << endl;
	cout << "1:";
	if (current->priority == USER) cout << current->name << "-";
	cur = rl2->getFirst();
	for (int i = 0; i < rl2->getSize(); i++) {
		if (i == (rl2->getSize() - 1)) cout << cur->name;
		else cout << cur->name << "-";
		cur = cur->succ;
	}
	cout << endl;
	cout << "0:";
	if (current->priority == INIT) cout << current->name << "-";
	cur = rl1->getFirst();
	for (int i = 0; i < rl1->getSize(); i++) {
		if (i == rl1->getSize() - 1) cout << cur->name;
		else cout << cur->name << "-";
		cur = cur->succ;
	}
	cout << endl;
}
//�����������
void Process::listBlocked() {
	cout << "R1:";
	PCBNode* cur = rcb->r1->wl->getFirst();
	//������������е����н���
	for (int i = 0; i < rcb->r1->wl->getSize(); i++) {
		if(i == rcb->r1->wl->getSize()-1) cout << cur->name;
		else cout << cur->name << "-";
		cur = cur->succ;
	}
	cout << endl;
	cout << "R2:";
	cur = rcb->r2->wl->getFirst();
	for (int i = 0; i < rcb->r2->wl->getSize(); i++) {
		if (i == rcb->r2->wl->getSize() - 1) cout << cur->name;
		else cout << cur->name << "-";
		cur = cur->succ;
	}
	cout << endl;
	cout << "R3:";
	cur = rcb->r3->wl->getFirst();
	for (int i = 0; i < rcb->r3->wl->getSize(); i++) {
		if (i == rcb->r3->wl->getSize() - 1) cout << cur->name;
		else cout << cur->name << "-";
		cur = cur->succ;
	}
	cout << endl;
	cout << "R4:";
	cur = rcb->r4->wl->getFirst();
	for (int i = 0; i < rcb->r4->wl->getSize(); i++) {
		if (i == rcb->r4->wl->getSize() - 1) cout << cur->name;
		else cout << cur->name << "-";
		cur = cur->succ;
	}
	cout << endl;
}
//���δ������Դ����
void Process::listAvailable() {
	cout << "R1 " << rcb->r1->available << endl;
	cout << "R2 " << rcb->r2->available << endl;
	cout << "R3 " << rcb->r3->available << endl;
	cout << "R4 " << rcb->r4->available << endl;
}
//���������Ϣ
void Process::provide(string n) {
	PCBNode* temp = *find(n);	//�ڼ�¼���н�����Ϣ���������ҵ�����
	cout << n << " " << "information:" << endl;
	cout << "State:" << temp->rawState() << endl;
	if (temp->parent) cout << "Parent:" << temp->parent->name << " ";
	else cout << "Parent:" << "NULL" << " ";
	if (temp->children.size() > 0) {
		cout << "Children:";
		for (int i = 0; i < temp->children.size(); i++) 
			cout << temp->children[i]->name << " ";
		cout << endl;
	}
	else cout << "Children: NULL" << endl;	
	cout << "Possession of resources:" << "R1:" << temp->r1 << " "
		<< "R2:" << temp->r2 << " " << "R3:" << temp->r3 << " " << "R4:" << temp->r4 << " " << endl;
	cout << "Request of resources:" << "R1:" << temp->r1r << " "
		<< "R2:" << temp->r2r << " " << "R3:" << temp->r3r << " " << "R4:" << temp->r4r << " " << endl;
}