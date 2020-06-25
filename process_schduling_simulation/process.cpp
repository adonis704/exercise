#include"process.h"

//初始化
Process::Process() {
	//初始化进程三个就绪队列
	rl1 = new PCBList();
	rl2 = new PCBList();
	rl3 = new PCBList();
	rcb = new RCBProcess();	//初始化资源块
	current = NULL;	//当前运行进程为空
	PCBNode* null = new PCBNode();
	nodeVec.push_back(null);	//将一个代表空的结点指针放入总进程，便于后面的查找等操作
}
Process::~Process() {

}

//init操作
void Process::init() {
	create("init", INIT);	//创建一个init进程
	cout << "init process is running." << endl;	//输出提示信息
}
//创建新进程
PCBNode* Process::create(string n, PCB_PRIORITY p) {
	PCBNode* temp = 0;
	//根据优先级，创建新进程并分配到相应的就绪队列
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

	//如果当前有进程运行且不是初始化进程，将新进程设为当前进程的子进程
	if (current && current->name != "init") {
		current->children.push_back(temp);
		temp->parent = current;
	}
	//如果新进程优先级比当前进程高，抢占
	if (current && temp->priority > current->priority)	schedule();
	//如果当前没有进程，出队（用于创建init进程）
	if (!current) dequeue();
	//将进程控制块指针加入记录所有进程信息的向量中
	if (temp != 0) nodeVec.push_back(temp);
	//如果当前进程不是init进程，则输出当前进程名字（init进程输出不一样）
	if(current->name != "init") print();
	return temp;
}

//撤销进程，参数是进程的名字
void Process::destroy(string n) {
	vector<PCBNode*>::iterator iter = find(n);	//找到进程名字对应的迭代器
	PCBNode* temp = *iter;	//对应进程控制块的指针
	
	//找不到进程，直接返回
	if (temp->name == "NULL") {
		cout << "no process named " << n << endl;
		return;
	}

	for (int i = 0; i < temp->children.size(); i++) destroy(temp->children[i]);	//如果有子进程，撤销子进程
	nodeVec.erase(iter);	//在向量中删除相应进程
	destroy(temp);	//重载的destroy函数，开始实现撤销操作
}
void Process::destroy(PCBNode* x) {
	//释放资源并尝试唤醒阻塞进程，唤醒操作在release方法中
	if (x->r1 != 0) { cout << "release R1. "; release(R1, x->r1, x);}
	if (x->r2 != 0) { cout << "release R2. "; release(R2, x->r2, x);}
	if (x->r3 != 0) { cout << "release R3. "; release(R3, x->r3, x);}
	if (x->r4 != 0) { cout << "release R4. "; release(R4, x->r4, x);}

	//找到进程所在的队列，删除进程
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
		dequeue();	//如果需要撤销的进程是正在运行的进程，那么下一个进程便可以运行
	}
}
//在记录所有进程信息的向量中找到名为n的进程，返回迭代器
vector<PCBNode*>::iterator Process::find(string n) {
	//使用迭代器遍历
	for (vector<PCBNode*>::iterator iter = nodeVec.begin(); iter != nodeVec.end(); iter++)
		if ((*iter)->name == n) return iter;
	//没有找到则返回代表空的迭代器
	vector<PCBNode*>::iterator iter = nodeVec.begin();
	return iter;
}

//调度
void Process::schedule() {
	enqueue(current);	//将当前运行进程放入相应队列
	dequeue();	//下一个运行的进程出队
}
PCBNode* Process::enqueue(PCBNode* x) {
	PCBNode* temp = 0;
	//根据进程优先级选择相应的就绪队列入队
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
	//优先级高的队列先得到调度，为空才到下一优先级队列
	int rank = 0;
	if (rl3->getSize() != 0) rank = 3;
	else if (rl2->getSize() != 0) rank = 2;
	else if (rl1->getSize() != 0) rank = 1;

	//运行相应队列第一个进程
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

//请求资源
void Process::request(RESOURCE_TYPE r, int num) {
	//调用资源块对象的request方法，请求资源的结点是当前正在运行的进程
	if (!rcb->request(r, num, current)) {	//如果请求资源不足，当前进程进入阻塞队列
		PCBNode* temp = current;
		dequeue();
		cout << "process " << current->name << " is running. ";
		cout << "process " << temp->name << " is blocked. " << endl;
	}
	else {	//资源可以请求，输出提示信息
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

//释放资源
void Process::release(RESOURCE_TYPE r, int num) {
	if(current) release(r, num, current);	//将当前结点传入重载的release函数
}
void Process::release(RESOURCE_TYPE r, int num, PCBNode* p) {
	rcb->release(r, num);	//资源块释放相应资源

	//进程占有的相应资源减少
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

	//尝试唤醒相应进程
	awakeCheck(r);
}

//尝试唤醒进程，参数为资源类型
void Process::awakeCheck(RESOURCE_TYPE r) {
	PCBNode* temp;

	//到相应资源的阻塞队列中尝试唤醒进程
	switch (r) {
	case R1:
		temp = rcb->r1->wl->getFirst();
		while (temp->name != "NULL") {	//当阻塞队列不空
			if (temp->r1r > rcb->r1->available) break;	//遇到第一个不符合唤醒条件的退出

			enqueue(rcb->r1->wl->dequeue());	//如果进程请求的资源充足，进入就绪队列
			rcb->r1->available -= temp->r1r;	//分配相应资源
			temp->r1 += temp->r1r;	//占有资源
			temp->r1r = 0;	//待请求资源为0
			cout << "wake up process " << temp->name << "." << endl;
			temp = temp->succ;	//继续检查下一个阻塞进程
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

//超时
void Process::timeOut() {
	if (!current) return;	//当前无进程运行，返回
	PCBNode* temp = current;
	schedule();	//进行调度
	cout << "process " << current->name << " is running. ";
	cout << "process " << temp->name << " is ready. " << endl ;
}

//输出进程信息
void Process::print() {
	cout << "process " << current->name << " is running." << endl;
}
//输出就绪进程
void Process::listReady() {
	cout << "2:";
	if (current->priority == SYSTEM) cout << current->name << "-";
	PCBNode* cur = rl3->getFirst();
	//输出就绪队列中的所有进程
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
//输出阻塞进程
void Process::listBlocked() {
	cout << "R1:";
	PCBNode* cur = rcb->r1->wl->getFirst();
	//输出阻塞队列中的所有进程
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
//输出未分配资源数量
void Process::listAvailable() {
	cout << "R1 " << rcb->r1->available << endl;
	cout << "R2 " << rcb->r2->available << endl;
	cout << "R3 " << rcb->r3->available << endl;
	cout << "R4 " << rcb->r4->available << endl;
}
//输出进程信息
void Process::provide(string n) {
	PCBNode* temp = *find(n);	//在记录所有进程信息的向量中找到进程
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