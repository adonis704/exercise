#pragma once
#include<iostream>
#include<stack>
#include"node.h"
const int MAX_COST = 20;
const int CITY_NUM = 3;
class Dijkstra {
private:
	vector<Node*> cities;	//存放所有城市信息

	void generate();
	vector<Node*> randNode(int pos);
	vector<int> randCost(int num);
	int find(Node* target, vector<Node*> done);
	Heap<Node*> update(Heap<Node*> heap);
	void print(Node* start, Node* traget);
	void init();

public:
	Dijkstra(int num);

	void print();
	void solve(int start, int end);
	int size();
};