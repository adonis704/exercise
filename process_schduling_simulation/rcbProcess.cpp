#include"rcbProcess.h"

RCBProcess::RCBProcess() {
	//初始化资源列表
	r1 = new RCBNode("R1", 1);
	r2 = new RCBNode("R2", 2);
	r3 = new RCBNode("R3", 3);
	r4 = new RCBNode("R4", 4);
}
RCBProcess::~RCBProcess() {

}

//请求资源，需输入请求结点
bool RCBProcess::request(RESOURCE_TYPE r, int num, PCBNode* p) {
	switch (r) {
	case R1:
		if (r1->available >= num) {
			r1->available -= num;	//如果资源充足则分配
			p->r1 += num;	//进程占有资源
			return true;
		}
		else {
			r1->wl->enqueue(p);	//资源不足，进程进入阻塞队列
			p->r1r += num;	//进程待请求资源增加
			p->state = BLOCKED;
		}
		break;
	case R2:
		if (r2->available >= num) {
			r2->available -= num;
			p->r2 += num;
			return true;
		}
		else {
			r2->wl->enqueue(p);
			p->r2r += num;
			p->state = BLOCKED;
		}
		break;
	case R3:
		if (r3->available >= num) {
			r3->available -= num;
			p->r3 += num;
			return true;
		}
		else {
			r3->wl->enqueue(p);
			p->r3r += num;
			p->state = BLOCKED;
		}
		break;
	case R4:
		if (r4->available >= num) {
			r4->available -= num;
			p->r4 += num;
			return true;
		}
		else {
			r4->wl->enqueue(p);
			p->r4r += num;
			p->state = BLOCKED;
		}
		break;
	}
	return false;
}

//释放资源
void RCBProcess::release(RESOURCE_TYPE r, int num) {
	switch (r) {
	case R1:
		r1->available += num;	//可用资源增加
		break;
	case R2:
		r2->available += num;
		break;
	case R3:
		r3->available += num;
		break;
	case R4:
		r4->available += num;
		break;
	}
}