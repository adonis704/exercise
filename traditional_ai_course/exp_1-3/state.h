#pragma once
#include<vector>
#define CHILDREN_MAX_NUM 100	//传道士、野人最大人数
using namespace std;

class State {
public:
	int m, c, n;	//状态三元组
	int deep;	//在解的序列中的第几步
	State* parent;	//上一状态
	vector<State*> children;	//下一可能状态

	State(int ms = 0,int cs = 0,int ns = 0, State* p = 0, int d = -1);
	~State();
	State* setChildren(int ms, int cs, int ns);
};