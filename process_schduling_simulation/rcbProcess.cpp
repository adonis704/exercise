#include"rcbProcess.h"

RCBProcess::RCBProcess() {
	//��ʼ����Դ�б�
	r1 = new RCBNode("R1", 1);
	r2 = new RCBNode("R2", 2);
	r3 = new RCBNode("R3", 3);
	r4 = new RCBNode("R4", 4);
}
RCBProcess::~RCBProcess() {

}

//������Դ��������������
bool RCBProcess::request(RESOURCE_TYPE r, int num, PCBNode* p) {
	switch (r) {
	case R1:
		if (r1->available >= num) {
			r1->available -= num;	//�����Դ���������
			p->r1 += num;	//����ռ����Դ
			return true;
		}
		else {
			r1->wl->enqueue(p);	//��Դ���㣬���̽�����������
			p->r1r += num;	//���̴�������Դ����
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

//�ͷ���Դ
void RCBProcess::release(RESOURCE_TYPE r, int num) {
	switch (r) {
	case R1:
		r1->available += num;	//������Դ����
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