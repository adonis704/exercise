#pragma once
#include<vector>
#include<string>
#include<functional>
using namespace std;

class StateA {
public:
	vector<int> state;	//��ǰ״̬
	StateA* parent;	//��ǰ״̬���ĸ�״̬���ģ���������ӡѰ·
	int dist;	//����Ŀ��״̬�ľ���
	int	deep;	//��ǰ״̬�ǵڼ���

	StateA(vector<int> s, int d,int de,StateA* p);
};