#include"stateA.h"

//����ȱʡ���ԣ�״̬�����룬��ȣ������
StateA::StateA(vector<int> s,int d, int de, StateA* p) {
	parent = p;
	dist = d; deep = de;
	state = s;
}
