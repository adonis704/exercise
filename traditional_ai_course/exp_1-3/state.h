#pragma once
#include<vector>
#define CHILDREN_MAX_NUM 100	//����ʿ��Ұ���������
using namespace std;

class State {
public:
	int m, c, n;	//״̬��Ԫ��
	int deep;	//�ڽ�������еĵڼ���
	State* parent;	//��һ״̬
	vector<State*> children;	//��һ����״̬

	State(int ms = 0,int cs = 0,int ns = 0, State* p = 0, int d = -1);
	~State();
	State* setChildren(int ms, int cs, int ns);
};