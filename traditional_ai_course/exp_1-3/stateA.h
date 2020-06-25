#pragma once
#include<vector>
#include<string>
#include<functional>
using namespace std;

class StateA {
public:
	vector<int> state;	//当前状态
	StateA* parent;	//当前状态由哪个状态来的，用于最后打印寻路
	int dist;	//距离目标状态的距离
	int	deep;	//当前状态是第几步

	StateA(vector<int> s, int d,int de,StateA* p);
};