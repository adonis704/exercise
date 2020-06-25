#pragma once
#include"heap.h"
#include"heap.cpp"
const int infinity = 65535;
class Node {
public:
	int name;	//城市名字
	Node* parent;	//通路中的上一个城市
	int weight;	//总开销
	vector<Node*> available;	//可达城市
	vector<int> cost;	//到达对应城市的开销

	Node(int n, int w = infinity, vector<Node*> a = { NULL }, vector<int> c = {-1});
	
};